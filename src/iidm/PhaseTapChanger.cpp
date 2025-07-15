/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChanger.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

PhaseTapChanger::PhaseTapChanger(PhaseTapChangerHolder& parent, long lowTapPosition, const std::vector<PhaseTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                 long tapPosition, bool regulating, const RegulationMode& regulationMode, double regulationValue, double targetDeadband) :
    TapChanger(parent.getNetwork(), parent, lowTapPosition, steps, regulationTerminal, tapPosition, regulating, targetDeadband, "phase tap changer"),
    m_regulationMode(regulationMode),
    m_regulationValue(parent.getNetwork().getVariantManager().getVariantArraySize(), regulationValue) {
    checkTapPosition(parent, tapPosition, lowTapPosition, getHighTapPosition());
    checkPhaseTapChangerRegulation(parent, regulationMode, regulationValue, regulating, regulationTerminal, parent.getNetwork());
}

void PhaseTapChanger::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    TapChanger::allocateVariantArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_regulationValue[index] = m_regulationValue[sourceIndex];
    }
}

void PhaseTapChanger::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    TapChanger::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_regulationValue.resize(m_regulationValue.size() + number, m_regulationValue[sourceIndex]);
}

const PhaseTapChanger::RegulationMode& PhaseTapChanger::getRegulationMode() const {
    return m_regulationMode;
}

double PhaseTapChanger::getRegulationValue() const {
    return m_regulationValue.at(getNetwork().getVariantIndex());
}

void PhaseTapChanger::reduceVariantArraySize(unsigned long number) {
    TapChanger::reduceVariantArraySize(number);

    m_regulationValue.resize(m_regulationValue.size() - number);
}

void PhaseTapChanger::remove() {
    getParent().setPhaseTapChanger(std::unique_ptr<PhaseTapChanger>());
}

PhaseTapChanger& PhaseTapChanger::setRegulationMode(const RegulationMode& regulationMode) {
    checkPhaseTapChangerRegulation(getParent(), regulationMode, getRegulationValue(), isRegulating(), getRegulationTerminal(), getNetwork());
    m_regulationMode = regulationMode;
    return *this;
}

PhaseTapChanger& PhaseTapChanger::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    checkPhaseTapChangerRegulation(getParent(), m_regulationMode, getRegulationValue(), isRegulating(), regulationTerminal, getNetwork());
    return TapChanger::setRegulationTerminal(regulationTerminal);
}

PhaseTapChanger& PhaseTapChanger::setRegulationValue(double regulationValue) {
    checkPhaseTapChangerRegulation(getParent(), m_regulationMode, regulationValue, isRegulating(), getRegulationTerminal(), getNetwork());
    m_regulationValue[getNetwork().getVariantIndex()] = regulationValue;
    return *this;
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<PhaseTapChanger::RegulationMode>() {
    static std::initializer_list<std::string> s_regulationModeNames {
        "CURRENT_LIMITER",
        "ACTIVE_POWER_CONTROL",
        "FIXED_TAP"
    };
    return s_regulationModeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
