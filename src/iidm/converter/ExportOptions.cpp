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
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
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

static const Parameter ANONYMISED_PARAMETER(ExportOptions::ANONYMISED, Parameter::Type::BOOLEAN, "Anonymize exported network", "false");
static const Parameter EXTENSIONS_LIST_PARAMETER(ExportOptions::EXTENSIONS_LIST, Parameter::Type::STRING_LIST, "The list of exported extensions", "");
static const Parameter IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR_PARAMETER (ExportOptions::IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR, Parameter::Type::STRING, "Behavior when there is an IIDM version incompatibility", "THROW_EXCEPTION");
static const Parameter INDENT_PARAMETER(ExportOptions::INDENT, Parameter::Type::BOOLEAN, "Indent export output file", "true");
static const Parameter ONLY_MAIN_CC_PARAMETER(ExportOptions::ONLY_MAIN_CC, Parameter::Type::BOOLEAN, "Export only main CC", "false");
static const Parameter THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER = Parameter(ExportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, Parameter::Type::BOOLEAN, "Throw exception if extension not found", "false").addAdditionalNames({"throwExceptionIfExtensionNotFound"});
static const Parameter TOPOLOGY_LEVEL_PARAMETER(ExportOptions::TOPOLOGY_LEVEL, Parameter::Type::STRING, "Export network in this topology level", "NODE_BREAKER");
static const Parameter VERSION_PARAMETER(ExportOptions::VERSION, Parameter::Type::STRING, "IIDM-XML version in which files will be generated", xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION().toString("."));
static const Parameter WITH_BRANCH_STATE_VARIABLES_PARAMETER(ExportOptions::WITH_BRANCH_STATE_VARIABLES, Parameter::Type::BOOLEAN, "Export network with branch state variables", "true");

std::ostream& operator<<(std::ostream& stream, const ExportOptions::IidmVersionIncompatibilityBehavior& value) {
    stream << iidm::Enum::toString(value);
    return stream;
}

ExportOptions::ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                             bool throwExceptionIfExtensionNotFound, const std::string& version,
                             const IidmVersionIncompatibilityBehavior& iidmVersionIncompatibilityBehavior) :
    m_indent(indent),
    m_onlyMainCc(onlyMainCc),
    m_throwExceptionIfExtensionNotFound(throwExceptionIfExtensionNotFound),
    m_topologyLevel(topologyLevel),
    m_withBranchSV(withBranchSV),
    m_version(version),
    m_iidmVersionIncompatibilityBehavior(iidmVersionIncompatibilityBehavior) {
}

ExportOptions::ExportOptions(const stdcxx::Properties& parameters) :
    m_anonymized(ConversionParameters::readBooleanParameter(parameters, ANONYMISED_PARAMETER)),
    m_indent(ConversionParameters::readBooleanParameter(parameters, INDENT_PARAMETER)),
    m_onlyMainCc(ConversionParameters::readBooleanParameter(parameters, ONLY_MAIN_CC_PARAMETER)),
    m_throwExceptionIfExtensionNotFound(ConversionParameters::readBooleanParameter(parameters, THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER)),
    m_topologyLevel(Enum::fromString<TopologyLevel>(ConversionParameters::readStringParameter(parameters, TOPOLOGY_LEVEL_PARAMETER))),
    m_withBranchSV(ConversionParameters::readBooleanParameter(parameters, WITH_BRANCH_STATE_VARIABLES_PARAMETER)),
    m_extensions(stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, EXTENSIONS_LIST_PARAMETER))),
    m_version(ConversionParameters::readStringParameter(parameters, VERSION_PARAMETER)),
    m_iidmVersionIncompatibilityBehavior(Enum::fromString<IidmVersionIncompatibilityBehavior>(ConversionParameters::readStringParameter(parameters, IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR_PARAMETER))) {
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

const ExportOptions::IidmVersionIncompatibilityBehavior& ExportOptions::getIidmVersionIncompatibilityBehavior() const {
    return m_iidmVersionIncompatibilityBehavior;
}

const TopologyLevel& ExportOptions::getTopologyLevel() const {
    return m_topologyLevel;
}

const std::string& ExportOptions::getVersion() const {
    return m_version;
}

bool ExportOptions::hasAtLeastOneExtension(const stdcxx::const_range<Extension>& extensions) const {
    if (m_extensions.empty()) {
        return true;
    }
    return (std::any_of(extensions.begin(), extensions.end(), [=](const Extension& extension) { 
                return m_extensions.find(extension.getName()) != m_extensions.end(); 
        } ));
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

ExportOptions& ExportOptions::setAnonymized(bool anonymized) {
    m_anonymized = anonymized;
    return *this;
}

ExportOptions& ExportOptions::setExtensions(const std::set<std::string>& extensions) {
    m_extensions = extensions;
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

const std::string& ExportOptions::getXmlEncoding() const {
    return m_encoding;
}

void ExportOptions::setXmlEncoding(const std::string& encoding) {
    m_encoding = encoding;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
