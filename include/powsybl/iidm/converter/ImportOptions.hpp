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

#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class ImportOptions {
public:
    static constexpr const char* const EXTENSIONS_LIST = "iidm.import.xml.extensions";
    static constexpr const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.import.xml.throw-exception-if-extension-not-found";
    static constexpr const char* const WITH_AUTOMATION_SYSTEMS = "iidm.import.xml.with-automation-systems";
    static constexpr const char* const MISSING_PERMANENT_LIMIT_PERCENTAGE = "iidm.import.xml.missing-permanent-limit-percentage";

public:
    ImportOptions() = default;

    explicit ImportOptions(const stdcxx::Properties& parameters);

    ImportOptions& addExtension(const std::string& extension);

    bool isThrowExceptionIfExtensionNotFound() const;

    bool isWithAutomationSystems() const;

    double getMissingPermanentLimitPercentage() const;

    ImportOptions& setExtensions(const std::set<std::string>& extensions);

    ImportOptions& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

    ImportOptions& setWithAutomationSystems(bool withAutomationSystems);

    ImportOptions& setMissingPermanentLimitPercentage(double missingPermanentLimitPercentage);

    bool withExtension(const std::string& extension) const;

private:
    bool m_throwExceptionIfExtensionNotFound = false;

    std::set<std::string> m_extensions;

    bool m_withAutomationSystems = true;

    double m_missingPermanentLimitPercentage = 100.0;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
