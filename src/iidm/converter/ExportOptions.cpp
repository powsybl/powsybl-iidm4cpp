/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ExportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

ExportOptions::ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                             bool throwExceptionIfExtensionNotFound) :
    m_indent(indent),
    m_onlyMainCc(onlyMainCc),
    m_throwExceptionIfExtensionNotFound(throwExceptionIfExtensionNotFound),
    m_topologyLevel(topologyLevel),
    m_withBranchSV(withBranchSV) {

}

ExportOptions& ExportOptions::addExtension(const std::string& extension) {
    m_extensions.insert(extension);
    return *this;
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
