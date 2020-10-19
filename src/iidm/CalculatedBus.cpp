/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CalculatedBus.hpp"

#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SynchronousComponentsManager.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

CalculatedBus::CalculatedBus(const std::string& id, const std::string& name, bool fictitious, NodeBreakerVoltageLevel& voltageLevel, const std::vector<unsigned long>& nodes, std::vector<std::reference_wrapper<NodeTerminal> >&& terminals) :
    Bus(id, name, fictitious),
    m_voltageLevel(voltageLevel),
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

    stdcxx::CReference<Terminal> terminal;

    VoltageLevel::NodeBreakerView::Traverser traverser = [&terminal, &voltageLevel](unsigned long /*node1*/, const stdcxx::Reference<Switch>& sw, unsigned long node2) {
        if (terminal) {
            return false;
        }

        if (static_cast<bool>(sw) && sw.get().isOpen()) {
            return false;
        }
        terminal = voltageLevel.getNodeBreakerView().getTerminal(node2);
        return !static_cast<bool>(terminal);
    };
    voltageLevel.getNodeBreakerView().traverse(nodes.front(), traverser);

    return stdcxx::cref<NodeTerminal>(terminal);
}

double CalculatedBus::getAngle() const {
    checkValidity();

    return static_cast<bool>(m_terminalRef) ? m_terminalRef.get().getAngle() : stdcxx::nan();
}

stdcxx::CReference<Component> CalculatedBus::getConnectedComponent() const {
    checkValidity();
    ConnectedComponentsManager& ccm = m_voltageLevel.get().getNetwork().getConnectedComponentsManager();
    ccm.update();
    return m_terminalRef ? stdcxx::cref<Component, ConnectedComponent>(ccm.getComponent(m_terminalRef.get().getConnectedComponentNumber())) : stdcxx::cref<Component>();
}

stdcxx::Reference<Component> CalculatedBus::getConnectedComponent() {
    return stdcxx::ref(static_cast<const CalculatedBus*>(this)->getConnectedComponent());
}

unsigned long CalculatedBus::getConnectedTerminalCount() const {
    checkValidity();

    return m_terminals.size();
}

stdcxx::const_range<Terminal> CalculatedBus::getConnectedTerminals() const {
    checkValidity();

    const auto& mapper = stdcxx::map<std::reference_wrapper<NodeTerminal>, Terminal>;

    return m_terminals | boost::adaptors::transformed(mapper);
}

stdcxx::range<Terminal> CalculatedBus::getConnectedTerminals() {
    checkValidity();

    const auto& mapper = stdcxx::map<std::reference_wrapper<NodeTerminal>, Terminal>;

    return m_terminals | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Component> CalculatedBus::getSynchronousComponent() const {
    checkValidity();
    SynchronousComponentsManager& scm = m_voltageLevel.get().getNetwork().getSynchronousComponentsManager();
    scm.update();
    return m_terminalRef ? stdcxx::cref<Component, SynchronousComponent>(scm.getComponent(m_terminalRef.get().getSynchronousComponentNumber())) : stdcxx::cref<Component>();
}

stdcxx::Reference<Component> CalculatedBus::getSynchronousComponent() {
    return stdcxx::ref(static_cast<const CalculatedBus*>(this)->getSynchronousComponent());
}

double CalculatedBus::getV() const {
    checkValidity();

    return static_cast<bool>(m_terminalRef) ? m_terminalRef.get().getV() : stdcxx::nan();
}

const VoltageLevel& CalculatedBus::getVoltageLevel() const {
    return m_voltageLevel;
}

VoltageLevel& CalculatedBus::getVoltageLevel() {
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

void CalculatedBus::setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber) {
    checkValidity();
    for (auto terminal : m_terminals) {
        terminal.get().setConnectedComponentNumber(connectedComponentNumber);
    }
}

void CalculatedBus::setSynchronousComponentNumber(const stdcxx::optional<unsigned long>& componentNumber) {
    checkValidity();
    for (auto terminal : m_terminals) {
        terminal.get().setSynchronousComponentNumber(componentNumber);
    }
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
