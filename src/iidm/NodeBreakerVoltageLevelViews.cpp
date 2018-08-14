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

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& /*busId*/) const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
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

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& /*busId*/) const {
    throw AssertionError("TODO");
}

std::vector<std::reference_wrapper<Bus> > BusViewImpl::getBuses() const {
    // TODO(mathbagu)
    return std::vector<std::reference_wrapper<Bus> >();
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& /*busbarSectionId*/) const {
    // TODO(mathbagu)
    return stdcxx::ref<Bus>();
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& /*bbsId*/) const {
    // TODO(mathbagu): return m_voltageLevel.getConnectable<BusbarSection>(bbsId);
    return stdcxx::ref<BusbarSection>();
}

unsigned long NodeBreakerViewImpl::getBusbarSectionCount() const {
    // TODO(mathbagu): return m_voltageLevel.getConnectableCount<BusbarSection>();
    return 0;
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

void NodeBreakerViewImpl::traverse(unsigned long /*node*/, const Traverser& /*traverser*/) {
    throw AssertionError("TODO");
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
