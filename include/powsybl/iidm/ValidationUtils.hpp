/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONUTILS_HPP
#define POWSYBL_IIDM_VALIDATIONUTILS_HPP

#include <string>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

enum class LoadType : unsigned int;
class Network;
class Terminal;

void checkActivePowerLimits(const Validable& validable, double minP, double maxP);

void checkActivePowerLimits(const Validable& validable, double minP, double maxP, double p);

double checkActivePowerSetpoint(const Validable& validable, double activePowerSetpoint);

double checkB(const Validable& validable, double b);

double checkB1(const Validable& validable, double b1);

double checkB2(const Validable& validable, double b2);

double checkBmax(const Validable& validable, double bMax);

double checkBmin(const Validable& validable, double bMin);

double checkbPerSection(const Validable& validable, double bPerSection);

const HvdcLine::ConvertersMode& checkConvertersMode(const Validable& validable, const HvdcLine::ConvertersMode& converterMode);

int checkForecastDistance(const Validable& validable, int forecastDistance);

double checkG(const Validable& validable, double g);

double checkG1(const Validable& validable, double g1);

double checkG2(const Validable& validable, double g2);

void checkHalf(const Validable& validable, const TieLine::HalfLine& half, int num);

double checkHvdcActivePowerSetpoint(const Validable& validable, double activePowerSetpoint);

double checkHvdcMaxP(const Validable& validable, double maxP);

double checkLinearBPerSection(const Validable& validable, double bPerSection);

const LoadType& checkLoadType(const Validable& validable, const LoadType& loadType);

double checkLossFactor(const Validable& validable, double lossFactor);

unsigned long checkMaximumSectionCount(const Validable& validable, const stdcxx::optional<unsigned long>& maximumSectionCount);

double checkMaxP(const Validable& validable, double maxP);

double checkMinP(const Validable& validable, double minP);

double checkNominalVoltage(const Validable& validable, double nominalVoltage);

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

void checkOnlyOneTapChangerRegulatingEnabled(const Validable& validable, unsigned long regulatingTapChangerCount, bool regulating);

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

double checkPermanentLimit(const Validable& validable, double permanentLimit);

void checkPhaseTapChangerRegulation(const Validable& validable, const PhaseTapChanger::RegulationMode& regulationMode, double regulationValue, bool regulating,
                                    const stdcxx::Reference<Terminal>& regulationTerminal, const Network& network);

double checkPowerFactor(const Validable& validable, double powerFactor);

double checkQ0(const Validable& validable, double q0);

double checkR(const Validable& validable, double r);

double checkRatedS(const Validable& validable, double ratedS);

double checkRatedU(const Validable& validable, double ratedU, const stdcxx::optional<unsigned long>& num = stdcxx::optional<unsigned long>());

double checkRatedU1(const Validable& validable, double ratedU1);

double checkRatedU2(const Validable& validable, double ratedU2);

void checkRatioTapChangerRegulation(const Validable& validable, bool regulating, const stdcxx::Reference<Terminal>& regulationTerminal, double targetV, const Network& network);

void checkRegulatingTerminal(const Validable& validable, const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network);

void checkSections(const Validable& validable, unsigned long currentSectionCount, unsigned long maximumSectionCount);

void checkSvcRegulator(const Validable& validable, double voltageSetpoint, double reactivePowerSetpoint, const stdcxx::optional<StaticVarCompensator::RegulationMode>& regulationMode);

long checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition);

double checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband);

double checkVoltage(const Validable& validable, double voltage);

bool checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint);

void checkVoltageControl(const Validable& validable, bool voltageRegulatorOn, double voltageSetpoint, double reactivePowerSetpoint);

void checkVoltageLimits(const Validable& validable, double lowVoltageLimit, double highVoltageLimit);

double checkX(const Validable& validable, double x);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONUTILS_HPP
