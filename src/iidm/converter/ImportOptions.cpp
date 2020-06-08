/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ImportOptions.hpp>

#include <powsybl/iidm/converter/ConversionParameters.hpp>
#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/iidm/converter/xml/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

static const converter::Parameter EXTENSIONS_LIST_PARAMETER(converter::xml::EXTENSIONS_LIST, converter::Parameter::Type::STRING_LIST, "The list of exported extensions", "");
static const converter::Parameter THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER = std::move(converter::Parameter(converter::xml::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, converter::Parameter::Type::BOOLEAN, "Throw exception if extension not found", "false").addAdditionalNames({"throwExceptionIfExtensionNotFound"}));

ImportOptions::ImportOptions(const stdcxx::Properties& parameters) :
    m_throwExceptionIfExtensionNotFound(converter::ConversionParameters::readBooleanParameter(parameters, THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER)) {
    const std::vector<std::string>& extensionVector = converter::ConversionParameters::readStringListParameter(parameters, EXTENSIONS_LIST_PARAMETER);
    std::set<std::string> extensionSet(std::make_move_iterator(extensionVector.begin()), std::make_move_iterator(extensionVector.end()));
    m_extensions = std::move(extensionSet);
}

ImportOptions& ImportOptions::addExtension(const std::string& extension) {
    m_extensions.insert(extension);
    return *this;
}

bool ImportOptions::isThrowExceptionIfExtensionNotFound() const {
    return m_throwExceptionIfExtensionNotFound;
}

ImportOptions& ImportOptions::setExtensions(const std::set<std::string>& extensions) {
    m_extensions = extensions;
    return *this;
}

ImportOptions& ImportOptions::setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound) {
    m_throwExceptionIfExtensionNotFound = throwExceptionIfExtensionNotFound;
    return *this;
}

bool ImportOptions::withExtension(const std::string& extension) const {
    return m_extensions.empty() || m_extensions.find(extension) != m_extensions.end();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
