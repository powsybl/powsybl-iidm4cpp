/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Connectable.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
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

void Connectable::attachTerminal(Terminal& oldTerminal, const std::string& /*oldConnectionInfo*/, VoltageLevel& voltageLevel, std::unique_ptr<Terminal>&& terminal) {
    // first, attach new terminal to connectable and to voltage level of destination, to ensure that the new terminal is valid
    terminal->setConnectable(stdcxx::ref(*this));
    voltageLevel.attach(*terminal, false);

    // then we can detach the old terminal, as we now know that the new terminal is valid
    oldTerminal.getVoltageLevel().detach(oldTerminal);

    // replace the old terminal by the new terminal in the connectable
    auto it = std::find_if(m_terminals.begin(), m_terminals.end(), [&oldTerminal](const std::unique_ptr<Terminal>& term) {
        return stdcxx::areSame(*term, oldTerminal);
    });
    *it = std::move(terminal);
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

    for (const auto& terminal : m_terminals) {
        terminals.push_back(std::ref(*terminal));
    }

    return terminals;
}

void Connectable::move(Terminal& oldTerminal, const std::string& oldConnectionInfo, const std::string& busId, bool connected) {
    const auto& bus = getNetwork().getBusBreakerView().getBus(busId);
    if (!bus) {
        throw PowsyblException(stdcxx::format("Bus '%1%' not found", busId));
    }

    // check bus topology
    if (bus.get().getVoltageLevel().getTopologyKind() != TopologyKind::BUS_BREAKER) {
        throw PowsyblException(stdcxx::format("Trying to move connectable %s to bus %s of voltage level %s, which is a node breaker voltage level",
            getId(), bus.get().getId(), bus.get().getVoltageLevel().getId()));
    }

    // create the new terminal and attach it to the voltage level of the given bus and links it to the connectable
    std::unique_ptr<Terminal> terminalExt = TerminalBuilder(bus.get().getVoltageLevel(), *this)
        .setBus(connected ? bus.get().getId() : "")
        .setConnectableBus(bus.get().getId())
        .build();

    // detach the terminal from its previous voltage level
    attachTerminal(oldTerminal, oldConnectionInfo, bus.get().getVoltageLevel(), std::move(terminalExt));
}

void Connectable::move(Terminal& oldTerminal, const std::string& oldConnectionInfo, unsigned long node, const std::string& voltageLevelId) {
    const auto& voltageLevel = getNetwork().find<VoltageLevel>(voltageLevelId);
    if (! voltageLevel) {
        throw PowsyblException(stdcxx::format("Voltage level '%1%' not found", voltageLevelId));
    }

    // check bus topology
    if (voltageLevel.get().getTopologyKind() != TopologyKind::NODE_BREAKER) {
        const std::string& msg = stdcxx::format("Trying to move connectable %1% to node %2% of voltage level %3%, which is a bus breaker voltage level", getId(), node, voltageLevel.get().getId());
        throw PowsyblException(msg);
    }

    // create the new terminal and attach it to the given voltage level and to the connectable
    std::unique_ptr<Terminal> terminalExt = TerminalBuilder(voltageLevel, *this)
        .setNode(node)
        .build();

    // detach the terminal from its previous voltage level
    attachTerminal(oldTerminal, oldConnectionInfo, voltageLevel, std::move(terminalExt));
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
