/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevel.hpp"

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "NodeBreakerVoltageLevelBusCache.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

NodeBreakerVoltageLevel::NodeBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                                 double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit),
    m_busNamingStrategy(*this),
    m_states(substation.getNetwork(), [this]() { return stdcxx::make_unique<node_breaker_voltage_level::StateImpl>(*this); }),
    m_nodeBreakerView(*this),
    m_busBreakerView(*this),
    m_busView(*this) {
}

Switch& NodeBreakerVoltageLevel::addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2) {
    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));

    unsigned long e = m_graph.addEdge(node1, node2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void NodeBreakerVoltageLevel::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_states.allocateStateArrayElement(indexes, [this, sourceIndex]() { return m_states.copy(sourceIndex); });
}

void NodeBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);
        unsigned long node = nodeTerminal.getNode();

        const stdcxx::Reference<NodeTerminal>& tmp = m_graph.getVertexObject(node);
        if (static_cast<bool>(tmp)) {
            throw ValidationException(terminal.getConnectable(),
                logging::format("An equipment (%1%) is already connected to the node %2% of voltage level %3%",
                                tmp.get().getConnectable().get().getId(), node, getId()));
        }

        // create the link terminal <-> voltage level
        terminal.setVoltageLevel(stdcxx::ref<VoltageLevel>(*this));

        // create the link terminal <-> graph vertex
        m_graph.setVertexObject(node, stdcxx::ref(nodeTerminal));
    }
}

void NodeBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<NodeTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
            logging::format("Voltage level %1% has a node/breaker topology, a node connection should be specified instead of a bus connection",
                            getId()));
    }
}

void NodeBreakerVoltageLevel::clean() {
    m_graph.removeIsolatedVertices();
}

bool NodeBreakerVoltageLevel::connect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    // paths are already sorted
    node_breaker_voltage_level::Graph::VertexVisitor isBusbarSection = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal) && terminal.get().getConnectable().get().getConnectableType() == ConnectableType::BUSBAR_SECTION;
    };
    node_breaker_voltage_level::Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch.get().getKind() == SwitchKind::DISCONNECTOR && aSwitch.get().isOpen();
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, isOpenedDisconnector);

    bool connected = false;
    if (!paths.empty()) {
        const auto& shortestPath = paths[0];
        for (unsigned long e : shortestPath) {
            Switch& aSwitch = m_graph.getEdgeObject(e).get();
            if (aSwitch.getKind() == SwitchKind::BREAKER && aSwitch.isOpen()) {
                aSwitch.setOpen(false);
                connected = true;
            }
        }
    }

    return connected;
}

void NodeBreakerVoltageLevel::deleteStateArrayElement(unsigned long index) {
    m_states.deleteStateArrayElement(index);
}

void NodeBreakerVoltageLevel::detach(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();
    assert(node < m_graph.getVertexCount());
    assert(stdcxx::areSame(m_graph.getVertexObject(node).get(), nodeTerminal));

    // remove the link terminal <-> graph vertex
    m_graph.setVertexObject(node, stdcxx::ref<NodeTerminal>());

    // remove the link terminal <-> voltage level
    nodeTerminal.setVoltageLevel(stdcxx::ref<VoltageLevel>());
}

bool NodeBreakerVoltageLevel::disconnect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    node_breaker_voltage_level::Graph::VertexVisitor isBusbarSection = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal) && terminal.get().getConnectable().get().getConnectableType() == ConnectableType::BUSBAR_SECTION;
    };
    node_breaker_voltage_level::Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch.get().getKind() == SwitchKind::DISCONNECTOR && aSwitch.get().isOpen();
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, isOpenedDisconnector);

    if (paths.empty()) {
        return false;
    }

    for (const auto& path : paths) {
        bool pathOpen = false;
        for (unsigned long e : path) {
            Switch& aSwitch = m_graph.getEdgeObject(e).get();
            if (aSwitch.getKind() == SwitchKind::BREAKER) {
                if (!aSwitch.isOpen()) {
                    aSwitch.setOpen(true);
                }
                // Open one breaker is sufficient to disconnect the terminal
                pathOpen = true;
                break;
            }
        }

        // No breaker found, the terminal is still connected
        if (!pathOpen) {
            return false;
        }
    }

    // For all paths, a breaker has been found, the terminal is disconnected
    return true;
}

void NodeBreakerVoltageLevel::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    m_states.extendStateArraySize(initStateArraySize, number, [this, sourceIndex]() { return m_states.copy(sourceIndex); });
}

const BusBreakerView& NodeBreakerVoltageLevel::getBusBreakerView() const {
    return m_busBreakerView;
}

BusBreakerView& NodeBreakerVoltageLevel::getBusBreakerView() {
    return m_busBreakerView;
}

const BusView& NodeBreakerVoltageLevel::getBusView() const {
    return m_busView;
}

BusView& NodeBreakerVoltageLevel::getBusView() {
    return m_busView;
}

node_breaker_voltage_level::BusNamingStrategy& NodeBreakerVoltageLevel::getBusNamingStrategy() {
    return m_busNamingStrategy;
}

stdcxx::optional<unsigned long> NodeBreakerVoltageLevel::getEdge(const std::string& switchId, bool throwException) const {
    checkNotEmpty(switchId, "switch id is null");

    const auto& it = m_switches.find(switchId);
    if (it != m_switches.end()) {
        return stdcxx::optional<unsigned long>(it->second);
    }
    if (!throwException) {
        return stdcxx::optional<unsigned long>();
    }

    throw PowsyblException(logging::format("Switch '%1%' not found in the voltage level '%2%'", switchId, getId()));
}

const node_breaker_voltage_level::Graph& NodeBreakerVoltageLevel::getGraph() const {
    return m_graph;
}

unsigned long NodeBreakerVoltageLevel::getNode1(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex1(*e);
}

unsigned long NodeBreakerVoltageLevel::getNode2(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex2(*e);
}

const NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() const {
    return m_nodeBreakerView;
}

NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() {
    return m_nodeBreakerView;
}

unsigned long NodeBreakerVoltageLevel::getNodeCount() const {
    return m_graph.getVertexCount();
}

stdcxx::Reference<Switch> NodeBreakerVoltageLevel::getSwitch(const std::string& switchId) const {
    stdcxx::Reference<Switch> aSwitch;

    const auto& e = getEdge(switchId, false);
    if (e.is_initialized()) {
        aSwitch = m_graph.getEdgeObject(*e);
        if (aSwitch.get().getId() != switchId) {
            throw PowsyblException(logging::format("Invalid switch id (expected: '%1%', actual: '%2%')", switchId, aSwitch.get().getId()));
        }
    }

    return aSwitch;
}

unsigned long NodeBreakerVoltageLevel::getSwitchCount() const {
    return m_graph.getEdgeCount();
}

stdcxx::Reference<Terminal> NodeBreakerVoltageLevel::getTerminal(unsigned long node) const {
    return stdcxx::ref<Terminal>(m_graph.getVertexObject(node));
}

const TopologyKind& NodeBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::NODE_BREAKER;

    return s_topologyKind;
}

void NodeBreakerVoltageLevel::invalidateCache() {
    // TODO(mathbagu)
}

bool NodeBreakerVoltageLevel::isConnected(const Terminal& /*terminal*/) const {
    throw AssertionError("TODO");
}

void NodeBreakerVoltageLevel::reduceStateArraySize(unsigned long number) {
    m_states.reduceStateArraySize(number);
}

void NodeBreakerVoltageLevel::removeSwitch(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(logging::format("Switch '%1%' not found in voltage level '%2%'", switchId, getId()));
    }

    const auto& aSwitch = m_graph.removeEdge(it->second);
    m_switches.erase(it);
    getNetwork().remove(aSwitch.get());
}

void NodeBreakerVoltageLevel::setNodeCount(unsigned long nodeCount) {
    unsigned long oldCount = m_graph.getVertexCount();
    for (unsigned long i = oldCount; i < nodeCount; ++i) {
        m_graph.addVertex();
    }
}

}  // namespace iidm

}  // namespace powsybl
