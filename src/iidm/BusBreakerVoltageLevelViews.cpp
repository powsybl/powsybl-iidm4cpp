/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelViews.hpp"

#include "BusBreakerVoltageLevel.hpp"
#include "ConfiguredBus.hpp"
#include "MergedBus.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

BusBreakerViewImpl::BusBreakerViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    const stdcxx::Reference<ConfiguredBus>& bus = m_voltageLevel.getConfiguredBus(busId, false);

    return stdcxx::ref<Bus>(bus);
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    const stdcxx::Reference<ConfiguredBus>& bus = m_voltageLevel.getConfiguredBus1(switchId);

    return stdcxx::ref<Bus>(bus);
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    const stdcxx::Reference<ConfiguredBus>& bus = m_voltageLevel.getConfiguredBus2(switchId);

    return stdcxx::ref<Bus>(bus);
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_voltageLevel.getSwitch(switchId, false);
}

BusAdder BusBreakerViewImpl::newBus() {
    return BusAdder(m_voltageLevel);
}

BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
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

BusViewImpl::BusViewImpl(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    const stdcxx::Reference<MergedBus> mergedBus = m_voltageLevel.getMergedBus(busId, false);

    return stdcxx::ref<Bus>(mergedBus);
}

}

}

}
