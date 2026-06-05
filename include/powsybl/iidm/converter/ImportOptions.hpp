/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP

#include <set>
#include <string>

#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/iidm/converter/AbstractOptions.hpp>
#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class ImportOptions : public AbstractOptions<ImportOptions> {
public:
    static constexpr const char* const EXTENSIONS_INCLUDED_LIST = "iidm.import.xml.included.extensions";
    static constexpr const char* const EXTENSIONS_EXCLUDED_LIST = "iidm.import.xml.excluded.extensions";
    static constexpr const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.import.xml.throw-exception-if-extension-not-found";
    static constexpr const char* const WITH_AUTOMATION_SYSTEMS = "iidm.import.xml.with-automation-systems";
    static constexpr const char* const MISSING_PERMANENT_LIMIT_PERCENTAGE = "iidm.import.xml.missing-permanent-limit-percentage";
    static constexpr const char* const MINIMAL_VALIDATION_LEVEL = "iidm.import.xml.minimal-validation-level";

public:
    ImportOptions() = default;

    explicit ImportOptions(const stdcxx::Properties& parameters);

    double getMissingPermanentLimitPercentage() const;

    const stdcxx::optional<ValidationLevel>& getMinimalValidationLevel() const;

    ImportOptions& setMissingPermanentLimitPercentage(double missingPermanentLimitPercentage);

    ImportOptions& setMinimalValidationLevel(const std::string& minimalValidationLevel);

private:
    double m_missingPermanentLimitPercentage = 100.0;

    stdcxx::optional<ValidationLevel> m_minimalValidationLevel;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
