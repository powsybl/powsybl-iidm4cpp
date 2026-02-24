/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerTopologyModel.hpp"

#include <cassert>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/flattened.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include "BusTerminal.hpp"

namespace powsybl {

namespace iidm {

BusBreakerTopologyModel::BusBreakerTopologyModel(VoltageLevel& voltageLevel) :
    TopologyModel(voltageLevel),
    m_variants(*this, [this]() { return stdcxx::make_unique<bus_breaker_topology_model::VariantImpl>(*this); }),
    m_busBreakerView(*this),
    m_busView(*this) {
}

Bus& BusBreakerTopologyModel::addBus(std::unique_ptr<ConfiguredBus>&& ptrBus) {
    ConfiguredBus& bus = getNetwork().checkAndAdd(std::move(ptrBus));

    unsigned long node = m_graph.addVertex();
    m_graph.setVertexObject(node, stdcxx::ref(bus));
    m_buses.insert(std::make_pair(bus.getId(), node));

    return bus;
}

Switch& BusBreakerTopologyModel::addSwitch(std::unique_ptr<Switch>&& ptrSwitch, const std::string& busId1, const std::string& busId2) {
    unsigned long v1 = *getVertex(busId1, true);
    unsigned long v2 = *getVertex(busId2, true);

    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));
    unsigned long e = m_graph.addEdge(v1, v2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void BusBreakerTopologyModel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void BusBreakerTopologyModel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
        const auto& connectableBus = getConfiguredBus(busTerminal.getConnectableBusId(), true);

        getNetwork().getVariantManager().forEachVariant([&connectableBus, &busTerminal, this]() {
            connectableBus.get().addTerminal(busTerminal);

            invalidateCache();
        });
    }
}

void BusBreakerTopologyModel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<BusTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
                                  stdcxx::format("Voltage level '%1%' has a bus/breaker topology, a bus connection should be specified instead of a node connection",
                                                  getVoltageLevel().getId()));
    }

    // check connectable buses exist
    auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
    if (!busTerminal.getConnectableBusId().empty()) {
        getConfiguredBus(busTerminal.getConnectableBusId(), true);
    }
}

bool BusBreakerTopologyModel::connect(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    if (terminal.isConnected()) {
        return false;
    }

    dynamic_cast<BusTerminal&>(terminal).setConnected(true);

    invalidateCache();

    return true;
}
bool BusBreakerTopologyModel::connect(Terminal& terminal, const stdcxx::Predicate<Switch>& /*isTypeSwitchToOperate*/) {
    return connect(terminal);
}


void BusBreakerTopologyModel::deleteVariantArrayElement(unsigned long index) {
    m_variants.deleteVariantArrayElement(index);
}

void BusBreakerTopologyModel::detach(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
    auto& bus = getConfiguredBus(busTerminal.getConnectableBusId(), true).get();

    getNetwork().getVariantManager().forEachVariant([&bus, &busTerminal, this]() {
        bus.removeTerminal(busTerminal);
        busTerminal.setConnectableBusId("");

        invalidateCache();
    });
}

bool BusBreakerTopologyModel::disconnect(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    if (!terminal.isConnected()) {
        return false;
    }

    dynamic_cast<BusTerminal&>(terminal).setConnected(false);

    invalidateCache();

    return true;
}
bool BusBreakerTopologyModel::disconnect(Terminal& terminal, const stdcxx::Predicate<Switch>& /*isSwitchOpenable*/) {
    return disconnect(terminal);
}

void BusBreakerTopologyModel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

const BusBreakerTopologyModel::BusBreakerView& BusBreakerTopologyModel::getBusBreakerView() const {
    return m_busBreakerView;
}

BusBreakerTopologyModel::BusBreakerView& BusBreakerTopologyModel::getBusBreakerView() {
    return m_busBreakerView;
}

const BusBreakerTopologyModel::BusView& BusBreakerTopologyModel::getBusView() const {
    return m_busView;
}

BusBreakerTopologyModel::BusView& BusBreakerTopologyModel::getBusView() {
    return m_busView;
}

bus_breaker_topology_model::CalculatedBusTopology& BusBreakerTopologyModel::getCalculatedBusTopology() {
    return m_variants.get().getCalculatedBusTopology();
}

stdcxx::CReference<ConfiguredBus> BusBreakerTopologyModel::getConfiguredBus(const std::string& busId, bool throwException) const {
    stdcxx::Reference<ConfiguredBus> bus;

    const auto& v = getVertex(busId, throwException);
    if (v) {
        bus = m_graph.getVertexObject(*v);
        if (bus.get().getId() != busId) {
            throw PowsyblException(stdcxx::format("Invalid bus id (expected: '%1%', actual: '%2%')", busId, bus.get().getId()));
        }
    }

    return stdcxx::cref<ConfiguredBus>(bus);
}

stdcxx::Reference<ConfiguredBus> BusBreakerTopologyModel::getConfiguredBus(const std::string& busId, bool throwException) {
    return stdcxx::ref(static_cast<const BusBreakerTopologyModel*>(this)->getConfiguredBus(busId, throwException));
}

stdcxx::Reference<ConfiguredBus> BusBreakerTopologyModel::getConfiguredBus1(const std::string& switchId) {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex1(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::Reference<ConfiguredBus> BusBreakerTopologyModel::getConfiguredBus2(const std::string& switchId) {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex2(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::optional<unsigned long> BusBreakerTopologyModel::getEdge(const std::string& switchId, bool throwException) const {
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

const BusBreakerTopologyModel::Graph& BusBreakerTopologyModel::getGraph() const {
    return m_graph;
}

stdcxx::Reference<MergedBus> BusBreakerTopologyModel::getMergedBus(const std::string& busId, bool throwException) {
    return getCalculatedBusTopology().getMergedBus(busId, throwException);
}

const BusBreakerTopologyModel::NodeBreakerView& BusBreakerTopologyModel::getNodeBreakerView() const {
    throw AssertionError("Not implemented");
}

BusBreakerTopologyModel::NodeBreakerView& BusBreakerTopologyModel::getNodeBreakerView() {
    throw AssertionError("Not implemented");
}

stdcxx::Reference<Switch> BusBreakerTopologyModel::getSwitch(const std::string& switchId, bool throwException) {
    stdcxx::Reference<Switch> aSwitch;

    const auto& e = getEdge(switchId, throwException);
    if (e) {
        aSwitch = m_graph.getEdgeObject(*e);
        if (aSwitch.get().getId() != switchId) {
            throw PowsyblException(stdcxx::format("Invalid switch id (expected: '%1%', actual: '%2%')", switchId, aSwitch.get().getId()));
        }
    }

    return aSwitch;
}

unsigned long BusBreakerTopologyModel::getSwitchCount() const {
    return m_graph.getEdgeCount();
}

stdcxx::const_range<Switch> BusBreakerTopologyModel::getSwitches() const {
    return getBusBreakerView().getSwitches();
}

stdcxx::range<Switch> BusBreakerTopologyModel::getSwitches() {
    return getBusBreakerView().getSwitches();
}

stdcxx::const_range<Terminal> BusBreakerTopologyModel::getTerminals() const {
    const auto& mapper = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return bus.get().getTerminals();
    };

    return m_graph.getVertexObjects() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<Terminal> BusBreakerTopologyModel::getTerminals() {
    const auto& mapper = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return bus.get().getTerminals();
    };

    return m_graph.getVertexObjects() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

const TopologyKind& BusBreakerTopologyModel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::BUS_BREAKER;

    return s_topologyKind;
}

math::TraverseResult BusBreakerTopologyModel::getTraverserResult(TerminalSet& visitedTerminals, BusTerminal& terminal, Terminal::TopologyTraverser& traverser) {
    if (visitedTerminals.insert(terminal).second) {
        return traverser.traverse(terminal, true);
    }
    return math::TraverseResult::TERMINATE_PATH;
}

stdcxx::optional<unsigned long> BusBreakerTopologyModel::getVertex(const std::string& busId, bool throwException) const {
    checkNotEmpty(busId, "bus id is null");

    const auto& it = m_buses.find(busId);
    if (it != m_buses.end()) {
        return {it->second};
    }
    if (!throwException) {
        return {};
    }

    throw PowsyblException(stdcxx::format("Bus '%1%' not found in the voltage level '%2%'", busId, getVoltageLevel().getId()));
}

void BusBreakerTopologyModel::invalidateCache(bool /*exceptBusBreakerView*/) {
    m_variants.get().getCalculatedBusTopology().invalidateCache();
    getNetwork().getBusView().invalidateCache();
    getNetwork().getBusBreakerView().invalidateCache();
    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();
}

void BusBreakerTopologyModel::reduceVariantArraySize(unsigned long number) {
    m_variants.reduceVariantArraySize(number);
}

void BusBreakerTopologyModel::removeAllBuses() {
    if (m_graph.getEdgeCount() > 0) {
        throw ValidationException(getVoltageLevel(), stdcxx::format("Cannot remove all buses because there is still some switches"));
    }
    for (const auto& it : m_graph.getVertexObjects()) {
        const auto& bus = it.get();
        if (bus.getTerminalCount() > 0) {
            throw ValidationException(getVoltageLevel(), stdcxx::format("Cannot remove bus '%1%' due to connected equipments", bus.getId()));
        }
    }
    for (const auto& it : m_graph.getVertexObjects()) {
        getNetwork().remove(it.get());
    }

    m_graph.removeAllVertices();
    m_buses.clear();
}

void BusBreakerTopologyModel::removeAllSwitches() {
    for (const auto& it : m_graph.getEdgeObjects()) {
        getNetwork().remove(it.get());
    }

    m_graph.removeAllEdges();
    m_switches.clear();
}

void BusBreakerTopologyModel::removeBus(const std::string& busId) {
    const auto& bus = getConfiguredBus(busId, true);
    if (bus.get().getTerminalCount() > 0) {
        throw ValidationException(getVoltageLevel(), stdcxx::format("Cannot remove bus '%1%' due to connectable equipments", busId));
    }

    for (const auto& it : m_switches) {
        const auto& switchId = it.first;

        unsigned long v1 = m_graph.getVertex1(it.second);
        unsigned long v2 = m_graph.getVertex2(it.second);

        const auto& bus1 = m_graph.getVertexObject(v1);
        const auto& bus2 = m_graph.getVertexObject(v2);
        if ((bus == bus1) || (bus == bus2)) {
            throw PowsyblException(stdcxx::format("Cannot remove bus '%1%' due to the connected switch '%2%'", busId, switchId));
        }
    }

    const auto& it = m_buses.find(busId);
    m_graph.removeVertex(it->second);
    m_buses.erase(it);
    getNetwork().remove(bus.get());
}

void BusBreakerTopologyModel::removeSwitch(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(stdcxx::format("Switch '%1%' not found in voltage level '%2%'", switchId, getVoltageLevel().getId()));
    }

    const auto& aSwitch = m_graph.removeEdge(it->second);
    m_switches.erase(it);
    getNetwork().remove(aSwitch.get());
}

void BusBreakerTopologyModel::removeTopology() {
    removeAllSwitches();
    removeAllBuses();
}

bool BusBreakerTopologyModel::traverse(BusTerminal& terminal, Terminal::TopologyTraverser& traverser, math::TraversalType traversalType) const {
    TerminalSet traversedTerminals;
    return traverse(terminal, traverser, traversedTerminals, traversalType);
}

bool BusBreakerTopologyModel::traverse(BusTerminal& terminal, Terminal::TopologyTraverser& traverser, TerminalSet& traversedTerminals, math::TraversalType traversalType) const {
    // check if we are allowed to traverse the terminal itself
    math::TraverseResult termTraverseResult = getTraverserResult(traversedTerminals, terminal, traverser);
    if (termTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
        return false;
    }

    if (termTraverseResult == math::TraverseResult::CONTINUE) {
        TerminalSet nextTerminals;
        addNextTerminals(terminal, nextTerminals);

        // then check we can traverse terminals connected to same bus
        unsigned long v = *getVertex(terminal.getConnectableBusId(), true);
        ConfiguredBus& bus = m_graph.getVertexObject(v).get();
        for (BusTerminal& t : bus.getTerminals()) {
            math::TraverseResult tTraverseResult = getTraverserResult(traversedTerminals, t, traverser);
            if (tTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
                return false;
            }
            if (tTraverseResult == math::TraverseResult::CONTINUE) {
                addNextTerminals(t, nextTerminals);
            }
        }

        // then go through other buses of the voltage level
        bool traversalTerminated = !m_graph.traverse(v, traversalType, [this, &nextTerminals, &traverser, &traversedTerminals](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
            Switch& aSwitch = m_graph.getEdgeObject(e);
            const stdcxx::range<BusTerminal>& otherBusTerminals = m_graph.getVertexObject(v2).get().getTerminals();
            math::TraverseResult switchTraverseResult = traverser.traverse(aSwitch);
            if (switchTraverseResult == math::TraverseResult::CONTINUE && !otherBusTerminals.empty()) {
                BusTerminal& otherTerminal = *otherBusTerminals.begin();
                math::TraverseResult otherTermTraverseResult = getTraverserResult(traversedTerminals, otherTerminal, traverser);
                if (otherTermTraverseResult == math::TraverseResult::CONTINUE) {
                    addNextTerminals(otherTerminal, nextTerminals);
                }
                return otherTermTraverseResult;
            }
            return switchTraverseResult;
        });
        if (traversalTerminated) {
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
