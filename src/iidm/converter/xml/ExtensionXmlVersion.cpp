/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/ExtensionXmlVersion.hpp>

#include <functional>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ExtensionXmlVersion::ExtensionXmlVersion(const std::string& namespaceUri, const std::string& namespacePrefix, const std::string& serializationName,
        const IidmXmlVersion& minIidmVersionIncluded, const IidmXmlVersion& maxIidmVersionExcluded, const std::pair<int, int>& versionNumbersArray, const std::string& versionSuffix) :
    m_namespaceUri(namespaceUri),
    m_namespacePrefix(namespacePrefix),
    m_serializationName(serializationName),
    m_minIidmVersionIncluded(minIidmVersionIncluded),
    m_maxIidmVersionExcluded(stdcxx::cref(maxIidmVersionExcluded)),
    m_versionNumbersArray(versionNumbersArray),
    m_versionSuffix(versionSuffix) {
        checkNotEmpty(m_namespaceUri, "ExtensionXmlVersion's namespace URI must be defined");
        checkNotEmpty(m_namespacePrefix, "ExtensionXmlVersion's namespace prefix must be defined");
        checkNotEmpty(m_serializationName, "ExtensionXmlVersion's serialization name must be defined");

        m_extensionVersionName = std::to_string(m_versionNumbersArray.first) + "." + std::to_string(m_versionNumbersArray.second);
        if(!m_versionSuffix.empty()) {
            m_extensionVersionName = m_extensionVersionName + "-" + m_versionSuffix;
        }
}

ExtensionXmlVersion::ExtensionXmlVersion(const std::string& namespaceUri, const std::string& namespacePrefix, const std::string& serializationName,
        const IidmXmlVersion& minIidmVersionIncluded, const std::pair<int, int>& versionNumbersArray, const std::string& versionSuffix) :
    m_namespaceUri(namespaceUri),
    m_namespacePrefix(namespacePrefix),
    m_serializationName(serializationName),
    m_minIidmVersionIncluded(minIidmVersionIncluded),
    m_versionNumbersArray(versionNumbersArray),
    m_versionSuffix(versionSuffix) {
        checkNotEmpty(m_namespaceUri, "ExtensionXmlVersion's namespace URI must be defined");
        checkNotEmpty(m_namespacePrefix, "ExtensionXmlVersion's namespace prefix must be defined");
        checkNotEmpty(m_serializationName, "ExtensionXmlVersion's serialization name must be defined");

        m_extensionVersionName = std::to_string(m_versionNumbersArray.first) + "." + std::to_string(m_versionNumbersArray.second);
        if(!m_versionSuffix.empty()) {
            m_extensionVersionName = m_extensionVersionName + "-" + m_versionSuffix;
        }
}

bool ExtensionXmlVersion::supports(const IidmXmlVersion& networkVersion) const {
    return m_minIidmVersionIncluded <= networkVersion && ( !static_cast<bool>(m_maxIidmVersionExcluded) || m_maxIidmVersionExcluded.get() > networkVersion);
}

const std::string& ExtensionXmlVersion::getNamespaceUri() const {
    return m_namespaceUri;
}
const std::string& ExtensionXmlVersion::getNamespacePrefix() const {
    return m_namespacePrefix;
}
const std::string& ExtensionXmlVersion::getSerializationName() const {
    return m_serializationName;
}

const IidmXmlVersion& ExtensionXmlVersion::getMinIidmVersionIncluded() const {
    return m_minIidmVersionIncluded;
}
stdcxx::CReference<IidmXmlVersion> ExtensionXmlVersion::getMaxIidmVersionExcluded() const {
    return m_maxIidmVersionExcluded;
}

const std::string& ExtensionXmlVersion::toString() const {
    return m_extensionVersionName;
}


bool ExtensionXmlVersion::operator<(const ExtensionXmlVersion& version) const {
    return compareTo(version) < 0;
}

bool ExtensionXmlVersion::operator<=(const ExtensionXmlVersion& version) const {
    return compareTo(version) <= 0;
}

bool ExtensionXmlVersion::operator>(const ExtensionXmlVersion& version) const {
    return compareTo(version) > 0;
}

bool ExtensionXmlVersion::operator>=(const ExtensionXmlVersion& version) const {
    return compareTo(version) >= 0;
}

bool ExtensionXmlVersion::operator==(const ExtensionXmlVersion& version) const {
    return compareTo(version) == 0;
}

int ExtensionXmlVersion::compareTo(const ExtensionXmlVersion& version) const {
    if(m_versionNumbersArray.first != version.m_versionNumbersArray.first) {
        return m_versionNumbersArray.first - version.m_versionNumbersArray.first;
    }
    if(m_versionNumbersArray.second != version.m_versionNumbersArray.second) {
        return m_versionNumbersArray.second - version.m_versionNumbersArray.second;
    }
    if (m_versionSuffix.empty() && version.m_versionSuffix.empty()) {
        return 0;
    }
    if (m_versionSuffix.empty() || version.m_versionSuffix.empty()) {
        return m_versionSuffix.empty() ? -1 : 1;
    }
    return m_versionSuffix.compare(version.m_versionSuffix);
}



}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
