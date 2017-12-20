/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/stdcxx.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

int checkForecastDistance(const Validable& validable, int value);

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

template <typename T>
const T& checkOptional(const Validable& validable, const stdcxx::optional<T> value, const std::string& message) {
#if __cplusplus >= 201703L
    bool isInitialized = value.has_value();
#else
    bool isInitialized = value.is_initialized();
#endif
    if (!isInitialized) {
        throw ValidationException(validable, message);
    }
    return *value;
}

double checkP0(const Validable& validable, double p0);

double checkQ0(const Validable& validable, double q0);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

}

}

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
