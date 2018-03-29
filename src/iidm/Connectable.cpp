/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Connectable.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

Connectable::Connectable(const std::string& id, const std::string& name, const ConnectableType& connectableType) :
    Identifiable(id, name),
    m_connectableType(connectableType) {
}

void Connectable::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto& terminal : m_terminals) {
        terminal->allocateStateArrayElement(indexes, sourceIndex);
    }
}

Terminal& Connectable::addTerminal(std::unique_ptr<Terminal>&& terminal) {
    m_terminals.emplace_back(std::move(terminal));
    m_terminals.back()->setConnectable(stdcxx::optref(*this));

    return *m_terminals.back();
}

void Connectable::deleteStateArrayElement(unsigned long index) {
    for (auto& terminal : m_terminals) {
        terminal->deleteStateArrayElement(index);
    }
}

void Connectable::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    for (auto& terminal : m_terminals) {
        terminal->extendStateArraySize(initStateArraySize, number, sourceIndex);
    }
}

const ConnectableType& Connectable::getConnectableType() const {
    return m_connectableType;
}

const Network& Connectable::getNetwork() const {
    if (m_terminals.empty()) {
        throw PowsyblException(getId() + " is not attached to a network");
    }

    return m_terminals.at(0)->getVoltageLevel().getNetwork();
}

Network& Connectable::getNetwork() {
    return const_cast<Network&>(static_cast<const Connectable*>(this)->getNetwork());
}

const Terminal& Connectable::getTerminal(unsigned long index) const {
    return *m_terminals.at(index);
}

Terminal& Connectable::getTerminal(unsigned long index) {
    return *m_terminals.at(index);
}

std::vector<std::reference_wrapper<Terminal> > Connectable::getTerminals() const {
    std::vector<std::reference_wrapper<Terminal> > terminals;

    for (auto& terminal : m_terminals) {
        terminals.push_back(std::ref(*terminal));
    }

    return terminals;
}

void Connectable::reduceStateArraySize(unsigned long number) {
    for (auto& terminal : m_terminals) {
        terminal->reduceStateArraySize(number);
    }
}

void Connectable::remove() {
    Network& network = getNetwork();
    network.remove(*this);

    for (auto& terminal : m_terminals) {
        VoltageLevel& voltageLevel = terminal->getVoltageLevel();
        voltageLevel.detach(*terminal);
        voltageLevel.clean();
    }
}

}

}
