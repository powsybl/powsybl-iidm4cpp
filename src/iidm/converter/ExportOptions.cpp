/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ExportOptions.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

ExportOptions::ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                             bool throwExceptionIfExtensionNotFound, const std::string& version) :
    m_indent(indent),
    m_onlyMainCc(onlyMainCc),
    m_throwExceptionIfExtensionNotFound(throwExceptionIfExtensionNotFound),
    m_topologyLevel(topologyLevel),
    m_withBranchSV(withBranchSV),
    m_version(version) {
}

ExportOptions& ExportOptions::addExtension(const std::string& extension) {
    m_extensions.insert(extension);
    return *this;
}

ExportOptions& ExportOptions::addExtensionVersion(const std::string& extensionName, const std::string& extensionVersion) {
    if (!m_extensions.empty() && m_extensions.find(extensionName) == m_extensions.end()) {
        throw PowsyblException(stdcxx::format("%1% is not an extension you have passed in the extensions list to export.", extensionName));
    }
    if (m_extensionsVersions.find(extensionName) != m_extensionsVersions.end()) {
        throw PowsyblException(stdcxx::format("The version of %1%'s XML serializer has already been set.", extensionName));
    }

    m_extensionsVersions.insert(std::make_pair(extensionName, extensionVersion));

    return *this;
}

const std::string& ExportOptions::getExtensionVersion(const std::string& extensionName) const {
    static std::string s_noVersion;

    const auto& it = m_extensionsVersions.find(extensionName);

    return it == m_extensionsVersions.end() ? s_noVersion : it->second;
}

const std::string& ExportOptions::getVersion() const {
    return m_version;
}

const TopologyLevel& ExportOptions::getTopologyLevel() const {
    return m_topologyLevel;
}

bool ExportOptions::isAnonymized() const {
    return m_anonymized;
}

bool ExportOptions::isIndent() const {
    return m_indent;
}

bool ExportOptions::isOnlyMainCc() const {
    return m_onlyMainCc;
}

bool ExportOptions::isThrowExceptionIfExtensionNotFound() const {
    return m_throwExceptionIfExtensionNotFound;
}

bool ExportOptions::isWithBranchSV() const {
    return m_withBranchSV;
}

ExportOptions& ExportOptions::setAnonymised(bool anonymized) {
    m_anonymized = anonymized;
    return *this;
}

ExportOptions& ExportOptions::setIndent(bool indent) {
    m_indent = indent;
    return *this;
}

ExportOptions& ExportOptions::setOnlyMainCc(bool onlyMainCc) {
    m_onlyMainCc = onlyMainCc;
    return *this;
}

ExportOptions& ExportOptions::setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound) {
    m_throwExceptionIfExtensionNotFound = throwExceptionIfExtensionNotFound;
    return *this;
}

ExportOptions& ExportOptions::setTopologyLevel(const TopologyLevel& topologyLevel) {
    m_topologyLevel = topologyLevel;
    return *this;
}

ExportOptions& ExportOptions::setVersion(const std::string& version) {
    m_version = version;
    return *this;
}

ExportOptions& ExportOptions::setWithBranchSV(bool withBranchSV) {
    m_withBranchSV = withBranchSV;
    return *this;
}

bool ExportOptions::withExtension(const std::string& extension) const {
    return m_extensions.empty() || m_extensions.find(extension) != m_extensions.end();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
