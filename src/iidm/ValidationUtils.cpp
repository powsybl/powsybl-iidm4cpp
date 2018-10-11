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

int checkForecastDistance(const Validable& validable, int forecastDistance) {
    if (forecastDistance < 0) {
        throw ValidationException(validable, "Forecast distance is < 0");
    }
    return forecastDistance;
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

double checkQ0(const Validable& validable, double q0) {
    if (std::isnan(q0)) {
        throw ValidationException(validable, "q0 is invalid");
    }
    return q0;
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

}  // namespace iidm

}  // namespace powsybl
