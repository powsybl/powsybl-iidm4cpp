/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelViews.hpp"

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/cast.hpp>

#include "BusBreakerVoltageLevel.hpp"
#include "BusBreakerVoltageLevelTopology.hpp"
#include "ConfiguredBus.hpp"
#include "MergedBus.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getConfiguredBus(busId, false));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) {
    const auto& bus = m_voltageLevel.getNetwork().find<Bus>(busId);
    if (bus) {
        return stdcxx::ref(bus.get());
    }
    return stdcxx::ref<Bus>(m_voltageLevel.getConfiguredBus(busId, false));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getConfiguredBus1(switchId));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getConfiguredBus1(switchId));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getConfiguredBus2(switchId));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getConfiguredBus2(switchId));
}

stdcxx::const_range<Bus> BusBreakerViewImpl::getBuses() const {
    const auto& filter = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return static_cast<bool>(bus);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ConfiguredBus>, Bus>;

    return m_voltageLevel.getGraph().getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusBreakerViewImpl::getBuses() {
    const auto& filter = [](const stdcxx::Reference<ConfiguredBus>& bus) {
        return static_cast<bool>(bus);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ConfiguredBus>, Bus>;

    return m_voltageLevel.getGraph().getVertexObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return stdcxx::cref(m_voltageLevel.getSwitch(switchId, false));
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) {
    return m_voltageLevel.getSwitch(switchId, false);
}

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getSwitchCount();
}

stdcxx::const_range<Switch> BusBreakerViewImpl::getSwitches() const {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_voltageLevel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Switch> BusBreakerViewImpl::getSwitches() {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_voltageLevel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

BusAdder BusBreakerViewImpl::newBus() {
    return BusAdder(m_voltageLevel);
}

VoltageLevel::BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
    return BusBreakerView::SwitchAdder(m_voltageLevel);
}

void BusBreakerViewImpl::removeAllBuses() {
    m_voltageLevel.removeAllBuses();
}

void BusBreakerViewImpl::removeAllSwitches() {
    m_voltageLevel.removeAllSwitches();
}

void BusBreakerViewImpl::removeBus(const std::string& busId) {
    m_voltageLevel.removeBus(busId);
}

void BusBreakerViewImpl::removeSwitch(const std::string& switchId) {
    m_voltageLevel.removeSwitch(switchId);
}

void BusBreakerViewImpl::traverse(const Bus& bus, TopologyTraverser& traverser) {
    math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        const auto& graph = m_voltageLevel.getGraph();
        return traverser(graph.getVertexObject(v1), graph.getEdgeObject(e), graph.getVertexObject(v2));
    };

    m_voltageLevel.getGraph().traverse(*m_voltageLevel.getVertex(bus.getId(), true), graphTraverser);
}

BusViewImpl::BusViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getMergedBus(busId, false));
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getMergedBus(busId, false));
}

stdcxx::const_range<Bus> BusViewImpl::getBuses() const {
    const auto& mergedBuses = m_voltageLevel.getCalculatedBusTopology().getMergedBuses();

    const auto& mapper = stdcxx::upcast<MergedBus, Bus>;

    return mergedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusViewImpl::getBuses() {
    const auto& mergedBuses = m_voltageLevel.getCalculatedBusTopology().getMergedBuses();

    const auto& mapper = stdcxx::upcast<MergedBus, Bus>;

    return mergedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Bus> BusViewImpl::getMergedBus(const std::string& configuredBusId) const {
    const auto& configuredBus = stdcxx::ref<ConfiguredBus>(m_voltageLevel.getBusBreakerView().getBus(configuredBusId));

    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& configuredBusId) {
    const auto& configuredBus = stdcxx::ref<ConfiguredBus>(m_voltageLevel.getBusBreakerView().getBus(configuredBusId));

    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
