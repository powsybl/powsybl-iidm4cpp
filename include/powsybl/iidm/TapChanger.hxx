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

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

ValidationLevel checkTapPosition(const Validable& validable, long tapPosition, long lowTapPosition, long highTapPosition, const ValidationLevel& vl);
ValidationLevel checkTargetDeadband(const Validable& validable, const std::string& validableType, bool regulating, double targetDeadband, const ValidationLevel& vl);

template<typename H, typename C, typename S, typename R>
TapChanger<H, C, S, R>::TapChanger(VariantManagerHolder& network, H& parent, long lowTapPosition, const std::vector<S>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                                long tapPosition, bool regulating, double targetDeadband, std::string&& type) :
   m_parent(parent),
   m_lowTapPosition(lowTapPosition),
   m_steps(steps),
   m_regulationTerminal(regulationTerminal),
   m_tapPosition(network.getVariantManager().getVariantArraySize(), tapPosition),
   m_regulating(network.getVariantManager().getVariantArraySize(), regulating),
   m_targetDeadband(network.getVariantManager().getVariantArraySize(), targetDeadband),
   m_type(std::move(type)) {
    if(static_cast<bool>(regulationTerminal)){
        regulationTerminal.get().registerReferrer(*this);
    }
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_tapPosition[index] = m_tapPosition[sourceIndex];
        m_regulating[index] = m_regulating[sourceIndex];
        m_targetDeadband[index] = m_targetDeadband[sourceIndex];
    }
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::deleteVariantArrayElement(unsigned long /*index*/) {
    // nothing to do
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_tapPosition.resize(m_tapPosition.size() + number, m_tapPosition[sourceIndex]);
    m_regulating.resize(m_regulating.size() + number, m_regulating[sourceIndex]);
    m_targetDeadband.resize(m_targetDeadband.size() + number, m_targetDeadband[sourceIndex]);
}

template<typename H, typename C, typename S, typename R>
std::map<long, std::reference_wrapper<const S>> TapChanger<H, C, S, R>::getAllSteps() const {
    std::map<long, std::reference_wrapper<const S>> allSteps;
    for (unsigned int i = 0U; i < getStepCount(); ++i) {
        allSteps.emplace(std::make_pair(i + getLowTapPosition(), std::cref(m_steps[i])));
    }
    return allSteps;
}

template<typename H, typename C, typename S, typename R>
std::map<long, std::reference_wrapper<S>> TapChanger<H, C, S, R>::getAllSteps() {
    std::map<long, std::reference_wrapper<S>> allSteps;
    for (unsigned int i = 0U; i < getStepCount(); ++i) {
        allSteps.emplace(std::make_pair(i + getLowTapPosition(), std::ref(m_steps[i])));
    }
    return allSteps;
}

template<typename H, typename C, typename S, typename R>
const S& TapChanger<H, C, S, R>::getCurrentStep() const {
    return getStep(getTapPosition());
}

template<typename H, typename C, typename S, typename R>
S& TapChanger<H, C, S, R>::getCurrentStep() {
    return getStep(getTapPosition());
}

template<typename H, typename C, typename S, typename R>
long TapChanger<H, C, S, R>::getHighTapPosition() const {
    return m_lowTapPosition + m_steps.size() - 1;
}

template<typename H, typename C, typename S, typename R>
long TapChanger<H, C, S, R>::getLowTapPosition() const {
    return m_lowTapPosition;
}

template<typename H, typename C, typename S, typename R>
const Network& TapChanger<H, C, S, R>::getNetwork() const {
    return m_parent.getNetwork();
}

template<typename H, typename C, typename S, typename R>
Network& TapChanger<H, C, S, R>::getNetwork() {
    return m_parent.getNetwork();
}

template<typename H, typename C, typename S, typename R>
const H& TapChanger<H, C, S, R>::getParent() const {
    return m_parent;
}

template<typename H, typename C, typename S, typename R>
H& TapChanger<H, C, S, R>::getParent() {
    return m_parent;
}

template<typename H, typename C, typename S, typename R>
stdcxx::CReference<Terminal> TapChanger<H, C, S, R>::getRegulationTerminal() const {
    return stdcxx::cref(m_regulationTerminal);
}

template<typename H, typename C, typename S, typename R>
stdcxx::Reference<Terminal> TapChanger<H, C, S, R>::getRegulationTerminal() {
    return m_regulationTerminal;
}

template<typename H, typename C, typename S, typename R>
const S& TapChanger<H, C, S, R>::getStep(long tapPosition) const {
    checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition(), ValidationLevel::STEADY_STATE_HYPOTHESIS);
    return m_steps[tapPosition - m_lowTapPosition];
}

template<typename H, typename C, typename S, typename R>
S& TapChanger<H, C, S, R>::getStep(long tapPosition) {
    checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition(), ValidationLevel::STEADY_STATE_HYPOTHESIS);
    return m_steps[tapPosition - m_lowTapPosition];
}

template<typename H, typename C, typename S, typename R>
stdcxx::CReference<S> TapChanger<H, C, S, R>::getNeutralStep() const {
    stdcxx::optional<long> neutralPosition = getNeutralPosition();
    return neutralPosition.has_value() ? stdcxx::cref<S>(getStep(*neutralPosition)) : stdcxx::cref<S>();
}

template<typename H, typename C, typename S, typename R>
stdcxx::Reference<S> TapChanger<H, C, S, R>::getNeutralStep() {
    stdcxx::optional<long> neutralPosition = getNeutralPosition();
    return neutralPosition.has_value() ? stdcxx::ref<S>(getStep(*neutralPosition)) : stdcxx::ref<S>();
}

template<typename H, typename C, typename S, typename R>
unsigned int TapChanger<H, C, S, R>::getStepCount() const {
    return m_steps.size();
}

template<typename H, typename C, typename S, typename R>
long TapChanger<H, C, S, R>::getTapPosition() const {
    return m_tapPosition.at(getNetwork().getVariantIndex());
}

template<typename H, typename C, typename S, typename R>
stdcxx::optional<long> TapChanger<H, C, S, R>::getNeutralPosition() const {
    stdcxx::optional<long> relativeNeutralPosition = getRelativeNeutralPosition();
    return relativeNeutralPosition.has_value() ? stdcxx::optional<long>(*relativeNeutralPosition + m_lowTapPosition) : stdcxx::optional<long>();
}

template<typename H, typename C, typename S, typename R>
double TapChanger<H, C, S, R>::getTargetDeadband() const {
    return m_targetDeadband.at(getNetwork().getVariantIndex());
}

template<typename H, typename C, typename S, typename R>
bool TapChanger<H, C, S, R>::isRegulating() const {
    return m_regulating.at(getNetwork().getVariantIndex());
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::reduceVariantArraySize(unsigned long number) {
    m_tapPosition.resize(m_tapPosition.size() - number);
    m_regulating.resize(m_regulating.size() - number);
    m_targetDeadband.resize(m_targetDeadband.size() - number);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setLowTapPosition(long lowTapPosition) {
    long oldValue = m_lowTapPosition;
    m_lowTapPosition = lowTapPosition;
    m_tapPosition[getNetwork().getVariantIndex()] = getTapPosition() + m_lowTapPosition - oldValue;

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setRegulating(bool regulating) {
    checkTargetDeadband(m_parent, m_type, regulating, m_targetDeadband[getNetwork().getVariantIndex()], getNetwork().getMinimumValidationLevel());
    m_regulating[getNetwork().getVariantIndex()] = regulating;
    getNetwork().invalidateValidationLevel();
    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    if (static_cast<bool>(regulationTerminal) && !stdcxx::areSame(regulationTerminal.get().getVoltageLevel().getNetwork(), getNetwork())) {
        throw ValidationException(m_parent, "regulation terminal is not part of the network");
    }

    if(static_cast<bool>(m_regulationTerminal)){
        m_regulationTerminal.get().unregisterReferrer(*this);
    }
    m_regulationTerminal = regulationTerminal;
    if(static_cast<bool>(m_regulationTerminal)){
        m_regulationTerminal.get().registerReferrer(*this);
    }

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setTapPosition(long tapPosition) {
    checkTapPosition(m_parent, tapPosition, m_lowTapPosition, getHighTapPosition(), ValidationLevel::STEADY_STATE_HYPOTHESIS);
    m_tapPosition[getNetwork().getVariantIndex()] = tapPosition;
    getNetwork().invalidateValidationLevel();
    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setTargetDeadband(double targetDeadband) {
    checkTargetDeadband(m_parent, m_type, m_regulating[getNetwork().getVariantIndex()], targetDeadband, getNetwork().getMinimumValidationLevel());
    m_targetDeadband[getNetwork().getVariantIndex()] = targetDeadband;
    getNetwork().invalidateValidationLevel();
    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
C& TapChanger<H, C, S, R>::setSteps(const std::vector<S>& steps) {

    if (steps.empty()) {
        throw ValidationException(m_parent, "a tap changer shall have at least one step");
    }

    long newHighTapPosition = m_lowTapPosition + steps.size() - 1;
    checkTapPosition(m_parent, getTapPosition(), m_lowTapPosition, newHighTapPosition, getNetwork().getMinimumValidationLevel());

    m_steps = steps;
    getNetwork().invalidateValidationLevel();

    return static_cast<C&>(*this);
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::remove() {
    if(static_cast<bool>(m_regulationTerminal)) {
        m_regulationTerminal.get().unregisterReferrer(*this);
    }
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::onReferencedRemoval(Terminal& /*removedReference*/) {
    if(static_cast<bool>(m_regulationTerminal) && static_cast<bool>(m_regulationTerminal.get().getConnectable())) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<TapChanger>();
        logger.warn(stdcxx::format("Connectable %1% was a regulating terminal for tap changer (%2%). Regulation is deactivated",
                        m_regulationTerminal.get().getConnectable().get().getId(), m_parent.getMessageHeader()));
    }
    m_regulationTerminal = stdcxx::Reference<Terminal>();
    m_regulating.assign(m_regulating.size(), false);
}

template<typename H, typename C, typename S, typename R>
void TapChanger<H, C, S, R>::onReferencedReplacement(Terminal& /*oldReference*/, Terminal& newReference) {
    if (!stdcxx::areSame(newReference.getVoltageLevel().getNetwork(), getNetwork())) {
        throw ValidationException(m_parent, "regulation terminal is not part of the network");
    }
    m_regulationTerminal = newReference;
    newReference.registerReferrer(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGER_HXX
