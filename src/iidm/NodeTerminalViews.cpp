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

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::cref<Bus>(voltageLevel.getCalculatedBusBreakerTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus() {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::ref<Bus>(voltageLevel.getCalculatedBusBreakerTopology().getBus(m_terminal.getNode()));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getConnectableBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::cref(voltageLevel.getCalculatedBusBreakerTopology().getConnectableBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getConnectableBus() {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return voltageLevel.getCalculatedBusBreakerTopology().getConnectableBus(m_terminal.getNode());
}

void BusBreakerViewImpl::moveConnectable(const std::string& busId, bool connected) {
    m_terminal.getConnectable().get().move(m_terminal, m_terminal.getConnectionInfo(), busId, connected);
}

void BusBreakerViewImpl::setConnectableBus(const std::string& /*busId*/) {
    throw AssertionError("Not implemented");
}

BusViewImpl::BusViewImpl(powsybl::iidm::NodeTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::cref<Bus>(voltageLevel.getCalculatedBusTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getBus() {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::ref<Bus>(voltageLevel.getCalculatedBusTopology().getBus(m_terminal.getNode()));
}

stdcxx::CReference<Bus> BusViewImpl::getConnectableBus() const {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return stdcxx::cref(voltageLevel.getCalculatedBusTopology().getConnectableBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getConnectableBus() {
    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_terminal.getVoltageLevel());

    return voltageLevel.getCalculatedBusTopology().getConnectableBus(m_terminal.getNode());
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeTerminal& terminal):
    m_terminal(terminal) {
}

unsigned long NodeBreakerViewImpl::getNode() const {
    return m_terminal.getNode();
}

void NodeBreakerViewImpl::moveConnectable(unsigned long node, const std::string& voltageLevelId) {
    m_terminal.getConnectable().get().move(m_terminal, m_terminal.getConnectionInfo(), node, voltageLevelId);
}

}  // namespace node_terminal

}  // namespace iidm

}  // namespace powsybl
