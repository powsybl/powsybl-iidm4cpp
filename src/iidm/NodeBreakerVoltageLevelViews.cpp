/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelViews.hpp"

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/cast.hpp>

#include "CalculatedBus.hpp"
#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus(busId, false));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus(busId, false));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus1(switchId, true));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus1(switchId, true));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus2(switchId, true));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusBreakerTopology().getBus2(switchId, true));
}

stdcxx::const_range<Bus> BusBreakerViewImpl::getBuses() const {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusBreakerTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusBreakerViewImpl::getBuses() {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusBreakerTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_voltageLevel.getCalculatedBusBreakerTopology().getSwitch(switchId, true);
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) {
    return stdcxx::ref(m_voltageLevel.getCalculatedBusBreakerTopology().getSwitch(switchId, true));
}

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getCalculatedBusBreakerTopology().getSwitchCount();
}

stdcxx::const_range<Switch> BusBreakerViewImpl::getSwitches() const {
    return m_voltageLevel.getCalculatedBusBreakerTopology().getSwitches();
}

stdcxx::range<Switch> BusBreakerViewImpl::getSwitches() {
    return m_voltageLevel.getCalculatedBusBreakerTopology().getSwitches();
}

BusAdder BusBreakerViewImpl::newBus() {
    throw AssertionError("Not implemented");
}

VoltageLevel::BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeAllBuses() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeAllSwitches() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeBus(const std::string& /*busId*/) {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeSwitch(const std::string& /*switchId*/) {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::traverse(const Bus& /*bus*/, const TopologyTraverser& /*traverser*/) {
    throw PowsyblException("Not supported in a node/breaker topology");
}

BusViewImpl::BusViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusTopology().getBus(busId, false));
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusTopology().getBus(busId, false));
}

stdcxx::const_range<Bus> BusViewImpl::getBuses() const {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusViewImpl::getBuses() {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}


stdcxx::CReference<Bus> BusViewImpl::getMergedBus(const std::string& busbarSectionId) const {
    auto& terminal = dynamic_cast<NodeTerminal&>(m_voltageLevel.getNodeBreakerView().getBusbarSection(busbarSectionId).get().getTerminal());

    return stdcxx::cref<Bus>(m_voltageLevel.getCalculatedBusTopology().getBus(terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& busbarSectionId) {
    auto& terminal = dynamic_cast<NodeTerminal&>(m_voltageLevel.getNodeBreakerView().getBusbarSection(busbarSectionId).get().getTerminal());

    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusTopology().getBus(terminal.getNode()));
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::CReference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& bbsId) const {
    return stdcxx::cref(m_voltageLevel.getConnectable<BusbarSection>(bbsId));
}

stdcxx::Reference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& bbsId) {
    return m_voltageLevel.getConnectable<BusbarSection>(bbsId);
}

unsigned long NodeBreakerViewImpl::getBusbarSectionCount() const {
    return m_voltageLevel.getConnectableCount<BusbarSection>();
}

stdcxx::const_range<BusbarSection> NodeBreakerViewImpl::getBusbarSections() const {
    return m_voltageLevel.getConnectables<BusbarSection>();
}

stdcxx::range<BusbarSection> NodeBreakerViewImpl::getBusbarSections() {
    return m_voltageLevel.getConnectables<BusbarSection>();
}

unsigned long NodeBreakerViewImpl::getInternalConnectionCount() const {
    return m_voltageLevel.getInternalConnectionCount();
}

stdcxx::const_range<NodeBreakerViewImpl::InternalConnection> NodeBreakerViewImpl::getInternalConnections() const {
    const auto& filter = [this](const unsigned long& e) {
        return !m_voltageLevel.getGraph().getEdgeObject(e);
    };

    const auto& mapper = [this](const unsigned long& e) {
        unsigned long node1 = m_voltageLevel.getGraph().getVertex1(e);
        unsigned long node2 = m_voltageLevel.getGraph().getVertex2(e);

        return InternalConnection(node1, node2);
    };

    return m_voltageLevel.getGraph().getEdges() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

long NodeBreakerViewImpl::getMaximumNodeIndex() const {
    return m_voltageLevel.getMaximumNodeIndex();
}

unsigned long NodeBreakerViewImpl::getNode1(const std::string& switchId) const {
    return m_voltageLevel.getNode1(switchId);
}

unsigned long NodeBreakerViewImpl::getNode2(const std::string& switchId) const {
    return m_voltageLevel.getNode2(switchId);
}

stdcxx::const_range<unsigned long> NodeBreakerViewImpl::getNodes() const {
    return m_voltageLevel.getGraph().getVertices();
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getOptionalTerminal(unsigned long node) const {
    if (m_voltageLevel.getGraph().vertexExists(node)) {
        return stdcxx::cref<Terminal>(m_voltageLevel.getGraph().getVertexObject(node));
    }
    return stdcxx::cref<Terminal>();
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getOptionalTerminal(unsigned long node) {
    if (m_voltageLevel.getGraph().vertexExists(node)) {
        return stdcxx::ref<Terminal>(m_voltageLevel.getGraph().getVertexObject(node));
    }
    return stdcxx::ref<Terminal>();
}

stdcxx::CReference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_voltageLevel.getSwitch(switchId);
}

stdcxx::Reference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& switchId) {
    return stdcxx::ref(m_voltageLevel.getSwitch(switchId));
}

unsigned long NodeBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getSwitchCount();
}

stdcxx::const_range<Switch> NodeBreakerViewImpl::getSwitches() const {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_voltageLevel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Switch> NodeBreakerViewImpl::getSwitches() {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_voltageLevel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long node) const {
    return stdcxx::cref(m_voltageLevel.getTerminal(node));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long node) {
    return m_voltageLevel.getTerminal(node);
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& switchId) const {
    return getTerminal(getNode1(switchId));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& switchId) {
    return getTerminal(getNode1(switchId));
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& switchId) const {
    return getTerminal(getNode2(switchId));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& switchId) {
    return getTerminal(getNode2(switchId));
}

bool NodeBreakerViewImpl::hasAttachedEquipment(unsigned long node) const {
    return m_voltageLevel.getGraph().vertexExists(node);
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newBreaker() {
    return SwitchAdder(m_voltageLevel).setKind(SwitchKind::BREAKER);
}

BusbarSectionAdder NodeBreakerViewImpl::newBusbarSection() {
    return BusbarSectionAdder(m_voltageLevel);
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newDisconnector() {
    return SwitchAdder(m_voltageLevel).setKind(SwitchKind::DISCONNECTOR);
}

NodeBreakerViewImpl::InternalConnectionAdder NodeBreakerViewImpl::newInternalConnection() {
    return InternalConnectionAdder(m_voltageLevel);
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newSwitch() {
    return SwitchAdder(m_voltageLevel);
}

void NodeBreakerViewImpl::removeInternalConnections(unsigned long node1, unsigned long node2) {
    m_voltageLevel.removeInternalConnections(node1, node2);
}

void NodeBreakerViewImpl::removeSwitch(const std::string& switchId) {
    m_voltageLevel.removeSwitch(switchId);
}

void NodeBreakerViewImpl::traverse(unsigned long node, const TopologyTraverser& traverser) const {
    math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(v1, m_voltageLevel.getGraph().getEdgeObject(e), v2);
    };

    m_voltageLevel.getGraph().traverse(node, graphTraverser);
}

void NodeBreakerViewImpl::traverse(stdcxx::const_range<unsigned long>& nodes, const TopologyTraverser& traverser) const {
    powsybl::math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(v1, m_voltageLevel.getGraph().getEdgeObject(e), v2);
    };

    m_voltageLevel.getGraph().traverse(nodes, graphTraverser);
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
