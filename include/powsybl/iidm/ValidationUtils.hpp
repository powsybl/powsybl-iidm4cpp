/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

enum class LoadType : std::uint8_t;
class Network;
class Terminal;

enum class ActionOnError : std::uint8_t {
    THROW_EXCEPTION,
    LOG_ERROR,
    IGNORE
};
std::ostream& operator<<(std::ostream& stream, const ActionOnError& value);

PowsyblException createUndefinedValueGetterException();

PowsyblException createUnsetMethodException();

void actionOnError(const Validable& validable, const std::string& message, const ActionOnError& action);
ActionOnError checkValidationActionOnError(const ValidationLevel& vl);

ValidationLevel checkAcDcConverterControl(const Validable& validable, const AcDcConverter::ControlMode& controlMode, double targetP, double targetVdc, const ValidationLevel& vl);

void checkAcDcConverterPccTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& pccTerminal, const VoltageLevel& voltageLevel);

void checkActivePowerLimits(const Validable& validable, double minP, double maxP);

ValidationLevel checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl);

DcNode& checkAndGetDcNode(Network& network, const Validable& validable, const std::string& dcNodeId, const std::string& attributeName);

double checkB(const Validable& validable, double b);

double checkB1(const Validable& validable, double b1);

double checkB2(const Validable& validable, double b2);

double checkBmax(const Validable& validable, double bMax);

double checkBmin(const Validable& validable, double bMin);

double checkBPerSection(const Validable& validable, double bPerSection);

double checkCoefficient(const Validable& validable, double coefficient);

ValidationLevel checkConvertersMode(const Validable& validable, const HvdcLine::ConvertersMode& converterMode, const ValidationLevel& vl);

double checkDoubleParamPositive(const Validable& validable, double param, const std::string& paramName);

double checkEquivalentLocalTargetV(const Validable& validable, double equivalentLocalTargetV);

double checkExponent(const Validable& validable, double n);

int checkForecastDistance(const Validable& validable, int forecastDistance);

double checkG(const Validable& validable, double g);

double checkG1(const Validable& validable, double g1);

double checkG2(const Validable& validable, double g2);

ValidationLevel checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl);

double checkHvdcMaxP(const Validable& validable, double maxP);

const LineCommutatedConverter::ReactiveModel& checkLccReactiveModel(const Validable& validable, const LineCommutatedConverter::ReactiveModel& reactiveModel);

const LoadType& checkLoadType(const Validable& validable, const LoadType& loadType);

ValidationLevel checkLossFactor(const Validable& validable, double lossFactor, const ValidationLevel& vl);

unsigned long checkMaximumSectionCount(const Validable& validable, const stdcxx::optional<unsigned long>& maximumSectionCount);

double checkMaxP(const Validable& validable, double maxP);

double checkMinP(const Validable& validable, double minP);

const ValidationLevel& checkMinValidationLevel(const Validable& validable, const ValidationLevel& minValidationLevel); 

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

ValidationLevel checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating, const ValidationLevel& vl);

template <typename T>
bool checkOptional(const stdcxx::optional<T>& value) {
    return static_cast<bool>(value);
}

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message);
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ActionOnError& action);
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ValidationLevel& vl);

template <typename T>
const T& checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message) {
    if (!value) {
        throw ValidationException(validable, message);
    }
    return *value;
}

template <typename T>
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message, const ActionOnError& action) {
    if (!value) {
        actionOnError(validable, message, action);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

template <typename T>
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message, const ValidationLevel& vl) {
    return checkOptional(validable, value, message, checkValidationActionOnError(vl));
}

ValidationLevel checkP0(const Validable& validable, double p0, const ValidationLevel& vl);

ValidationLevel checkPermanentLimit(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ValidationLevel& vl);

void checkTemporaryLimits(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits);

ValidationLevel checkLoadingLimits(const Validable& validable, double permanentLimit, const stdcxx::const_range<LoadingLimits::TemporaryLimit>& temporaryLimits, const ValidationLevel& vl);

ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::CReference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl);
ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating, bool loadTapChangingCapabilities,
                                    const stdcxx::Reference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl);

double checkPowerFactor(const Validable& validable, double powerFactor);

double checkPowerFactorPositive(const Validable& validable, double powerFactor);

ValidationLevel checkQ0(const Validable& validable, double q0, const ValidationLevel& vl);

double checkR(const Validable& validable, double r);

double checkRate(const Validable& validable, const std::string& type, double rate, const std::string& attributeName);

double checkRatedS(const Validable& validable, double ratedS);

double checkRatedU(const Validable& validable, double ratedU, const stdcxx::optional<unsigned long>& num = stdcxx::optional<unsigned long>());

double checkRatedU1(const Validable& validable, double ratedU1);

double checkRatedU2(const Validable& validable, double ratedU2);

ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::CReference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ValidationLevel& vl);
ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::Reference<Terminal>& regulationTerminal, const RatioTapChanger::RegulationMode& regulationMode, double regulationValue, const Network& network, const ValidationLevel& vl);

void checkRegulatingTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network);

void checkSameParentNetwork(const std::string& validableNetworkId, const Validable& validable, const DcNode& dcNode);
void checkSameParentNetwork(const std::string& validableNetworkId, const Validable& validable, const DcNode& dcNode1, const DcNode& dcNode2);

ValidationLevel checkSections(const Validable& validable, const stdcxx::optional<unsigned long>& currentSectionCount, unsigned long maximumSectionCount, const ValidationLevel& vl);

stdcxx::optional<unsigned long> checkSolvedSection(const Validable& validable, const stdcxx::optional<unsigned long>& solvedSectionCount, unsigned long maximumSectionCount);

void checkSolvedTapPosition(const Validable& validable, long solvedTapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl);

ValidationLevel checkSvcRegulator(const Validable& validable, bool regulating, double voltageSetpoint, double reactivePowerSetpoint, const StaticVarCompensator::RegulationMode& regulationMode, const ValidationLevel& vl);

ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl);

ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ValidationLevel& vl);

ThreeSides checkThreeSides(const Validable& validable, const ThreeSides& side);
TwoSides checkTwoSides(const Validable& validable, const TwoSides& side);

double checkVoltage(const Validable& validable, double voltage);

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, const ValidationLevel& vl);

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint, const ValidationLevel& vl);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

double checkX(const Validable& validable, double x);

ValidationLevel validateIdentifiables(const stdcxx::const_range<Identifiable>& identifiables, bool allChecks, const ValidationLevel& previous, const ActionOnError& action);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
