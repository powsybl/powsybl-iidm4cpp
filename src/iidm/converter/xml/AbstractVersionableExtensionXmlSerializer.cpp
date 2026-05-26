/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

AbstractVersionableExtensionXmlSerializer::AbstractVersionableExtensionXmlSerializer(std::string&& extensionName, std::string&& extensionCategory, std::string&& namespacePrefix,
                                             ExtensionXmlVersions&& versions) :
    ExtensionXmlSerializer(std::move(extensionName), std::move(extensionCategory), std::move(namespacePrefix)),
    m_versions(std::move(versions)) {
    
    if(m_versions.empty()) {
        throw PowsyblException(stdcxx::format("The %1% extension's serializer must define at least one version", extensionName));
    }
}

void AbstractVersionableExtensionXmlSerializer::checkExtensionVersionSupported(const std::string& extensionVersion) const {
    versionOf(extensionVersion, true);
}

void AbstractVersionableExtensionXmlSerializer::checkReadingCompatibility(const NetworkXmlReaderContext& networkContext) const {
    const auto& networkVersion = networkContext.getVersion();
    checkCompatibilityNetworkVersion(networkVersion);

    for (const auto& extVersion : m_versions) {
        if (extVersion.supports(networkVersion) && networkContext.containsExtensionNamespaceUri(extVersion.getNamespaceUri())) {
            // at least one version is compatible
            return;
        }
    }
    //no version compatible :
    throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not compatible with the %2% extension's namespace URI", networkVersion.toString("."), getExtensionName()));
}

void AbstractVersionableExtensionXmlSerializer::checkWritingCompatibility(const std::string& extensionVersion, const IidmXmlVersion& version) const {

    auto extVersionRef = versionOf(extensionVersion, true);
    if( !(extVersionRef.get().supports(version)) ) {
        throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not compatible with the version %2% of the %3% extension", version.toString("."), extensionVersion, getExtensionName()));
    }

}

void AbstractVersionableExtensionXmlSerializer::checkCompatibilityNetworkVersion(const IidmXmlVersion& version) const {
    if(!isIIDMVersionSupported(version)) {
        throw PowsyblException(stdcxx::format("IIDM-XML version of network (%1%) is not supported by the %2% extension's XML serializer", version.toString("."), getExtensionName()));
    }
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespaceUri() const {
    return getNamespaceUri(getVersion());
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespaceUri(const std::string& extensionVersion) const {
    if(extensionVersion.empty()) {
        return getNamespaceUri();
    }
    return versionOf(extensionVersion, true).get().getNamespaceUri();
}

const std::string& AbstractVersionableExtensionXmlSerializer::getNamespacePrefix() const {
    return getDefaultVersion().getNamespacePrefix();
}
const std::string& AbstractVersionableExtensionXmlSerializer::getNamespacePrefix(const std::string& extensionVersion) const {
    if(extensionVersion.empty()) {
        return getNamespacePrefix();
    }    
    return versionOf(extensionVersion, true).get().getNamespacePrefix();
}

const std::string& AbstractVersionableExtensionXmlSerializer::getSerializationName(const std::string& extensionVersion) const {
    auto ext = versionOf(extensionVersion, false);
    return (static_cast<bool>(ext)) ? ext.get().getSerializationName() : getExtensionName();
}

std::set<std::string> AbstractVersionableExtensionXmlSerializer::getSerializationNames() const {
    std::set<std::string> serializationNames;
    serializationNames.emplace(getExtensionName());
    for (const auto& extVersion : m_versions) {
        serializationNames.emplace(extVersion.getSerializationName());
    }
    return serializationNames;
}

std::string AbstractVersionableExtensionXmlSerializer::getVersion() const {
    return getVersion(IidmXmlVersion::CURRENT_IIDM_XML_VERSION()).toString();
}

bool AbstractVersionableExtensionXmlSerializer::isIIDMVersionSupported(const IidmXmlVersion& networkVersion) const {
    auto it = std::find_if(m_versions.begin(), m_versions.end(), [&networkVersion](const ExtensionXmlVersion& ext){
        return ext.supports(networkVersion);
    });
    return it!=m_versions.end();
}

const ExtensionXmlVersion& AbstractVersionableExtensionXmlSerializer::getVersion(const IidmXmlVersion& networkVersion) const {
    //We should returns the default version even if it is not the greatest in terms of SemVer (getDefaultVersion() can be overwritten not to be the "last" one)
    const auto& defaultExtensionVersion = getDefaultVersion();
    if(defaultExtensionVersion.supports(networkVersion)) {
        return defaultExtensionVersion;
    }

    for (auto rit = m_versions.crbegin(); rit != m_versions.crend(); ++rit) {
        if(rit->supports(networkVersion)) {
            return *rit;
        }
    }
    throw PowsyblException(stdcxx::format("No compatible version for %1% extension for IIDM version %2%", getExtensionName(), networkVersion.toString(".")));
}

std::set<std::string> AbstractVersionableExtensionXmlSerializer::getVersions() const {
    std::set<std::string> s_extensionNames;
    for (const auto& ext : m_versions) {
        s_extensionNames.emplace(ext.toString());
    }
    return s_extensionNames;
}

const ExtensionXmlVersion& AbstractVersionableExtensionXmlSerializer::versionOf(const std::string& extensionVersion) const {
    return versionOf(extensionVersion, true).get();
}

stdcxx::CReference<ExtensionXmlVersion> AbstractVersionableExtensionXmlSerializer::versionOf(const std::string& extensionVersion, bool throwIfUnknown) const {
    auto it = std::find_if(m_versions.begin(), m_versions.end(), [&extensionVersion](const ExtensionXmlVersion& ext){
        return extensionVersion == ext.toString();
    });

    if (it == m_versions.end()) {
        if(throwIfUnknown) {
            throw PowsyblException(stdcxx::format("The %1% extension's version (%2%) is unknown", getExtensionName(), extensionVersion));
        }
        return stdcxx::CReference<ExtensionXmlVersion>();
    }
    return stdcxx::cref(*it);
}

const ExtensionXmlVersion& AbstractVersionableExtensionXmlSerializer::getExtensionVersionImported(const NetworkXmlReaderContext& networkContext) const {
    const std::string& extVersionFromContext = networkContext.getExtensionVersion(*this);
    return versionOf(extVersionFromContext);
}
const ExtensionXmlVersion& AbstractVersionableExtensionXmlSerializer::getExtensionVersionToExport(const NetworkXmlWriterContext& networkContext) const {
    const std::string& extVersionFromContext = networkContext.getExtensionVersion(getExtensionName());

    auto ext = versionOf(extVersionFromContext, false);
    return (static_cast<bool>(ext)) ? ext.get() : getVersion(networkContext.getVersion());
}

const ExtensionXmlVersion& AbstractVersionableExtensionXmlSerializer::getDefaultVersion() const {
    return *m_versions.rbegin();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
