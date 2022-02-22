/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevel.hpp"

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

BusBreakerVoltageLevel::BusBreakerVoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                                               Network& network, double nominalV, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, fictitious, substation, network, nominalV, lowVoltageLimit, highVoltagelimit),
    m_variants(*this, [this]() { return stdcxx::make_unique<bus_breaker_voltage_level::VariantImpl>(*this); }),
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
    unsigned long v1 = *getVertex(busId1, true);
    unsigned long v2 = *getVertex(busId2, true);

    Switch& aSwitch = getNetwork().checkAndAdd(std::move(ptrSwitch));
    unsigned long e = m_graph.addEdge(v1, v2, stdcxx::ref(aSwitch));
    m_switches.insert(std::make_pair(aSwitch.getId(), e));

    return aSwitch;
}

void BusBreakerVoltageLevel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    VoltageLevel::allocateVariantArrayElement(indexes, sourceIndex);
    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void BusBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
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

void BusBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    if (!stdcxx::isInstanceOf<BusTerminal>(terminal)) {
        throw ValidationException(terminal.getConnectable(),
                                  stdcxx::format("Voltage level '%1%' has a bus/breaker topology, a bus connection should be specified instead of a node connection",
                                                  getId()));
    }

    // check connectable buses exist
    auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
    if (!busTerminal.getConnectableBusId().empty()) {
        getConfiguredBus(busTerminal.getConnectableBusId(), true);
    }
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


void BusBreakerVoltageLevel::deleteVariantArrayElement(unsigned long index) {
    VoltageLevel::deleteVariantArrayElement(index);
    m_variants.deleteVariantArrayElement(index);
}

void BusBreakerVoltageLevel::detach(Terminal& terminal) {
    assert(stdcxx::isInstanceOf<BusTerminal>(terminal));

    auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
    auto& bus = getConfiguredBus(busTerminal.getConnectableBusId(), true).get();

    getNetwork().getVariantManager().forEachVariant([&bus, &busTerminal, this]() {
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

void BusBreakerVoltageLevel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    VoltageLevel::extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

const BusBreakerVoltageLevel::BusBreakerView& BusBreakerVoltageLevel::getBusBreakerView() const {
    return m_busBreakerView;
}

BusBreakerVoltageLevel::BusBreakerView& BusBreakerVoltageLevel::getBusBreakerView() {
    return m_busBreakerView;
}

const BusBreakerVoltageLevel::BusView& BusBreakerVoltageLevel::getBusView() const {
    return m_busView;
}

BusBreakerVoltageLevel::BusView& BusBreakerVoltageLevel::getBusView() {
    return m_busView;
}

bus_breaker_voltage_level::CalculatedBusTopology& BusBreakerVoltageLevel::getCalculatedBusTopology() {
    return m_variants.get().getCalculatedBusTopology();
}

stdcxx::CReference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus(const std::string& busId, bool throwException) const {
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

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus(const std::string& busId, bool throwException) {
    return stdcxx::ref(static_cast<const BusBreakerVoltageLevel*>(this)->getConfiguredBus(busId, throwException));
}

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus1(const std::string& switchId) {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex1(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::Reference<ConfiguredBus> BusBreakerVoltageLevel::getConfiguredBus2(const std::string& switchId) {
    const auto& e = getEdge(switchId, true);
    const auto& v = m_graph.getVertex2(*e);
    return m_graph.getVertexObject(v);
}

stdcxx::optional<unsigned long> BusBreakerVoltageLevel::getEdge(const std::string& switchId, bool throwException) const {
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

const BusBreakerVoltageLevel::Graph& BusBreakerVoltageLevel::getGraph() const {
    return m_graph;
}

stdcxx::Reference<MergedBus> BusBreakerVoltageLevel::getMergedBus(const std::string& busId, bool throwException) {
    return getCalculatedBusTopology().getMergedBus(busId, throwException);
}

const BusBreakerVoltageLevel::NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() const {
    throw AssertionError("Not implemented");
}

BusBreakerVoltageLevel::NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() {
    throw AssertionError("Not implemented");
}

stdcxx::Reference<Switch> BusBreakerVoltageLevel::getSwitch(const std::string& switchId, bool throwException) {
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

unsigned long BusBreakerVoltageLevel::getSwitchCount() const {
    return m_graph.getEdgeCount();
}

stdcxx::const_range<Switch> BusBreakerVoltageLevel::getSwitches() const {
    return getBusBreakerView().getSwitches();
}

stdcxx::range<Switch> BusBreakerVoltageLevel::getSwitches() {
    return getBusBreakerView().getSwitches();
}

stdcxx::const_range<Terminal> BusBreakerVoltageLevel::getTerminals() const {
    const auto& mapper = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return bus.get().getTerminals();
    };

    return m_graph.getVertexObjects() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<Terminal> BusBreakerVoltageLevel::getTerminals() {
    const auto& mapper = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return bus.get().getTerminals();
    };

    return m_graph.getVertexObjects() | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

const TopologyKind& BusBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::BUS_BREAKER;

    return s_topologyKind;
}

stdcxx::optional<unsigned long> BusBreakerVoltageLevel::getVertex(const std::string& busId, bool throwException) const {
    checkNotEmpty(busId, "bus id is null");

    const auto& it = m_buses.find(busId);
    if (it != m_buses.end()) {
        return {it->second};
    }
    if (!throwException) {
        return {};
    }

    throw PowsyblException(stdcxx::format("Bus '%1%' not found in the voltage level '%2%'", busId, getId()));
}

void BusBreakerVoltageLevel::invalidateCache() {
    m_variants.get().getCalculatedBusTopology().invalidateCache();
    getNetwork().getBusView().invalidateCache();
    getNetwork().getBusBreakerView().invalidateCache();
    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();
}

void BusBreakerVoltageLevel::reduceVariantArraySize(unsigned long number) {
    VoltageLevel::reduceVariantArraySize(number);
    m_variants.reduceVariantArraySize(number);
}

void BusBreakerVoltageLevel::removeAllBuses() {
    if (m_graph.getEdgeCount() > 0) {
        throw ValidationException(*this, stdcxx::format("Cannot remove all buses because there is still some switches"));
    }
    for (const auto& it : m_graph.getVertexObjects()) {
        const auto& bus = it.get();
        if (bus.getTerminalCount() > 0) {
            throw ValidationException(*this, stdcxx::format("Cannot remove bus '%1%' due to connected equipments", bus.getId()));
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
        throw ValidationException(*this, stdcxx::format("Cannot remove bus '%1%' due to connectable equipments", busId));
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

void BusBreakerVoltageLevel::removeSwitch(const std::string& switchId) {
    const auto& it = m_switches.find(switchId);
    if (it == m_switches.end()) {
        throw PowsyblException(stdcxx::format("Switch '%1%' not found in voltage level '%2%'", switchId, getId()));
    }

    const auto& aSwitch = m_graph.removeEdge(it->second);
    m_switches.erase(it);
    getNetwork().remove(aSwitch.get());
}

void BusBreakerVoltageLevel::removeTopology() {
    removeAllSwitches();
    removeAllBuses();
}

void BusBreakerVoltageLevel::traverse(BusTerminal& terminal, VoltageLevel::TopologyTraverser& traverser) const {
    TerminalSet traversedTerminals;
    traverse(terminal, traverser, traversedTerminals);
}

void BusBreakerVoltageLevel::traverse(BusTerminal& terminal, VoltageLevel::TopologyTraverser& traverser, TerminalSet& traversedTerminals) const {
    if (traversedTerminals.find(terminal) != traversedTerminals.end()) {
        return;
    }

    TerminalSet nextTerminals;

    // check if we are allowed to traverse the terminal itself
    if (traverser.traverse(terminal, terminal.isConnected())) {
        traversedTerminals.emplace(terminal);

        addNextTerminals(terminal, nextTerminals);

        // then check we can traverse terminal connected to same bus
        unsigned long v = *getVertex(terminal.getConnectableBusId(), true);
        ConfiguredBus& bus = m_graph.getVertexObject(v);
        for (Terminal& t : bus.getTerminals()) {
            if (!stdcxx::areSame(t, terminal) && traverser.traverse(t, t.isConnected())) {
                addNextTerminals(t, nextTerminals);
            }
        }

        // then go through other buses of the voltage level
        m_graph.traverse(v, [this, &traverser, &traversedTerminals, &nextTerminals](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
            Switch& aSwitch = m_graph.getEdgeObject(e);
            ConfiguredBus& otherBus = m_graph.getVertexObject(v2);
            if (traverser.traverse(aSwitch)) {
                if (otherBus.getTerminalCount() == 0) {
                    return math::TraverseResult::CONTINUE;
                }

                BusTerminal& otherTerminal = *otherBus.getTerminals().begin();
                if (traverser.traverse(otherTerminal, otherTerminal.isConnected())) {
                    traversedTerminals.emplace(otherTerminal);

                    addNextTerminals(otherTerminal, nextTerminals);
                    return math::TraverseResult::CONTINUE;
                }
            }
            return math::TraverseResult::TERMINATE;
        });

        for (Terminal& t : nextTerminals) {
            t.traverse(traverser, traversedTerminals);
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
