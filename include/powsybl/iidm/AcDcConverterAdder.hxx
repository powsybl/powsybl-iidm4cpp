/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ACDCCONVERTERADDER_HXX
#define POWSYBL_IIDM_ACDCCONVERTERADDER_HXX

#include <powsybl/iidm/AcDcConverterAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

template <typename Added, typename Adder>
AcDcConverterAdder<Added, Adder>::AcDcConverterAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

template <typename Added, typename Adder>
const Network& AcDcConverterAdder<Added, Adder>::getNetwork() const {
    return m_voltageLevel.getNetwork();
}

template <typename Added, typename Adder>
Network& AcDcConverterAdder<Added, Adder>::getNetwork() {
    return m_voltageLevel.getNetwork();
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setNode1(unsigned long node1) {
    m_node1 = node1;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setBus1(const std::string& bus1) {
    m_bus1 = bus1;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setConnectableBus1(const std::string& connectableBus1) {
    m_connectableBus1 = connectableBus1;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setNode2(unsigned long node2) {
    m_node2 = node2;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setBus2(const std::string& bus2) {
    m_bus2 = bus2;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setConnectableBus2(const std::string& connectableBus2) {
    m_connectableBus2 = connectableBus2;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setDcNode1(const std::string& dcNode1) {
    m_dcNode1 = dcNode1;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setDcConnected1(bool connected1) {
    m_dcConnected1 = connected1;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setDcNode2(const std::string& dcNode2) {
    m_dcNode2 = dcNode2;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setDcConnected2(bool connected2) {
    m_dcConnected2 = connected2;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setIdleLoss(double idleLoss) {
    m_idleLoss = idleLoss;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setSwitchingLoss(double switchingLoss) {
    m_switchingLoss = switchingLoss;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setResistiveLoss(double resistiveLoss) {
    m_resistiveLoss = resistiveLoss;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal) {
    m_pccTerminal = pccTerminal;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setControlMode(const AcDcConverter::ControlMode& controlMode) {
    m_controlMode = controlMode;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setTargetP(double targetP) {
    m_targetP = targetP;
    return static_cast<Adder&>(*this);
}
template <typename Added, typename Adder>
Adder& AcDcConverterAdder<Added, Adder>::setTargetVdc(double targetVdc) {
    m_targetVdc = targetVdc;
    return static_cast<Adder&>(*this);
}

template <typename Added, typename Adder>
std::unique_ptr<Terminal> AcDcConverterAdder<Added, Adder>::checkAndGetTerminal1() {
    return TerminalBuilder(m_voltageLevel, *this, TerminalNumber::ONE)
               .setNode(m_node1)
               .setBus(m_bus1)
               .setConnectableBus(m_connectableBus1)
               .build();
}
template <typename Added, typename Adder>
std::unique_ptr<Terminal> AcDcConverterAdder<Added, Adder>::checkAndGetTerminal2() {
    if(hasTwoAcTerminals()) {
        return TerminalBuilder(m_voltageLevel, *this, TerminalNumber::TWO)
               .setNode(m_node2)
               .setBus(m_bus2)
               .setConnectableBus(m_connectableBus2)
               .build();
    }
    return std::unique_ptr<Terminal>();
}

template <typename Added, typename Adder>
bool AcDcConverterAdder<Added, Adder>::hasTwoAcTerminals() {
    return (!m_bus2.empty() || !m_connectableBus2.empty() || m_node2.has_value());
}
template <typename Added, typename Adder>
void AcDcConverterAdder<Added, Adder>::checkAcDcConverter() {

    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkAcDcConverterControl(*this, m_controlMode, m_targetP, m_targetVdc, network.getMinimumValidationLevel()));
    checkAcDcConverterPccTerminal(*this, m_pccTerminal, m_voltageLevel);

}
template <typename Added, typename Adder>
Added& AcDcConverterAdder<Added, Adder>::checkAndAdd(std::unique_ptr<Added>&& converter) {

    std::unique_ptr<Terminal> ptrTerminal1 = checkAndGetTerminal1();
    DcNode& dcNode1 = checkAndGetDcNode(getNetwork(), *this, m_dcNode1, "dcNode1");
    DcNode& dcNode2 = checkAndGetDcNode(getNetwork(), *this, m_dcNode2, "dcNode2");
    checkSameParentNetwork(m_voltageLevel.getParentNetwork().getId(), *this, dcNode1, dcNode2);
    std::unique_ptr<Terminal> ptrTerminal2 = checkAndGetTerminal2();

    Terminal& acTerminal1 = converter->addTerminal(std::move(ptrTerminal1));
    m_voltageLevel.getTopologyModel().attach(acTerminal1, false);
    
    if(ptrTerminal2) {
        Terminal& acTerminal2 = converter->addTerminal(std::move(ptrTerminal2));
        m_voltageLevel.getTopologyModel().attach(acTerminal2, false);
    }

    std::unique_ptr<DcTerminal> ptrDcTerminal1 = stdcxx::make_unique<DcTerminal>(dcNode1, TerminalNumber::ONE, m_dcConnected1);
    converter->addDcTerminal(std::move(ptrDcTerminal1));

    std::unique_ptr<DcTerminal> ptrDcTerminal2 = stdcxx::make_unique<DcTerminal>(dcNode2, TerminalNumber::TWO, m_dcConnected2);
    converter->addDcTerminal(std::move(ptrDcTerminal2));

    Added& acdcConverter = getNetwork().checkAndAdd(std::move(converter));
    return acdcConverter;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ACDCCONVERTERADDER_HXX
