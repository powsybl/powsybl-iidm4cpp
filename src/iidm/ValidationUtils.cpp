/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ValidationUtils.hpp>

#include <cmath>

#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

ValidationLevel errorOrWarningForRtc(const Validable& validable, bool loadTapChangingCapabilities, const std::string& message, const ValidationLevel& vl) {
    if (loadTapChangingCapabilities) {
        throwExceptionOrLogError(validable, message, vl);
        return ValidationLevel::EQUIPMENT;
    }
    logging::Logger& logger = logging::LoggerFactory::getLogger("powsybl::iidm::ValidationUtils");
    logger.warn(stdcxx::format("%1% %2%", validable.getMessageHeader(), message));
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

PowsyblException createUndefinedValueGetterException()
{
    return PowsyblException("This getter cannot be used if the value is not defined");
}

PowsyblException createUnsetMethodException()
{
    return PowsyblException("Unset method is not defined. Implement SCADA mode in order to use it");
}

ValidationException createInvalidValueException(const Validable& validable, double value, const std::string& valueName, const std::string& reason = "") {
    std::string r = reason.empty() ? "" : stdcxx::format(" (%1%)", reason);
    return ValidationException(validable, stdcxx::format("invalid value (%1%) for %2%%3%", value, valueName, r));
}

std::string createInvalidValueMessage(double value, const std::string& valueName, const std::string& reason) {
    return stdcxx::format("invalid value (%1%) for %2%%3%", value, valueName, reason);
}

void logError(const Validable& validable, const std::string& message) {
    logging::Logger& logger = logging::LoggerFactory::getLogger("powsybl::iidm::ValidationUtils");
    logger.error(stdcxx::format("%1% %2%", validable.getMessageHeader(), message));
}

void throwExceptionOrLogError(const Validable& validable, const std::string& message, const ValidationLevel& vl) {
    if(vl >= ValidationLevel::STEADY_STATE_HYPOTHESIS) {
        throw ValidationException(validable, message);
    }
    logError(validable, message);
}

void throwExceptionOrLogErrorForInvalidValue(const Validable& validable, double value, const std::string& valueName, const ValidationLevel& vl, const std::string& reason = "") {
    if (vl >= ValidationLevel::STEADY_STATE_HYPOTHESIS) {
        throw createInvalidValueException(validable, value, valueName, reason);
    }
    logError(validable, createInvalidValueMessage(value, valueName, reason));
}

void checkActivePowerLimits(const Validable& validable, double minP, double maxP) {
    if (minP > maxP) {
        throw ValidationException(validable, stdcxx::format("Invalid active limits [%1%, %2%]", minP, maxP));
    }
}

ValidationLevel checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl) {
    if (std::isnan(activePowerSetpoint)) {
        throwExceptionOrLogErrorForInvalidValue(validable, activePowerSetpoint,converter::ACTIVE_POWER_SETPOINT, vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
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

double checkBPerSection(const Validable& validable, double bPerSection) {
    if (std::isnan(bPerSection)) {
        throw ValidationException(validable, "susceptance per section is invalid");
    }
    return bPerSection;
}

ValidationLevel checkConvertersMode(const Validable& /*validable*/, const HvdcLine::ConvertersMode& converterMode, const ValidationLevel& /*vl*/) {
    switch (converterMode) {
        case HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER:
        case HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER:
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected converter mode value: %1%", converterMode));
            return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
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

ValidationLevel checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl) {
    if (std::isnan(activePowerSetpoint)) {
        throwExceptionOrLogErrorForInvalidValue(validable, activePowerSetpoint, converter::ACTIVE_POWER_SETPOINT, vl);
        return ValidationLevel::EQUIPMENT;
    } 
    if (activePowerSetpoint < 0) {
        throw createInvalidValueException(validable, activePowerSetpoint, converter::ACTIVE_POWER_SETPOINT, "active power setpoint should not be negative");
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

double checkHvdcMaxP(const Validable& validable, double maxP) {
    if (std::isnan(maxP)) {
        throw createInvalidValueException(validable, maxP, converter::MAX_P);
    }
    if (maxP < 0) {
        throw createInvalidValueException(validable, maxP, converter::MAX_P, "maximum P should not be negative");
    }
    return maxP;
}

unsigned long checkMaximumSectionCount(const Validable& validable, const stdcxx::optional<unsigned long>& maximumSectionCount) {
    if (!maximumSectionCount) {
        throw ValidationException(validable, "the maximum number of section is not set");
    }
    if (*maximumSectionCount == 0UL) {
        throw ValidationException(validable, stdcxx::format("the maximum number of section (%1%) should be greater than 0", *maximumSectionCount));
    }
    return *maximumSectionCount;
}

const LoadType& checkLoadType(const Validable& /*validable*/, const LoadType& loadType) {
    switch (loadType) {
        case LoadType::UNDEFINED:
        case LoadType::AUXILIARY:
        case LoadType::FICTITIOUS:
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected load type value: %1%", loadType));
    }
    return loadType;
}

double checkLossFactor(const Validable& validable, double lossFactor) {
    if (std::isnan(lossFactor)) {
        throw ValidationException(validable, "loss factor is invalid");
    }
    if (lossFactor < 0 || lossFactor > 100) {
        throw ValidationException(validable, "loss factor must be >= 0 and <= 100");
    }
    return lossFactor;
}

double checkMaxP(const Validable& validable, double maxP) {
    if (std::isnan(maxP)) {
        throw createInvalidValueException(validable, maxP, converter::MAX_P);
    }
    return maxP;
}

double checkMinP(const Validable& validable, double minP) {
    if (std::isnan(minP)) {
        throw createInvalidValueException(validable, minP, converter::MIN_P);
    }
    return minP;
}

double checkNominalVoltage(const Validable& validable, double nominalVoltage) {
    if (std::isnan(nominalVoltage)) {
        throw ValidationException(validable, "Nominal voltage is undefined");
    }
    if (nominalVoltage <= 0) {
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

const ValidationLevel& checkMinValidationLevel(const Validable& /*validable*/, const ValidationLevel& minValidationLevel) {
    switch (minValidationLevel) {
        case ValidationLevel::EQUIPMENT:
        case ValidationLevel::STEADY_STATE_HYPOTHESIS:
            break;
        case ValidationLevel::UNVALID:
        default:
            throw AssertionError(stdcxx::format("Unexpected mininimum Validation Level value: %1%", minValidationLevel));
    }
    return minValidationLevel;
}

ValidationLevel checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating, const ValidationLevel& vl) {
    if (regulating && regulatingTapChangerCount > 0) {
        throwExceptionOrLogError(validable, "Only one regulating control enabled is allowed", vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message) {
    if (!value || std::isnan(*value)) {
        throw ValidationException(validable, message);
    }
    return *value;
}

ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ValidationLevel& vl) {
    if (!value || std::isnan(*value)) {
        throwExceptionOrLogError(validable, message, vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkP0(const Validable& validable, double p0, const ValidationLevel& vl) {
    if (std::isnan(p0)) {
        throwExceptionOrLogError(validable, "p0 is invalid", vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

double checkPermanentLimit(const Validable& validable, double permanentLimit) {
    if (std::islessequal(permanentLimit, 0.0)) {
        throw ValidationException(validable, "permanent limit must be > 0");
    }
    return permanentLimit;
}

ValidationLevel checkPhaseTapChangerRegulationWithouthTerminal(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating,
                                    const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    switch (regulationMode) {
        case PhaseTapChanger::RegulationMode::CURRENT_LIMITER:
        case PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL:
        case PhaseTapChanger::RegulationMode::FIXED_TAP:
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected regulation mode value: %1%", regulationMode));
    }
    if (regulating) {
        if (regulationMode != PhaseTapChanger::RegulationMode::FIXED_TAP && std::isnan(regulationValue)) {
            throwExceptionOrLogError(validable, "phase regulation is on and threshold/setpoint value is not set",vl);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
        if (regulationMode == PhaseTapChanger::RegulationMode::FIXED_TAP) {
            throwExceptionOrLogError(validable, "phase regulation cannot be on if mode is FIXED", vl);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
    }
    return checkValidationLevel;
}

ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating,
                                    const stdcxx::CReference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulationWithouthTerminal(validable, regulationMode, regulationValue, regulating, vl));

    if (regulating && regulationMode != PhaseTapChanger::RegulationMode::FIXED_TAP && !regulationTerminal) {
        throwExceptionOrLogError(validable, "phase regulation is on and regulated terminal is not set", vl);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "phase regulation terminal is not part of the network");
    }

    return checkValidationLevel;
}
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating,
                                    const stdcxx::Reference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulationWithouthTerminal(validable, regulationMode, regulationValue, regulating, vl));

    if(regulating && regulationMode != PhaseTapChanger::RegulationMode::FIXED_TAP && !regulationTerminal) {
        throwExceptionOrLogError(validable, "phase regulation is on and regulated terminal is not set", vl);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }

    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "phase regulation terminal is not part of the network");
    }
    
    return checkValidationLevel;
}

double checkPowerFactor(const Validable& validable, double powerFactor) {
    if (std::isnan(powerFactor)) {
        throw ValidationException(validable, "power factor is invalid");
    }
    if (std::abs(powerFactor) > 1) {
        throw ValidationException(validable, "power factor is invalid, it should be between -1 and 1");
    }
    return powerFactor;
}

ValidationLevel checkQ0(const Validable& validable, double q0, const ValidationLevel& vl) {
    if (std::isnan(q0)) {
        throwExceptionOrLogError(validable, "q0 is invalid", vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

double checkR(const Validable& validable, double r) {
    if (std::isnan(r)) {
        throw ValidationException(validable, "r is invalid");
    }
    return r;
}

double checkRatedS(const Validable& validable, double ratedS) {
    if (!std::isnan(ratedS) && ratedS <= 0.0) {
        throw ValidationException(validable, stdcxx::format("Invalid rated S value: %1%", ratedS));
    }
    return ratedS;
}

double checkRatedU(const Validable& validable, double ratedU, const stdcxx::optional<unsigned long>& num) {
    if (std::isnan(ratedU)) {
        std::string strNum = num ? std::to_string(*num) : "";
        throw ValidationException(validable, stdcxx::format("rated U%1% is invalid", strNum));
    }
    return ratedU;
}

double checkRatedU1(const Validable& validable, double ratedU1) {
    return checkRatedU(validable, ratedU1, 1);
}

double checkRatedU2(const Validable& validable, double ratedU2) {
    return checkRatedU(validable, ratedU2, 2);
}

ValidationLevel checkRatioTapChangerRegulationWithouthTerminal(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, double targetV,
                                    const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (regulating) {
        if (std::isnan(targetV)) {
            checkValidationLevel = validationLevel::min(checkValidationLevel,errorOrWarningForRtc(validable, loadTapChangingCapabilities, "a target voltage has to be set for a regulating ratio tap changer", vl));
        }
        if (std::islessequal(targetV, 0.0)) {
            throw ValidationException(validable, stdcxx::format("bad target voltage %1%", targetV));
        }
    }
    return checkValidationLevel;
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::CReference<Terminal>& regulationTerminal, double targetV, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulationWithouthTerminal(validable, regulating, loadTapChangingCapabilities, targetV, vl));

    if (regulating && !regulationTerminal) {
        checkValidationLevel = validationLevel::min(checkValidationLevel,errorOrWarningForRtc(validable, loadTapChangingCapabilities, "a regulation terminal has to be set for a regulating ratio tap changer", vl));
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "regulation terminal is not part of the network");
    }

    return checkValidationLevel;
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::Reference<Terminal>& regulationTerminal, double targetV, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulationWithouthTerminal(validable, regulating, loadTapChangingCapabilities, targetV, vl));

    if (regulating && !regulationTerminal) {
        checkValidationLevel = validationLevel::min(checkValidationLevel,errorOrWarningForRtc(validable, loadTapChangingCapabilities, "a regulation terminal has to be set for a regulating ratio tap changer", vl));
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "regulation terminal is not part of the network");
    }
    
    return checkValidationLevel;
}

void checkRegulatingTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network) {
    if (regulatingTerminal && !stdcxx::areSame(regulatingTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "Regulating terminal is not part of the network");
    }
}

ValidationLevel checkSections(const Validable& validable, const stdcxx::optional<unsigned long>& currentSectionCount, unsigned long maximumSectionCount, const ValidationLevel& vl) {
    checkMaximumSectionCount(validable, maximumSectionCount);
    if (!currentSectionCount) {
        throwExceptionOrLogError(validable, "the current section count is not set", vl);
        return ValidationLevel::EQUIPMENT;
    } 

    if (maximumSectionCount == 0UL) {
        throw ValidationException(validable, stdcxx::format("the maximum number of section (%1%) should be greater than 0", maximumSectionCount));
    }
    if (*currentSectionCount > maximumSectionCount) {
        throw ValidationException(validable, stdcxx::format("the current number (%1%) of section should be lesser than the maximum number of section (%2%)", *currentSectionCount, maximumSectionCount));
    }

    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkSvcRegulator(const Validable& validable, double voltageSetpoint, double reactivePowerSetpoint, const stdcxx::optional<StaticVarCompensator::RegulationMode>& regulationMode, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel;
    checkValidationLevel = checkOptional(validable, regulationMode, "Regulation mode is invalid", vl);

    if(checkValidationLevel == ValidationLevel::EQUIPMENT) {
        return ValidationLevel::EQUIPMENT;
    }

    switch (*regulationMode) {
        case StaticVarCompensator::RegulationMode::VOLTAGE:
            if (std::isnan(voltageSetpoint)) {
                throwExceptionOrLogErrorForInvalidValue(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, vl);
                return ValidationLevel::EQUIPMENT;
            }
            break;

        case StaticVarCompensator::RegulationMode::REACTIVE_POWER:
            if (std::isnan(reactivePowerSetpoint)) {
                throwExceptionOrLogErrorForInvalidValue(validable, reactivePowerSetpoint, converter::REACTIVE_POWER_SETPOINT, vl);
                return ValidationLevel::EQUIPMENT;
            }
            break;

        case StaticVarCompensator::RegulationMode::OFF:
            // nothing to check
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected regulation mode value: %1%", *regulationMode));
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl) {
    if ((tapPosition < lowTapPosition) || (tapPosition > highTapPosition)) {
        throwExceptionOrLogError(validable, stdcxx::format("incorrect tap position %1% [%2%, %3%]", tapPosition, lowTapPosition, highTapPosition), vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ValidationLevel& vl) {
    if (regulating && std::isnan(targetDeadband)) {
        throwExceptionOrLogError(validable, "Undefined value for target deadband of regulating " + validableType, vl);
        return ValidationLevel::EQUIPMENT;
    }
    if (targetDeadband < 0) {
        throw ValidationException(validable, stdcxx::format("Unexpected value for target deadband of tap changer: %1%", targetDeadband));
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

double checkVoltage(const Validable& validable, double voltage) {
    if (!std::isnan(voltage) && voltage < 0) {
        throw ValidationException(validable, "voltage cannot be < 0");
    }
    return voltage;
}

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, const ValidationLevel& vl) {
    if (voltageRegulatorOn) {
        if (std::isnan(voltageSetpoint)) {
            throwExceptionOrLogErrorForInvalidValue(validable,voltageSetpoint, converter::VOLTAGE_SETPOINT, vl, "voltage regulator is on");
            return ValidationLevel::EQUIPMENT;
        }
        if (voltageSetpoint <= 0) {
            throw createInvalidValueException(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, "voltage regulator is on");
        }
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint, const ValidationLevel& vl) {
    if (voltageRegulatorOn) {
        if (std::isnan(voltageSetpoint)) {
            throwExceptionOrLogErrorForInvalidValue(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, vl, "voltage regulator is on");
            return ValidationLevel::EQUIPMENT;
        } 
        if (voltageSetpoint <= 0) {
            throw createInvalidValueException(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, "voltage regulator is on");
        }
    } else if (std::isnan(reactivePowerSetpoint)) {
        throwExceptionOrLogErrorForInvalidValue(validable, reactivePowerSetpoint, converter::REACTIVE_POWER_SETPOINT, vl, "voltage regulator is off");
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit) {
    if (lowVoltageLimit < 0) {
        throw ValidationException(validable, "Low voltage limit is < 0");
    }
    if (highVoltageLimit < 0) {
        throw ValidationException(validable, "High voltage limit is < 0");
    }
    if (lowVoltageLimit > highVoltageLimit) {
        throw ValidationException(validable, stdcxx::format("Inconsistent voltage limit range [%1%, %2%]", lowVoltageLimit, highVoltageLimit));
    }
}

double checkX(const Validable& validable, double x) {
    if (std::isnan(x)) {
        throw ValidationException(validable, "x is invalid");
    }
    return x;
}

ValidationLevel checkRtc(const Validable& validable, const RatioTapChanger& rtc, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulation(validable, rtc.isRegulating(), rtc.hasLoadTapChangingCapabilities(), rtc.getRegulationTerminal(), rtc.getTargetV(), network, vl));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "ratio tap changer", rtc.isRegulating(), rtc.getTargetDeadband(), vl));
    return checkValidationLevel;
}

ValidationLevel checkPtc(const Validable& validable, const PhaseTapChanger& ptc, const Network& network, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulation(validable, ptc.getRegulationMode(), ptc.getRegulationValue(), ptc.isRegulating(), ptc.getRegulationTerminal(), network, vl));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "phase tap changer", ptc.isRegulating(), ptc.getTargetDeadband(), vl));
    return checkValidationLevel;
}

ValidationLevel checkThreeWindingsTransformer(const Validable& validable, const ThreeWindingsTransformer& twt, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    unsigned long regulatingTc = 0;
    for (const auto& leg : twt.getLegs()){
        if(leg.hasRatioTapChanger()){
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkRtc(validable, leg.getRatioTapChanger(), twt.getNetwork(), vl));
            if(leg.getRatioTapChanger().isRegulating()) {
                regulatingTc += 1;
            }
        }
        if(leg.hasPhaseTapChanger()){
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkPtc(validable, leg.getPhaseTapChanger(), twt.getNetwork(), vl));
            if(leg.getPhaseTapChanger().isRegulating()) {
                regulatingTc += 1;
            }
        }
    }
    if(regulatingTc > 0) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkOnlyOneTapChangerRegulatingEnabled(validable, regulatingTc, true, vl));
    }
    return checkValidationLevel;
}

ValidationLevel checkTwoWindingsTransformer(const Validable& validable, const TwoWindingsTransformer& twt, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    unsigned long regulatingTc = 0;
    if (twt.hasRatioTapChanger()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkRtc(validable, twt.getRatioTapChanger(), twt.getNetwork(), vl));
        if(twt.getRatioTapChanger().isRegulating()) {
            regulatingTc += 1;
        }
    }
    if (twt.hasPhaseTapChanger()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkPtc(validable, twt.getPhaseTapChanger(), twt.getNetwork(), vl));
        if(twt.getPhaseTapChanger().isRegulating()) {
            regulatingTc += 1;
        }
    }
    if (regulatingTc > 0) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkOnlyOneTapChangerRegulatingEnabled(validable, regulatingTc, true, vl));
    }
    return checkValidationLevel;
}

ValidationLevel checkIdentifiable(const Identifiable& identifiable,const ValidationLevel& previous, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = previous;
    if (stdcxx::isInstanceOf<Validable>(identifiable)) {
        const auto& validable = dynamic_cast<const Validable&>(identifiable);
        if (stdcxx::isInstanceOf<Battery>(identifiable)) {
            const auto& battery = dynamic_cast<const Battery&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, battery.getP0(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, battery.getQ0(), vl));
        } else if (stdcxx::isInstanceOf<DanglingLine>(identifiable)) {
            const auto& danglingLine = dynamic_cast<const DanglingLine&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, danglingLine.getP0(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, danglingLine.getQ0(), vl));
            const auto& generation = danglingLine.getGeneration();
            if (generation) {
                checkValidationLevel = validationLevel::min(checkValidationLevel, checkActivePowerSetpoint(validable, generation.get().getTargetP(), vl));
                checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, generation.get().isVoltageRegulationOn(), generation.get().getTargetV(), generation.get().getTargetQ(), vl));
            }
        } else if (stdcxx::isInstanceOf<Generator>(identifiable)) {
            const auto& generator = dynamic_cast<const Generator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkActivePowerSetpoint(validable, generator.getTargetP(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, generator.isVoltageRegulatorOn(), generator.getTargetV(), generator.getTargetQ(), vl));
        } else if (stdcxx::isInstanceOf<HvdcLine>(identifiable)) {
            const auto& hvdcLine = dynamic_cast<const HvdcLine&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkConvertersMode(validable, hvdcLine.getConvertersMode(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkHvdcActivePowerSetpoint(validable, hvdcLine.getActivePowerSetpoint(), vl));
        } else if (stdcxx::isInstanceOf<Load>(identifiable)) {
            const auto& load = dynamic_cast<const Load&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, load.getP0(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, load.getQ0(), vl));
        } else if (stdcxx::isInstanceOf<ShuntCompensator>(identifiable)) {
            const auto& shunt = dynamic_cast<const ShuntCompensator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, shunt.isVoltageRegulatorOn(), shunt.getTargetV(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "shunt compensator", shunt.isVoltageRegulatorOn(), shunt.getTargetDeadband(), vl));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkSections(validable, shunt.getSectionCount(), shunt.getMaximumSectionCount(), vl));
        } else if (stdcxx::isInstanceOf<StaticVarCompensator>(identifiable)) {
            const auto& svc = dynamic_cast<const StaticVarCompensator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkSvcRegulator(validable, svc.getVoltageSetpoint(), svc.getReactivePowerSetpoint(), svc.getRegulationMode(), vl));
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)) {
            const auto& threewt = dynamic_cast<const ThreeWindingsTransformer&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkThreeWindingsTransformer(validable, threewt, vl));
        } else if (stdcxx::isInstanceOf<TwoWindingsTransformer>(identifiable)) {
            const auto& twowt = dynamic_cast<const TwoWindingsTransformer&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkTwoWindingsTransformer(validable, twowt, vl));
        } else if (stdcxx::isInstanceOf<VscConverterStation>(identifiable)) {
            const auto& converterStation = dynamic_cast<const VscConverterStation&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, converterStation.isVoltageRegulatorOn(), converterStation.getVoltageSetpoint(), converterStation.getReactivePowerSetpoint(), vl));
        }
    }
    return checkValidationLevel;
}

ValidationLevel validateIdentifiables(const stdcxx::const_range<Identifiable>& identifiables, bool allChecks, const ValidationLevel& previous, const ValidationLevel& vl) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if(previous >= ValidationLevel::STEADY_STATE_HYPOTHESIS) {
        return previous;
    }
    for (const auto& identifiable : identifiables) {
        checkValidationLevel = checkIdentifiable(identifiable, checkValidationLevel, vl);
        if(!allChecks && checkValidationLevel == ValidationLevel::EQUIPMENT){
            return checkValidationLevel;
        }
    }
    return checkValidationLevel;
}


}  // namespace iidm

}  // namespace powsybl
