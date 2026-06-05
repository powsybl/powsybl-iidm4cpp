/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ValidationUtils.hpp>

#include <cmath>

#include <unordered_set>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<ActionOnError>() {
    static std::initializer_list<std::string> s_typesNames {
        "THROW_EXCEPTION",
        "LOG_ERROR",
        "IGNORE"
    };
    return s_typesNames;
}

}  // namespace Enum



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

void actionOnError(const Validable& validable, const std::string& message, const ActionOnError& action) {
    switch (action) {
        case ActionOnError::THROW_EXCEPTION:
            throw ValidationException(validable, message);
        case ActionOnError::LOG_ERROR:
            logError(validable, message);
        case ActionOnError::IGNORE:
        default:
            return;
    }
}

void actionOnErrorForInvalidValue(const Validable& validable, double value, const std::string& valueName, const ActionOnError& action, const std::string& reason = "") {
    if (action == ActionOnError::THROW_EXCEPTION) {
        throw createInvalidValueException(validable, value, valueName, reason);
    }
    if (action == ActionOnError::LOG_ERROR) {
        logError(validable, createInvalidValueMessage(value, valueName, reason));
    }
}

ActionOnError checkValidationActionOnError(const ValidationLevel& vl) {
    return (vl >= ValidationLevel::STEADY_STATE_HYPOTHESIS) ? ActionOnError::THROW_EXCEPTION : ActionOnError::IGNORE;
}

ValidationLevel checkAcDcConverterControl(const Validable& validable, const AcDcConverter::ControlMode& controlMode, double targetP, double targetVdc, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    switch (controlMode) {
        case AcDcConverter::ControlMode::P_PCC:
            if(std::isnan(targetP)) {
                actionOnError(validable, "targetP is invalid", action);
                checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
            }
            break;
        case AcDcConverter::ControlMode::V_DC:
            if(std::isnan(targetVdc)) {
                actionOnError(validable, "targetVdc is invalid", action);
                checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
            }
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected converter control mode value: %1%", controlMode));
            return ValidationLevel::EQUIPMENT;
    }
    return checkValidationLevel;
}
ValidationLevel checkAcDcConverterControl(const Validable& validable, const AcDcConverter::ControlMode& controlMode, double targetP, double targetVdc, const ValidationLevel& vl) {
    return checkAcDcConverterControl(validable, controlMode, targetP, targetVdc, checkValidationActionOnError(vl));
}

void checkAcDcConverterPccTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& pccTerminal, const VoltageLevel& voltageLevel) {
    if(static_cast<bool>(pccTerminal)) {
        stdcxx::Reference<Connectable> connectable = pccTerminal.get().getConnectable();
        if(!(stdcxx::isInstanceOf<Branch>(connectable) || stdcxx::isInstanceOf<ThreeWindingsTransformer>(connectable) || stdcxx::isInstanceOf<AcDcConverter>(connectable))) {
            throw ValidationException(validable, "pccTerminal is not a line or transformer or converter terminal");
        }
        if(stdcxx::isInstanceOf<AcDcConverter>(connectable) && !stdcxx::areSame(connectable.get(), validable)) {
            throw ValidationException(validable, "pccTerminal cannot be the terminal of another converter");
        }
        if (!stdcxx::areSame(connectable.get().getParentNetwork(), voltageLevel.getParentNetwork())) {
            throw ValidationException(validable, "pccTerminal is not in the same parent network as the voltage level");
        }
    }
}

void checkActivePowerLimits(const Validable& validable, double minP, double maxP) {
    if (minP > maxP) {
        throw ValidationException(validable, stdcxx::format("Invalid active limits [%1%, %2%]", minP, maxP));
    }
}

ValidationLevel checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ActionOnError& action) {
    if (std::isnan(activePowerSetpoint)) {
        actionOnErrorForInvalidValue(validable, activePowerSetpoint,converter::ACTIVE_POWER_SETPOINT, action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl) {
    return checkActivePowerSetpoint(validable, activePowerSetpoint, checkValidationActionOnError(vl));
}

DcNode& checkAndGetDcNode(Network& network, const Validable& validable, const std::string& dcNodeId, const std::string& attributeName) {
    if(dcNodeId.empty()) {
        throw ValidationException(validable, stdcxx::format("%1% is not set", attributeName));
    }

    stdcxx::Reference<DcNode> dcNodeRef = network.find<DcNode>(dcNodeId);
    if (!dcNodeRef) {
        throw ValidationException(validable, stdcxx::format("DcNode '%1%' not found", dcNodeId));
    }
    return dcNodeRef.get();
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

double checkCoefficient(const Validable& validable, double coefficient) {
    if (std::isnan(coefficient)) {
        throw ValidationException(validable, stdcxx::format("Invalid zip load model coefficient: %1%", coefficient));
    }
    return coefficient;
}

ValidationLevel checkConvertersMode(const Validable& /*validable*/, const HvdcLine::ConvertersMode& converterMode, const ActionOnError& /*action*/) {
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
ValidationLevel checkConvertersMode(const Validable& validable, const HvdcLine::ConvertersMode& converterMode, const ValidationLevel& vl) {
    return checkConvertersMode(validable, converterMode, checkValidationActionOnError(vl));
}

double checkDoubleParamPositive(const Validable& validable, double param, const std::string& paramName) {
    if (std::isnan(param) || param < 0) {
        throw ValidationException(validable, stdcxx::format("%1% is invalid", paramName));
    }
    return param;
}

double checkExponent(const Validable& validable, double n) {
    if (std::isnan(n) || n < 0) {
        throw ValidationException(validable, stdcxx::format("Invalid load model exponential value: %1%", n));
    }
    return n;
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

ValidationLevel checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ActionOnError& action) {
    if (std::isnan(activePowerSetpoint)) {
        actionOnErrorForInvalidValue(validable, activePowerSetpoint, converter::ACTIVE_POWER_SETPOINT, action);
        return ValidationLevel::EQUIPMENT;
    } 
    if (activePowerSetpoint < 0) {
        throw createInvalidValueException(validable, activePowerSetpoint, converter::ACTIVE_POWER_SETPOINT, "active power setpoint should not be negative");
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl) {
    return checkHvdcActivePowerSetpoint(validable, activePowerSetpoint, checkValidationActionOnError(vl));
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

const LineCommutatedConverter::ReactiveModel& checkLccReactiveModel(const Validable& /*validable*/, const LineCommutatedConverter::ReactiveModel& reactiveModel) {
    switch (reactiveModel) {
        case LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR:
        case LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR:
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected converter's reactive power model: %1%", reactiveModel));
    }
    return reactiveModel;
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

ValidationLevel checkLossFactor(const Validable& validable, double lossFactor, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (std::isnan(lossFactor)) {
        actionOnError(validable, "loss factor is invalid", action);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }
    if (lossFactor < 0 || lossFactor > 100) {
        throw ValidationException(validable, "loss factor must be >= 0 and <= 100");
    }
    return checkValidationLevel;
}
ValidationLevel checkLossFactor(const Validable& validable, double lossFactor, const ValidationLevel& vl) {
    return checkLossFactor(validable, lossFactor, checkValidationActionOnError(vl));
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

ValidationLevel checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating, const ActionOnError& action) {
    if (regulating && regulatingTapChangerCount > 0) {
        actionOnError(validable, "Only one regulating control enabled is allowed", action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating, const ValidationLevel& vl) {
    return checkOnlyOneTapChangerRegulatingEnabled(validable, regulatingTapChangerCount, regulating, checkValidationActionOnError(vl));
}

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message) {
    if (!value || std::isnan(*value)) {
        throw ValidationException(validable, message);
    }
    return *value;
}

ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ActionOnError& action) {
    if (!value || std::isnan(*value)) {
        actionOnError(validable, message, action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ValidationLevel& vl) {
    return checkOptional(validable, value, message, checkValidationActionOnError(vl));
}

ValidationLevel checkP0(const Validable& validable, double p0, const ActionOnError& action) {
    if (std::isnan(p0)) {
        actionOnError(validable, "p0 is invalid", action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkP0(const Validable& validable, double p0, const ValidationLevel& vl) {
    return checkP0(validable, p0, checkValidationActionOnError(vl));
}

ValidationLevel checkPermanentLimit(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ActionOnError& action) {
    if (std::isnan(permanentLimit) && !temporaryLimits.empty()) {
        actionOnError(validable, "permanent limit must be defined if temporary limits are present", action);
        return ValidationLevel::EQUIPMENT;
    }
    if(!std::isnan(permanentLimit)) {
        if(std::isless(permanentLimit, 0.0)) {
            //Forbidden for both STEADY_STATE_HYPOTHESIS and EQUIPMENT
            throw ValidationException(validable, "permanent limit must be >= 0");
        } 
        if(permanentLimit == 0.0) {
            //Log if null
            logging::Logger& logger = logging::LoggerFactory::getLogger("powsybl::iidm::ValidationUtils");
            logger.info(stdcxx::format("%1% permanent limit is set to 0", validable.getMessageHeader()));
        }
    }

    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkPermanentLimit(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ValidationLevel& vl) {
    return checkPermanentLimit(validable, permanentLimit, temporaryLimits, checkValidationActionOnError(vl));
}

void checkTemporaryLimits(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits) {
    logging::Logger& logger = logging::LoggerFactory::getLogger("powsybl::iidm::ValidationUtils");

    // check temporary limits are consistents with permanent
    double previousLimit = stdcxx::nan();
    bool wrongOrderMessageAlreadyLogged = false;
    for (const LoadingLimits::TemporaryLimit& tl : temporaryLimits) {
        if (tl.getValue() <= permanentLimit) {
            logger.debug(stdcxx::format("%1%temporary limit should be greater than permanent limit", validable.getMessageHeader()));
        }
        if (!wrongOrderMessageAlreadyLogged && !std::isnan(previousLimit) && tl.getValue() <= previousLimit) {
            logger.debug(stdcxx::format("%1%temporary limits should be in ascending value order", validable.getMessageHeader()));
            wrongOrderMessageAlreadyLogged = true;
        }
        previousLimit = tl.getValue();
    }
}

ValidationLevel checkLoadingLimits(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPermanentLimit(validable, permanentLimit, temporaryLimits, action));
    checkTemporaryLimits(validable, permanentLimit, temporaryLimits);

    return checkValidationLevel;
}
ValidationLevel checkLoadingLimits(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ValidationLevel& vl) {
    return checkLoadingLimits(validable, permanentLimit, temporaryLimits, checkValidationActionOnError(vl));
}

ValidationLevel checkLoadingLimits(const Validable& validable, const LoadingLimits& limits, const ActionOnError& action) {
    return checkLoadingLimits(validable, limits.getPermanentLimit(), limits.getTemporaryLimits(), action);
}
ValidationLevel checkLoadingLimits(const Validable& validable, const LoadingLimits& limits, const ValidationLevel& vl) {
    return checkLoadingLimits(validable, limits.getPermanentLimit(), limits.getTemporaryLimits(), checkValidationActionOnError(vl));
}

ValidationLevel checkPhaseTapChangerRegulationWithoutTerminal(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    switch (regulationMode) {
        case PhaseTapChanger::RegulationMode::CURRENT_LIMITER:
        case PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL:
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected regulation mode value: %1%", regulationMode));
    }
    if (regulating) {
        if(regulationMode == PhaseTapChanger::RegulationMode::CURRENT_LIMITER && regulationValue < 0) {
            throw ValidationException(validable, "phase tap changer in CURRENT_LIMITER mode must have a non-negative regulation value");
        }
        if(!loadTapChangingCapabilities) {
            actionOnError(validable, "regulation cannot be enabled on phase tap changer without load tap changing capabilities", action);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
        if (std::isnan(regulationValue)) {
            actionOnError(validable, "phase regulation is on and threshold/setpoint value is not set", action);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
    }
    return checkValidationLevel;
}
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::CReference<Terminal>& regulationTerminal, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulationWithoutTerminal(validable, regulationMode, regulationValue, regulating, loadTapChangingCapabilities, action));

    if (regulating && !regulationTerminal) {
        actionOnError(validable, "phase regulation is on and regulated terminal is not set", action);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "phase regulation terminal is not part of the network");
    }

    return checkValidationLevel;
}
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::CReference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl) {
    return checkPhaseTapChangerRegulation(validable, regulationMode, regulationValue, regulating, loadTapChangingCapabilities, regulationTerminal, network, checkValidationActionOnError(vl));
}
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::Reference<Terminal>& regulationTerminal, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulationWithoutTerminal(validable, regulationMode, regulationValue, regulating, loadTapChangingCapabilities, action));

    if(regulating && !regulationTerminal) {
        actionOnError(validable, "phase regulation is on and regulated terminal is not set", action);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }

    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "phase regulation terminal is not part of the network");
    }
    
    return checkValidationLevel;
}
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::Reference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl) {
    return checkPhaseTapChangerRegulation(validable, regulationMode, regulationValue, regulating, loadTapChangingCapabilities, regulationTerminal, network, checkValidationActionOnError(vl));
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

double checkPowerFactorPositive(const Validable& validable, double powerFactor) {
    if (std::isnan(powerFactor)) {
        throw ValidationException(validable, "power factor is invalid");
    }
    if (powerFactor < 0 || powerFactor > 1) {
        throw ValidationException(validable, "power factor is invalid, it must be between 0 and 1");
    }
    return powerFactor;
}

ValidationLevel checkQ0(const Validable& validable, double q0, const ActionOnError& action) {
    if (std::isnan(q0)) {
        actionOnError(validable, "q0 is invalid", action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkQ0(const Validable& validable, double q0, const ValidationLevel& vl) {
    return checkQ0(validable, q0, checkValidationActionOnError(vl));
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

ValidationLevel checkRatioTapChangerRegulationWithoutTerminal(const Validable& validable, bool regulating, bool loadTapChangingCapabilities,
                                    const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    switch (regulationMode) {
        case RatioTapChanger::RegulationMode::VOLTAGE:
        case RatioTapChanger::RegulationMode::REACTIVE_POWER:
            break;
        default:
            throw AssertionError(stdcxx::format("Unexpected regulation mode value: %1%", regulationMode));
    }
    if (regulating) {
        if(!loadTapChangingCapabilities) {
            actionOnError(validable, "regulation cannot be enabled on ratio tap changer without load tap changing capabilities", action);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
        if (std::isnan(regulationValue)) {
            actionOnError(validable, "a target voltage has to be set for a regulating ratio tap changer", action);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
        if (regulationMode == RatioTapChanger::RegulationMode::VOLTAGE && std::islessequal(regulationValue, 0.0)) {
            actionOnError(validable, stdcxx::format("bad target voltage %1%", regulationValue), action);
            checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
        }
    }
    return checkValidationLevel;
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::CReference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulationWithoutTerminal(validable, regulating, loadTapChangingCapabilities, regulationMode, regulationValue, action));

    if (regulating && !regulationTerminal) {
        actionOnError(validable, "a regulation terminal has to be set for a regulating ratio tap changer", action);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "regulation terminal is not part of the network");
    }

    return checkValidationLevel;
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::CReference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ValidationLevel& vl) {
    return checkRatioTapChangerRegulation(validable, regulating, loadTapChangingCapabilities, regulationTerminal, regulationMode, regulationValue, network, checkValidationActionOnError(vl));
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::Reference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;

    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulationWithoutTerminal(validable, regulating, loadTapChangingCapabilities, regulationMode, regulationValue, action));

    if (regulating && !regulationTerminal) {
        actionOnError(validable, "a regulation terminal has to be set for a regulating ratio tap changer", action);
        checkValidationLevel = validationLevel::min(checkValidationLevel, ValidationLevel::EQUIPMENT);
    }
    if (regulationTerminal && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "regulation terminal is not part of the network");
    }
    
    return checkValidationLevel;
}
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::Reference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ValidationLevel& vl) {
    return checkRatioTapChangerRegulation(validable, regulating, loadTapChangingCapabilities, regulationTerminal, regulationMode, regulationValue, network, checkValidationActionOnError(vl));
}

void checkRegulatingTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network) {
    if (regulatingTerminal && !stdcxx::areSame(regulatingTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw ValidationException(validable, "Regulating terminal is not part of the network");
    }
}

void checkSameParentNetwork(const std::string& validableNetworkId, const Validable& validable, const DcNode& dcNode) {
    if(validableNetworkId.empty()) {
        throw ValidationException(validable, "invalid empty network Id");
    }
    if(validableNetworkId != dcNode.getParentNetwork().getId()) {
        throw ValidationException(validable, stdcxx::format("DC Node '%1%' is in network '%2%' but DC Equipment is in '%3%'", dcNode.getId(), dcNode.getParentNetwork().getId(), validableNetworkId));
    }
}
void checkSameParentNetwork(const std::string& validableNetworkId, const Validable& validable, const DcNode& dcNode1, const DcNode& dcNode2) {
    if(validableNetworkId.empty()) {
        throw ValidationException(validable, "invalid empty network Id");
    }
    if(dcNode1.getParentNetwork().getId() != dcNode2.getParentNetwork().getId()) {
        throw ValidationException(validable, stdcxx::format("DC Nodes '%1%' and '%2%' are in different networks '%3%' and '%4%'", dcNode1.getId(), dcNode2.getId(), dcNode1.getParentNetwork().getId(), dcNode2.getParentNetwork().getId()));
    }
    if(validableNetworkId != dcNode1.getParentNetwork().getId()) {
        throw ValidationException(validable, stdcxx::format("DC Nodes '%1%' and '%2%' are in network '%3%' but DC Equipment is in '%4%'", dcNode1.getId(), dcNode2.getId(), dcNode1.getParentNetwork().getId(), validableNetworkId));
    }
}

ValidationLevel checkSections(const Validable& validable, const stdcxx::optional<unsigned long>& currentSectionCount, unsigned long maximumSectionCount, const ActionOnError& action) {
    checkMaximumSectionCount(validable, maximumSectionCount);
    if (!currentSectionCount) {
        actionOnError(validable, "the current section count is not set", action);
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
ValidationLevel checkSections(const Validable& validable, const stdcxx::optional<unsigned long>& currentSectionCount, unsigned long maximumSectionCount, const ValidationLevel& vl) {
    return checkSections(validable, currentSectionCount, maximumSectionCount, checkValidationActionOnError(vl));
}

stdcxx::optional<unsigned long> checkSolvedSection(const Validable& validable, const stdcxx::optional<unsigned long>& solvedSectionCount, unsigned long maximumSectionCount) {
    checkMaximumSectionCount(validable, maximumSectionCount);
    if (solvedSectionCount.has_value() && *solvedSectionCount > maximumSectionCount) {
        throw ValidationException(validable, stdcxx::format("unexpected solved section number (%1%): no existing associated section", *solvedSectionCount));
    }
    return solvedSectionCount;
}

void checkSolvedTapPosition(const Validable& validable, long solvedTapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl) {
    if ((solvedTapPosition < lowTapPosition) || (solvedTapPosition > highTapPosition)) {
        actionOnError(validable, stdcxx::format("incorrect solved tap position %1% [%2%, %3%]", solvedTapPosition, lowTapPosition, highTapPosition), checkValidationActionOnError(vl));
    }
}

ValidationLevel checkSvcRegulator(const Validable& validable, bool regulating, double voltageSetpoint, double reactivePowerSetpoint, const StaticVarCompensator::RegulationMode& regulationMode, const ActionOnError& action) {
    switch (regulationMode) {
        case StaticVarCompensator::RegulationMode::VOLTAGE:
            if (regulating && std::isnan(voltageSetpoint)) {
                actionOnErrorForInvalidValue(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, action);
                return ValidationLevel::EQUIPMENT;
            }
            break;
        case StaticVarCompensator::RegulationMode::REACTIVE_POWER:
            if (regulating && std::isnan(reactivePowerSetpoint)) {
                actionOnErrorForInvalidValue(validable, reactivePowerSetpoint, converter::REACTIVE_POWER_SETPOINT, action);
                return ValidationLevel::EQUIPMENT;
            }
            break;
        default:
            throw AssertionError(stdcxx::format("Unexpected regulation mode value: %1%", regulationMode));
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkSvcRegulator(const Validable& validable, bool regulating, double voltageSetpoint, double reactivePowerSetpoint, const StaticVarCompensator::RegulationMode& regulationMode, const ValidationLevel& vl) {
    return checkSvcRegulator(validable, regulating, voltageSetpoint, reactivePowerSetpoint, regulationMode, checkValidationActionOnError(vl));
}

ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ActionOnError& action) {
    if ((tapPosition < lowTapPosition) || (tapPosition > highTapPosition)) {
        actionOnError(validable, stdcxx::format("incorrect tap position %1% [%2%, %3%]", tapPosition, lowTapPosition, highTapPosition), action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl) {
    return checkTapPosition(validable, tapPosition, lowTapPosition, highTapPosition, checkValidationActionOnError(vl));
}

ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ActionOnError& action) {
    if (regulating && std::isnan(targetDeadband)) {
        actionOnError(validable, "Undefined value for target deadband of regulating " + validableType, action);
        return ValidationLevel::EQUIPMENT;
    }
    if (targetDeadband < 0) {
        throw ValidationException(validable, stdcxx::format("Unexpected value for target deadband of tap changer: %1%", targetDeadband));
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ValidationLevel& vl) {
    return checkTargetDeadband(validable, validableType, regulating, targetDeadband, checkValidationActionOnError(vl));
}

ThreeSides checkThreeSides(const Validable& validable, const ThreeSides& side) {
    switch (side) {
    case ThreeSides::ONE:
    case ThreeSides::TWO:
    case ThreeSides::THREE:
        return side;
    case ThreeSides::UNDEFINED:
    default:
        throw ValidationException(validable, stdcxx::format("Unexpected ThreeSides value: %1%", side));
    }
}
TwoSides checkTwoSides(const Validable& validable, const TwoSides& side) {
    switch (side) {
    case TwoSides::ONE:
    case TwoSides::TWO:
        return side;
    case TwoSides::UNDEFINED:
    default:
        throw ValidationException(validable, stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

double checkVoltage(const Validable& validable, double voltage) {
    if (!std::isnan(voltage) && voltage < 0) {
        throw ValidationException(validable, "voltage cannot be < 0");
    }
    return voltage;
}

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, const ActionOnError& action) {
    if (voltageRegulatorOn) {
        if (std::isnan(voltageSetpoint)) {
            actionOnErrorForInvalidValue(validable,voltageSetpoint, converter::VOLTAGE_SETPOINT, action, "voltage regulator is on");
            return ValidationLevel::EQUIPMENT;
        }
        if (voltageSetpoint <= 0) {
            throw createInvalidValueException(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, "voltage regulator is on");
        }
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, const ValidationLevel& vl) {
    return checkVoltageControl(validable, voltageRegulatorOn, voltageSetpoint, checkValidationActionOnError(vl));
}

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint, const ActionOnError& action) {
    if (voltageRegulatorOn) {
        if (std::isnan(voltageSetpoint)) {
            actionOnErrorForInvalidValue(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, action, "voltage regulator is on");
            return ValidationLevel::EQUIPMENT;
        } 
        if (voltageSetpoint <= 0) {
            throw createInvalidValueException(validable, voltageSetpoint, converter::VOLTAGE_SETPOINT, "voltage regulator is on");
        }
    } else if (std::isnan(reactivePowerSetpoint)) {
        actionOnErrorForInvalidValue(validable, reactivePowerSetpoint, converter::REACTIVE_POWER_SETPOINT, action, "voltage regulator is off");
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}
ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint, const ValidationLevel& vl) {
    return checkVoltageControl(validable, voltageRegulatorOn, voltageSetpoint, reactivePowerSetpoint, checkValidationActionOnError(vl));
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


ValidationLevel checkOperationalLimitsGroup(const Validable& validable, const OperationalLimitsGroup& operationalLimitsGroup, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if(operationalLimitsGroup.getCurrentLimits()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkLoadingLimits(validable, operationalLimitsGroup.getOperationalLimits<LoadingLimits>(LimitType::CURRENT).get(), action));
    }
    if(operationalLimitsGroup.getApparentPowerLimits()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkLoadingLimits(validable, operationalLimitsGroup.getOperationalLimits<LoadingLimits>(LimitType::APPARENT_POWER).get(), action));
    }
    if(operationalLimitsGroup.getActivePowerLimits()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkLoadingLimits(validable, operationalLimitsGroup.getOperationalLimits<LoadingLimits>(LimitType::ACTIVE_POWER).get(), action));
    }
    return checkValidationLevel;
}

ValidationLevel checkOperationalLimitsGroups(const Validable& validable, const stdcxx::const_range<OperationalLimitsGroup>& operationalLimitsGroups, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    for (const auto& operationalLimitGroup : operationalLimitsGroups) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroup(validable, operationalLimitGroup, action));
    }
    return checkValidationLevel;
}


ValidationLevel checkRtc(const Validable& validable, const RatioTapChanger& rtc, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkRatioTapChangerRegulation(validable, rtc.isRegulating(), rtc.hasLoadTapChangingCapabilities(), rtc.getRegulationTerminal(), rtc.getRegulationMode(), rtc.getRegulationValue(), network, action));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "ratio tap changer", rtc.isRegulating(), rtc.getTargetDeadband(), action));
    return checkValidationLevel;
}

ValidationLevel checkPtc(const Validable& validable, const PhaseTapChanger& ptc, const Network& network, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkPhaseTapChangerRegulation(validable, ptc.getRegulationMode(), ptc.getRegulationValue(), ptc.isRegulating(), ptc.hasLoadTapChangingCapabilities(), ptc.getRegulationTerminal(), network, action));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "phase tap changer", ptc.isRegulating(), ptc.getTargetDeadband(), action));
    return checkValidationLevel;
}

ValidationLevel checkThreeWindingsTransformer(const Validable& validable, const ThreeWindingsTransformer& twt, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    unsigned long regulatingTc = 0;
    for (const auto& leg : twt.getLegs()){
        if(leg.hasRatioTapChanger()){
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkRtc(validable, leg.getRatioTapChanger(), twt.getNetwork(), action));
            if(leg.getRatioTapChanger().isRegulating()) {
                regulatingTc += 1;
            }
        }
        if(leg.hasPhaseTapChanger()){
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkPtc(validable, leg.getPhaseTapChanger(), twt.getNetwork(), action));
            if(leg.getPhaseTapChanger().isRegulating()) {
                regulatingTc += 1;
            }
        }
    }
    if(regulatingTc > 0) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkOnlyOneTapChangerRegulatingEnabled(validable, regulatingTc, true, action));
    }
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, twt.getLeg1().getOperationalLimitsGroups(), action));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, twt.getLeg2().getOperationalLimitsGroups(), action));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, twt.getLeg3().getOperationalLimitsGroups(), action));
    return checkValidationLevel;
}

ValidationLevel checkTwoWindingsTransformer(const Validable& validable, const TwoWindingsTransformer& twt, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    unsigned long regulatingTc = 0;
    if (twt.hasRatioTapChanger()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkRtc(validable, twt.getRatioTapChanger(), twt.getNetwork(), action));
        if(twt.getRatioTapChanger().isRegulating()) {
            regulatingTc += 1;
        }
    }
    if (twt.hasPhaseTapChanger()) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkPtc(validable, twt.getPhaseTapChanger(), twt.getNetwork(), action));
        if(twt.getPhaseTapChanger().isRegulating()) {
            regulatingTc += 1;
        }
    }
    if (regulatingTc > 0) {
        checkValidationLevel = validationLevel::min(checkValidationLevel, checkOnlyOneTapChangerRegulatingEnabled(validable, regulatingTc, true, action));
    }
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, twt.getOperationalLimitsGroups1(), action));
    checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, twt.getOperationalLimitsGroups2(), action));
    return checkValidationLevel;
}

ValidationLevel checkIdentifiable(const Identifiable& identifiable,const ValidationLevel& previous, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = previous;
    if (stdcxx::isInstanceOf<Validable>(identifiable)) {
        const auto& validable = dynamic_cast<const Validable&>(identifiable);
        if (stdcxx::isInstanceOf<Battery>(identifiable)) {
            const auto& battery = dynamic_cast<const Battery&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, battery.getTargetP(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, battery.getTargetQ(), action));
        } else if (stdcxx::isInstanceOf<DanglingLine>(identifiable)) {
            const auto& danglingLine = dynamic_cast<const DanglingLine&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, danglingLine.getP0(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, danglingLine.getQ0(), action));
            const auto& generation = danglingLine.getGeneration();
            if (generation) {
                checkValidationLevel = validationLevel::min(checkValidationLevel, checkActivePowerSetpoint(validable, generation.get().getTargetP(), action));
                checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, generation.get().isVoltageRegulationOn(), generation.get().getTargetV(), generation.get().getTargetQ(), action));
            }
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, danglingLine.getOperationalLimitsGroups(), action));
        } else if (stdcxx::isInstanceOf<Generator>(identifiable)) {
            const auto& generator = dynamic_cast<const Generator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkActivePowerSetpoint(validable, generator.getTargetP(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, generator.isVoltageRegulatorOn(), generator.getTargetV(), generator.getTargetQ(), action));
        } else if (stdcxx::isInstanceOf<HvdcLine>(identifiable)) {
            const auto& hvdcLine = dynamic_cast<const HvdcLine&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkConvertersMode(validable, hvdcLine.getConvertersMode(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkHvdcActivePowerSetpoint(validable, hvdcLine.getActivePowerSetpoint(), action));
        } else if (stdcxx::isInstanceOf<Load>(identifiable)) {
            const auto& load = dynamic_cast<const Load&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkP0(validable, load.getP0(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkQ0(validable, load.getQ0(), action));
        } else if (stdcxx::isInstanceOf<ShuntCompensator>(identifiable)) {
            const auto& shunt = dynamic_cast<const ShuntCompensator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, shunt.isVoltageRegulatorOn(), shunt.getTargetV(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkTargetDeadband(validable, "shunt compensator", shunt.isVoltageRegulatorOn(), shunt.getTargetDeadband(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkSections(validable, shunt.getSectionCount(), shunt.getMaximumSectionCount(), action));
        } else if (stdcxx::isInstanceOf<StaticVarCompensator>(identifiable)) {
            const auto& svc = dynamic_cast<const StaticVarCompensator&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkSvcRegulator(validable, svc.isRegulating(), svc.getVoltageSetpoint(), svc.getReactivePowerSetpoint(), svc.getRegulationMode(), action));
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)) {
            const auto& threewt = dynamic_cast<const ThreeWindingsTransformer&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkThreeWindingsTransformer(validable, threewt, action));
        } else if (stdcxx::isInstanceOf<TwoWindingsTransformer>(identifiable)) {
            const auto& twowt = dynamic_cast<const TwoWindingsTransformer&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkTwoWindingsTransformer(validable, twowt, action));
        } else if (stdcxx::isInstanceOf<VscConverterStation>(identifiable)) {
            const auto& converterStation = dynamic_cast<const VscConverterStation&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkVoltageControl(validable, converterStation.isVoltageRegulatorOn(), converterStation.getVoltageSetpoint(), converterStation.getReactivePowerSetpoint(), action));
        } else if ( stdcxx::isInstanceOf<Branch>(identifiable)) {
            const auto& branch = dynamic_cast<const Branch&>(identifiable);
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, branch.getOperationalLimitsGroups1(), action));
            checkValidationLevel = validationLevel::min(checkValidationLevel, checkOperationalLimitsGroups(validable, branch.getOperationalLimitsGroups2(), action));
        }
    }
    return checkValidationLevel;
}

ValidationLevel validateIdentifiables(const stdcxx::const_range<Identifiable>& identifiables, bool allChecks, const ValidationLevel& previous, const ActionOnError& action) {
    ValidationLevel checkValidationLevel = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if(previous >= ValidationLevel::STEADY_STATE_HYPOTHESIS) {
        return previous;
    }
    for (const auto& identifiable : identifiables) {
        checkValidationLevel = checkIdentifiable(identifiable, checkValidationLevel, action);
        if(!allChecks && checkValidationLevel == ValidationLevel::EQUIPMENT){
            return checkValidationLevel;
        }
    }
    return checkValidationLevel;
}


}  // namespace iidm

}  // namespace powsybl
