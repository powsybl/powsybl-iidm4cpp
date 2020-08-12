/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SwitchAdder.hpp>

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

#include "BusBreakerVoltageLevel.hpp"
#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_view {

SwitchAdder::SwitchAdder(VoltageLevel& voltageLevel) :
    AbstractSwitchAdder(voltageLevel) {
}

Switch& SwitchAdder::add() {
    checkOptional(*this, m_node1, "First connection node is not set");
    checkOptional(*this, m_node2, "Second connection node is not set");
    checkOptional(*this, m_kind, "Kind is not set");

    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(getVoltageLevel());

    std::unique_ptr<Switch> ptrSwitch = stdcxx::make_unique<Switch>(voltageLevel, checkAndGetUniqueId(), getName(), isFictitious(), *m_kind, isOpen(), m_retained);
    Switch& aSwitch = voltageLevel.addSwitch(std::move(ptrSwitch), *m_node1, *m_node2);

    return aSwitch;
}

SwitchAdder& SwitchAdder::setKind(const SwitchKind& kind) {
    m_kind = kind;
    return *this;
}

SwitchAdder& SwitchAdder::setNode1(unsigned long node1) {
    m_node1 = node1;
    return *this;
}

SwitchAdder& SwitchAdder::setNode2(unsigned long node2) {
    m_node2 = node2;
    return *this;
}

SwitchAdder& SwitchAdder::setRetained(bool retained) {
    m_retained = retained;
    return *this;
}

}  // namespace node_breaker_view

namespace bus_breaker_view {

SwitchAdder::SwitchAdder(VoltageLevel& voltageLevel) :
    AbstractSwitchAdder(voltageLevel) {
}

Switch& SwitchAdder::add() {
    checkNotEmpty(*this, m_bus1, "First connection bus is not set");
    checkNotEmpty(*this, m_bus2, "Second connection bus is not set");

    auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(getVoltageLevel());

    std::unique_ptr<Switch> ptrSwitch = stdcxx::make_unique<Switch>(voltageLevel, checkAndGetUniqueId(), getName(), isFictitious(), SwitchKind::BREAKER, isOpen(), true);
    Switch& aSwitch = voltageLevel.addSwitch(std::move(ptrSwitch), m_bus1, m_bus2);

    return aSwitch;
}

SwitchAdder& SwitchAdder::setBus1(const std::string& bus1) {
    m_bus1 = bus1;
    return *this;
}

SwitchAdder& SwitchAdder::setBus2(const std::string& bus2) {
    m_bus2 = bus2;
    return *this;
}

}  // namespace bus_breaker_view

}  // namespace iidm

}  // namespace powsybl
