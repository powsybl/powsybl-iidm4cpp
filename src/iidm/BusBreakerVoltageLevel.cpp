/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevel.hpp"

#include "ConfiguredBus.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

BusBreakerVoltageLevel::BusBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                               double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit),
    m_busBreakerView(*this),
    m_busView(*this) {
}

Bus& BusBreakerVoltageLevel::addBus(std::unique_ptr<ConfiguredBus>&& ptrBus) {
    ConfiguredBus& bus = getNetwork().checkAndAdd(std::move(ptrBus));

    unsigned long node = m_graph.addVertex();
    m_graph.setVertexObject(node, stdcxx::Optional<ConfiguredBus>(bus));
    m_buses.insert(std::make_pair(bus.getId(), node));

    return bus;
}

void BusBreakerVoltageLevel::attach(Terminal& terminal, bool test) {
    checkTerminal(terminal);
    if (!test) {
        // create the link terminal -> voltage level
        terminal.setVoltageLevel(stdcxx::Optional<VoltageLevel>(*this));

        auto& busTerminal = dynamic_cast<BusTerminal&>(terminal);
        const stdcxx::Optional<ConfiguredBus>& connectableBus = getBus(busTerminal.getConnectableBusId(), true);

        getNetwork().getStateManager().forEachState([&connectableBus, &busTerminal, this]() {
            connectableBus.get().addTerminal(busTerminal);

            invalidateCache();
        });
    }
}

void BusBreakerVoltageLevel::checkTerminal(Terminal& terminal) const {
    auto busTerminal = dynamic_cast<BusTerminal*>(&terminal);
    if (busTerminal == nullptr) {
        throw ValidationException(terminal.getConnectable(),
                                  logging::format("Voltage level %1% has a bus/breaker topology, a bus connection should be specified instead of a node connection",
                                                  getId()));
    }
}

void BusBreakerVoltageLevel::clean() {
    // TODO
}

bool BusBreakerVoltageLevel::connect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

void BusBreakerVoltageLevel::detach(Terminal& /*terminal*/) {
    // TODO
}

bool BusBreakerVoltageLevel::disconnect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

stdcxx::Optional<ConfiguredBus> BusBreakerVoltageLevel::getBus(const std::string& busId, bool throwException) const {
    stdcxx::Optional<ConfiguredBus> bus;

    const auto& v = getVertex(busId, throwException);
    if (v.is_initialized()) {
        bus = m_graph.getVertexObject(*v);
        if (bus.get().getId() != busId) {
            throw PowsyblException(logging::format("Invalid bus id (expected: %1%, actual: %2%", busId, bus.get().getId()));
        }
    }

    return bus;
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

const NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() const {
    throw AssertionError("Not implemented");
}

NodeBreakerView& BusBreakerVoltageLevel::getNodeBreakerView() {
    throw AssertionError("Not implemented");
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
    } else if (!throwException) {
        return stdcxx::optional<unsigned long>();
    }

    throw PowsyblException(logging::format("Bus %1% not found in the voltage level %2%", busId, getId()));
}

void BusBreakerVoltageLevel::invalidateCache() {
    // TODO
}

}

}
