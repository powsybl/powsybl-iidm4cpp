/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ExportOptions.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/set.hpp>

#include "ConversionParameters.hpp"
#include "Parameter.hpp"

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<converter::ExportOptions::IidmVersionIncompatibilityBehavior>() {
    static std::initializer_list<std::string> s_names {
        "THROW_EXCEPTION",
        "LOG_ERROR"
    };
    return s_names;
}

}  // namespace Enum

namespace converter {

static const Parameter EXPORT_ANONYMISED_PARAMETER(ExportOptions::ANONYMISED, Parameter::Type::BOOLEAN, "Anonymize exported network", "false");
static const Parameter EXPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER(ExportOptions::EXTENSIONS_INCLUDED_LIST, Parameter::Type::STRING_LIST, "The list of exported extensions", "");
static const Parameter EXPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER(ExportOptions::EXTENSIONS_EXCLUDED_LIST, Parameter::Type::STRING_LIST, "The list of extensions that will be excluded during export", "");
static const Parameter EXPORT_IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR_PARAMETER (ExportOptions::IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR, Parameter::Type::STRING, "Behavior when there is an IIDM version incompatibility", "THROW_EXCEPTION");
static const Parameter EXPORT_INDENT_PARAMETER(ExportOptions::INDENT, Parameter::Type::BOOLEAN, "Indent export output file", "true");
static const Parameter EXPORT_ONLY_MAIN_CC_PARAMETER(ExportOptions::ONLY_MAIN_CC, Parameter::Type::BOOLEAN, "Export only main CC", "false");
static const Parameter EXPORT_THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER = Parameter(ExportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, Parameter::Type::BOOLEAN, "Throw exception if extension not found", "false").addAdditionalNames({"throwExceptionIfExtensionNotFound"});
static const Parameter EXPORT_TOPOLOGY_LEVEL_PARAMETER(ExportOptions::TOPOLOGY_LEVEL, Parameter::Type::STRING, "Export network in this topology level", "NODE_BREAKER");
static const Parameter EXPORT_VERSION_PARAMETER(ExportOptions::VERSION, Parameter::Type::STRING, "IIDM-XML version in which files will be generated", xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION().toString("."));
static const Parameter EXPORT_WITH_BRANCH_STATE_VARIABLES_PARAMETER(ExportOptions::WITH_BRANCH_STATE_VARIABLES, Parameter::Type::BOOLEAN, "Export network with branch state variables", "true");
static const Parameter EXPORT_WITH_AUTOMATION_SYSTEMS_PARAMETER(ExportOptions::WITH_AUTOMATION_SYSTEMS, Parameter::Type::BOOLEAN, "Export network with automation systems", "true");
static const Parameter EXPORT_VOLTAGE_LEVELS_NODEBREAKER_PARAMETER(ExportOptions::VOLTAGE_LEVELS_NODE_BREAKER, Parameter::Type::STRING_LIST, "Apply Node/Breaker topology level at export for listed voltage levels", "");
static const Parameter EXPORT_VOLTAGE_LEVELS_BUSBREAKER_PARAMETER(ExportOptions::VOLTAGE_LEVELS_BUS_BREAKER, Parameter::Type::STRING_LIST, "Apply Bus/Breaker topology level at export for listed voltage levels", "");
static const Parameter EXPORT_VOLTAGE_LEVELS_BUSBRANCH_PARAMETER(ExportOptions::VOLTAGE_LEVELS_BUS_BRANCH, Parameter::Type::STRING_LIST, "Apply Bus/Branch topology level at export for listed voltage levels", "");


std::ostream& operator<<(std::ostream& stream, const ExportOptions::IidmVersionIncompatibilityBehavior& value) {
    stream << iidm::Enum::toString(value);
    return stream;
}

ExportOptions::ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                             bool throwExceptionIfExtensionNotFound, const std::string& version,
                             const IidmVersionIncompatibilityBehavior& iidmVersionIncompatibilityBehavior) :
    AbstractOptions(throwExceptionIfExtensionNotFound),
    m_indent(indent),
    m_onlyMainCc(onlyMainCc),
    m_topologyLevel(topologyLevel),
    m_withBranchSV(withBranchSV),
    m_version(version),
    m_iidmVersionIncompatibilityBehavior(iidmVersionIncompatibilityBehavior) {
}

ExportOptions::ExportOptions(const stdcxx::Properties& parameters) :
    m_anonymized(ConversionParameters::readBooleanParameter(parameters, EXPORT_ANONYMISED_PARAMETER)),
    m_indent(ConversionParameters::readBooleanParameter(parameters, EXPORT_INDENT_PARAMETER)),
    m_onlyMainCc(ConversionParameters::readBooleanParameter(parameters, EXPORT_ONLY_MAIN_CC_PARAMETER)),
    m_topologyLevel(Enum::fromString<TopologyLevel>(ConversionParameters::readStringParameter(parameters, EXPORT_TOPOLOGY_LEVEL_PARAMETER))),
    m_withBranchSV(ConversionParameters::readBooleanParameter(parameters, EXPORT_WITH_BRANCH_STATE_VARIABLES_PARAMETER)),
    m_version(ConversionParameters::readStringParameter(parameters, EXPORT_VERSION_PARAMETER)),
    m_iidmVersionIncompatibilityBehavior(Enum::fromString<IidmVersionIncompatibilityBehavior>(ConversionParameters::readStringParameter(parameters, EXPORT_IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR_PARAMETER))) {
        setThrowExceptionIfExtensionNotFound(ConversionParameters::readBooleanParameter(parameters, EXPORT_THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER));
        setWithAutomationSystems(ConversionParameters::readBooleanParameter(parameters, EXPORT_WITH_AUTOMATION_SYSTEMS_PARAMETER));

        bool someExtensionsDefined = checkAndAddExtensions(ConversionParameters::containsParameter(parameters, EXPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER),
                                                           stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER)),
                                                           ConversionParameters::containsParameter(parameters, EXPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER),
                                                           stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER)),
                                                           true);
        if(someExtensionsDefined) {
            addExtensionVersions(parameters);
        }
        
        addTopologyLevelVoltageLevels(parameters);
}

void ExportOptions::addExtensionVersions(const stdcxx::Properties& parameters) {

    powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
    for (const auto& extensionProvider : extensionProviders.getProviders()) {
        std::string extensionName = extensionProvider.getExtensionName();
        Parameter parameter = Parameter("iidm.export.xml." + extensionName + ".version", Parameter::Type::STRING, "Version of " + extensionName, "");
        std::string extensionVersion = ConversionParameters::readStringParameter(parameters, parameter);
        if (!extensionVersion.empty()) {
            if (m_includedExtensions.has_value() && m_includedExtensions->count(extensionName) && 
               m_excludedExtensions.has_value() && !m_excludedExtensions->count(extensionName)) {
                addExtensionVersion(extensionName, extensionVersion);
            } else {
                logging::Logger& logger = logging::LoggerFactory::getLogger<ExportOptions>();
                logger.warn(stdcxx::format("Version (%1%) of %2% is ignored since %2% is not in the extensions list to export.", extensionVersion, extensionName));
            }
            
        }
    }

}

void ExportOptions::addTopologyLevelVoltageLevels(const stdcxx::Properties& parameters) {
    std::set<std::string> nodeBreakerVLs = stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXPORT_VOLTAGE_LEVELS_NODEBREAKER_PARAMETER));
    std::set<std::string> busBreakerVLs = stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXPORT_VOLTAGE_LEVELS_BUSBREAKER_PARAMETER));
    std::set<std::string> busBranchVLs = stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXPORT_VOLTAGE_LEVELS_BUSBRANCH_PARAMETER));

    std::set<std::string> allVLIds;
    allVLIds.insert(nodeBreakerVLs.begin(), nodeBreakerVLs.end());
    allVLIds.insert(busBreakerVLs.begin(), busBreakerVLs.end());
    allVLIds.insert(busBranchVLs.begin(), busBranchVLs.end());

    for (const auto& vlID : allVLIds) {
        unsigned int foundVL = 0;
        TopologyLevel topology;

        if(nodeBreakerVLs.count(vlID)) {
            foundVL++;
            topology = TopologyLevel::NODE_BREAKER;
        }
        if(busBreakerVLs.count(vlID)) {
            foundVL++;
            topology = TopologyLevel::BUS_BREAKER;
        }
        if(busBranchVLs.count(vlID)) {
            foundVL++;
            topology = TopologyLevel::BUS_BRANCH;
        }

        if(foundVL == 1) {
            addVoltageLevelTopologyLevel(vlID, topology);
        } else if(foundVL > 1) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<ExportOptions>();
            logger.warn(stdcxx::format("VoltageLevel %1% is associated with different topology levels in property => ignored", vlID));
        } // else (foundVL == 0) should not be possible, and in that case we would do nothing.
    }
}



ExportOptions& ExportOptions::addExtensionVersion(const std::string& extensionName, const std::string& extensionVersion) {
    if (m_includedExtensions.has_value() && !m_includedExtensions->count(extensionName)) {
        throw PowsyblException(stdcxx::format("%1% is not an extension you have included in the extensions inclusion list to export.", extensionName));
    }
    if (m_excludedExtensions.has_value() && m_excludedExtensions->count(extensionName)) {
        throw PowsyblException(stdcxx::format("%1% is an extension you have excluded in the extensions exclusion list to export.", extensionName));
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

ExportOptions& ExportOptions::addVoltageLevelTopologyLevel(const std::string& voltageLevelId, const TopologyLevel& topologyLevel) {
    if(!voltageLevelId.empty()) {
        const auto& it = m_voltageLevelTopologyLevels.find(voltageLevelId);
        if(it != m_voltageLevelTopologyLevels.end()) {
            //Erase previous value, so we can override
            m_voltageLevelTopologyLevels.erase(it);
        }

        m_voltageLevelTopologyLevels.insert(std::make_pair(voltageLevelId, topologyLevel));
    }

    return *this;
}

stdcxx::optional<TopologyLevel> ExportOptions::getVoltageLevelTopologyLevel(const std::string& voltageLevelId) const {
    const auto& it = m_voltageLevelTopologyLevels.find(voltageLevelId);

    return it == m_voltageLevelTopologyLevels.end() ? stdcxx::optional<TopologyLevel>() : stdcxx::optional<TopologyLevel>(it->second);
}

const ExportOptions::IidmVersionIncompatibilityBehavior& ExportOptions::getIidmVersionIncompatibilityBehavior() const {
    return m_iidmVersionIncompatibilityBehavior;
}

const TopologyLevel& ExportOptions::getTopologyLevel() const {
    return m_topologyLevel;
}

const std::string& ExportOptions::getVersion() const {
    return m_version;
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

bool ExportOptions::isWithBranchSV() const {
    return m_withBranchSV;
}

ExportOptions& ExportOptions::setAnonymized(bool anonymized) {
    m_anonymized = anonymized;
    return *this;
}

ExportOptions& ExportOptions::setIidmVersionIncompatibilityBehavior(const IidmVersionIncompatibilityBehavior& iidmVersionIncompatibilityBehavior) {
    m_iidmVersionIncompatibilityBehavior = iidmVersionIncompatibilityBehavior;
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

const std::string& ExportOptions::getXmlEncoding() const {
    return m_encoding;
}

void ExportOptions::setXmlEncoding(const std::string& encoding) {
    m_encoding = encoding;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
