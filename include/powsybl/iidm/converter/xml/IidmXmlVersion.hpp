/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_IIDMXMLVERSION_HPP
#define POWSYBL_IIDM_CONVERTER_XML_IIDMXMLVERSION_HPP

#include <functional>
#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class IidmXmlVersion {
public:
    static const std::vector<std::reference_wrapper<const IidmXmlVersion>>& all();

    /**
     * The current IIDM version supported by this version of the project
     */
    static const IidmXmlVersion& CURRENT_IIDM_XML_VERSION();

    /**
     * IIDM V1.0 (http://www.itesla_project.eu)
     */
    static const IidmXmlVersion& V1_0();

public:
    /**
     * Get an {@link IidmXmlVersion} instance from a namespace URI
     *
     * @param namespaceURI the namespace URI of the searched IIDM version
     *
     * @return a {@link IidmXmlVersion} instance
     *
     * @throw a {@link PowsyblException} if the namespace URI is associated to no version
     */
    static const IidmXmlVersion& fromNamespaceURI(const std::string& namespaceURI);

    /**
     * Get an {@link IidmXmlVersion} instance from its string representation
     *
     * @param version a string representation of an IIDM version
     * @param separator the separator used to parse the version
     *
     * @return a {@link IidmXmlVersion} instance
     *
     * @throw a {@link PowsyblException} if the version is not supported
     */
    static const IidmXmlVersion& of(const std::string& version, const std::string& separator);

public:
    IidmXmlVersion(const IidmXmlVersion& version) = delete;

    IidmXmlVersion(IidmXmlVersion&& version) = delete;

    ~IidmXmlVersion() = default;

    /**
     * Compare this version to another IIDM version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is before the other version, false otherwise
     */
    bool operator<(const IidmXmlVersion& version) const;

    /**
     * Compare this version to another IIDM version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is before or equal to the other version, false otherwise
     */
    bool operator<=(const IidmXmlVersion& version) const;

    /**
     * Compare this version to another IIDM version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is after the other version, false otherwise
     */
    bool operator>(const IidmXmlVersion& version) const;

    /**
     * Compare this version to another IIDM version
     *
     * @param version the version to compare to this version
     *
     * @return true if this version is after or equal to the other version, false otherwise
     */
    bool operator>=(const IidmXmlVersion& version) const;

    /**
     * Get the namespace URI associated to this IIDM version
     *
     * @return the namespace URI of this version
     */
    std::string getNamespaceUri() const;

    /**
     * Get the name of the XSD file for this IIDM version
     *
     * @return the XSD file name of this version
     */
    std::string getXsd() const;

    /**
     * Get a string representation of this IIDM version
     *
     * @param separator the separator used during the concatenation
     *
     * @return a string represation of this version
     */
    std::string toString(const std::string& separator) const;

private:
    IidmXmlVersion(std::string&& domain, std::vector<int>&& versionArray) noexcept;

private:
    std::string m_domain;

    std::vector<int> m_versionArray;
};

using IidmXmlVersions = std::vector<std::reference_wrapper<const IidmXmlVersion>>;

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_IIDMXMLVERSION_HPP
