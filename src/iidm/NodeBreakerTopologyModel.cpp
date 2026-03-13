/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerTopologyModel.hpp"

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include "NodeBreakerTopologyBusCache.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

NodeBreakerTopologyModel::NodeBreakerTopologyModel(VoltageLevel& voltageLevel) :
    TopologyModel(voltageLevel),
    m_busNamingStrategy(voltageLevel),
    m_variants(*this, [this]() { return stdcxx::make_unique<node_breaker_topology_model::VariantImpl>(*this); }),
    m_nodeBreakerView(*this),
    m_busBreakerView(*this),
    m_busView(*this) {
}

void NodeBreakerTopologyModel::addInternalConnection(unsigned long node1, unsigned long node2) {
    m_graph.addVertexIfNotPresent(node1);
    m_graph.addVertexIfNotPresent(node2);
    m_graph.addEdge(node1, node2, stdcxx::ref<Switch>());
    invalidateCache();
}

Switch& NodeBreakerTopologyModel::addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2) {
    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));
    m_graph.addVertexIfNotPresent(node1);
    m_graph.addVertexIfNotPresent(node2);
    unsigned long e = m_graph.addEdge(node1, node2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void NodeBreakerTopologyModel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void NodeBreakerTopologyModel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);
        unsigned long node = nodeTerminal.getNode();
        m_graph.addVertexIfNotPresent(node);

        const stdcxx::Reference<NodeTerminal>& tmp = m_graph.getVertexObject(node);
        if (static_cast<bool>(tmp)) {
            throw ValidationException(terminal.getConnectable(),
                stdcxx::format("An equipment (%1%) is already connected to the node %2% of voltage level %3%",
                                tmp.get().getConnectable().get().getId(), node, getVoltageLevel().getId()));
        }

        // create the link terminal <-> graph vertex
        m_graph.setVertexObject(node, stdcxx::ref(nodeTerminal));

        getNetwork().getVariantManager().forEachVariant([this]() {
            invalidateCache();
        });
    }
}

void NodeBreakerTopologyModel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<NodeTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
            stdcxx::format("Voltage level %1% has a node/breaker topology, a node connection should be specified instead of a bus connection",
                            getVoltageLevel().getId()));
    }
}

void NodeBreakerTopologyModel::clean() {
    m_graph.removeIsolatedVertices();
}

bool NodeBreakerTopologyModel::connect(Terminal& terminal) {
    //Only keep the closed non-fictionnal breakers in the nominal case
    return connect(terminal, SwitchPredicate::IS_NONFICTIONAL_BREAKER());
}
bool NodeBreakerTopologyModel::connect(Terminal& terminal, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    checkTerminal(terminal);
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    if (terminal.isConnected()) {
        return false;
    }

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a busbar section that does not contain an open switch
    // that is not of the type of switch the user wants to disconnect
    // paths are already sorted
    node_breaker_topology_model::Graph::VertexVisitor isBusbarSection = [](const stdcxx::Reference<NodeTerminal>& refTerminal) {
        return static_cast<bool>(refTerminal) && refTerminal.get().getConnectable().get().getType() == IdentifiableType::BUSBAR_SECTION;
    };
    node_breaker_topology_model::Graph::EdgeVisitor checkNonClosableSwitch = [&isTypeSwitchToOperate](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch && SwitchPredicate::IS_OPEN()(aSwitch.get()) && !isTypeSwitchToOperate(aSwitch.get());
    };
    const auto& paths = m_graph.findAllPaths(node, isBusbarSection, checkNonClosableSwitch);

    bool connected = false;
    if (!paths.empty()) {
        const auto& shortestPath = paths[0];
        //Close all open switches on the path
        for (unsigned long e : shortestPath) {
            const auto& aSwitch = m_graph.getEdgeObject(e);
            if (aSwitch && SwitchPredicate::IS_OPEN()(aSwitch.get())) {
                aSwitch.get().setOpen(false);
            }
        }
        //check that the terminal is indeed connected
        connected = terminal.isConnected();
    }

    return connected;
}

void NodeBreakerTopologyModel::deleteVariantArrayElement(unsigned long index) {
    m_variants.deleteVariantArrayElement(index);
}

void NodeBreakerTopologyModel::detach(Terminal& terminal) {
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    unsigned long node = nodeTerminal.getNode();
    assert(node < m_graph.getVertexCapacity());
    assert(stdcxx::areSame(m_graph.getVertexObject(node).get(), nodeTerminal));

    getNetwork().getVariantManager().forEachVariant([this]() {
        invalidateCache();
    });

    // remove the link terminal <-> graph vertex
    m_graph.setVertexObject(node, stdcxx::ref<NodeTerminal>());
    clean();
}

bool NodeBreakerTopologyModel::disconnect(Terminal& terminal) {
    // Only keep the closed non-fictional breakers in the nominal case
    return disconnect(terminal, SwitchPredicate::IS_CLOSED_BREAKER());
}
bool NodeBreakerTopologyModel::disconnect(Terminal& terminal, const stdcxx::Predicate<Switch>& isSwitchOpenable) {
    checkTerminal(terminal);
    auto& nodeTerminal = dynamic_cast<NodeTerminal&>(terminal);

    if (!terminal.isConnected()) {
        return false;
    }

    unsigned long node = nodeTerminal.getNode();

    // find all paths starting from the current terminal to a non null terminal that does not contain an open switch
    node_breaker_topology_model::Graph::VertexVisitor isTerminalNonNull = [](const stdcxx::Reference<NodeTerminal>& refTerminal) {
        return static_cast<bool>(refTerminal);
    };
    node_breaker_topology_model::Graph::EdgeVisitor isOpenedSwitch = [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch && SwitchPredicate::IS_OPEN()(aSwitch.get());
    };
    const auto& paths = m_graph.findAllPaths(node, isTerminalNonNull, isOpenedSwitch);

    if (paths.empty()) {
        return false;
    }

    for (const auto& path : paths) {
        //Disconnect each path:
        bool pathOpen = false;

        for (unsigned long e : path) {
            const auto& aSwitch = m_graph.getEdgeObject(e);
            if(isOpenedSwitch(aSwitch)) { //Path might have been opened when opening another path
                pathOpen = true;
                break;
            }
            if (aSwitch && isSwitchOpenable(aSwitch.get())) {
                aSwitch.get().setOpen(true);
                // Just opening the first one is sufficient to disconnect the terminal
                pathOpen = true;
                break;
            }
        }

        // No suitable openable switch found but path still closed -> the terminal is still connected
        if (!pathOpen) {
            return false;
        }
    }

    // For all paths, a breaker has been found, the terminal is disconnected
    return true;
}

void NodeBreakerTopologyModel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

const NodeBreakerTopologyModel::BusBreakerView& NodeBreakerTopologyModel::getBusBreakerView() const {
    return m_busBreakerView;
}

NodeBreakerTopologyModel::BusBreakerView& NodeBreakerTopologyModel::getBusBreakerView() {
    return m_busBreakerView;
}

const NodeBreakerTopologyModel::BusView& NodeBreakerTopologyModel::getBusView() const {
    return m_busView;
}

NodeBreakerTopologyModel::BusView& NodeBreakerTopologyModel::getBusView() {
    return m_busView;
}

node_breaker_topology_model::BusNamingStrategy& NodeBreakerTopologyModel::getBusNamingStrategy() {
    return m_busNamingStrategy;
}

const node_breaker_topology_model::CalculatedBusBreakerTopology& NodeBreakerTopologyModel::getCalculatedBusBreakerTopology() const {
    return m_variants.get().getCalculatedBusBreakerTopology();
}

node_breaker_topology_model::CalculatedBusBreakerTopology& NodeBreakerTopologyModel::getCalculatedBusBreakerTopology() {
    return m_variants.get().getCalculatedBusBreakerTopology();
}

node_breaker_topology_model::CalculatedBusTopology& NodeBreakerTopologyModel::getCalculatedBusTopology() {
    return m_variants.get().getCalculatedBusTopology();
}

stdcxx::optional<unsigned long> NodeBreakerTopologyModel::getEdge(const std::string& switchId, bool throwException) const {
    checkNotEmpty(switchId, "switch id is null");

    const auto& it = m_switches.find(switchId);
    if (it != m_switches.end()) {
        return {it->second};
    }
    if (!throwException) {
        return {};
    }

    throw PowsyblException(stdcxx::format("Switch '%1%' not found in the voltage level '%2%'", switchId, getVoltageLevel().getId()));
}

const node_breaker_topology_model::Graph& NodeBreakerTopologyModel::getGraph() const {
    return m_graph;
}

unsigned long NodeBreakerTopologyModel::getInternalConnectionCount() const {
    const auto& switches = m_graph.getEdgeObjects();
    return std::count(std::begin(switches), std::end(switches), stdcxx::ref<Switch>());
}

long NodeBreakerTopologyModel::getMaximumNodeIndex() const {
    return m_graph.getVertexCapacity() - 1;
}

unsigned long NodeBreakerTopologyModel::getNode1(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex1(*e);
}

unsigned long NodeBreakerTopologyModel::getNode2(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    return m_graph.getVertex2(*e);
}

const NodeBreakerTopologyModel::NodeBreakerView& NodeBreakerTopologyModel::getNodeBreakerView() const {
    return m_nodeBreakerView;
}

NodeBreakerTopologyModel::NodeBreakerView& NodeBreakerTopologyModel::getNodeBreakerView() {
    return m_nodeBreakerView;
}

stdcxx::CReference<Switch> NodeBreakerTopologyModel::getSwitch(const std::string& switchId) const {
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

unsigned long NodeBreakerTopologyModel::getSwitchCount() const {
    const auto& switches = m_graph.getEdgeObjects();
    return std::count_if(std::begin(switches), std::end(switches), [](stdcxx::Reference<Switch> sw) {
        return static_cast<bool>(sw);
    });
}

stdcxx::const_range<Switch> NodeBreakerTopologyModel::getSwitches() const {
    return getNodeBreakerView().getSwitches();
}

stdcxx::range<Switch> NodeBreakerTopologyModel::getSwitches() {
    return getNodeBreakerView().getSwitches();
}

stdcxx::CReference<Terminal> NodeBreakerTopologyModel::getTerminal(unsigned long node) const {
    return stdcxx::cref<Terminal>(m_graph.getVertexObject(node));
}

stdcxx::Reference<Terminal> NodeBreakerTopologyModel::getTerminal(unsigned long node) {
    return stdcxx::ref<Terminal>(m_graph.getVertexObject(node));
}

stdcxx::const_range<Terminal> NodeBreakerTopologyModel::getTerminals() const {
    const auto& filter = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<NodeTerminal>, Terminal>;

    return m_graph.getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Terminal> NodeBreakerTopologyModel::getTerminals() {
    const auto& filter = [](const stdcxx::Reference<NodeTerminal>& terminal) {
        return static_cast<bool>(terminal);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<NodeTerminal>, Terminal>;

    return m_graph.getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

const TopologyKind& NodeBreakerTopologyModel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::NODE_BREAKER;

    return s_topologyKind;
}

math::TraverseResult NodeBreakerTopologyModel::getTraverseResult(TerminalSet& visitedTerminals, NodeTerminal& terminal, Terminal::TopologyTraverser& traverser) {
    if (visitedTerminals.insert(terminal).second) {
        return traverser.traverse(terminal, true);
    }
    return math::TraverseResult::TERMINATE_PATH;
}

void NodeBreakerTopologyModel::invalidateCache(bool exceptBusBreakerView) {
    if(!exceptBusBreakerView) {
        m_variants.get().getCalculatedBusBreakerTopology().invalidateCache();
        getNetwork().getBusBreakerView().invalidateCache();
    }
    m_variants.get().getCalculatedBusTopology().invalidateCache();
    getNetwork().getBusView().invalidateCache();
    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();
}

bool NodeBreakerTopologyModel::isConnected(const Terminal& terminal) const {
    const auto& nodeTerminal = dynamic_cast<const NodeTerminal&>(terminal);

    return nodeTerminal.getBusView().getBus();
}

void NodeBreakerTopologyModel::reduceVariantArraySize(unsigned long number) {
    m_variants.reduceVariantArraySize(number);
}

void NodeBreakerTopologyModel::removeInternalConnections(unsigned long node1, unsigned long node2) {
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

void NodeBreakerTopologyModel::removeSwitch(const std::string& switchId) {
    removeSwitchFromTopology(switchId);

    //remove switch from network
    getNetwork().remove(getNetwork().getIdentifiable(switchId));
}

void NodeBreakerTopologyModel::removeTopology() {
    for (Switch& s : m_graph.getEdgeObjects()) {
        getNetwork().getIndex().remove(s);
    }
    m_graph.removeAllEdges();
    m_switches.clear();
}

void NodeBreakerTopologyModel::removeSwitchFromTopology(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(stdcxx::format("Switch '%1%' not found in voltage level '%2%'", switchId, getVoltageLevel().getId()));
    }

    m_graph.removeEdge(it->second);
    clean();

    m_switches.erase(it);
}

bool NodeBreakerTopologyModel::traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser, math::TraversalType traversalType) const {
    TerminalSet traversedTerminals;
    return traverse(terminal, traverser, traversedTerminals, traversalType);
}

bool NodeBreakerTopologyModel::traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser, TerminalSet& traversedTerminals, math::TraversalType traversalType) const {
    const math::TraverseResult& termTraverseResult = getTraverseResult(traversedTerminals, terminal, traverser);
    if (termTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
        return false;
    }
    if (termTraverseResult == math::TraverseResult::CONTINUE) {
        TerminalSet nextTerminals;
        addNextTerminals(terminal, nextTerminals);

        unsigned long node = terminal.getNode();
        bool traverseTerminated = !m_graph.traverse(node, traversalType, [this, &traverser, &traversedTerminals, &nextTerminals](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
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
            if (!nextTerminal.traverse(traverser, traversedTerminals, traversalType)) {
                return false;
            }
        }
    }

    return true;
}

}  // namespace iidm

}  // namespace powsybl
