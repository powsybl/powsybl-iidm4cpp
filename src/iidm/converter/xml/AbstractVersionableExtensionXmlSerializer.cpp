/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

AbstractVersionableExtensionXmlSerializer::AbstractVersionableExtensionXmlSerializer(std::string&& extensionName, std::string&& extensionCategory, std::string&& namespacePrefix,
                                                                                     VersionsCompatibility&& extensionVersions, std::map<std::string, std::string>&& namespaceUris) :
    ExtensionXmlSerializer(std::move(extensionName), std::move(extensionCategory), std::move(namespacePrefix)),
    m_extensionVersions(std::move(extensionVersions)),
    m_namespaceUris(std::move(namespaceUris)) {
}

AbstractVersionableExtensionXmlSerializer::AbstractVersionableExtensionXmlSerializer(std::string&& extensionName, std::string&& extensionCategory, std::string&& namespacePrefix,
                                              VersionsCompatibility&& extensionVersions, std::map<std::string, std::string>&& namespaceUris, 
                                              const std::map<std::string, AlternativeSerializationData>& alternativeData) :
    ExtensionXmlSerializer(std::move(extensionName), std::move(extensionCategory), std::move(namespacePrefix)),
    m_extensionVersions(std::move(extensionVersions)),
    m_namespaceUris(std::move(namespaceUris)) {

        std::set<std::string> alternativeNames;
        for(const auto& data: alternativeData) {
            if(data.first.empty()) {
                continue;
            }
            if(alternativeNames.count(data.first)) {
                throw PowsyblException(stdcxx::format("Duplicate alternative serialization name : %1%", data.first));
            }
            alternativeNames.emplace(data.first);
            for(const auto& extVersion : data.second.m_extensionVersions) {
                m_serializationNameByVersion[extVersion] = data.first;
                if(!data.second.m_namespacePrefix.empty()){
                    m_namespacePrefixByVersion[extVersion] = data.second.m_namespacePrefix;
                }
            }
        }
}

void AbstractVersionableExtensionXmlSerializer::checkExtensionVersionSupported(const std::string& extensionVersion) const {
    if (m_namespaceUris.find(extensionVersion) == m_namespaceUris.end()) {
        throw PowsyblException(stdcxx::format("The version %1% of the %2% extension is not supported.", extensionVersion, getExtensionName()));
    }
}

void AbstractVersionableExtensionXmlSerializer::checkReadingCompatibility(const NetworkXmlReaderContext& networkContext) const {
    const auto& version = networkContext.getVersion().toString(".");
    const auto& it = m_extensionVersions.find(version);
    if (it == m_extensionVersions.end()) {
        throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not supported by the %2% extension's XML serializer", version, getExtensionName()));
    }
    for (const auto& v : it->second) {
        if (networkContext.containsExtensionNamespaceUri(getNamespaceUri(v))) {
            return;
        }
    }
    throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not compatible with the %2% extension's namespace URI", version, getExtensionName()));
}

void AbstractVersionableExtensionXmlSerializer::checkWritingCompatibility(const std::string& extensionVersion, const IidmXmlVersion& version) const {
    checkExtensionVersionSupported(extensionVersion);
    const auto& strVersion = version.toString(".");
    const auto& it = m_extensionVersions.find(strVersion);
    if (it == m_extensionVersions.end()) {
        throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not supported by the %2% extension's XML serializer", strVersion, getExtensionName()));
    }
    if (std::find(it->second.begin(), it->second.end(), extensionVersion) == it->second.end()) {
        throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not compatible with the version %2% of the %3% extension", strVersion, extensionVersion, getExtensionName()));
    }
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespaceUri() const {
    return getNamespaceUri(getVersion());
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespaceUri(const std::string& extensionVersion) const {
    const auto& it = m_namespaceUris.find(extensionVersion);
    if (it == m_namespaceUris.end()) {
        throw PowsyblException(stdcxx::format("Namespace URI null for %1% extension's version %2%", getExtensionName(), extensionVersion));
    }

    return it->second;
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespacePrefix() const {
    return ExtensionXmlSerializer::getNamespacePrefix();
}
const std::string& AbstractVersionableExtensionXmlSerializer::getNamespacePrefix(const std::string& extensionVersion) const {
    return (m_namespacePrefixByVersion.count(extensionVersion)) ? m_namespacePrefixByVersion.at(extensionVersion) : ExtensionXmlSerializer::getNamespacePrefix();
}

const std::string& AbstractVersionableExtensionXmlSerializer::getSerializationName(const std::string& extensionVersion) const {
    return (m_serializationNameByVersion.count(extensionVersion)) ? m_serializationNameByVersion.at(extensionVersion) : getExtensionName();
}

std::set<std::string> AbstractVersionableExtensionXmlSerializer::getSerializationNames() const {
    std::set<std::string> names;
    names.emplace(getExtensionName());
    for (auto pairNames : m_serializationNameByVersion) {
        names.emplace(pairNames.second);
    }
    return names;
}

const std::string& AbstractVersionableExtensionXmlSerializer::getVersion() const {
    return getVersion(IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
}

bool AbstractVersionableExtensionXmlSerializer::versionExists(const std::string& networkVersion) const {
    return m_extensionVersions.find(networkVersion) != m_extensionVersions.end();
}

bool AbstractVersionableExtensionXmlSerializer::versionExists(const IidmXmlVersion& networkVersion) const {
    const std::string strNetworkVersion = networkVersion.toString(".");
    return versionExists(strNetworkVersion);
}

const std::string& AbstractVersionableExtensionXmlSerializer::getVersion(const IidmXmlVersion& networkVersion) const {
    std::string version = networkVersion.toString(".");
    const auto& it = m_extensionVersions.find(version);
    if (it == m_extensionVersions.end()) {
        throw PowsyblException(stdcxx::format("No compatible version for %1% extension's version %2%", getExtensionName(), version));
    }

    return it->second.back();
}

stdcxx::const_range<std::string> AbstractVersionableExtensionXmlSerializer::getVersions() const {
    return m_namespaceUris | boost::adaptors::map_keys;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
