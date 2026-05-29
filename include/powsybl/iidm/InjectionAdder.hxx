/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INJECTIONADDER_HXX
#define POWSYBL_IIDM_INJECTIONADDER_HXX

#include <powsybl/iidm/InjectionAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

template <typename Added, typename Adder>
InjectionAdder<Added, Adder>::InjectionAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

template <typename Added, typename Adder>
std::unique_ptr<Terminal> InjectionAdder<Added, Adder>::checkAndGetTerminal() {
    return TerminalBuilder(m_voltageLevel, *this)
               .setNode(m_node)
               .setBus(m_bus)
               .setConnectableBus(m_connectableBus)
               .build();
}

template <typename Added, typename Adder>
const Network& InjectionAdder<Added, Adder>::getNetwork() const {
    return m_voltageLevel.getNetwork();
}

template <typename Added, typename Adder>
Network& InjectionAdder<Added, Adder>::getNetwork() {
    return m_voltageLevel.getNetwork();
}

template <typename Added, typename Adder>
const VoltageLevel& InjectionAdder<Added, Adder>::getVoltageLevel() const {
    return m_voltageLevel;
}

template <typename Added, typename Adder>
VoltageLevel& InjectionAdder<Added, Adder>::getVoltageLevel() {
    return m_voltageLevel;
}

template <typename Added, typename Adder>
Adder& InjectionAdder<Added, Adder>::setBus(const std::string& bus) {
    m_bus = bus;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& InjectionAdder<Added, Adder>::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& InjectionAdder<Added, Adder>::setNode(unsigned long node) {
    m_node = node;
    return static_cast<Adder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INJECTIONADDER_HXX
