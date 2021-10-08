/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/SwitchesFlow.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

SwitchesFlow::SwFlow::SwFlow(double p1, double q1, double p2, double q2) :
    m_p1(p1), m_p2(p2), m_q1(q1), m_q2(q2) {
}

SwitchesFlow::SwNode::SwNode(unsigned long node) :
    m_node(node) {
}

SwitchesFlow::SwNode::SwNode(const Bus& bus) :
    m_bus(bus) {
}

bool SwitchesFlow::SwNode::isNodeBreaker() const {
    return static_cast<bool>(m_node);
}

SwitchesFlow::SwNode& SwitchesFlow::SwNode::addPQ(double p, double q) {
    m_p = m_p + p;
    m_q = m_q + q;

    return *this;
}

SwitchesFlow::SwNode& SwitchesFlow::SwNode::addFlow(double pFlow, double qFlow) {
    m_pflow = m_pflow + pFlow;
    m_qflow = m_qflow + qFlow;

    return *this;
}

SwitchesFlow::SwitchesFlow(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
    compute();
}

const SwitchesFlow::SwNode& SwitchesFlow::addSwNode(std::map<std::string, SwNode>& swNodeInjection, unsigned long node) {
    auto it = swNodeInjection.find(getKey(node));
    if (it != swNodeInjection.end()) {
        return it->second;
    }
    auto itInsertion = swNodeInjection.emplace(getKey(node), SwNode(node));
    return itInsertion.first->second;
}

const SwitchesFlow::SwNode& SwitchesFlow::addSwNode(std::map<std::string, SwNode>& swNodeInjection, const iidm::Bus& bus) {
    auto it = swNodeInjection.find(getKey(bus));
    if (it != swNodeInjection.end()) {
        return it->second;
    }
    auto itInsertion = swNodeInjection.emplace(getKey(bus), SwNode(bus));
    return itInsertion.first->second;
}

SwitchesFlow::SwEdge::SwEdge(Switch& sw, SwNode& swNode1, SwNode& swNode2) :
    m_sw(sw), m_swNode1(swNode1), m_swNode2(swNode2) {
}

SwitchesFlow::SwEdge::SwEdge(SwNode& swNode1, SwNode& swNode2) :
    m_swNode1(swNode1), m_swNode2(swNode2) {
}

std::string SwitchesFlow::SwEdge::getSwitchId() const {
    if (m_sw) {
        return m_sw.get().getId();
    }
    return "";
}

bool SwitchesFlow::SwEdge::isSwitch() const {
    return static_cast<bool>(m_sw);
}

void SwitchesFlow::assignZeroFlowToEdgesOutsideAllTrees(const math::UndirectedGraph<SwNode, SwEdge>& graph) {

}

void SwitchesFlow::buildGraph(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph) {
    if (m_voltageLevel.getTopologyKind() == TopologyKind::NODE_BREAKER) {
        buildGraphFromNodeBreaker(swNodeInjection, graph);
    } else {
        buildGraphFromBusBreaker(swNodeInjection, graph);
    }
}

void SwitchesFlow::buildGraphFromBusBreaker(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph) {
    for (Switch& sw : m_voltageLevel.getBusBreakerView().getSwitches()) {
        if (sw.isOpen()) {
            return;
        }
        SwNode swNode1 = addSwNode(swNodeInjection, m_voltageLevel.getBusBreakerView().getBus1(sw.getId()).get());
        SwNode swNode2 = addSwNode(swNodeInjection, m_voltageLevel.getBusBreakerView().getBus2(sw.getId()).get());
        // Discard loops
        if (stdcxx::areSame(swNode1, swNode2)) { // sebalaig: to be checked
            return;
        }
        unsigned long v1 = graph.addVertex();
        graph.setVertexObject(v1, stdcxx::ref(swNode1));
        unsigned long v2 = graph.addVertex();
        graph.setVertexObject(v2, stdcxx::ref(swNode2));
        m_edges.emplace_back(stdcxx::make_unique<SwEdge>(sw, swNode1, swNode2));
        graph.addEdge(v1, v2, stdcxx::ref(*m_edges.back()));
    }
}

void SwitchesFlow::buildGraphFromNodeBreaker(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph) {
    for (Switch& sw : m_voltageLevel.getNodeBreakerView().getSwitches()) {
        if (sw.isOpen()) {
            return;
        }
        SwNode swNode1 = addSwNode(swNodeInjection, m_voltageLevel.getBusBreakerView().getBus1(sw.getId()).get());
        SwNode swNode2 = addSwNode(swNodeInjection, m_voltageLevel.getBusBreakerView().getBus2(sw.getId()).get());
        // Discard loops
        if (stdcxx::areSame(swNode1, swNode2)) { // sebalaig: to be checked
            return;
        }
        unsigned long v1 = graph.addVertex();
        graph.setVertexObject(v1, stdcxx::ref(swNode1));
        unsigned long v2 = graph.addVertex();
        graph.setVertexObject(v2, stdcxx::ref(swNode2));
        m_edges.emplace_back(stdcxx::make_unique<SwEdge>(sw, swNode1, swNode2));
        graph.addEdge(v1, v2, stdcxx::ref(*m_edges.back()));
    }

    for (const node_breaker_view::InternalConnection& ic : m_voltageLevel.getNodeBreakerView().getInternalConnections()) {
        SwNode swNode1 = addSwNode(swNodeInjection, ic.getNode1());
        SwNode swNode2 = addSwNode(swNodeInjection, ic.getNode2());
        // Discard loops
        if (stdcxx::areSame(swNode1, swNode2)) { // sebalaig: to be checked
            return;
        }
        unsigned long v1 = graph.addVertex();
        graph.setVertexObject(v1, stdcxx::ref(swNode1));
        unsigned long v2 = graph.addVertex();
        graph.setVertexObject(v2, stdcxx::ref(swNode2));
        m_edges.emplace_back(stdcxx::make_unique<SwEdge>(swNode1, swNode2));
        graph.addEdge(v1, v2, stdcxx::ref(*m_edges.at(m_edges.size() - 1)));
    }
}

void SwitchesFlow::calculateInjections(std::map<std::string, SwNode>& swNodeInjection) {
    if (m_voltageLevel.getTopologyKind() == TopologyKind::NODE_BREAKER) {
        calculateInjectionsNodeBreaker(m_voltageLevel, swNodeInjection);
    } else {
        calculateInjectionsBusBreaker(m_voltageLevel, swNodeInjection);
    }
}

void SwitchesFlow::calculateInjectionsNodeBreaker(VoltageLevel& voltageLevel, std::map<std::string, SwNode>& swNodeInjection) {
    for (unsigned long node : voltageLevel.getNodeBreakerView().getNodes()) {
        const auto& terminal = voltageLevel.getNodeBreakerView().getTerminal(node);
        if (terminal) {
            auto it = swNodeInjection.find(getKey(node));
            if (it != swNodeInjection.end()) {
                double p = getTerminalP(terminal);
                double q = getTerminalQ(terminal);
                it->second.addPQ(p, q);
            }
        }
    }
}

void SwitchesFlow::calculateInjectionsBusBreaker(VoltageLevel& voltageLevel, std::map<std::string, SwNode>& swNodeInjection) {
    for (Bus& bus : voltageLevel.getBusBreakerView().getBuses()) {
        for (Terminal& terminal : bus.getConnectedTerminals()) {
            auto it = swNodeInjection.find(getKey(bus));
            if (it != swNodeInjection.end()) {
                double p = getTerminalP(terminal);
                double q = getTerminalQ(terminal);
                it->second.addPQ(p, q);
            }
        }
    }
}

void SwitchesFlow::compute() {
    std::map<std::string, SwNode> swNodeInjection;
    math::UndirectedGraph<SwNode, SwEdge> graph;

    buildGraph(swNodeInjection, graph);
    calculateInjections(swNodeInjection);

//    // FIXME(sebalaig)
//    ConnectivityInspector<SwNode, SwEdge> ci = new ConnectivityInspector<>(graph);
//    ci.connectedSets().forEach(connectedSet -> connectedComponentSwitchesFlow(swNodeInjection, graph, connectedSet));

    assignZeroFlowToEdgesOutsideAllTrees(graph);
}

std::string SwitchesFlow::getKey(unsigned long node) {
    return stdcxx::format("N-%1%", node);
}

std::string SwitchesFlow::getKey(const Bus& bus) {
    return stdcxx::format("B-%1%", bus);
}

double SwitchesFlow::getTerminalP(const Terminal& terminal) {
    if (std::isnan(terminal.getP())) {
        return 0.0;
    }
    return terminal.getP();
}

double SwitchesFlow::getTerminalQ(const Terminal& terminal) {
    if (std::isnan(terminal.getQ())) {
        return 0.0;
    }
    return terminal.getQ();
}

bool SwitchesFlow::hasFlow(const std::string& id) const {
    return m_switchesFlows.find(id) != m_switchesFlows.end();
}

bool SwitchesFlow::isEmpty() const {
    return m_switchesFlows.empty();
}

double SwitchesFlow::getP1(const std::string& switchId) const {
    auto it = m_switchesFlows.find(switchId);
    if (it != m_switchesFlows.end()) {
        return it->second.m_p1;
    }
    return 0.0;
}

double SwitchesFlow::getQ1(const std::string& switchId) const {
    auto it = m_switchesFlows.find(switchId);
    if (it != m_switchesFlows.end()) {
        return it->second.m_q1;
    }
    return 0.0;
}

double SwitchesFlow::getP2(const std::string& switchId) const {
    auto it = m_switchesFlows.find(switchId);
    if (it != m_switchesFlows.end()) {
        return it->second.m_p2;
    }
    return 0.0;
}

double SwitchesFlow::getQ2(const std::string& switchId) const {
    auto it = m_switchesFlows.find(switchId);
    if (it != m_switchesFlows.end()) {
        return it->second.m_q2;
    }
    return 0.0;
}

}  // namespace iidm

}  // namespace powsybl
