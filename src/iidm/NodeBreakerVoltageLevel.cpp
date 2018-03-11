/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevel.hpp"

#include <powsybl/iidm/Switch.hpp>

namespace powsybl {

namespace iidm {

NodeBreakerVoltageLevel::NodeBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                                 double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit),
    m_nodeBreakerView(*this),
    m_busBreakerView(*this),
    m_busView(*this) {
}

void NodeBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);
        unsigned long node = nodeTerminal.getNode();

        if (stdcxx::Optional<NodeTerminal> tmp = m_graph.getVertexObject(node)) {
            throw ValidationException(terminal.getConnectable(),
                logging::format("An equipment (%1%) is already connected to the node %2% of voltage level %3%",
                                tmp.get().getConnectable().getId(), node, getId()));
        }

        // create the link terminal <-> voltage level
        terminal.setVoltageLevel(stdcxx::Optional<VoltageLevel>(*this));

        // create the link terminal <-> graph vertex
        m_graph.setVertexObject(node, stdcxx::Optional<NodeTerminal>(nodeTerminal));
    }
}

void NodeBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    auto nodeTerminal = dynamic_cast<NodeTerminal*>(&terminal);
    if (nodeTerminal == nullptr) {
        throw ValidationException(terminal.getConnectable(),
            logging::format("Voltage level %1% has a node/breaker topology, a node connection should be specified instead of a bus connection",
                            getId()));
    }
}

void NodeBreakerVoltageLevel::clean() {
    // TODO
}

bool NodeBreakerVoltageLevel::connect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    // paths are already sorted
    Graph::VertexVisitor isBusbarSection = [](const stdcxx::Optional<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal) && terminal.get().getConnectable().getConnectableType() == ConnectableType::BUSBAR_SECTION;
    };
    Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Optional<Switch>& aSwitch) {
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

void NodeBreakerVoltageLevel::detach(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();
    assert(node < m_graph.getVertexCount());
    assert(std::addressof(m_graph.getVertexObject(node).get()) == std::addressof(nodeTerminal));

    // remove the link terminal <-> graph vertex
    m_graph.setVertexObject(node, stdcxx::Optional<NodeTerminal>());

    // remove the link terminal <-> voltage level
    nodeTerminal.setVoltageLevel(stdcxx::Optional<VoltageLevel>());
}

bool NodeBreakerVoltageLevel::disconnect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    Graph::VertexVisitor isBusbarSection = [](const stdcxx::Optional<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal) && terminal.get().getConnectable().getConnectableType() == ConnectableType::BUSBAR_SECTION;
    };
    Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Optional<Switch>& aSwitch) {
        return aSwitch.get().getKind() == SwitchKind::DISCONNECTOR && aSwitch.get().isOpen();
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, isOpenedDisconnector);

    if (paths.empty()) {
        return false;
    } else {
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

const NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() const {
    return m_nodeBreakerView;
}

NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() {
    return m_nodeBreakerView;
}

const TopologyKind& NodeBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::NODE_BREAKER;

    return s_topologyKind;
}

void NodeBreakerVoltageLevel::invalidateCache() {
    // TODO
}

}

}
