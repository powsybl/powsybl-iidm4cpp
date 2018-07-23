/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevel.hpp"

#include <cassert>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>

#include "ConfiguredBus.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

BusBreakerVoltageLevel::BusBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                               double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit),
    m_states(substation.getNetwork(), [this]() { return stdcxx::make_unique<bus_breaker_voltage_level::StateImpl>(*this); }),
    m_busBreakerView(*this),
    m_busView(*this) {
}

Bus& BusBreakerVoltageLevel::addBus(std::unique_ptr<ConfiguredBus>&& ptrBus) {
    ConfiguredBus& bus = getNetwork().checkAndAdd(std::move(ptrBus));

    unsigned long node = m_graph.addVertex();
    m_graph.setVertexObject(node, stdcxx::ref(bus));
    m_buses.insert(std::make_pair(bus.getId(), node));

    return bus;
}

Switch& BusBreakerVoltageLevel::addSwitch(std::unique_ptr<Switch>&& ptrSwitch, const std::string& busId1, const std::string& busId2) {
    unsigned long v1 = getVertex(busId1, true).get();
    unsigned long v2 = getVertex(busId2, true).get();

    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));
    unsigned long e = m_graph.addEdge(v1, v2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void BusBreakerVoltageLevel::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_states.allocateStateArrayElement(indexes, [this, sourceIndex]() { return m_states.copy(sourceIndex); });
}

void BusBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        // create the link terminal -> voltage level
        terminal.setVoltageLevel(stdcxx::ref<VoltageLevel>(*this));

        auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
        const stdcxx::Reference<ConfiguredBus>& connectableBus = getConfiguredBus(busTerminal.getConnectableBusId(), true);

        getNetwork().getStateManager().forEachState([&connectableBus, &busTerminal, this]() {
            connectableBus.get().addTerminal(busTerminal);

            invalidateCache();
        });
    }
}

void BusBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<BusTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
                                  logging::format("Voltage level '%1%' has a bus/breaker topology, a bus connection should be specified instead of a node connection",
                                                  getId()));
    }
}

void BusBreakerVoltageLevel::clean() {
    // Nothing to do
}

bool BusBreakerVoltageLevel::connect(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    if (terminal.isConnected()) {
        return false;
    }

    dynamic_cast<BusTerminal&>(terminal).setConnected(true);

    invalidateCache();

    return true;
}


void BusBreakerVoltageLevel::deleteStateArrayElement(unsigned long index) {
    m_states.deleteStateArrayElement(index);
}

void BusBreakerVoltageLevel::detach(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    terminal.setVoltageLevel(stdcxx::ref<VoltageLevel>());

    auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
    auto& bus = getConfiguredBus(busTerminal.getConnectableBusId(), true).get();

    getNetwork().getStateManager().forEachState([&bus, &busTerminal, this]() {
        bus.removeTerminal(busTerminal);
        busTerminal.setConnectableBusId("");

        invalidateCache();
    });
}

bool BusBreakerVoltageLevel::disconnect(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    if (!terminal.isConnected()) {
        return false;
    }

    dynamic_cast<BusTerminal&>(terminal).setConnected(false);

    invalidateCache();

    return true;
}


void BusBreakerVoltageLevel::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    m_states.extendStateArraySize(initStateArraySize, number, [this, sourceIndex]() { return m_states.copy(sourceIndex); });
}

const BusBreakerView& BusBreakerVoltageLevel::getBusBreakerView() const {
    return m_busBreakerView;
}

BusBreakerView& BusBreakerVoltageLevel::getBusBreakerView() {
    return m_busBreakerView;
}

const BusView& BusBreakerVoltageLevel::getBusView() const {
    return m_busView;
}

BusView& BusBreakerVoltageLevel::getBusView() {
    return m_busView;
}

bus_breaker_voltage_level::CalculatedBusTopology& BusBreakerVoltageLevel::getCalculatedBusTopology() {
    return m_states.get().getCalculatedBusTopology();
}

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus(const std::string& busId, bool throwException) const {
    stdcxx::Reference<ConfiguredBus> bus;

    const auto& v = getVertex(busId, throwException);
    if (v.is_initialized()) {
        bus = m_graph.getVertexObject(*v);
        if (bus.get().getId() != busId) {
            throw PowsyblException(logging::format("Invalid bus id (expected: '%1%', actual: '%2%')", busId, bus.get().getId()));
        }
    }

    return bus;
}

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus1(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex1(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus2(const std::string& switchId) const {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex2(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::optional<unsigned long> BusBreakerVoltageLevel::getEdge(const std::string& switchId, bool throwException) const {
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

const BusBreakerVoltageLevel::Graph& BusBreakerVoltageLevel::getGraph() const {
    return m_graph;
}

stdcxx::Reference<MergedBus> BusBreakerVoltageLevel::getMergedBus(const std::string& /*busId*/, bool /*throwException*/) const {
    throw AssertionError("TODO");
}

const NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() const {
    throw AssertionError("Not implemented");
}

NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() {
    throw AssertionError("Not implemented");
}

stdcxx::Reference<Switch> BusBreakerVoltageLevel::getSwitch(const std::string& switchId, bool throwException) const {
    stdcxx::Reference<Switch> aSwitch;

    const auto& e = getEdge(switchId, throwException);
    if (e.is_initialized()) {
        aSwitch = m_graph.getEdgeObject(*e);
        if (aSwitch.get().getId() != switchId) {
            throw PowsyblException(logging::format("Invalid switch id (expected: '%1%', actual: '%2%')", switchId, aSwitch.get().getId()));
        }
    }

    return aSwitch;
}

const TopologyKind& BusBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::BUS_BREAKER;

    return s_topologyKind;
}

stdcxx::optional<unsigned long> BusBreakerVoltageLevel::getVertex(const std::string& busId, bool throwException) const {
    checkNotEmpty(busId, "bus id is null");

    const auto& it = m_buses.find(busId);
    if (it != m_buses.end()) {
        return stdcxx::optional<unsigned long>(it->second);
    }
    if (!throwException) {
        return stdcxx::optional<unsigned long>();
    }

    throw PowsyblException(logging::format("Bus '%1%' not found in the voltage level '%2%'", busId, getId()));
}

void BusBreakerVoltageLevel::invalidateCache() {
    m_states.get().getCalculatedBusTopology().invalidateCache();

    // getNetwork().getConnectedComponentsManager().invalidate();
    // getNetwork().getSynchronousComponentsManager().invalidate();
}

void BusBreakerVoltageLevel::reduceStateArraySize(unsigned long number) {
    m_states.reduceStateArraySize(number);
}

void BusBreakerVoltageLevel::removeAllBuses() {
    if (m_graph.getEdgeCount() > 0) {
        throw ValidationException(*this, logging::format("Cannot remove all buses because there is still some switches"));
    }
    for (const auto& it : m_graph.getVertexObjects()) {
        const auto& bus = it.get();
        if (bus.getTerminalCount() > 0) {
            throw ValidationException(*this, logging::format("Cannot remove bus '%1%' due to connected equipments", bus.getId()));
        }
    }
    for (const auto& it : m_graph.getVertexObjects()) {
        getNetwork().remove(it.get());
    }

    m_graph.removeAllVertices();
    m_buses.clear();
}

void BusBreakerVoltageLevel::removeAllSwitches() {
    for (const auto& it : m_graph.getEdgeObjects()) {
        getNetwork().remove(it.get());
    }

    m_graph.removeAllEdges();
    m_switches.clear();
}

void BusBreakerVoltageLevel::removeBus(const std::string& busId) {
    const auto& bus = getConfiguredBus(busId, true);
    if (bus.get().getTerminalCount() > 0) {
        throw ValidationException(*this, logging::format("Cannot remove bus '%1%' due to connectable equipments", busId));
    }

    for (const auto& it : m_switches) {
        const auto& switchId = it.first;

        unsigned long v1 = m_graph.getVertex1(it.second);
        unsigned long v2 = m_graph.getVertex2(it.second);

        const auto& bus1 = m_graph.getVertexObject(v1);
        const auto& bus2 = m_graph.getVertexObject(v2);
        if ((bus == bus1) || (bus == bus2)) {
            throw PowsyblException(logging::format("Cannot remove bus '%1%' due to the connected switch '%2%'", busId, switchId));
        }
    }

    const auto& it = m_buses.find(busId);
    m_graph.removeVertex(it->second);
    m_buses.erase(it);
    getNetwork().remove(bus.get());
}

void BusBreakerVoltageLevel::removeSwitch(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(logging::format("Switch '%1%' not found in voltage level '%2%'", switchId, getId()));
    }

    const auto& aSwitch = m_graph.removeEdge(it->second);
    m_switches.erase(it);
    getNetwork().remove(aSwitch.get());
}

}  // namespace iidm

}  // namespace powsybl
