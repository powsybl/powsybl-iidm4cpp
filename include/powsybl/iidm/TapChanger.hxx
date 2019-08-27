/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGER_HXX
#define POWSYBL_IIDM_TAPCHANGER_HXX

#include <powsybl/iidm/TapChanger.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

long checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition);
double checkTargetDeadband(const Validable& validable, double targetDeadband);

template<typename H, typename C, typename S>
TapChanger<H, C, S>::TapChanger(VariantManagerHolder& network, H& parent, long lowTapPosition, const std::vector<S>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                long tapPosition, bool regulating, double targetDeadband) :
   m_network(network),
   m_parent(parent),
   m_lowTapPosition(lowTapPosition),
   m_steps(steps),
   m_regulationTerminal(regulationTerminal),
   m_tapPosition(network.getVariantManager().getVariantArraySize(), tapPosition),
   m_regulating(network.getVariantManager().getVariantArraySize(), regulating),
   m_targetDeadband(network.getVariantManager().getVariantArraySize(), targetDeadband) {
}

template<typename H, typename C, typename S>
void TapChanger<H, C, S>::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_tapPosition[index] = m_tapPosition[sourceIndex];
        m_regulating[index] = m_regulating[sourceIndex];
        m_targetDeadband[index] = m_targetDeadband[sourceIndex];
    }
}

template<typename H, typename C, typename S>
void TapChanger<H, C, S>::deleteVariantArrayElement(unsigned long /*index*/) {
    // nothing to do
}

template<typename H, typename C, typename S>
void TapChanger<H, C, S>::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_tapPosition.resize(m_tapPosition.size() + number, m_tapPosition[sourceIndex]);
    m_regulating.resize(m_regulating.size() + number, m_regulating[sourceIndex]);
    m_targetDeadband.resize(m_targetDeadband.size() + number, m_targetDeadband[sourceIndex]);
}

template<typename H, typename C, typename S>
const S& TapChanger<H, C, S>::getCurrentStep() const {
    return getStep(getTapPosition());
}

template<typename H, typename C, typename S>
S& TapChanger<H, C, S>::getCurrentStep() {
    return getStep(getTapPosition());
}

template<typename H, typename C, typename S>
long TapChanger<H, C, S>::getHighTapPosition() const {
    return m_lowTapPosition + m_steps.size() - 1;
}

template<typename H, typename C, typename S>
long TapChanger<H, C, S>::getLowTapPosition() const {
    return m_lowTapPosition;
}

template<typename H, typename C, typename S>
const Network& TapChanger<H, C, S>::getNetwork() const {
    return m_parent.getNetwork();
}

template<typename H, typename C, typename S>
Network& TapChanger<H, C, S>::getNetwork() {
    return m_parent.getNetwork();
}

template<typename H, typename C, typename S>
const H& TapChanger<H, C, S>::getParent() const {
    return m_parent;
}

template<typename H, typename C, typename S>
H& TapChanger<H, C, S>::getParent() {
    return m_parent;
}

template<typename H, typename C, typename S>
stdcxx::CReference<Terminal> TapChanger<H, C, S>::getRegulationTerminal() const {
    return stdcxx::cref(m_regulationTerminal);
}

template<typename H, typename C, typename S>
stdcxx::Reference<Terminal>& TapChanger<H, C, S>::getRegulationTerminal() {
    return m_regulationTerminal;
}

template<typename H, typename C, typename S>
const S& TapChanger<H, C, S>::getStep(long tapPosition) const {
    checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition());
    return m_steps[tapPosition - m_lowTapPosition];
}

template<typename H, typename C, typename S>
S& TapChanger<H, C, S>::getStep(long tapPosition) {
    checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition());
    return m_steps[tapPosition - m_lowTapPosition];
}

template<typename H, typename C, typename S>
unsigned int TapChanger<H, C, S>::getStepCount() const {
    return m_steps.size();
}

template<typename H, typename C, typename S>
long TapChanger<H, C, S>::getTapPosition() const {
    return m_tapPosition.at(m_network.get().getVariantIndex());
}

template<typename H, typename C, typename S>
double TapChanger<H, C, S>::getTargetDeadband() const {
    return m_targetDeadband.at(m_network.get().getVariantIndex());
}

template<typename H, typename C, typename S>
bool TapChanger<H, C, S>::isRegulating() const {
    return m_regulating.at(m_network.get().getVariantIndex());
}

template<typename H, typename C, typename S>
void TapChanger<H, C, S>::reduceVariantArraySize(unsigned long number) {
    m_tapPosition.resize(m_tapPosition.size() - number);
    m_regulating.resize(m_regulating.size() - number);
    m_targetDeadband.resize(m_targetDeadband.size() - number);
}

template<typename H, typename C, typename S>
C& TapChanger<H, C, S>::setRegulating(bool regulating) {
    m_regulating[m_network.get().getVariantIndex()] = regulating;

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S>
C& TapChanger<H, C, S>::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    if (static_cast<bool>(regulationTerminal) && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), getNetwork())) {
        throw ValidationException(m_parent, "regulation terminal is not part of the network");
    }
    m_regulationTerminal = regulationTerminal;

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S>
C& TapChanger<H, C, S>::setTapPosition(long tapPosition) {
    m_tapPosition[m_network.get().getVariantIndex()] = checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition());

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S>
C& TapChanger<H, C, S>::setTargetDeadband(double targetDeadband) {
    m_targetDeadband[m_network.get().getVariantIndex()] = checkTargetDeadband(m_parent, targetDeadband);

    return static_cast<C&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGER_HXX
