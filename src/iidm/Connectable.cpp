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

Connectable::Connectable(const std::string& id, const std::string& name, bool fictitious, const ConnectableType& connectableType) :
    Identifiable(id, name, fictitious),
    m_connectableType(connectableType) {
}

void Connectable::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto& terminal : m_terminals) {
        terminal->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

Terminal& Connectable::addTerminal(std::unique_ptr<Terminal>&& terminal) {
    m_terminals.emplace_back(std::move(terminal));
    m_terminals.back()->setConnectable(stdcxx::ref(*this));

    return *m_terminals.back();
}

void Connectable::deleteVariantArrayElement(unsigned long index) {
    Identifiable::deleteVariantArrayElement(index);

    for (auto& terminal : m_terminals) {
        terminal->deleteVariantArrayElement(index);
    }
}

void Connectable::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    for (auto& terminal : m_terminals) {
        terminal->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

const ConnectableType& Connectable::getType() const {
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

void Connectable::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    for (auto& terminal : m_terminals) {
        terminal->reduceVariantArraySize(number);
    }
}

void Connectable::remove() {
    Network& network = getNetwork();

    for (auto& terminal : m_terminals) {
        VoltageLevel& voltageLevel = terminal->getVoltageLevel();
        voltageLevel.detach(*terminal);
    }

    network.remove(*this);
}

}  // namespace iidm

}  // namespace powsybl
