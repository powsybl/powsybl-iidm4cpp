/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerTopologyViews.hpp"

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/cast.hpp>

#include "BusBreakerTopologyCalculatedBus.hpp"
#include "BusBreakerTopologyModel.hpp"
#include "ConfiguredBus.hpp"
#include "MergedBus.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_topology_model {

BusBreakerViewImpl::BusBreakerViewImpl(BusBreakerTopologyModel& topologyModel) :
    m_topologyModel(topologyModel) {
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getConfiguredBus(busId, false));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) {
    const auto& bus = m_topologyModel.getNetwork().find<Bus>(busId);
    if (bus) {
        return stdcxx::ref(bus.get());
    }
    return stdcxx::ref<Bus>(m_topologyModel.getConfiguredBus(busId, false));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getConfiguredBus1(switchId));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_topologyModel.getConfiguredBus1(switchId));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getConfiguredBus2(switchId));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_topologyModel.getConfiguredBus2(switchId));
}

unsigned long BusBreakerViewImpl::getBusCount() const {
    return m_topologyModel.getGraph().getVertexCount();
}

stdcxx::const_range<Bus> BusBreakerViewImpl::getBuses() const {
    const auto& filter = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return static_cast<bool>(bus);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ConfiguredBus>, Bus>;

    return m_topologyModel.getGraph().getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusBreakerViewImpl::getBuses() {
    const auto& filter = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return static_cast<bool>(bus);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ConfiguredBus>, Bus>;

    return m_topologyModel.getGraph().getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return stdcxx::cref(m_topologyModel.getSwitch(switchId, false));
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) {
    return m_topologyModel.getSwitch(switchId, false);
}

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_topologyModel.getSwitchCount();
}

stdcxx::const_range<Switch> BusBreakerViewImpl::getSwitches() const {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_topologyModel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Switch> BusBreakerViewImpl::getSwitches() {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_topologyModel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

BusAdder BusBreakerViewImpl::newBus() {
    return BusAdder(m_topologyModel.getVoltageLevel());
}

VoltageLevel::BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
    return BusBreakerView::SwitchAdder(m_topologyModel.getVoltageLevel());
}

void BusBreakerViewImpl::removeAllBuses() {
    m_topologyModel.removeAllBuses();
}

void BusBreakerViewImpl::removeAllSwitches() {
    m_topologyModel.removeAllSwitches();
}

void BusBreakerViewImpl::removeBus(const std::string& busId) {
    m_topologyModel.removeBus(busId);
}

void BusBreakerViewImpl::removeSwitch(const std::string& switchId) {
    m_topologyModel.removeSwitch(switchId);
}

void BusBreakerViewImpl::traverse(const Bus& bus, const TopologyTraverser& traverser) {
    const auto& graph = m_topologyModel.getGraph();
    math::Traverser graphTraverser = [&graph, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(graph.getVertexObject(v1), graph.getEdgeObject(e), graph.getVertexObject(v2));
    };

    graph.traverse(*m_topologyModel.getVertex(bus.getId(), true), math::TraversalType::DEPTH_FIRST, graphTraverser);
}

BusViewImpl::BusViewImpl(BusBreakerTopologyModel& topologyModel) :
    m_topologyModel(topologyModel) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getMergedBus(busId, false));
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_topologyModel.getMergedBus(busId, false));
}

unsigned long BusViewImpl::getBusCount() const {
    return m_topologyModel.getCalculatedBusTopology().getMergedBusesCount();
}

stdcxx::const_range<Bus> BusViewImpl::getBuses() const {
    const auto& mergedBuses = m_topologyModel.getCalculatedBusTopology().getMergedBuses();

    const auto& mapper = stdcxx::upcast<MergedBus, Bus>;

    return mergedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusViewImpl::getBuses() {
    const auto& mergedBuses = m_topologyModel.getCalculatedBusTopology().getMergedBuses();

    const auto& mapper = stdcxx::upcast<MergedBus, Bus>;

    return mergedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Bus> BusViewImpl::getMergedBus(const std::string& configuredBusId) const {
    const auto& configuredBus = stdcxx::ref<ConfiguredBus>(m_topologyModel.getBusBreakerView().getBus(configuredBusId));

    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& configuredBusId) {
    const auto& configuredBus = stdcxx::ref<ConfiguredBus>(m_topologyModel.getBusBreakerView().getBus(configuredBusId));

    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

}  // namespace bus_breaker_topology_model

}  // namespace iidm

}  // namespace powsybl
