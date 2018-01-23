/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NodeTerminal.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/StateManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

NodeTerminal::NodeTerminal(MultiStateObject& network, int node) :
    Terminal(network),
    m_node(node),
    m_v(network.getStateManager().getStateArraySize(), stdcxx::nan()),
    m_angle(network.getStateManager().getStateArraySize(), stdcxx::nan()) {

}

void NodeTerminal::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Terminal::allocateStateArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_v[index] = m_v[sourceIndex];
        m_angle[index] = m_angle[sourceIndex];
    }
}

void NodeTerminal::deleteStateArrayElement(unsigned long index) {
    Terminal::deleteStateArrayElement(index);
}

void NodeTerminal::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    Terminal::extendStateArraySize(initStateArraySize, number, sourceIndex);

    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_angle.resize(m_angle.size() + number, m_angle[sourceIndex]);
}

double NodeTerminal::getAngle() const {
    return m_angle[m_network.get().getStateIndex()];
}

int NodeTerminal::getNode() const {
    return m_node;
}

double NodeTerminal::getV() const {
    return m_v[m_network.get().getStateIndex()];
}

void NodeTerminal::reduceStateArraySize(unsigned long number) {
    Terminal::reduceStateArraySize(number);

    m_v.resize(m_v.size() - number);
    m_angle.resize(m_angle.size() - number);

}

NodeTerminal& NodeTerminal::setAngle(double angle) {
    m_angle[m_network.get().getStateIndex()] = angle;

    return *this;
}

NodeTerminal& NodeTerminal::setV(double v) {
    checkVoltage(m_connectable.get(), v);

    m_v[m_network.get().getStateIndex()] = v;

    return *this;
}

}

}

