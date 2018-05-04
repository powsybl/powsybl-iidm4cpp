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

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& /*bbsId*/) const {
    throw AssertionError("TODO");
}

unsigned long NodeBreakerViewImpl::getBusbarSectionCount() const {
    throw AssertionError("TODO");
}

unsigned long NodeBreakerViewImpl::getNode1(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

unsigned long NodeBreakerViewImpl::getNode2(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

unsigned long NodeBreakerViewImpl::getNodeCount() const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

unsigned long NodeBreakerViewImpl::getSwitchCount() const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long /*node*/) const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newBreaker() {
    throw AssertionError("TODO");
}

BusbarSectionAdder NodeBreakerViewImpl::newBusbarSection() {
    throw AssertionError("TODO");
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newDisconnector() {
    throw AssertionError("TODO");
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newSwitch() {
    throw AssertionError("TODO");
}

void NodeBreakerViewImpl::removeSwitch(const std::string& /*switchId*/) {
    throw AssertionError("TODO");
}

void NodeBreakerViewImpl::setNodeCount(unsigned long /*nodeCount*/) {
    throw AssertionError("TODO");
}

void NodeBreakerViewImpl::traverse(unsigned long /*node*/, const Traverser& /*traverser*/) {
    throw AssertionError("TODO");
}

}

}

}
