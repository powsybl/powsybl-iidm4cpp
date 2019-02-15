/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/logging/MessageFormat.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace logging {

/**
 * toString template specialization for RegulationMode
 */
template <>
std::string toString(const iidm::PhaseTapChanger::RegulationMode& value) {
    static std::array<std::string, 3> s_regulationModeNames {{
        "CURRENT_LIMITER",
        "ACTIVE_POWER_CONTROL",
        "FIXED_TAP"
    }};

    return toString(s_regulationModeNames, value);
}

}  // namespace logging

namespace iidm {

PhaseTapChanger::PhaseTapChanger(TwoWindingsTransformer& parent, long lowTapPosition, const std::vector<PhaseTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                 long tapPosition, bool regulating, const RegulationMode& regulationMode, double regulationValue) :
    TapChanger(parent.getNetwork(), parent, lowTapPosition, steps, regulationTerminal, tapPosition, regulating),
    m_regulationMode(regulationMode),
    m_regulationValue(parent.getNetwork().getVariantManager().getVariantArraySize(), regulationValue) {
    checkTapPosition(parent, tapPosition, lowTapPosition, getHighTapPosition());
    checkPhaseTapChangerRegulation(parent, regulationMode, regulationValue, regulating, regulationTerminal, getNetwork());
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
    return getParent().setPhaseTapChanger(std::unique_ptr<PhaseTapChanger>());
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

std::ostream& operator<<(std::ostream& stream, const PhaseTapChanger::RegulationMode& mode) {
    stream << logging::toString(mode);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
