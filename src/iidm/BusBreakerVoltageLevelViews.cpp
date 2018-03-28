/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelViews.hpp"

#include "BusBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

Bus& BusBreakerViewImpl::getBus(const std::string& /*busId*/) const {
    throw AssertionError("TODO");
}

Bus& BusBreakerViewImpl::getBus1(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

Bus& BusBreakerViewImpl::getBus2(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

Switch& BusBreakerViewImpl::getSwitch(const std::string& /*switchId*/) const {
    throw AssertionError("TODO");
}

BusAdder BusBreakerViewImpl::newBus() {
    return BusAdder(m_voltageLevel);
}

BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
    return BusBreakerView::SwitchAdder(m_voltageLevel);
}

void BusBreakerViewImpl::removeAllBuses() {
    throw AssertionError("TODO");
}

void BusBreakerViewImpl::removeAllSwitches() {
    throw AssertionError("TODO");
}

void BusBreakerViewImpl::removeBus(const std::string& /*busId*/) {
    throw AssertionError("TODO");
}

void BusBreakerViewImpl::removeSwitch(const std::string& /*switchId*/) {
    throw AssertionError("TODO");
}

BusViewImpl::BusViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

Bus& BusViewImpl::getBus(const std::string& /*busId*/) const {
    throw AssertionError("TODO");
}

}

}

}