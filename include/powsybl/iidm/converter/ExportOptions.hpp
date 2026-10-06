/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

#include <map>
#include <set>
#include <string>

#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/xml/XmlEncoding.hpp>

namespace powsybl {

namespace iidm {

class Extension;

namespace converter {

class ExportOptions {
public:
    static constexpr const char* const ANONYMISED = "iidm.export.xml.anonymised";
    static constexpr const char* const EXTENSIONS_LIST = "iidm.export.xml.extensions";
    static constexpr const char* const IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR = "iidm.export.xml.iidm-version-incompatibility-behavior";
    static constexpr const char* const INDENT = "iidm.export.xml.indent";
    static constexpr const char* const ONLY_MAIN_CC = "iidm.export.xml.only-main-cc";
    static constexpr const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.export.xml.throw-exception-if-extension-not-found";
    static constexpr const char* const TOPOLOGY_LEVEL = "iidm.export.xml.topology-level";
    static constexpr const char* const VERSION = "iidm.export.xml.version";
    static constexpr const char* const WITH_BRANCH_STATE_VARIABLES = "iidm.export.xml.with-branch-state-variables";

    enum class IidmVersionIncompatibilityBehavior : unsigned char {
        THROW_EXCEPTION,
        LOG_ERROR
    };

public:
    /**
     * Default constructor
     */
    ExportOptions() = default;

    /**
     * Constructor
     *
     * @param withBranchSV
     * @param indent The indentation status
     * @param onlyMainCc The connected components status
     * @param topologyLevel The maximum topology level to use
     * @param throwExceptionIfExtensionNotFound The extension's serializer lookup mode
     * @param version The expected XIIDM version
     * @param iidmVersionIncompatibilityBehavior The expected behaviour when an IIDM's version incompatibility occurs
     */
    ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                  bool throwExceptionIfExtensionNotFound, const std::string& version,
                  const IidmVersionIncompatibilityBehavior& iidmVersionIncompatibilityBehavior);

    /**
     * Constructor
     *
     * @param parameters import parameters
     */
    explicit ExportOptions(const stdcxx::Properties& parameters);

    /**
     * Add an extension to the list of exported extensions.
     *
     * @param extension The name of the extension to add
     *
     * @return this ExportOptions object
     */
    ExportOptions& addExtension(const std::string& extension);

    /**
     * Add a given version in which the extension with the given name will be exported if this version is supported by
     * the extension's XML serializer and if it is compatible with the IIDM version in which the network will be exported.
     *
     * If the version is not added for an extension configured to be serialized, the extension will be serialized in the
     * most recent version compatible with the IIDM version in which the network will be exported.
     *
     * If a version is added for an extension configured <b>not</b> to be serialized, the version will be ignored.
     *
     * If a version has already been added for the extension, throw an exception.
     *
     * @param extensionName The name of the extension
     * @param extensionVersion The version in which the extension should be exported
     *
     * @return this ExportOptions object
     */
    ExportOptions& addExtensionVersion(const std::string& extensionName, const std::string& extensionVersion);

    /**
     * Return the expected version for the given extension
     *
     * @param extensionName The name of the extension
     *
     * @return the version in which the extension should be exported or an empty string if the extension is not found
     */
    const std::string& getExtensionVersion(const std::string& extensionName) const;

    /**
     * Return the expecting behaviour if an IIDM's version incompatibility occurs
     *
     * @return the expecting behaviour if an IIDM's version incompatibility occurs
     */
    const IidmVersionIncompatibilityBehavior& getIidmVersionIncompatibilityBehavior() const;

    /**
     * Return the maximal topology level to use
     *
     * The TopologyLevel is used to decrease the level of detail of a voltage level topology.
     *
     * @return the maximal TopologyLevel to use
     */
    const TopologyLevel& getTopologyLevel() const;

    /**
     * Return the XIIDM version to use
     *
     * @return the XIIDM version
     */
    const std::string& getVersion() const;

    /**
     * Return true if one (or more) extension of the given list should be exported
     *
     * @param extensions the list of extensions to be checked.
     *
     * @return true if at least one extension should be exported, false otherwise
     */
    bool hasAtLeastOneExtension(const stdcxx::const_range<Extension>& extensions) const;

    /**
     * Return true if the identifier should be anonymized
     *
     * @return true if the identifier should be anonymized, false otherwise
     */
    bool isAnonymized() const;

    /**
     * Return true if the XML should be indented
     *
     * @return true if the XML should be indented, false otherwise
     */
    bool isIndent() const;

    /**
     * Return true if the main connected component only should be exported. If false, all the connected components are exported.
     *
     * @return true if the main connected component only should be exported, false otherwise.
     */
    bool isOnlyMainCc() const;

    /**
     * Return true if an exception should be thrown if an XML serializer for an extension is not found.
     *
     * @return true if an exception should be thrown, false otherwise
     */
    bool isThrowExceptionIfExtensionNotFound() const;

    /**
     * Return true if the state variables must be exported for branches.
     *
     * @return true if te state variables must be exported, false otherwise
     */
    bool isWithBranchSV() const;

    /**
     * Enable/Disable the anonymization of the identifiers.
     *
     * @param anonymized the anonymization status
     *
     * @return this ExportOptions object
     */
    ExportOptions& setAnonymized(bool anonymized);

    /**
     * Set the list of exported extensions.
     *
     * @param extensions The exported extension list
     *
     * @return this ExportOptions object
     */
    ExportOptions& setExtensions(const std::set<std::string>& extensions);

    /**
     * Set the expected behaviour if an IIDM's version incompatibility occurs
     *
     * @param iidmVersionIncompatibilityBehavior The behaviour if an IIDM's version incompatibility occurs
     *
     * @return this ExportOptions object
     */
    ExportOptions& setIidmVersionIncompatibilityBehavior(const IidmVersionIncompatibilityBehavior& iidmVersionIncompatibilityBehavior);

    /**
     * Enable/disable the indentation of the XML
     *
     * @param indent the indentation status
     *
     * @return this ExportOptions object
     */
    ExportOptions& setIndent(bool indent);

    /**
     * Enable/disable the export of all connected components
     *
     * @param onlyMainCc the component export status
     *
     * @return this ExportOptions obejct
     */
    ExportOptions& setOnlyMainCc(bool onlyMainCc);

    /**
     * Enable/Disable the lookup of extension's serializers permissive mode.
     *
     * @param throwExceptionIfExtensionNotFound The extension's serializer lookup status
     *
     * @return This ExportOptions object
     */
    ExportOptions& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

    /**
     * Set the maximum topology level, to decrease the level of details of the exported topology
     *
     * @param topologyLevel The maximum TopologyLevel to use for the export
     *
     * @return this ExportOptions object
     */
    ExportOptions& setTopologyLevel(const TopologyLevel& topologyLevel);

    /**
     * Set the version of XIIDM that should be used
     *
     * @param version the version of the exported XIIDM
     *
     * @return this ExportOptions object
     */
    ExportOptions& setVersion(const std::string& version);

    /**
     * Enable/Disable the export of the state variables for branches
     *
     * @param withBranchSV Set to true to export the state variables
     *
     * @return this ExportOptions object
     */
    ExportOptions& setWithBranchSV(bool withBranchSV);

    /**
     * Return true if the given extension should be exported.
     *
     * @param extension The name of the extension
     *
     * @return true if the given extension should be exported, false otherwise
     */
    bool withExtension(const std::string& extension) const;

    const std::string& getXmlEncoding() const;
    void setXmlEncoding(const std::string& encoding);

private:
    bool m_anonymized = false;

    bool m_indent = true;

    bool m_onlyMainCc = false;

    bool m_throwExceptionIfExtensionNotFound = false;

    TopologyLevel m_topologyLevel = TopologyLevel::NODE_BREAKER;

    bool m_withBranchSV = true;

    std::set<std::string> m_extensions;

    std::map<std::string, std::string> m_extensionsVersions;

    std::string m_version;

    IidmVersionIncompatibilityBehavior m_iidmVersionIncompatibilityBehavior = IidmVersionIncompatibilityBehavior::THROW_EXCEPTION;

    std::string m_encoding = powsybl::xml::DEFAULT_ENCODING;
};

std::ostream& operator<<(std::ostream& stream, const ExportOptions::IidmVersionIncompatibilityBehavior& value);

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

