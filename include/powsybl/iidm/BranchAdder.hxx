/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCHADDER_HXX
#define POWSYBL_IIDM_BRANCHADDER_HXX

#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

template <typename Adder>
std::unique_ptr<Terminal> BranchAdder<Adder>::checkAndGetTerminal1(VoltageLevel& voltageLevel) {
    return TerminalBuilder(voltageLevel, *this)
               .setNode(m_node1)
               .setBus(m_bus1)
               .setConnectableBus(m_connectableBus1)
               .build();
}

template <typename Adder>
std::unique_ptr<Terminal> BranchAdder<Adder>::checkAndGetTerminal2(VoltageLevel& voltageLevel) {
    return TerminalBuilder(voltageLevel, *this)
               .setNode(m_node2)
               .setBus(m_bus2)
               .setConnectableBus(m_connectableBus2)
               .build();
}

template <typename Adder>
VoltageLevel& BranchAdder<Adder>::checkAndGetVoltageLevel1() {
    if (m_voltageLevelId1.empty()) {
        std::string defaultVoltageLevelId1 = checkAndGetDefaultVoltageLevelId(m_connectableBus1);
        if(defaultVoltageLevelId1.empty()) {
            throw ValidationException(*this, "First voltage level is not set and has no default value");
        } else {
            m_voltageLevelId1 = defaultVoltageLevelId1;
        }
    }

    stdcxx::Reference<VoltageLevel> voltageLevel1 = this->getNetwork().template find<VoltageLevel>(m_voltageLevelId1);
    if (!voltageLevel1) {
        throw ValidationException(*this, stdcxx::format("First voltage level '%1%' not found", m_voltageLevelId1));
    }

    return voltageLevel1.get();
}

template <typename Adder>
VoltageLevel& BranchAdder<Adder>::checkAndGetVoltageLevel2() {
    if (m_voltageLevelId2.empty()) {
        std::string defaultVoltageLevelId2 = checkAndGetDefaultVoltageLevelId(m_connectableBus2);
        if(defaultVoltageLevelId2.empty()) {
            throw ValidationException(*this, "Second voltage level is not set and has no default value");
        } else {
            m_voltageLevelId2 = defaultVoltageLevelId2;
        }
    }

    stdcxx::Reference<VoltageLevel> voltageLevel2 = this->getNetwork().template find<VoltageLevel>(m_voltageLevelId2);
    if (!voltageLevel2) {
        throw ValidationException(*this, stdcxx::format("Second voltage level '%1%' not found", m_voltageLevelId2));
    }

    return voltageLevel2.get();
}

template <typename Adder>
std::string BranchAdder<Adder>::checkAndGetDefaultVoltageLevelId(const std::string& connectableBusId) {
    if(connectableBusId.empty()) {
        return "";
    }
    stdcxx::Reference<Bus> bus = this->getNetwork().getBusBreakerView().getBus(connectableBusId);
    if(!bus) {
        throw ValidationException(*this, stdcxx::format("Bus '%1%' not found", connectableBusId));
    }
    return bus.get().getVoltageLevel().getId();
}

template <typename Adder>
void BranchAdder<Adder>::checkConnectableBuses() {
    if(m_connectableBus1.empty() && !m_bus1.empty()) {
        m_connectableBus1 = m_bus1;
    }
    if(m_connectableBus2.empty() && !m_bus2.empty()) {
        m_connectableBus2 = m_bus2;
    }
}

template <typename Adder>
Adder& BranchAdder<Adder>::setBus1(const std::string& bus1) {
    m_bus1 = bus1;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setBus2(const std::string& bus2) {
    m_bus2 = bus2;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setConnectableBus1(const std::string& connectableBus1) {
    m_connectableBus1 = connectableBus1;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setConnectableBus2(const std::string& connectableBus2) {
    m_connectableBus2 = connectableBus2;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setNode1(unsigned long node1) {
    m_node1 = node1;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setNode2(unsigned long node2) {
    m_node2 = node2;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setVoltageLevel1(const std::string& voltageLevelId1) {
    m_voltageLevelId1 = voltageLevelId1;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& BranchAdder<Adder>::setVoltageLevel2(const std::string& voltageLevelId2) {
    m_voltageLevelId2 = voltageLevelId2;
    return static_cast<Adder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCHADDER_HXX
