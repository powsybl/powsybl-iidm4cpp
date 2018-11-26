/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Terminal;

void checkActiveLimits(const Validable& validable, double minP, double maxP);

double checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint);

double checkB1(const Validable& validable, double b1);

double checkB2(const Validable& validable, double b2);

int checkForecastDistance(const Validable& validable, int value);

double checkG1(const Validable& validable, double g1);

double checkG2(const Validable& validable, double g2);

void checkHalf(const Validable& validable, const TieLine::HalfLine& half, int num);

double checkMaxP(const Validable& validable, double maxP);

double checkMinP(const Validable& validable, double minP);

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

template <typename T>
bool checkOptional(const stdcxx::optional<T>& value) {
#if __cplusplus >= 201703L
    return value.has_value();
#else
    return value.is_initialized();
#endif
}

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

double checkR(const Validable& validable, double r);

double checkRatedS(const Validable& validable, double ratedS);

void checkRegulatingTerminal(const Validable& validable, const Terminal& regulatingTerminal, const Network& network);

double checkVoltage(const Validable& validable, double voltage);

void checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

double checkX(const Validable& validable, double x);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
