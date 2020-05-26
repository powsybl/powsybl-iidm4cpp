/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP
#define POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP

#include <set>
#include <string>

namespace stdcxx {

class Properties;

}  // namespace stdcxx

namespace powsybl {

namespace iidm {

namespace converter {

class Parameter;

class ConversionParameters {
public:
    template <typename T>
    static T readParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter);

public:
    ConversionParameters() = delete;

    ~ConversionParameters() = delete;

    ConversionParameters(const ConversionParameters&) = delete;

    ConversionParameters(ConversionParameters&&) noexcept = delete;

    ConversionParameters& operator=(const ConversionParameters&) = delete;

    ConversionParameters& operator=(ConversionParameters&&) noexcept = delete;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/ConversionParameters.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HPP
