/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevel.hpp"

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include "NodeBreakerVoltageLevelBusCache.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

NodeBreakerVoltageLevel::NodeBreakerVoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                                                 Network& network, double nominalV, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, fictitious, substation, network, nominalV, lowVoltageLimit, highVoltagelimit),
    m_busNamingStrategy(*this),
    m_variants(*this, [this]() { return stdcxx::make_unique<node_breaker_voltage_level::VariantImpl>(*this); }),
    m_nodeBreakerView(*this),
    m_busBreakerView(*this),
    m_busView(*this) {
}

void NodeBreakerVoltageLevel::addInternalConnection(unsigned long node1, unsigned long node2) {
    m_graph.addVertexIfNotPresent(node1);
    m_graph.addVertexIfNotPresent(node2);
    m_graph.addEdge(node1, node2, stdcxx::ref<Switch>());
    invalidateCache();
}

Switch& NodeBreakerVoltageLevel::addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2) {
    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));
    m_graph.addVertexIfNotPresent(node1);
    m_graph.addVertexIfNotPresent(node2);
    unsigned long e = m_graph.addEdge(node1, node2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void NodeBreakerVoltageLevel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    VoltageLevel::allocateVariantArrayElement(indexes, sourceIndex);
    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void NodeBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);
        unsigned long node = nodeTerminal.getNode();
        m_graph.addVertexIfNotPresent(node);

        const stdcxx::Reference<NodeTerminal>& tmp = m_graph.getVertexObject(node);
        if (static_cast<bool>(tmp)) {
            throw ValidationException(terminal.getConnectable(),
                stdcxx::format("An equipment (%1%) is already connected to the node %2% of voltage level %3%",
                                tmp.get().getConnectable().get().getId(), node, getId()));
        }

        // create the link terminal <-> graph vertex
        m_graph.setVertexObject(node, stdcxx::ref(nodeTerminal));

        getNetwork().getVariantManager().forEachVariant([this]() {
            invalidateCache();
        });
    }
}

void NodeBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<NodeTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
            stdcxx::format("Voltage level %1% has a node/breaker topology, a node connection should be specified instead of a bus connection",
                            getId()));
    }
}

void NodeBreakerVoltageLevel::clean() {
    m_graph.removeIsolatedVertices();
}

bool NodeBreakerVoltageLevel::connect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    if (terminal.isConnected()) {
        return false;
    }

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    // paths are already sorted
    node_breaker_voltage_level::Graph::VertexVisitor isBusbarSection = [](const stdcxx::Reference<NodeTerminal>& refTerminal) {
        return static_cast<bool>(refTerminal) && refTerminal.get().getConnectable().get().getType() == IdentifiableType::BUSBAR_SECTION;
    };
    node_breaker_voltage_level::Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch && aSwitch.get().getKind() == SwitchKind::DISCONNECTOR && aSwitch.get().isOpen();
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, isOpenedDisconnector);

    bool connected = false;
    if (!paths.empty()) {
        const auto& shortestPath = paths[0];
        for (unsigned long e : shortestPath) {
            const auto& aSwitch = m_graph.getEdgeObject(e);
            if (aSwitch && aSwitch.get().getKind() == SwitchKind::BREAKER && aSwitch.get().isOpen()) {
                aSwitch.get().setOpen(false);
                connected = true;
            }
        }
    }

    return connected;
}

void NodeBreakerVoltageLevel::deleteVariantArrayElement(unsigned long index) {
    VoltageLevel::deleteVariantArrayElement(index);
    m_variants.deleteVariantArrayElement(index);
}

void NodeBreakerVoltageLevel::detach(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();
    assert(node < m_graph.getVertexCount());
    assert(stdcxx::areSame(m_graph.getVertexObject(node).get(), nodeTerminal));

    getNetwork().getVariantManager().forEachVariant([this]() {
        invalidateCache();
    });

    // remove the link terminal <-> graph vertex
    m_graph.setVertexObject(node, stdcxx::ref<NodeTerminal>());
    clean();
}

bool NodeBreakerVoltageLevel::disconnect(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    if (!terminal.isConnected()) {
        return false;
    }

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open disconnector
    node_breaker_voltage_level::Graph::VertexVisitor isBusbarSection = [](const stdcxx::Reference<NodeTerminal>& refTerminal) {
        return static_cast<bool>(refTerminal) && refTerminal.get().getConnectable().get().getType() == IdentifiableType::BUSBAR_SECTION;
    };
    node_breaker_voltage_level::Graph::EdgeVisitor isOpenedDisconnector = [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch && aSwitch.get().getKind() == SwitchKind::DISCONNECTOR && aSwitch.get().isOpen();
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, isOpenedDisconnector);

    if (paths.empty()) {
        return false;
    }

    for (const auto& path : paths) {
        bool pathOpen = false;
        for (unsigned long e : path) {
            const auto& aSwitch = m_graph.getEdgeObject(e);
            if (aSwitch && aSwitch.get().getKind() == SwitchKind::BREAKER) {
                if (!aSwitch.get().isOpen()) {
                    aSwitch.get().setOpen(true);
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

void NodeBreakerVoltageLevel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    VoltageLevel::extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

const NodeBreakerVoltageLevel::BusBreakerView& NodeBreakerVoltageLevel::getBusBreakerView() const {
    return m_busBreakerView;
}

NodeBreakerVoltageLevel::BusBreakerView& NodeBreakerVoltageLevel::getBusBreakerView() {
    return m_busBreakerView;
}

const NodeBreakerVoltageLevel::BusView& NodeBreakerVoltageLevel::getBusView() const {
    return m_busView;
}

NodeBreakerVoltageLevel::BusView& NodeBreakerVoltageLevel::getBusView() {
    return m_busView;
}

node_breaker_voltage_level::BusNamingStrategy& NodeBreakerVoltageLevel::getBusNamingStrategy() {
    return m_busNamingStrategy;
}

const node_breaker_voltage_level::CalculatedBusBreakerTopology& NodeBreakerVoltageLevel::getCalculatedBusBreakerTopology() const {
    return m_variants.get().getCalculatedBusBreakerTopology();
}

node_breaker_voltage_level::CalculatedBusBreakerTopology& NodeBreakerVoltageLevel::getCalculatedBusBreakerTopology() {
    return m_variants.get().getCalculatedBusBreakerTopology();
}

node_breaker_voltage_level::CalculatedBusTopology& NodeBreakerVoltageLevel::getCalculatedBusTopology() {
    return m_variants.get().getCalculatedBusTopology();
}

stdcxx::optional<unsigned long> NodeBreakerVoltageLevel::getEdge(const std::string& switchId, bool throwException) const {
    checkNotEmpty(switchId, "switch id is null");

    const auto& it = m_switches.find(switchId);
    if (it != m_switches.end()) {
        return {it->second};
    }
    if (!throwException) {
        return {};
    }

    throw PowsyblException(stdcxx::format("Switch '%1%' not found in the voltage level '%2%'", switchId, getId()));
}

const node_breaker_voltage_level::Graph& NodeBreakerVoltageLevel::getGraph() const {
    return m_graph;
}

unsigned long NodeBreakerVoltageLevel::getInternalConnectionCount() const {
    const auto& switches = m_graph.getEdgeObjects();
    return std::count(std::begin(switches), std::end(switches), stdcxx::ref<Switch>());
}

long NodeBreakerVoltageLevel::getMaximumNodeIndex() const {
    return m_graph.getVertexCapacity() - 1;
}

unsigned long NodeBreakerVoltageLevel::getNode1(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex1(*e);
}

unsigned long NodeBreakerVoltageLevel::getNode2(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex2(*e);
}

const NodeBreakerVoltageLevel::NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() const {
    return m_nodeBreakerView;
}

NodeBreakerVoltageLevel::NodeBreakerView& NodeBreakerVoltageLevel::getNodeBreakerView() {
    return m_nodeBreakerView;
}

stdcxx::CReference<Switch> NodeBreakerVoltageLevel::getSwitch(const std::string& switchId) const {
    stdcxx::Reference<Switch> aSwitch;

    const auto& e = getEdge(switchId, false);
    if (e) {
        aSwitch = m_graph.getEdgeObject(*e);
        if (aSwitch.get().getId() != switchId) {
            throw PowsyblException(stdcxx::format("Invalid switch id (expected: '%1%', actual: '%2%')", switchId, aSwitch.get().getId()));
        }
    }

    return stdcxx::cref<Switch>(aSwitch);
}

unsigned long NodeBreakerVoltageLevel::getSwitchCount() const {
    const auto& switches = m_graph.getEdgeObjects();
    return std::count_if(std::begin(switches), std::end(switches), [](stdcxx::Reference<Switch> sw) {
        return static_cast<bool>(sw);
    });
}

stdcxx::const_range<Switch> NodeBreakerVoltageLevel::getSwitches() const {
    return getNodeBreakerView().getSwitches();
}

stdcxx::range<Switch> NodeBreakerVoltageLevel::getSwitches() {
    return getNodeBreakerView().getSwitches();
}

stdcxx::CReference<Terminal> NodeBreakerVoltageLevel::getTerminal(unsigned long node) const {
    return stdcxx::cref<Terminal>(m_graph.getVertexObject(node));
}

stdcxx::Reference<Terminal> NodeBreakerVoltageLevel::getTerminal(unsigned long node) {
    return stdcxx::ref<Terminal>(m_graph.getVertexObject(node));
}

stdcxx::const_range<Terminal> NodeBreakerVoltageLevel::getTerminals() const {
    const auto& filter = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<NodeTerminal>, Terminal>;

    return m_graph.getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Terminal> NodeBreakerVoltageLevel::getTerminals() {
    const auto& filter = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<NodeTerminal>, Terminal>;

    return m_graph.getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

const TopologyKind& NodeBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::NODE_BREAKER;

    return s_topologyKind;
}

math::TraverseResult NodeBreakerVoltageLevel::getTraverseResult(TerminalSet& visitedTerminals, NodeTerminal& terminal, Terminal::TopologyTraverser& traverser) {
    if (visitedTerminals.insert(terminal).second) {
        return traverser.traverse(terminal, true);
    }
    return math::TraverseResult::TERMINATE_PATH;
}

void NodeBreakerVoltageLevel::invalidateCache() {
    m_variants.get().getCalculatedBusTopology().invalidateCache();
    m_variants.get().getCalculatedBusBreakerTopology().invalidateCache();
    getNetwork().getBusBreakerView().invalidateCache();
    getNetwork().getBusView().invalidateCache();
    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();
}

bool NodeBreakerVoltageLevel::isConnected(const Terminal& terminal) const {
    const auto& nodeTerminal = dynamic_cast<const NodeTerminal&>(terminal);

    return nodeTerminal.getBusView().getBus();
}

void NodeBreakerVoltageLevel::reduceVariantArraySize(unsigned long number) {
    VoltageLevel::reduceVariantArraySize(number);
    m_variants.reduceVariantArraySize(number);
}

void NodeBreakerVoltageLevel::removeInternalConnections(unsigned long node1, unsigned long node2) {
    const auto& filter = [this, node1, node2](const unsigned long& edge) {
        return !m_graph.getEdgeObject(edge) && ((m_graph.getVertex1(edge) == node1 && m_graph.getVertex2(edge) == node2) || (m_graph.getVertex1(edge) == node2 && m_graph.getVertex2(edge) == node1));
    };

    const auto& internalConnectionsToBeRemoved = m_graph.getEdges() | boost::adaptors::filtered(filter);
    if (boost::empty(internalConnectionsToBeRemoved)) {
        throw PowsyblException(stdcxx::format("Internal connection not found between %1% and %2%", node1, node2));
    }
    for (unsigned long ic : internalConnectionsToBeRemoved) {
        m_graph.removeEdge(ic);
    }
    clean();
    invalidateCache();
}

void NodeBreakerVoltageLevel::removeSwitch(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(stdcxx::format("Switch '%1%' not found in voltage level '%2%'", switchId, getId()));
    }

    const auto& aSwitch = m_graph.removeEdge(it->second);
    clean();

    m_switches.erase(it);
    getNetwork().remove(aSwitch.get());
}

void NodeBreakerVoltageLevel::removeTopology() {
    for (Switch& s : m_graph.getEdgeObjects()) {
        getNetwork().getIndex().remove(s);
    }
    m_graph.removeAllEdges();
    m_switches.clear();
}

bool NodeBreakerVoltageLevel::traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser) const {
    TerminalSet traversedTerminals;
    return traverse(terminal, traverser, traversedTerminals);
}

bool NodeBreakerVoltageLevel::traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser, TerminalSet& traversedTerminals) const {
    const math::TraverseResult& termTraverseResult = getTraverseResult(traversedTerminals, terminal, traverser);
    if (termTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
        return false;
    }
    if (termTraverseResult == math::TraverseResult::CONTINUE) {
        TerminalSet nextTerminals;
        addNextTerminals(terminal, nextTerminals);

        unsigned long node = terminal.getNode();
        bool traverseTerminated = !m_graph.traverse(node, [this, &traverser, &traversedTerminals, &nextTerminals](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
            const auto& aSwitch = m_graph.getEdgeObject(e);
            const auto& otherTerminal = m_graph.getVertexObject(v2);
            const math::TraverseResult& edgeTraverseResult = aSwitch ? traverser.traverse(aSwitch) : math::TraverseResult::CONTINUE; // internal connection case
            if (edgeTraverseResult == math::TraverseResult::CONTINUE && otherTerminal) {
                math::TraverseResult otherTermTraverseResult = getTraverseResult(traversedTerminals, otherTerminal, traverser);
                if (otherTermTraverseResult == math::TraverseResult::CONTINUE) {
                    addNextTerminals(otherTerminal, nextTerminals);
                }
                return otherTermTraverseResult;
            }
            return edgeTraverseResult;
        });

        if (traverseTerminated) {
            return false;
        }

        for (Terminal& nextTerminal : nextTerminals) {
            if (!nextTerminal.traverse(traverser, traversedTerminals)) {
                return false;
            }
        }
    }

    return true;
}

}  // namespace iidm

}  // namespace powsybl
