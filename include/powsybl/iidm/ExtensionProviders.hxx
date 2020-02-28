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

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Extension.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Dummy>
std::map<std::string, std::unique_ptr<T>> ExtensionProviders<T, Dummy>::m_providers;

template <typename T, typename Dummy>
std::vector<boost::dll::shared_library> ExtensionProviders<T, Dummy>::m_handlers;

template <typename T, typename Dummy>
std::set<std::string> ExtensionProviders<T, Dummy>::m_extensionPaths;

template <typename T, typename Dummy>
ExtensionProviders<T, Dummy>::~ExtensionProviders() noexcept {
    m_providers.clear();
    m_handlers.clear();
    m_extensionPaths.clear();
}

template <typename T, typename Dummy>
stdcxx::CReference<T> ExtensionProviders<T, Dummy>::findProvider(const std::string& name) {
    stdcxx::CReference<T> provider;

    const auto& it = m_providers.find(name);
    if (it != m_providers.end()) {
        provider = stdcxx::cref(*it->second);
    }

    return provider;
}

template <typename T, typename Dummy>
const T& ExtensionProviders<T, Dummy>::findProviderOrThrowException(const std::string& name) {
    const auto& it = m_providers.find(name);
    if (it == m_providers.end()) {
        throw PowsyblException(logging::format("No provider found for extension '%1'", name));
    }
    return *it->second;
}

template <typename T, typename Dummy>
ExtensionProviders<T, Dummy>& ExtensionProviders<T, Dummy>::getInstance() {
    static ExtensionProviders<T, Dummy> s_instance;
    return s_instance;
}

template <typename T, typename Dummy>
std::vector<std::string> ExtensionProviders<T, Dummy>::getLibraries(const std::string& directory) {
    std::vector<std::string> libs;
    if (boost::filesystem::exists(directory) && boost::filesystem::is_directory(directory)) {
        boost::filesystem::recursive_directory_iterator it(directory);
        boost::filesystem::recursive_directory_iterator endit;

        while (it != endit)
        {
            if (boost::filesystem::is_regular_file(*it) && it->path().extension() == boost::dll::shared_library::suffix().string()) {
                libs.push_back((directory / it->path().filename()).string());
            }
            ++it;
        }
    }
    return libs;
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::addExtensionPath(const std::string& path) {
    if (!boost::filesystem::exists(path)) {
        throw PowsyblException(logging::format("Path %1% does not exist", path));
    }

    if (!boost::filesystem::is_directory(path)) {
        throw PowsyblException(logging::format("Path %1% is not a directory", path));
    }

    if (m_extensionPaths.find(path) != m_extensionPaths.end()) {
        throw PowsyblException(logging::format("Path %1% has already been registered", path));
    }

    const std::vector<std::string> &libFiles = getLibraries(path);
    for (const std::string &libFile : libFiles) {
        boost::dll::shared_library lib(libFile);
        logging::Logger &logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
        if (lib.has("createSerializers")) {
            boost::dll::shared_library library(lib);
            const auto& createSerializers = boost::dll::import_alias<std::map<std::string, std::unique_ptr<T>>()>(library, "createSerializers");
            for (auto &it : createSerializers()) {
                const auto &status = m_providers.emplace(std::make_pair(it.first, std::move(it.second)));

                // check that extension was not already registered
                if (!status.second) {
                    throw PowsyblException(logging::format("Extension %1% was already registered", it.first));
                }
            }
            m_handlers.emplace_back(library);
        } else {
            logger.info(logging::format(
                "Unable to load %1%: one of the required symbol is missing (getExtensionsNames / create)",
                libFile));
        }
    }

    if (!libFiles.empty()) {
        m_extensionPaths.insert(path);
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
