/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>

#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

RatioTapChanger::RatioTapChanger(RatioTapChangerHolder& parent, long lowTapPosition, const std::vector<RatioTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                 bool loadTapChangingCapabilities, long tapPosition, bool regulating, double targetV, double targetDeadband) :
    TapChanger(parent.getNetwork(), parent, lowTapPosition, steps, regulationTerminal, tapPosition, regulating, targetDeadband, "ratio tap changer"),
    m_loadTapChangingCapabilities(loadTapChangingCapabilities),
    m_targetV(parent.getNetwork().getVariantManager().getVariantArraySize(), targetV) {
    checkTapPosition(parent, tapPosition, lowTapPosition, getHighTapPosition(), parent.getNetwork().getMinimumValidationLevel());
    checkRatioTapChangerRegulation(parent, regulating, loadTapChangingCapabilities, regulationTerminal, targetV, parent.getNetwork(), parent.getNetwork().getMinimumValidationLevel());
}

void RatioTapChanger::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    TapChanger::allocateVariantArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_targetV[index] = m_targetV[sourceIndex];
    }
}

void RatioTapChanger::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    TapChanger::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
}

double RatioTapChanger::getTargetV() const {
    return m_targetV.at(getNetwork().getVariantIndex());
}

bool RatioTapChanger::hasLoadTapChangingCapabilities() const {
    return m_loadTapChangingCapabilities;
}

void RatioTapChanger::reduceVariantArraySize(unsigned long number) {
    TapChanger::reduceVariantArraySize(number);

    m_targetV.resize(m_targetV.size() - number);
}

void RatioTapChanger::remove() {
    getParent().setRatioTapChanger(std::unique_ptr<RatioTapChanger>());
}

RatioTapChanger& RatioTapChanger::setLoadTapChangingCapabilities(bool loadTapChangingCapabilities) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), m_loadTapChangingCapabilities, getRegulationTerminal(), getTargetV(), getNetwork(), getNetwork().getMinimumValidationLevel());
    m_loadTapChangingCapabilities = loadTapChangingCapabilities;
    getNetwork().invalidateValidationLevel();
    return *this;
}

RatioTapChanger& RatioTapChanger::setRegulating(bool regulating) {
    checkRatioTapChangerRegulation(getParent(), regulating, m_loadTapChangingCapabilities, getRegulationTerminal(), getTargetV(), getNetwork(), getNetwork().getMinimumValidationLevel());
    getNetwork().invalidateValidationLevel();
    return TapChanger::setRegulating(regulating);
}

RatioTapChanger& RatioTapChanger::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), m_loadTapChangingCapabilities, regulationTerminal, getTargetV(), getNetwork(), getNetwork().getMinimumValidationLevel());
    getNetwork().invalidateValidationLevel();
    return TapChanger::setRegulationTerminal(regulationTerminal);
}

RatioTapChanger& RatioTapChanger::setTargetV(double targetV) {
    checkRatioTapChangerRegulation(getParent(), isRegulating(), m_loadTapChangingCapabilities, getRegulationTerminal(), targetV, getNetwork(), getNetwork().getMinimumValidationLevel());
    m_targetV[getNetwork().getVariantIndex()] = targetV;
    getNetwork().invalidateValidationLevel();
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
