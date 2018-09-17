/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeTerminalViews.hpp"

#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_terminal {

BusBreakerViewImpl::BusBreakerViewImpl(powsybl::iidm::NodeTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::ref<Bus>(voltageLevel.getCalculatedBusBreakerTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getConnectableBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return voltageLevel.getCalculatedBusBreakerTopology().getConnectableBus(m_terminal.getNode());
}

void BusBreakerViewImpl::setConnectableBus(const std::string& /*busId*/) {
    throw AssertionError("Not implemented");
}

BusViewImpl::BusViewImpl(powsybl::iidm::NodeTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::Reference<Bus> BusViewImpl::getBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::ref<Bus>(voltageLevel.getCalculatedBusTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getConnectableBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return voltageLevel.getCalculatedBusTopology().getConnectableBus(m_terminal.getNode());
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeTerminal& terminal):
    m_terminal(terminal) {
}

unsigned long NodeBreakerViewImpl::getNode() const {
    return m_terminal.getNode();
}

}  // namespace node_terminal

}  // namespace iidm

}  // namespace powsybl
