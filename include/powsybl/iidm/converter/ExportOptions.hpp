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
#include <powsybl/iidm/converter/AbstractOptions.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/xml/XmlEncoding.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class ExportOptions : public AbstractOptions<ExportOptions> {
public:
    static constexpr const char* const ANONYMISED = "iidm.export.xml.anonymised";
    static constexpr const char* const EXTENSIONS_INCLUDED_LIST = "iidm.export.xml.included.extensions";
    static constexpr const char* const EXTENSIONS_EXCLUDED_LIST = "iidm.export.xml.excluded.extensions";
    static constexpr const char* const IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR = "iidm.export.xml.iidm-version-incompatibility-behavior";
    static constexpr const char* const BUS_BRANCH_VOLTAGE_LEVEL_INCOMPATIBILITY_BEHAVIOR = "iidm.export.xml.bus-branch.voltage-level.incompatibility-behavior";
    static constexpr const char* const INDENT = "iidm.export.xml.indent";
    static constexpr const char* const ONLY_MAIN_CC = "iidm.export.xml.only-main-cc";
    static constexpr const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.export.xml.throw-exception-if-extension-not-found";
    static constexpr const char* const TOPOLOGY_LEVEL = "iidm.export.xml.topology-level";
    static constexpr const char* const VERSION = "iidm.export.xml.version";
    static constexpr const char* const WITH_BRANCH_STATE_VARIABLES = "iidm.export.xml.with-branch-state-variables";
    static constexpr const char* const WITH_AUTOMATION_SYSTEMS = "iidm.export.xml.with-automation-systems";

    static constexpr const char* const VOLTAGE_LEVELS_NODE_BREAKER = "iidm.export.xml.topology-level.voltage-levels.node-breaker";
    static constexpr const char* const VOLTAGE_LEVELS_BUS_BREAKER = "iidm.export.xml.topology-level.voltage-levels.bus-breaker";
    static constexpr const char* const VOLTAGE_LEVELS_BUS_BRANCH = "iidm.export.xml.topology-level.voltage-levels.bus-branch";

    enum class IidmVersionIncompatibilityBehavior : unsigned char {
        THROW_EXCEPTION,
        LOG_ERROR
    };

    enum class BusBranchVoltageLevelIncompatibilityBehavior : unsigned char {
        THROW_EXCEPTION,
        KEEP_ORIGINAL_TOPOLOGY
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
     * Add a specific topology level to be used on a voltage.
     * That TopologyLevel should be "simpler" than the VoltageLevel's modeled topology.
     *
     * @param voltageLevelId The id of the VoltageLevel
     * @param topologyLevel The TopologyLevel in which that VoltageLevel should be exported
     */
    ExportOptions& addVoltageLevelTopologyLevel(const std::string& voltageLevelId, const TopologyLevel& topologyLevel);

    /**
     * Return the expected TopologyLevel for the export of the given VoltageLevel
     */
    stdcxx::optional<TopologyLevel> getVoltageLevelTopologyLevel(const std::string& voltageLevelId) const;

    /**
     * Return the expected behaviour for incompatible BUS_BRANCH export of VoltageLevel
     */
    const BusBranchVoltageLevelIncompatibilityBehavior& getBusBranchVoltageLevelIncompatibilityBehavior() const;

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
     * Return true if the state variables must be exported for branches.
     *
     * @return true if the state variables must be exported, false otherwise
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
     * Set the expected behaviour when exporting a VoltageLevel in BUS_BRANCH would result in non-readable iidm (for instance if all switches are open or references to a non-exported bus)
     *
     * @param behavior the expected behaviour
     *
     * @return this ExportOptions object
     */
    ExportOptions& setBusBranchVoltageLevelIncompatibilityBehavior(const BusBranchVoltageLevelIncompatibilityBehavior& behavior);

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

    const std::string& getXmlEncoding() const;
    void setXmlEncoding(const std::string& encoding);

private:
    void addExtensionVersions(const stdcxx::Properties& parameters);
    void addTopologyLevelVoltageLevels(const stdcxx::Properties& parameters);

    bool m_anonymized = false;

    bool m_indent = true;

    bool m_onlyMainCc = false;

    TopologyLevel m_topologyLevel = TopologyLevel::NODE_BREAKER;

    std::map<std::string, TopologyLevel> m_voltageLevelTopologyLevels;

    bool m_withBranchSV = true;

    std::map<std::string, std::string> m_extensionsVersions;

    std::string m_version;

    IidmVersionIncompatibilityBehavior m_iidmVersionIncompatibilityBehavior = IidmVersionIncompatibilityBehavior::THROW_EXCEPTION;

    std::string m_encoding = powsybl::xml::DEFAULT_ENCODING;

    BusBranchVoltageLevelIncompatibilityBehavior m_busBranchVoltageLevelIncompatibilityBehavior = BusBranchVoltageLevelIncompatibilityBehavior::THROW_EXCEPTION;

};

std::ostream& operator<<(std::ostream& stream, const ExportOptions::IidmVersionIncompatibilityBehavior& value);
std::ostream& operator<<(std::ostream& stream, const ExportOptions::BusBranchVoltageLevelIncompatibilityBehavior& value);

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

