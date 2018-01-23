/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Terminal.hpp>

#include <cmath>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/StateManager.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

Terminal::Terminal(MultiStateObject& network) :
    m_network(network),
    m_p(network.getStateManager().getStateArraySize(), stdcxx::nan()),
    m_q(network.getStateManager().getStateArraySize(), stdcxx::nan()) {

}

void Terminal::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_p[index] = m_p[sourceIndex];
        m_q[index] = m_q[sourceIndex];
    }
}

bool Terminal::connect() {
    return m_voltageLevel.get().connect(*this);
}

void Terminal::deleteStateArrayElement(unsigned long /*index*/) {

}

bool Terminal::disconnect() {
    return m_voltageLevel.get().disconnect(*this);
}

void Terminal::extendStateArraySize(unsigned long /*initStateArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_p.resize(m_p.size() + number, m_p[sourceIndex]);
    m_q.resize(m_q.size() + number, m_q[sourceIndex]);
}

double Terminal::getI() const {
    if (m_connectable.get().getConnectableType() == ConnectableType::BUSBAR_SECTION) {
        return 0;
    } else {
        return std::hypot(getP(), getQ()) / std::sqrt(3.0) * getV() / 1000.0;
    }
}

double Terminal::getP() const {
    return m_p.at(m_network.get().getStateIndex());
}

double Terminal::getQ() const {
    return m_q.at(m_network.get().getStateIndex());
}

const VoltageLevel& Terminal::getVoltageLevel() const {
    return m_voltageLevel.get();
}

VoltageLevel& Terminal::getVoltageLevel() {
    return m_voltageLevel.get();
}

void Terminal::reduceStateArraySize(unsigned long number) {
    m_p.resize(m_p.size() - number);
    m_q.resize(m_q.size() - number);
}

Terminal& Terminal::setConnectable(const stdcxx::optional_reference_wrapper<Connectable>& connectable) {
    m_connectable = connectable;

    return *this;
}

Terminal& Terminal::setP(double p) {
    const Connectable& connectable = m_connectable.get();

    if (connectable.getConnectableType() == ConnectableType::BUSBAR_SECTION) {
        throw ValidationException(connectable, "cannot set active power on a busbar section");
    }
    if (!std::isnan(p) && connectable.getConnectableType() == ConnectableType::SHUNT_COMPENSATOR) {
        throw ValidationException(connectable, "cannot set active power on a shunt compensator");
    }

    m_p[m_network.get().getStateIndex()] = p;

    return *this;
}

Terminal& Terminal::setQ(double q) {
    const Connectable& connectable = m_connectable.get();

    if (connectable.getConnectableType() == ConnectableType::BUSBAR_SECTION) {
        throw ValidationException(connectable, "cannot set reactive power on a busbar section");
    }

    m_q[m_network.get().getStateIndex()] = q;

    return *this;
}

Terminal& Terminal::setVoltageLevel(const stdcxx::optional_reference_wrapper<VoltageLevel>& voltageLevel) {
    m_voltageLevel = voltageLevel;

    return *this;
}

}

}
