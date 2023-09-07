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
    /**
     * The current IIDM version supported by this version of the project
     */
    static const IidmXmlVersion& CURRENT_IIDM_XML_VERSION();

    /**
     * IIDM V1.0 (http://www.itesla_project.eu)
     */
    static const IidmXmlVersion& V1_0();

    /**
     * IIDM V1.1 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_1();

    /**
     * IIDM V1.2 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_2();

    /**
     * IIDM V1.3 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_3();

    /**
     * IIDM V1.4 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_4();

    /**
     * IIDM V1.5 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_5();

    /**
     * IIDM V1.6 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_6();

    /**
     * IIDM V1.7 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_7();

    /**
     * IIDM V1.8 (http://www.powsybl.org)
     */
    static const IidmXmlVersion& V1_8();

public:
    /**
     * Return the list of all supported XIIDM versions
     *
     * @return the supported versions
     */
    static const std::vector<std::reference_wrapper<const IidmXmlVersion>>& all();

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
     * Get the default XIIDM prefix
     *
     * @return the default XIIDM prefix
     */
     static const std::string& getDefaultPrefix();

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
    IidmXmlVersion(const IidmXmlVersion&) = delete;

    IidmXmlVersion(IidmXmlVersion&&) = delete;

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
     * Get the XML prefix associated to this IIDM version
     *
     * @return the XML prefix of this version
     */
    const std::string& getPrefix() const;

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
