/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusTerminal.hpp>

#include <powsybl/iidm/StateManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

BusTerminal::BusTerminal(MultiStateObject& network, const std::string& connectableBusId, bool connected) :
    Terminal(network),
    m_connected(network.getStateManager().getStateArraySize(), connected),
    m_connectableBusId(network.getStateManager().getStateArraySize(), checkNotEmpty(connectableBusId, "ConnectableBusId is required")) {
}

void BusTerminal::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Terminal::allocateStateArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_connected[index] = m_connected[sourceIndex];
        m_connectableBusId[index] = m_connectableBusId[sourceIndex];
    }
}

void BusTerminal::deleteStateArrayElement(unsigned long index) {
    Terminal::deleteStateArrayElement(index);

    m_connectableBusId[index] = "";
}

void BusTerminal::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    Terminal::extendStateArraySize(initStateArraySize, number, sourceIndex);

    m_connected.resize(m_connected.size() + number, m_connected[sourceIndex]);
    m_connectableBusId.resize(m_connectableBusId.size() + number, m_connectableBusId[sourceIndex]);
}

const std::string& BusTerminal::getConnectableBusId() const {
    return m_connectableBusId[m_network.get().getStateIndex()];
}

double BusTerminal::getV() const {
    // TODO: need views
    return 0.0;
}

bool BusTerminal::isConnected() const {
    return m_connected[m_network.get().getStateIndex()];

}

void BusTerminal::reduceStateArraySize(unsigned long number) {
    Terminal::reduceStateArraySize(number);

    m_connected.resize(m_connected.size() - number);
    m_connectableBusId.resize(m_connectableBusId.size() - number);
}

BusTerminal& BusTerminal::setConnectableBusId(const std::string& connectableBusId) {
    m_connectableBusId[m_network.get().getStateIndex()] = checkNotEmpty(connectableBusId, "ConnectableBusId is required");

    return *this;
}

BusTerminal& BusTerminal::setConnected(bool connected) {
    m_connected[m_network.get().getStateIndex()] = connected;

    return *this;
}









}

}