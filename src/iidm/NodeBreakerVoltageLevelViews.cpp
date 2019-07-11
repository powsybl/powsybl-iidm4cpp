/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelViews.hpp"

#include <powsybl/iidm/BusbarSection.hpp>

#include "CalculatedBus.hpp"
#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    const stdcxx::Reference<CalculatedBus>& bus = m_voltageLevel.getCalculatedBusBreakerTopology().getBus(busId, false);

    return stdcxx::ref<Bus>(bus);
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    const stdcxx::Reference<CalculatedBus>& bus = m_voltageLevel.getCalculatedBusBreakerTopology().getBus1(switchId, true);

    return stdcxx::ref<Bus>(bus);
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    const stdcxx::Reference<CalculatedBus>& bus = m_voltageLevel.getCalculatedBusBreakerTopology().getBus2(switchId, true);

    return stdcxx::ref<Bus>(bus);
}

std::vector<std::reference_wrapper<Bus> > BusBreakerViewImpl::getBuses() const {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusBreakerTopology().getBuses();

    std::vector<std::reference_wrapper<Bus> > buses;
    std::transform(calculatedBuses.begin(), calculatedBuses.end(), std::back_inserter(buses), [](const std::reference_wrapper<CalculatedBus>& bus) {
        return std::ref<Bus>(bus);
    });

    return buses;
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_voltageLevel.getCalculatedBusBreakerTopology().getSwitch(switchId, true);
}

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getSwitchCount();
}

BusAdder BusBreakerViewImpl::newBus() {
    throw AssertionError("Not implemented");
}

BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
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

BusViewImpl::BusViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    const stdcxx::Reference<CalculatedBus>& bus = m_voltageLevel.getCalculatedBusTopology().getBus(busId, false);

    return stdcxx::ref<Bus>(bus);
}

std::vector<std::reference_wrapper<Bus> > BusViewImpl::getBuses() const {
    const auto& calculatedBuses = m_voltageLevel.getCalculatedBusTopology().getBuses();

    std::vector<std::reference_wrapper<Bus> > buses;
    std::transform(calculatedBuses.begin(), calculatedBuses.end(), std::back_inserter(buses), [](const std::reference_wrapper<CalculatedBus>& bus) {
        return std::ref<Bus>(bus);
    });

    return buses;
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& busbarSectionId) const {
    auto& terminal = dynamic_cast<NodeTerminal&>(m_voltageLevel.getNodeBreakerView().getBusbarSection(busbarSectionId)->getTerminal());

    const stdcxx::Reference<CalculatedBus>& bus = m_voltageLevel.getCalculatedBusTopology().getBus(terminal.getNode());

    return stdcxx::ref<Bus>(bus);
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& bbsId) const {
    return m_voltageLevel.getConnectable<BusbarSection>(bbsId);
}

unsigned long NodeBreakerViewImpl::getBusbarSectionCount() const {
    return m_voltageLevel.getConnectableCount<BusbarSection>();
}

unsigned long NodeBreakerViewImpl::getInternalConnectionCount() const {
    return m_voltageLevel.getInternalConnectionCount();
}

unsigned long NodeBreakerViewImpl::getNode1(const std::string& switchId) const {
    return m_voltageLevel.getNode1(switchId);
}

unsigned long NodeBreakerViewImpl::getNode2(const std::string& switchId) const {
    return m_voltageLevel.getNode2(switchId);
}

unsigned long NodeBreakerViewImpl::getNodeCount() const {
    return m_voltageLevel.getNodeCount();
}

stdcxx::Reference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_voltageLevel.getSwitch(switchId);
}

unsigned long NodeBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getSwitchCount();
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long node) const {
    return m_voltageLevel.getTerminal(node);
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& switchId) const {
    return getTerminal(getNode1(switchId));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& switchId) const {
    return getTerminal(getNode2(switchId));
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

void NodeBreakerViewImpl::removeSwitch(const std::string& switchId) {
    m_voltageLevel.removeSwitch(switchId);
}

NodeBreakerView& NodeBreakerViewImpl::setNodeCount(unsigned long nodeCount) {
    m_voltageLevel.setNodeCount(nodeCount);
    return *this;
}

void NodeBreakerViewImpl::traverse(unsigned long node, const Traverser& traverser) const {
    powsybl::math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(v1, m_voltageLevel.getGraph().getEdgeObject(e), v2) ? powsybl::math::TraverseResult::CONTINUE : powsybl::math::TraverseResult::TERMINATE;
    };

    m_voltageLevel.getGraph().traverse(node, graphTraverser);
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
