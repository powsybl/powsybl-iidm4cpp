/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CalculatedBus.hpp"

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

CalculatedBus::CalculatedBus(const std::string& id, NodeBreakerVoltageLevel& voltageLevel, const std::vector<unsigned long>& nodes, std::vector<std::reference_wrapper<NodeTerminal> >&& terminals) :
    Bus(id),
    m_voltageLevel(voltageLevel),
    m_valid(true),
    m_terminals(std::move(terminals)),
    m_terminalRef(findTerminal(voltageLevel, nodes, m_terminals)) {
}

void CalculatedBus::checkValidity() const {
    if (!m_valid) {
        throw PowsyblException("Bus has been invalidated");
    }
}

stdcxx::CReference<NodeTerminal> CalculatedBus::findTerminal(const NodeBreakerVoltageLevel& voltageLevel, const std::vector<unsigned long>& nodes, const std::vector<std::reference_wrapper<NodeTerminal> >& terminals) {
    if (!terminals.empty()) {
        return stdcxx::cref<NodeTerminal>(terminals.front().get());
    }

    stdcxx::Reference<Terminal> terminal;

    NodeBreakerView::Traverser traverser = [&terminal, &voltageLevel](unsigned long /*node1*/, const stdcxx::Reference<Switch>& sw, unsigned long node2) {
        if (static_cast<bool>(sw) && sw.get().isOpen()) {
            return false;
        }
        terminal = voltageLevel.getNodeBreakerView().getTerminal(node2);
        return !static_cast<bool>(terminal);
    };
    voltageLevel.getNodeBreakerView().traverse(nodes.front(), traverser);

    return static_cast<bool>(terminal) ? stdcxx::cref<NodeTerminal>(dynamic_cast<NodeTerminal&>(terminal.get())) : stdcxx::CReference<NodeTerminal>();
}

double CalculatedBus::getAngle() const {
    checkValidity();

    return static_cast<bool>(m_terminalRef) ? m_terminalRef.get().getAngle() : stdcxx::nan();
}

unsigned long CalculatedBus::getConnectedTerminalCount() const {
    checkValidity();

    return m_terminals.size();
}

bus::Terminals CalculatedBus::getConnectedTerminals() const {
    checkValidity();

    std::vector<std::reference_wrapper<Terminal> > terminals;
    terminals.reserve(m_terminals.size());

    for (auto& terminal : m_terminals) {
        terminals.emplace_back(std::ref<Terminal>(terminal));
    }

    return terminals;
}

double CalculatedBus::getV() const {
    checkValidity();

    return static_cast<bool>(m_terminalRef) ? m_terminalRef.get().getV() : stdcxx::nan();
}

VoltageLevel& CalculatedBus::getVoltageLevel() const {
    return m_voltageLevel;
}

void CalculatedBus::invalidate() {
    m_valid = false;
    m_voltageLevel.reset();
    m_terminals.clear();
    m_terminalRef.reset();
}

Bus& CalculatedBus::setAngle(double angle) {
    checkValidity();
    for (auto& terminal : m_terminals) {
        terminal.get().setAngle(angle);
    }

    return *this;
}

Bus& CalculatedBus::setV(double v) {
    checkValidity();
    for (auto& terminal : m_terminals) {
        terminal.get().setV(v);
    }

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
