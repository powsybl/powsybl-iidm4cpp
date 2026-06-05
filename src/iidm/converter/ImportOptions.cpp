/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ImportOptions.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/set.hpp>

#include "ConversionParameters.hpp"
#include "Parameter.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

static const Parameter IMPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER(ImportOptions::EXTENSIONS_INCLUDED_LIST, converter::Parameter::Type::STRING_LIST, "The list of extensions to be imported", "");
static const Parameter IMPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER(ImportOptions::EXTENSIONS_EXCLUDED_LIST, converter::Parameter::Type::STRING_LIST, "The list of extensions that will be excluded and not imported", "");
static const Parameter IMPORT_THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER = Parameter(ImportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, converter::Parameter::Type::BOOLEAN, "Throw exception if extension not found", "false")
    .addAdditionalNames({"throwExceptionIfExtensionNotFound"});
static const Parameter IMPORT_WITH_AUTOMATION_SYSTEMS_PARAMETER = Parameter(ImportOptions::WITH_AUTOMATION_SYSTEMS, converter::Parameter::Type::BOOLEAN, 
    "Import network with automation systems", "true");
static const Parameter IMPORT_MISSING_PERMANENT_LIMIT_PERCENTAGE_PARAMETER = Parameter(ImportOptions::MISSING_PERMANENT_LIMIT_PERCENTAGE, converter::Parameter::Type::DOUBLE, 
    "Percentage applied to lowest temporary limit to compute the permanent limit when missing (for IIDM < 1.12 only)", "100.0");
static const Parameter IMPORT_MINIMAL_VALIDATION_LEVEL_PARAMETER = Parameter(ImportOptions::MINIMAL_VALIDATION_LEVEL, converter::Parameter::Type::STRING, "Minimal validation level accepted", "");


ImportOptions::ImportOptions(const stdcxx::Properties& parameters) : 
    m_missingPermanentLimitPercentage(ConversionParameters::readDoubleParameter(parameters, IMPORT_MISSING_PERMANENT_LIMIT_PERCENTAGE_PARAMETER)) {
        setThrowExceptionIfExtensionNotFound(ConversionParameters::readBooleanParameter(parameters, IMPORT_THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND_PARAMETER));
        setWithAutomationSystems(ConversionParameters::readBooleanParameter(parameters, IMPORT_WITH_AUTOMATION_SYSTEMS_PARAMETER));

        checkAndAddExtensions(ConversionParameters::containsParameter(parameters, IMPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER),
                              stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, IMPORT_EXTENSIONS_INCLUDED_LIST_PARAMETER)),
                              ConversionParameters::containsParameter(parameters, IMPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER),
                              stdcxx::toSet(ConversionParameters::readStringListParameter(parameters, IMPORT_EXTENSIONS_EXCLUDED_LIST_PARAMETER)),
                              false);

        setMinimalValidationLevel(ConversionParameters::readStringParameter(parameters, IMPORT_MINIMAL_VALIDATION_LEVEL_PARAMETER));
}

double ImportOptions::getMissingPermanentLimitPercentage() const {
    return m_missingPermanentLimitPercentage;
}

const stdcxx::optional<ValidationLevel>& ImportOptions::getMinimalValidationLevel() const {
    return m_minimalValidationLevel;
}

ImportOptions& ImportOptions::setMissingPermanentLimitPercentage(double missingPermanentLimitPercentage) {
    m_missingPermanentLimitPercentage = missingPermanentLimitPercentage;
    return *this;
}

ImportOptions& ImportOptions::setMinimalValidationLevel(const std::string& minimalValidationLevel) {
    if(!minimalValidationLevel.empty()) {
        m_minimalValidationLevel = Enum::fromString<ValidationLevel>(minimalValidationLevel);
    }
    return *this;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
