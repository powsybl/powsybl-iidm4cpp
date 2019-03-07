/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelViews.hpp"

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

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_voltageLevel.getSwitchCount();
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
    const stdcxx::Reference<MergedBus>& mergedBus = m_voltageLevel.getMergedBus(busId, false);

    return stdcxx::ref<Bus>(mergedBus);
}

std::vector<std::reference_wrapper<Bus> > BusViewImpl::getBuses() const {
    const auto& mergedBuses = m_voltageLevel.getCalculatedBusTopology().getMergedBuses();

    std::vector<std::reference_wrapper<Bus> > buses;
    buses.reserve(mergedBuses.size());
    std::transform(mergedBuses.begin(), mergedBuses.end(), std::back_inserter(buses), [](const std::reference_wrapper<MergedBus>& bus) {
        return std::ref<Bus>(bus);
    });

    return buses;
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& configuredBusId) const {
    stdcxx::Reference<ConfiguredBus> configuredBus = stdcxx::ref<ConfiguredBus>(m_voltageLevel.getBusBreakerView().getBus(configuredBusId));

    return stdcxx::ref<Bus>(m_voltageLevel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
