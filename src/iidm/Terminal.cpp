/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Terminal.hpp>

#include <cmath>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "BusTerminal.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

Terminal::Terminal(VariantManagerHolder& network) :
    m_network(network),
    m_p(network.getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_q(network.getVariantManager().getVariantArraySize(), stdcxx::nan()) {

}

void Terminal::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_p[index] = m_p[sourceIndex];
        m_q[index] = m_q[sourceIndex];
    }
}

bool Terminal::connect() {
    return m_voltageLevel->connect(*this);
}

void Terminal::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

bool Terminal::disconnect() {
    return m_voltageLevel->disconnect(*this);
}

void Terminal::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_p.resize(m_p.size() + number, m_p[sourceIndex]);
    m_q.resize(m_q.size() + number, m_q[sourceIndex]);
}

const stdcxx::Reference<Connectable>& Terminal::getConnectable() const {
    return m_connectable;
}

double Terminal::getI() const {
    if (m_connectable->getType() == ConnectableType::BUSBAR_SECTION) {
        return 0;
    }

    return std::hypot(getP(), getQ()) / std::sqrt(3.0) * getV() / 1000.0;
}

const VariantManagerHolder& Terminal::getNetwork() const {
    return m_network.get();
}

double Terminal::getP() const {
    return m_p.at(m_network->getVariantIndex());
}

double Terminal::getQ() const {
    return m_q.at(m_network->getVariantIndex());
}

const VoltageLevel& Terminal::getVoltageLevel() const {
    return m_voltageLevel.get();
}

VoltageLevel& Terminal::getVoltageLevel() {
    return m_voltageLevel.get();
}

void Terminal::reduceVariantArraySize(unsigned long number) {
    m_p.resize(m_p.size() - number);
    m_q.resize(m_q.size() - number);
}

Terminal& Terminal::setConnectable(const stdcxx::Reference<Connectable>& connectable) {
    m_connectable = connectable;

    return *this;
}

Terminal& Terminal::setP(double p) {
    const Connectable& connectable = m_connectable.get();

    if (connectable.getType() == ConnectableType::BUSBAR_SECTION) {
        throw ValidationException(connectable, "cannot set active power on a busbar section");
    }
    if (!std::isnan(p) && connectable.getType() == ConnectableType::SHUNT_COMPENSATOR) {
        throw ValidationException(connectable, "cannot set active power on a shunt compensator");
    }

    m_p[m_network->getVariantIndex()] = p;

    return *this;
}

Terminal& Terminal::setQ(double q) {
    const Connectable& connectable = m_connectable.get();

    if (connectable.getType() == ConnectableType::BUSBAR_SECTION) {
        throw ValidationException(connectable, "cannot set reactive power on a busbar section");
    }

    m_q[m_network->getVariantIndex()] = q;

    return *this;
}

Terminal& Terminal::setVoltageLevel(const stdcxx::Reference<VoltageLevel>& voltageLevel) {
    m_voltageLevel = voltageLevel;

    return *this;
}

std::unique_ptr<Terminal> createBusTerminal(Network& network, const std::string& connectableBusId, bool connected) {
    return stdcxx::make_unique<BusTerminal>(network, connectableBusId, connected);
}

std::unique_ptr<Terminal> createNodeTerminal(Network& network, unsigned long node) {
    return stdcxx::make_unique<NodeTerminal>(network, node);
}

}  // namespace iidm

}  // namespace powsybl
