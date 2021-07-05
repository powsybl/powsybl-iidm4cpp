/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP
#define POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP

#include <string>
#include <vector>

#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class Parameter;

class ConversionParameters {
public:
    static bool readBooleanParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter);

    static double readDoubleParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter);

    static std::vector<std::string> readStringListParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter);

    static const std::string& readStringParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter);

public:
    ConversionParameters() = delete;

    ConversionParameters(const ConversionParameters&) = delete;

    ConversionParameters(ConversionParameters&&) noexcept = delete;

    ~ConversionParameters() = delete;

    ConversionParameters& operator=(const ConversionParameters&) = delete;

    ConversionParameters& operator=(ConversionParameters&&) noexcept = delete;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP
