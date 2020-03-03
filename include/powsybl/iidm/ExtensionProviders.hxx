/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
#define POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX

#include <powsybl/iidm/ExtensionProviders.hpp>

#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Extension.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Dummy>
stdcxx::CReference<T> ExtensionProviders<T, Dummy>::findProvider(const std::string& name) const {
    stdcxx::CReference<T> provider;

    const auto& it = m_providers.find(name);
    if (it != m_providers.end()) {
        provider = stdcxx::cref(*it->second);
    }

    return provider;
}

template <typename T, typename Dummy>
const T& ExtensionProviders<T, Dummy>::findProviderOrThrowException(const std::string& name) const {
    const auto& it = m_providers.find(name);
    if (it == m_providers.end()) {
        throw PowsyblException(logging::format("No provider found for extension '%1'", name));
    }
    return *it->second;
}

template <typename T, typename Dummy>
std::vector<std::string> ExtensionProviders<T, Dummy>::getFiles(const std::string& directory, const boost::regex& file) {
    std::vector<std::string> libs;
    if (boost::filesystem::exists(directory) && boost::filesystem::is_directory(directory)) {
        for (const auto& it : boost::filesystem::directory_iterator(directory)) {
            if (boost::filesystem::is_regular_file(it) && boost::regex_match(it.path().filename().c_str(), file)) {
                libs.push_back((directory / it.path().filename()).string());
            }
        }
    }

    return libs;
}

template <typename T, typename Dummy>
ExtensionProviders<T, Dummy>& ExtensionProviders<T, Dummy>::getInstance() {
    static ExtensionProviders<T, Dummy> s_instance;
    return s_instance;
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::loadExtensions(const std::string& path, const boost::regex& files) {
    if (!boost::filesystem::exists(path)) {
        throw PowsyblException(logging::format("Path %1% does not exist", path));
    }

    if (!boost::filesystem::is_directory(path)) {
        throw PowsyblException(logging::format("Path %1% is not a directory", path));
    }

    logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
    const std::vector<std::string>& libFiles = getFiles(path, files);
    for (const std::string& libFile : libFiles) {
        try {
            loadLibrary(libFile);
        } catch (const boost::system::system_error& err) {
            logger.info(logging::format("Unable to load file %1%: %2%", libFile, err.what()));
        }
    }
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::loadLibrary(const std::string& library) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
    boost::dll::shared_library lib(library);

    const boost::filesystem::path& libraryPath = boost::filesystem::canonical(lib.location());
    if (m_loadedLibraries.find(libraryPath) == m_loadedLibraries.end()) {
        if (lib.has("createSerializers")) {
            const auto& createSerializers = boost::dll::import_alias<std::set<std::unique_ptr<T>>()>(lib, "createSerializers");
            for (auto& it : createSerializers()) {
                const std::string& extensionName = it->getExtensionName();
                registerExtension(extensionName, std::move(const_cast<std::unique_ptr<T>&>(it)));
                logger.debug(logging::format("Extension %1% has been loaded from %2%", extensionName, library));
            }
            m_loadedLibraries.insert(libraryPath);
        }
    }
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::registerExtension(const std::string& name, std::unique_ptr<T>&& provider) {
    const auto& status = m_providers.emplace(std::make_pair(name, std::move(provider)));
    if (!status.second) {
        throw PowsyblException(logging::format("Extension %1% was already registered", name));
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
