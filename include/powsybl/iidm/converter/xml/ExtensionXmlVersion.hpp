/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLVERSION_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLVERSION_HPP

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <list>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class ExtensionXmlVersion {

public:
    ExtensionXmlVersion(const std::string& namespaceUri, const std::string& namespacePrefix, const std::string& serializationName,
        const IidmXmlVersion& minIidmVersionIncluded, const IidmXmlVersion& maxIidmVersionExcluded, const std::pair<int, int>& versionNumbersArray, const std::string& versionSuffix = "");

    ExtensionXmlVersion(const std::string& namespaceUri, const std::string& namespacePrefix, const std::string& serializationName,
        const IidmXmlVersion& minIidmVersionIncluded, const std::pair<int, int>& versionNumbersArray, const std::string& versionSuffix = "");

    ExtensionXmlVersion(const ExtensionXmlVersion&) = default;

    ExtensionXmlVersion(ExtensionXmlVersion&&) = delete;

    ~ExtensionXmlVersion() = default;

    /**
     * Compare the network version to minIidmVersionIncluded and maxIidmVersionExcluded (if defined)
     * 
     * @return true if network version is inside these limits
     */
    bool supports(const IidmXmlVersion& networkVersion) const;

    const std::string& getNamespaceUri() const;
    const std::string& getNamespacePrefix() const;
    const std::string& getSerializationName() const;

    const IidmXmlVersion& getMinIidmVersionIncluded() const;
    stdcxx::CReference<IidmXmlVersion> getMaxIidmVersionExcluded() const;

    /**
     * Get a string representation of this Extension version
     */
    const std::string& toString() const;



    /**
     * Compare this version to another Extension version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is before the other version, false otherwise
     */
    bool operator<(const ExtensionXmlVersion& version) const;

    /**
     * Compare this version to another Extension version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is before or equal to the other version, false otherwise
     */
    bool operator<=(const ExtensionXmlVersion& version) const;

    /**
     * Compare this version to another Extension version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is after the other version, false otherwise
     */
    bool operator>(const ExtensionXmlVersion& version) const;

    /**
     * Compare this version to another Extension version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is after or equal to the other version, false otherwise
     */
    bool operator>=(const ExtensionXmlVersion& version) const;

    /**
     * Compare this version to another Extension version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is equal to the other version, false otherwise
     */
    bool operator==(const ExtensionXmlVersion& version) const;

private:
    int compareTo(const ExtensionXmlVersion& version) const;

private:
    std::string m_namespaceUri;
    std::string m_namespacePrefix;
    std::string m_serializationName;
    const IidmXmlVersion& m_minIidmVersionIncluded;
    stdcxx::CReference<IidmXmlVersion> m_maxIidmVersionExcluded;

    std::pair<int, int> m_versionNumbersArray;
    std::string m_versionSuffix;

    std::string m_extensionVersionName;
};

using ExtensionXmlVersions = std::list<ExtensionXmlVersion>;

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLVERSION_HPP
