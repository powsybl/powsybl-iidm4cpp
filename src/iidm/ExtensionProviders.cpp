/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ExtensionProviders.hpp>

#include <boost/dll/import.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Dummy>
stdcxx::CReference<T> ExtensionProviders<T, Dummy>::findProvider(const std::string& name) const {
    stdcxx::CReference<T> provider;

    const auto& it = m_providers.find(name);
    if (it != m_providers.end()) {
        provider = stdcxx::cref(it->second);
    }

    return provider;
}

template <typename T, typename Dummy>
const T& ExtensionProviders<T, Dummy>::findProviderOrThrowException(const std::string& name) const {
    const auto& it = m_providers.find(name);
    if (it == m_providers.end()) {
        throw PowsyblException(stdcxx::format("No provider found for extension '%1'", name));
    }
    return *it->second;
}

template <typename T, typename Dummy>
ExtensionProviders<T, Dummy>& ExtensionProviders<T, Dummy>::getInstance() {
    static ExtensionProviders<T, Dummy> s_instance;
    return s_instance;
}

template <typename T, typename Dummy>
stdcxx::const_range<T> ExtensionProviders<T, Dummy>::getProviders() const {
    return boost::adaptors::values(m_providers) | boost::adaptors::indirected;
}

template <typename T, typename Dummy>
stdcxx::range<T> ExtensionProviders<T, Dummy>::getProviders() {
    return boost::adaptors::values(m_providers) | boost::adaptors::indirected;
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::loadExtensions(const boost::filesystem::path& directory, const std::regex& pattern) {
    if (!boost::filesystem::exists(directory)) {
        throw PowsyblException(stdcxx::format("Path %1% does not exist", directory));
    }

    if (!boost::filesystem::is_directory(directory)) {
        throw PowsyblException(stdcxx::format("Path %1% is not a directory", directory));
    }

    for (const auto& it : boost::filesystem::directory_iterator(directory)) {
        if (boost::filesystem::is_regular_file(it) && std::regex_match(it.path().string(), pattern)) {
            loadLibrary(boost::filesystem::canonical(it.path()));
        }
    }
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::loadLibrary(const boost::filesystem::path& libraryPath) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();

    if (m_loadedLibraries.find(libraryPath) == m_loadedLibraries.end()) {
        try {
            boost::dll::shared_library library(libraryPath);
            if (library.has("createSerializers")) {
                const auto& createSerializers = boost::dll::import_alias<std::vector<std::unique_ptr<T>>()>(library, "createSerializers");
                std::vector<std::unique_ptr<T>> serializers = createSerializers();
                for (auto& it : serializers) {
                    const std::string& extensionName = it->getExtensionName();
                    const auto& status = m_providers.emplace(std::make_pair(extensionName, std::move(it)));
                    if (!status.second) {
                        throw PowsyblException(stdcxx::format("Extension %1% was already registered", extensionName));
                    }
                    logger.debug(stdcxx::format("Extension %1% has been loaded from %2%", extensionName, libraryPath));
                }
                m_loadedLibraries.insert(std::make_pair(libraryPath, library));
            }
        } catch (const boost::system::system_error& err) {
            logger.info(stdcxx::format("Unable to load file %1%: %2%", libraryPath, err.what()));
        }
    }
}

template class
ExtensionProviders<converter::xml::ExtensionXmlSerializer>;

}  // namespace iidm

}  // namespace powsybl
