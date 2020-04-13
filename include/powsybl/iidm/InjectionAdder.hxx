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

template <typename Adder>
InjectionAdder<Adder>::InjectionAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

template <typename Adder>
std::unique_ptr<Terminal> InjectionAdder<Adder>::checkAndGetTerminal() {
    return TerminalBuilder(m_voltageLevel, *this)
               .setNode(m_node)
               .setBus(m_bus)
               .setConnectableBus(m_connectableBus)
               .build();
}

template <typename Adder>
Network& InjectionAdder<Adder>::getNetwork() {
    return m_voltageLevel.getNetwork();
}

template <typename Adder>
VoltageLevel& InjectionAdder<Adder>::getVoltageLevel() {
    return m_voltageLevel;
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setBus(const std::string& bus) {
    m_bus = bus;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setNode(unsigned long node) {
    m_node = node;
    return static_cast<Adder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INJECTIONADDER_HXX
