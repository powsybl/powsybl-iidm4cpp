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

#include "extensions/LoadDetailXmlSerializer.hpp"
#include "extensions/SlackTerminalXmlSerializer.hpp"

namespace powsybl {

namespace iidm {

template <>
ExtensionProviders<converter::xml::ExtensionXmlSerializer, void>::ExtensionProviders() {
    registerExtension(stdcxx::make_unique<extensions::LoadDetailXmlSerializer>());
    registerExtension(stdcxx::make_unique<extensions::SlackTerminalXmlSerializer>());
}

template <typename T, typename Dummy>
stdcxx::CReference<T> ExtensionProviders<T, Dummy>::findProvider(const std::string& name) const {
    stdcxx::CReference<T> provider;

    const auto& it = m_providers.find(name);
    if (it != m_providers.end()) {
        provider = stdcxx::cref(it->second);
    } else { //not found in "real name" extensions
        //Look into "alternative names"
        const auto& itAlternative = m_providersAliases.find(name);
        if(itAlternative != m_providersAliases.end() && m_providers.count(itAlternative->second)) {
            provider = stdcxx::cref(m_providers.at(itAlternative->second));
        }
    }

    return provider;
}

template <typename T, typename Dummy>
const T& ExtensionProviders<T, Dummy>::findProviderOrThrowException(const std::string& name) const {
    const auto& it = m_providers.find(name);
    if (it == m_providers.end()) {

        //Look into aliases also
        const auto& itAlternative = m_providersAliases.find(name);
        if(itAlternative == m_providersAliases.end() || m_providers.count(itAlternative->second)==0) {
            throw PowsyblException(stdcxx::format("No provider found for extension '%1%'", name));
        }
        return *m_providers.at(itAlternative->second);
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

template <>
const std::string& ExtensionProviders<converter::xml::ExtensionXmlSerializer, void>::getSymbolName() const {
    static std::string s_symbolName = "createSerializers";

    return s_symbolName;
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
            if (library.has(getSymbolName())) {
                const auto& createSerializers = boost::dll::import_alias<std::vector<std::unique_ptr<T>>()>(library, getSymbolName());
                std::vector<std::unique_ptr<T>> serializers = createSerializers();
                for (auto& it : serializers) {
                    registerExtension(std::move(it), libraryPath);
                }
                m_loadedLibraries.insert(std::make_pair(libraryPath, library));
            } else {
                logger.error("Symbol not found");
            }
        } catch (const boost::system::system_error& err) {
            logger.info(stdcxx::format("Unable to load file %1%: %2%", libraryPath, err.what()));
        }
    }
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::registerExtension(std::unique_ptr<T>&& provider, const boost::filesystem::path& libraryPath) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();

    const std::string& extensionName = provider->getExtensionName();
    
    const auto& status = m_providers.emplace(std::make_pair(extensionName, std::move(provider)));
    if (!status.second) {
        throw PowsyblException(stdcxx::format("Unable to load file %1%: Extension %2% is already registered", libraryPath, status.first->first));
    }
    //When provider registered, also map alternativesNames if any
    //First of all "real name" of this extensionProvider has priority over any alternative name
    if(m_providersAliases.count(extensionName)) {
        logger.warn(stdcxx::format("Extension name %1% was used as alternative name for extension %2% - Alternative name unregistered.", 
                    extensionName, m_providersAliases[extensionName]));
        m_providersAliases.erase(extensionName);
    }
    registerExtensionAlternativeNames(status.first->second);

    logger.debug(stdcxx::format("Extension %1% has been loaded from %2%", extensionName, libraryPath));
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::unregisterExtension(const std::string& extensionName) {
    if(m_providers.erase(extensionName)) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
        logger.warn(stdcxx::format("Extension name %1% unregistered.", extensionName));
    }
    m_providersAliases.erase(extensionName);

    for (auto& providerIt : m_providers) {
        registerExtensionAlternativeNames(providerIt.second);
    }

}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::registerExtensionAlternativeNames(std::unique_ptr<T>& provider) {
    if(stdcxx::isInstanceOf<converter::xml::ExtensionXmlSerializer>(provider)) {
        auto serializer = dynamic_cast<converter::xml::ExtensionXmlSerializer*>(provider.get());
        if(!serializer) {
            return;
        }
        const std::string& extensionName = provider->getExtensionName();
        auto alternativeNames = serializer->getSerializationNames();
        for (const auto& alternativeName : alternativeNames) {
            //not consider real name as an alternative name :
            if(alternativeName == extensionName) {
                continue;
            }
            //alternative name already register for this provider :
            if(m_providersAliases.count(alternativeName) && m_providersAliases.at(alternativeName)==extensionName) {
                continue;
            }

            logging::Logger& logger = logging::LoggerFactory::getLogger<ExtensionProviders>();
            // If alternative name is the real name of this or any other registered provieder, that alternative name is disabled.
            if(m_providers.count(alternativeName)) {
                logger.warn(stdcxx::format("Alternative extension name %1% for extension %2% is already used by extension %3% - Skipped", 
                    alternativeName, extensionName, alternativeName));
                    continue;
            }
            // Or if alternative name is already used by another provider, ignore it
            if(m_providersAliases.count(alternativeName)) {
                logger.warn(stdcxx::format("Alternative extension name %1% for extension %2% is already used by extension %3% - Skipped", 
                    alternativeName, extensionName, m_providersAliases[alternativeName]));
                continue;
            }

            //Register alternative name
            m_providersAliases[alternativeName] = extensionName;
        }
    }
}

template class
ExtensionProviders<converter::xml::ExtensionXmlSerializer>;

}  // namespace iidm

}  // namespace powsybl
