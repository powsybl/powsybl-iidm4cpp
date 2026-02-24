/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeTerminalViews.hpp"

#include "NodeBreakerTopologyModel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_terminal {

BusBreakerViewImpl::BusBreakerViewImpl(powsybl::iidm::NodeTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus() const {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::cref<Bus>(topologyModel.getCalculatedBusBreakerTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus() {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::ref<Bus>(topologyModel.getCalculatedBusBreakerTopology().getBus(m_terminal.getNode()));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getConnectableBus() const {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::cref(topologyModel.getCalculatedBusBreakerTopology().getConnectableBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getConnectableBus() {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return topologyModel.getCalculatedBusBreakerTopology().getConnectableBus(m_terminal.getNode());
}

void BusBreakerViewImpl::setConnectableBus(const std::string& /*busId*/) {
    throw AssertionError("Not implemented");
}

BusViewImpl::BusViewImpl(powsybl::iidm::NodeTerminal& terminal):
    m_terminal(terminal) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus() const {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::cref<Bus>(topologyModel.getCalculatedBusTopology().getBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getBus() {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::ref<Bus>(topologyModel.getCalculatedBusTopology().getBus(m_terminal.getNode()));
}

stdcxx::CReference<Bus> BusViewImpl::getConnectableBus() const {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return stdcxx::cref(topologyModel.getCalculatedBusTopology().getConnectableBus(m_terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getConnectableBus() {
    auto& topologyModel = m_terminal.getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();

    return topologyModel.getCalculatedBusTopology().getConnectableBus(m_terminal.getNode());
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
