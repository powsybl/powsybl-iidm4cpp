/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusTerminalViews.hpp"

#include "BusBreakerVoltageLevel.hpp"
#include "BusTerminal.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

namespace bus_terminal {

BusBreakerViewImpl::BusBreakerViewImpl(BusTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus() const {
    return m_terminal.isConnected() ? getConnectableBus() : stdcxx::ref<Bus>();
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getConnectableBus() const {
    const auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::ref<Bus>(voltageLevel.getConfiguredBus(m_terminal.getConnectableBusId(), true));
}

void BusBreakerViewImpl::setConnectableBus(const std::string& busId) {
    checkNotEmpty(busId, "busId is empty");

    auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    // Assert that the new bus exists
    voltageLevel.getConfiguredBus(busId, true);

    voltageLevel.detach(m_terminal);
    m_terminal.setConnectableBusId(busId);
    voltageLevel.attach(m_terminal, false);
}

BusViewImpl::BusViewImpl(powsybl::iidm::BusTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::Reference<Bus> BusViewImpl::getBus() const {
    return m_terminal.isConnected() ? getConnectableBus() : stdcxx::ref<Bus>();
}

stdcxx::Reference<Bus> BusViewImpl::getConnectableBus() const {
    auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    const auto& configuredBus = voltageLevel.getConfiguredBus(m_terminal.getConnectableBusId(), true);

    return stdcxx::ref<Bus>(voltageLevel.getCalculatedBusTopology().getMergedBus(configuredBus));
}

}  // namespace bus_terminal

}  // namespace iidm

}  // namespace powsybl
