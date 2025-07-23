/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

enum class LoadType : std::uint8_t;
class Network;
class Terminal;

PowsyblException createUndefinedValueGetterException();

PowsyblException createUnsetMethodException();

void throwExceptionOrLogError(const Validable& validable, const std::string& message, const ValidationLevel& vl);

void checkActivePowerLimits(const Validable& validable, double minP, double maxP);

ValidationLevel checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl);

double checkB(const Validable& validable, double b);

double checkB1(const Validable& validable, double b1);

double checkB2(const Validable& validable, double b2);

double checkBmax(const Validable& validable, double bMax);

double checkBmin(const Validable& validable, double bMin);

double checkBPerSection(const Validable& validable, double bPerSection);

ValidationLevel checkConvertersMode(const Validable& validable, const HvdcLine::ConvertersMode& converterMode, const ValidationLevel& vl);

int checkForecastDistance(const Validable& validable, int forecastDistance);

double checkG(const Validable& validable, double g);

double checkG1(const Validable& validable, double g1);

double checkG2(const Validable& validable, double g2);

ValidationLevel checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint, const ValidationLevel& vl);

double checkHvdcMaxP(const Validable& validable, double maxP);

const LoadType& checkLoadType(const Validable& validable, const LoadType& loadType);

double checkLossFactor(const Validable& validable, double lossFactor);

unsigned long checkMaximumSectionCount(const Validable& validable, const stdcxx::optional<unsigned long>& maximumSectionCount);

double checkMaxP(const Validable& validable, double maxP);

double checkMinP(const Validable& validable, double minP);

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

const ValidationLevel& checkMinValidationLevel(const Validable& validable, const ValidationLevel& minValidationLevel); 

ValidationLevel checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating, const ValidationLevel& vl);

template <typename T>
bool checkOptional(const stdcxx::optional<T>& value) {
    return static_cast<bool>(value);
}

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message);
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message, const ValidationLevel& vl);

template <typename T>
const T& checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message) {
    if (!value) {
        throw ValidationException(validable, message);
    }
    return *value;
}

template <typename T>
ValidationLevel checkOptional(const Validable& validable, const stdcxx::optional<T>& value, const std::string& message, const ValidationLevel& vl) {
    if (!value) {
        throwExceptionOrLogError(validable, message, vl);
        return ValidationLevel::EQUIPMENT;
    }
    return ValidationLevel::STEADY_STATE_HYPOTHESIS;
}

ValidationLevel checkP0(const Validable& validable, double p0, const ValidationLevel& vl);

double checkPermanentLimit(const Validable& validable, double permanentLimit);

ValidationLevel checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating,
                                    const stdcxx::CReference<Terminal>& regulationTerminal, const Network& network, const ValidationLevel& vl);

double checkPowerFactor(const Validable& validable, double powerFactor);

ValidationLevel checkQ0(const Validable& validable, double q0, const ValidationLevel& vl);

double checkR(const Validable& validable, double r);

double checkRatedS(const Validable& validable, double ratedS);

double checkRatedU(const Validable& validable, double ratedU, const stdcxx::optional<unsigned long>& num = stdcxx::optional<unsigned long>());

double checkRatedU1(const Validable& validable, double ratedU1);

double checkRatedU2(const Validable& validable, double ratedU2);

ValidationLevel checkRatioTapChangerRegulation(const Validable& validable, bool regulating, bool loadTapChangingCapabilities, const stdcxx::CReference<Terminal>& regulationTerminal, double targetV, const Network& network, const ValidationLevel& vl);

void checkRegulatingTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network);

ValidationLevel checkSections(const Validable& validable, const stdcxx::optional<unsigned long>& currentSectionCount, unsigned long maximumSectionCount, const ValidationLevel& vl);

ValidationLevel checkSvcRegulator(const Validable& validable, double voltageSetpoint, double reactivePowerSetpoint, const stdcxx::optional<StaticVarCompensator::RegulationMode>& regulationMode, const ValidationLevel& vl);

ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl);

ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ValidationLevel& vl);

double checkVoltage(const Validable& validable, double voltage);

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, const ValidationLevel& vl);

ValidationLevel checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint, const ValidationLevel& vl);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

double checkX(const Validable& validable, double x);

ValidationLevel validateIdentifiables(stdcxx::const_range<Identifiable> identifiables, bool allChecks, const ValidationLevel& previous, const ValidationLevel& vl);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
