/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATEARRAY_HXX
#define POWSYBL_IIDM_STATEARRAY_HXX

#include <powsybl/iidm/StateManager.hpp>

#include "StateArray.hpp"

namespace powsybl {

namespace iidm {

template <typename T>
StateArray<T>::StateArray(MultiStateObject& multiStateObject, const StateFactory& stateFactory) :
    m_multiStateObject(multiStateObject) {

    const auto& stateManager = multiStateObject.getStateManager();
    m_states.resize(stateManager.getStateArraySize());
    for (unsigned long i : stateManager.getStateIndices()) {
        m_states.at(i) = stateFactory();
    }
}

template <typename T>
void StateArray<T>::allocateStateArrayElement(const std::set<unsigned long>& indexes, const StateFactory& stateFactory) {
    for (auto index : indexes) {
        m_states[index] = stateFactory();
    }
}

template <typename T>
std::unique_ptr<T> StateArray<T>::copy(unsigned long index) const {
    return m_states[index]->copy();
}

template <typename T>
void StateArray<T>::deleteStateArrayElement(unsigned long index) {
    m_states[index].reset();
}

template <typename T>
void StateArray<T>::extendStateArraySize(unsigned long /*initStateArraySize*/, unsigned long number, const StateFactory& stateFactory) {
    for (unsigned long i = 0; i < number; ++i) {
        m_states.emplace_back(std::move(stateFactory()));
    }
}

template <typename T>
const T& StateArray<T>::get() const {
    return *m_states.at(m_multiStateObject.getStateIndex());
}

template <typename T>
T& StateArray<T>::get() {
    return *m_states.at(m_multiStateObject.getStateIndex());
}

template <typename T>
void StateArray<T>::reduceStateArraySize(unsigned long number) {
    m_states.resize(m_states.size() - number);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATEARRAY_HXX
