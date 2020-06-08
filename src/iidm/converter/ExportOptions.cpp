/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ExportOptions.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/ConversionParameters.hpp>
#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/iidm/converter/xml/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

static const converter::Parameter ANONYMISED_PARAMETER(converter::xml::ANONYMISED, converter::Parameter::Type::BOOLEAN, "Anonymise exported network", "false");
static const converter::Parameter INDENT_PARAMETER(converter::xml::INDENT, converter::Parameter::Type::BOOLEAN, "Indent export output file", "true");
static const converter::Parameter EXTENSIONS_LIST_PARAMETER(converter::xml::EXTENSIONS_LIST, converter::Parameter::Type::STRING_LIST, "The list of exported extensions", "");
static const converter::Parameter ONLY_MAIN_CC_PARAMETER(converter::xml::ONLY_MAIN_CC, converter::Parameter::Type::BOOLEAN, "Export only main CC", "false");
static const converter::Parameter THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER = std::move(converter::Parameter(converter::xml::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, converter::Parameter::Type::BOOLEAN, "Throw exception if extension not found", "false").addAdditionalNames({"throwExceptionIfExtensionNotFound"}));
static const converter::Parameter TOPOLOGY_LEVEL_PARAMETER(converter::xml::TOPOLOGY_LEVEL, converter::Parameter::Type::STRING, "Export network in this topology level", "NODE_BREAKER");
static const converter::Parameter VERSION_PARAMETER(converter::xml::VERSION, converter::Parameter::Type::STRING, "IIDM-XML version in which files will be generated", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION().toString("."));
static const converter::Parameter WITH_BRANCH_STATE_VARIABLES_PARAMETER(converter::xml::WITH_BRANCH_STATE_VARIABLES, converter::Parameter::Type::BOOLEAN, "Export network with branch state variables", "true");

ExportOptions::ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                             bool throwExceptionIfExtensionNotFound, const std::string& version) :
    m_indent(indent),
    m_onlyMainCc(onlyMainCc),
    m_throwExceptionIfExtensionNotFound(throwExceptionIfExtensionNotFound),
    m_topologyLevel(topologyLevel),
    m_withBranchSV(withBranchSV),
    m_version(version) {
}

ExportOptions::ExportOptions(const stdcxx::Properties& parameters) :
    m_anonymized(converter::ConversionParameters::readBooleanParameter(parameters, ANONYMISED_PARAMETER)),
    m_indent(converter::ConversionParameters::readBooleanParameter(parameters, INDENT_PARAMETER)),
    m_onlyMainCc(converter::ConversionParameters::readBooleanParameter(parameters, ONLY_MAIN_CC_PARAMETER)),
    m_throwExceptionIfExtensionNotFound(converter::ConversionParameters::readBooleanParameter(parameters, THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER)),
    m_topologyLevel(Enum::fromString<TopologyLevel>(converter::ConversionParameters::readStringParameter(parameters, TOPOLOGY_LEVEL_PARAMETER))),
    m_withBranchSV(converter::ConversionParameters::readBooleanParameter(parameters, WITH_BRANCH_STATE_VARIABLES_PARAMETER)),
    m_version(converter::ConversionParameters::readStringParameter(parameters, VERSION_PARAMETER)) {
    const std::vector<std::string>& extensionVector = converter::ConversionParameters::readStringListParameter(parameters, EXTENSIONS_LIST_PARAMETER);
    std::set<std::string> extensionSet(std::make_move_iterator(extensionVector.begin()), std::make_move_iterator(extensionVector.end()));
    m_extensions = std::move(extensionSet);
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

ExportOptions& ExportOptions::setAnonymized(bool anonymized) {
    m_anonymized = anonymized;
    return *this;
}

ExportOptions& ExportOptions::setExtensions(const std::set<std::string>& extensions) {
    m_extensions = extensions;
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
