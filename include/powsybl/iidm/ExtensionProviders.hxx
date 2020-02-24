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
#include <powsybl/stdcxx/filesystem.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Dummy>
std::map<std::string, std::unique_ptr<T>> ExtensionProviders<T, Dummy>::m_providers = std::map<std::string, std::unique_ptr<T>>();

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::clear() {
    m_providers.clear();
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::initialize(const std::string& extensionsPath) {
    std::vector<std::string> searchPath;
    searchPath.push_back(extensionsPath);
    initialize(searchPath);
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::initialize(const std::vector<std::string>& searchPaths) {
    for (const std::string& path : searchPaths) {
        const std::vector<std::string>& libFiles = stdcxx::getFileList(path, stdcxx::sharedLibExtension());

        for (const std::string& libFile : libFiles) {
            boost::dll::shared_library lib(libFile);
            logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
            if (lib.has("getExtensionsNames") && lib.has("create")) {
                auto getExtensionNames = boost::dll::import_alias<const std::initializer_list<std::string>&()>(lib, "getExtensionsNames");
                const std::initializer_list<std::string>& extensions = getExtensionNames();

                for (const std::string& extensionName : extensions) {
                    auto create = boost::dll::import_alias<std::unique_ptr<T>(const std::string&)>(lib, "create");
                    std::unique_ptr<T> provider = create(extensionName);
                    m_providers.emplace(std::make_pair(provider->getExtensionName(), std::move(provider)));
                }
            } else {
                logger.info(logging::format("Unable to load %1%: one of the required symbol is missing (getExtensionsNames / create)", libFile));
            }
        }
    }
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
void ExtensionProviders<T, Dummy>::registerExtension(const std::string& name, std::unique_ptr<T>&& provider) {
    m_providers.emplace(std::make_pair(name, std::move(provider)));
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
