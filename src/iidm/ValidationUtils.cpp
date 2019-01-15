/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ValidationUtils.hpp"

#include <cmath>

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

ValidationException createInvalidValueException(const Validable& validable, double value, const std::string& valueName, const std::string& reason = "") {
    std::string r = reason.empty() ? "" : logging::format(" (%1%)", reason);
    return ValidationException(validable, logging::format("invalid value (%1%) for %2%%3%", value, valueName, r));
}

void checkActiveLimits(const Validable& validable, double minP, double maxP) {
    if (minP > maxP) {
        throw ValidationException(validable, logging::format("Invalid active limits [%1%, %2%]", minP, maxP));
    }
}

double checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint) {
    if (std::isnan(activePowerSetpoint)) {
        throw ValidationException(validable, "Active power setpoint is not set");
    }
    return activePowerSetpoint;
}

double checkB(const Validable& validable, double b) {
    if (std::isnan(b)) {
        throw ValidationException(validable, "b is invalid");
    }
    return b;
}

double checkB1(const Validable& validable, double b1) {
    if (std::isnan(b1)) {
        throw ValidationException(validable, "b1 is invalid");
    }
    return b1;
}

double checkB2(const Validable& validable, double b2) {
    if (std::isnan(b2)) {
        throw ValidationException(validable, "b2 is invalid");
    }
    return b2;
}

double checkBmax(const Validable& validable, double bMax) {
    if (std::isnan(bMax)) {
        throw ValidationException(validable, "bMax is invalid");
    }
    return bMax;
}

double checkBmin(const Validable& validable, double bMin) {
    if (std::isnan(bMin)) {
        throw ValidationException(validable, "bMin is invalid");
    }
    return bMin;
}

double checkbPerSection(const Validable& validable, double bPerSection) {
    if (std::isnan(bPerSection)) {
        throw ValidationException(validable, "susceptance per section is invalid");
    }
    if (!std::islessgreater(bPerSection, 0.0)) {
        throw ValidationException(validable, "susceptance per section is equal to zero");
    }
    return bPerSection;
}

int checkForecastDistance(const Validable& validable, int forecastDistance) {
    if (forecastDistance < 0) {
        throw ValidationException(validable, "Forecast distance is < 0");
    }
    return forecastDistance;
}

double checkG(const Validable& validable, double g) {
    if (std::isnan(g)) {
        throw ValidationException(validable, "g is invalid");
    }
    return g;
}

double checkG1(const Validable& validable, double g1) {
    if (std::isnan(g1)) {
        throw ValidationException(validable, "g1 is invalid");
    }
    return g1;
}

double checkG2(const Validable& validable, double g2) {
    if (std::isnan(g2)) {
        throw ValidationException(validable, "g2 is invalid");
    }
    return g2;
}

void checkHalf(const Validable& validable, const TieLine::HalfLine& half, int num) {
    checkNotEmpty(validable, half.getId(), logging::format("id is not set for half line %1%", num));
    if (std::isnan(half.getB1())) {
        throw ValidationException(validable, logging::format("b1 is not set for half line %1%", num));
    }
    if (std::isnan(half.getB2())) {
        throw ValidationException(validable, logging::format("b2 is not set for half line %1%", num));
    }
    if (std::isnan(half.getG1())) {
        throw ValidationException(validable, logging::format("g1 is not set for half line %1%", num));
    }
    if (std::isnan(half.getG2())) {
        throw ValidationException(validable, logging::format("g2 is not set for half line %1%", num));
    }
    if (std::isnan(half.getR())) {
        throw ValidationException(validable, logging::format("r is not set for half line %1%", num));
    }
    if (std::isnan(half.getX())) {
        throw ValidationException(validable, logging::format("x is not set for half line %1%", num));
    }
    if (std::isnan(half.getXnodeP())) {
        throw ValidationException(validable, logging::format("xnodeP is not set for half line %1%", num));
    }
    if (std::isnan(half.getXnodeQ())) {
        throw ValidationException(validable, logging::format("xnodeQ is not set for half line %1%", num));
    }
}

double checkLossFactor(const Validable& validable, double lossFactor) {
    if (std::isnan(lossFactor)) {
        throw ValidationException(validable, "loss factor is invalid");
    } else if (lossFactor < 0) {
        throw ValidationException(validable, "loss factor must be >= 0");
    }
    return lossFactor;
}

double checkMaxP(const Validable& validable, double maxP) {
    if (std::isnan(maxP)) {
        throw ValidationException(validable, "Maximum active power is not set");
    }
    return maxP;
}

double checkMinP(const Validable& validable, double minP) {
    if (std::isnan(minP)) {
        throw ValidationException(validable, "Minimum active power is not set");
    }
    return minP;
}

double checkNominalVoltage(const Validable& validable, double nominalVoltage) {
    if (std::isnan(nominalVoltage)) {
        throw ValidationException(validable, "Nominal voltage is undefined");
    } else if (nominalVoltage <= 0) {
        throw ValidationException(validable, "Nominal voltage is <= 0");
    }
    return nominalVoltage;
}

const std::string& checkNotEmpty(const std::string& value, const std::string& message) {
    if (value.empty()) {
        throw PowsyblException(message);
    }
    return value;
}

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message) {
    if (value.empty()) {
        throw ValidationException(validable, message);
    }
    return value;
}

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message) {
#if __cplusplus >= 201703L
    bool isInitialized = value.has_value();
#else
    bool isInitialized = value.is_initialized();
#endif
    if (!isInitialized || std::isnan(*value)) {
        throw ValidationException(validable, message);
    }
    return *value;
}

double checkP0(const Validable& validable, double p0) {
    if (std::isnan(p0)) {
        throw ValidationException(validable, "p0 is invalid");
    }
    return p0;
}

double checkPowerFactor(const Validable& validable, double powerFactor) {
    if (std::isnan(powerFactor)) {
        throw ValidationException(validable, "power factor is invalid");
    }
    return powerFactor;
}

double checkQ0(const Validable& validable, double q0) {
    if (std::isnan(q0)) {
        throw ValidationException(validable, "q0 is invalid");
    }
    return q0;
}

double checkR(const Validable& validable, double r) {
    if (std::isnan(r)) {
        throw ValidationException(validable, "r is invalid");
    }
    return r;
}

double checkRatedS(const Validable& validable, double ratedS) {
    if (!std::isnan(ratedS) && ratedS <= 0.0) {
        throw ValidationException(validable, logging::format("Invalid rated S value: %1%", ratedS));
    }
    return ratedS;
}

void checkRegulatingTerminal(const Validable& validable, const Terminal& regulatingTerminal, const Network& network) {
    if (!stdcxx::areSame(regulatingTerminal.getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "Regulating terminal is not part of the network");
    }
}

void checkSections(const Validable& validable, unsigned long currentSectionCount, unsigned long maximumSectionCount) {
    if (maximumSectionCount == 0UL) {
        throw ValidationException(validable, logging::format("the maximum number of section (%1%) should be greater than 0", maximumSectionCount));
    }
    if (currentSectionCount > maximumSectionCount) {
        throw ValidationException(validable, logging::format("the current number (%1%) of section should be lesser than the maximum number of section (%2%)", currentSectionCount, maximumSectionCount));
    }
}

void checkSvcRegulator(const Validable& validable, double voltageSetpoint, double reactivePowerSetpoint, const stdcxx::optional<StaticVarCompensator::RegulationMode>& regulationMode) {
    checkOptional(validable, regulationMode, "Regulation mode is invalid");

    switch (*regulationMode) {
        case StaticVarCompensator::RegulationMode::VOLTAGE:
            if (std::isnan(voltageSetpoint)) {
                throw createInvalidValueException(validable, voltageSetpoint, "voltage setpoint");
            }
            break;

        case StaticVarCompensator::RegulationMode::REACTIVE_POWER:
            if (std::isnan(reactivePowerSetpoint)) {
                throw createInvalidValueException(validable, reactivePowerSetpoint, "reactive power setpoint");
            }
            break;

        case StaticVarCompensator::RegulationMode::OFF:
            // nothing to check
            break;

        default:
            throw AssertionError(logging::format("Unexpected regulation mode value: %1%", *regulationMode));
    }
}

double checkVoltage(const Validable& validable, double voltage) {
    if (!std::isnan(voltage) && voltage < 0) {
        throw ValidationException(validable, "voltage cannot be < 0");
    }
    return voltage;
}

void checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint) {
    if (voltageRegulatorOn) {
        if (std::isnan(voltageSetpoint) || voltageSetpoint <= 0) {
            throw ValidationException(validable, logging::format("Invalid voltage setpoint value (%1%) while voltage regulator is on", voltageSetpoint));
        }
    } else if (std::isnan(reactivePowerSetpoint)) {
        throw ValidationException(validable, logging::format("Invalid reactive power setpoint (%1%) while voltage regulator is off", reactivePowerSetpoint));
    }
}

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit) {
    if (lowVoltageLimit < 0) {
        throw ValidationException(validable, "Low voltage limit is < 0");
    }
    if (highVoltageLimit < 0) {
        throw ValidationException(validable, "High voltage limit is < 0");
    }
    if (lowVoltageLimit > highVoltageLimit) {
        throw ValidationException(validable, logging::format("Inconsistent voltage limit range [%1%, %2%]", lowVoltageLimit, highVoltageLimit));
    }
}

double checkX(const Validable& validable, double x) {
    if (std::isnan(x)) {
        throw ValidationException(validable, "x is invalid");
    }
    return x;
}

}  // namespace iidm

}  // namespace powsybl
