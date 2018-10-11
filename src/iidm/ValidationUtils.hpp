/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Terminal;

int checkForecastDistance(const Validable& validable, int value);

void checkActiveLimits(const Validable& validable, double minP, double maxP);

double checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint);

double checkMaxP(const Validable& validable, double maxP);

double checkMinP(const Validable& validable, double minP);

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message);

template <typename T>
const T& checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message) {
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

double checkRatedS(const Validable& validable, double ratedS);

void checkRegulatingTerminal(const Validable& validable, const Terminal& regulatingTerminal, const Network& network);

double checkVoltage(const Validable& validable, double voltage);

void checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
