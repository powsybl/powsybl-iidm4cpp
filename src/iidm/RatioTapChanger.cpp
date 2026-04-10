/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>

#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerStepsReplacer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

RatioTapChanger::RatioTapChanger(RatioTapChangerHolder& parent, long lowTapPosition, const std::vector<RatioTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                 bool loadTapChangingCapabilities, long tapPosition, bool regulating, const RegulationMode& regulationMode, double regulationValue, double targetDeadband) :
    TapChanger(parent.getNetwork(), parent, lowTapPosition, steps, regulationTerminal, loadTapChangingCapabilities, tapPosition, regulating, targetDeadband, "ratio tap changer"),
    m_regulationMode(regulationMode),
    m_regulationValue(parent.getNetwork().getVariantManager().getVariantArraySize(), regulationValue) {
    checkTapPosition(parent, tapPosition, lowTapPosition, getHighTapPosition(), parent.getNetwork().getMinimumValidationLevel());
    checkRatioTapChangerRegulation(parent, regulating, loadTapChangingCapabilities, regulationTerminal, regulationMode, regulationValue, parent.getNetwork(), parent.getNetwork().getMinimumValidationLevel());
}

void RatioTapChanger::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    TapChanger::allocateVariantArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_regulationValue[index] = m_regulationValue[sourceIndex];
    }
}

void RatioTapChanger::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    TapChanger::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_regulationValue.resize(m_regulationValue.size() + number, m_regulationValue[sourceIndex]);
}

double RatioTapChanger::getTargetV() const {
    if(m_regulationMode != RegulationMode::VOLTAGE) {
        return stdcxx::nan();
    }
    return getRegulationValue();
}
double RatioTapChanger::getRegulationValue() const {
    return m_regulationValue.at(getNetwork().getVariantIndex());
}

const RatioTapChanger::RegulationMode& RatioTapChanger::getRegulationMode() const {
    return m_regulationMode;
}

void RatioTapChanger::reduceVariantArraySize(unsigned long number) {
    TapChanger::reduceVariantArraySize(number);

    m_regulationValue.resize(m_regulationValue.size() - number);
}

void RatioTapChanger::remove() {
    TapChanger::remove();
    getParent().setRatioTapChanger(std::unique_ptr<RatioTapChanger>());
}

RatioTapChanger& RatioTapChanger::setRegulating(bool regulating) {
    checkRatioTapChangerRegulation(getParent(), regulating, hasLoadTapChangingCapabilities(), getRegulationTerminal(), getRegulationMode(), getRegulationValue(), getNetwork(), getNetwork().getMinimumValidationLevel());
    getNetwork().invalidateValidationLevel();
    return TapChanger::setRegulating(regulating);
}

RatioTapChanger& RatioTapChanger::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), hasLoadTapChangingCapabilities(), regulationTerminal, getRegulationMode(), getRegulationValue(), getNetwork(), getNetwork().getMinimumValidationLevel());
    getNetwork().invalidateValidationLevel();
    return TapChanger::setRegulationTerminal(regulationTerminal);
}

RatioTapChanger& RatioTapChanger::setLoadTapChangingCapabilities(bool loadTapChangingCapabilities) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), loadTapChangingCapabilities, getRegulationTerminal(), getRegulationMode(), getRegulationValue(), getNetwork(), getNetwork().getMinimumValidationLevel());
    m_loadTapChangingCapabilities[getNetwork().getVariantIndex()] = loadTapChangingCapabilities;
    getNetwork().invalidateValidationLevel();
    return *this;
}

RatioTapChanger& RatioTapChanger::setTargetV(double targetV) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), hasLoadTapChangingCapabilities(), getRegulationTerminal(), RegulationMode::VOLTAGE, targetV, getNetwork(), getNetwork().getMinimumValidationLevel());
    m_regulationValue[getNetwork().getVariantIndex()] = targetV;
    m_regulationMode = RegulationMode::VOLTAGE;
    getNetwork().invalidateValidationLevel();
    return *this;
}

RatioTapChanger& RatioTapChanger::setRegulationValue(double regulationValue) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), hasLoadTapChangingCapabilities(), getRegulationTerminal(), getRegulationMode(), regulationValue, getNetwork(), getNetwork().getMinimumValidationLevel());
    m_regulationValue[getNetwork().getVariantIndex()] = regulationValue;
    getNetwork().invalidateValidationLevel();
    return *this;
}

RatioTapChanger& RatioTapChanger::setRegulationMode(const RegulationMode& regulationMode) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), hasLoadTapChangingCapabilities(), getRegulationTerminal(), regulationMode, getRegulationValue(), getNetwork(), getNetwork().getMinimumValidationLevel());
    m_regulationMode = regulationMode;
    getNetwork().invalidateValidationLevel();
    return *this;
}

RatioTapChangerStepsReplacer RatioTapChanger::stepsReplacer() {
    return RatioTapChangerStepsReplacer(*this);
}

stdcxx::optional<long> RatioTapChanger::getRelativeNeutralPosition() const {
    for (auto step : getAllSteps()) {
        if(step.second.get().getRho() == 1.0) {
            return stdcxx::optional<long>(step.first - getLowTapPosition());
        }
    }
    return stdcxx::optional<long>();
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<RatioTapChanger::RegulationMode>() {
    static std::initializer_list<std::string> s_rtcRegulationModeNames {
        "VOLTAGE",
        "REACTIVE_POWER"
    };
    return s_rtcRegulationModeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
