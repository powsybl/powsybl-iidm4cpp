/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createTestNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL2")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl2.getId())
        .setBus2(vl2Bus1.getId())
        .setConnectableBus2(vl2Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::ES)
        .setTso("TSO2")
        .add();

    VoltageLevel& vl3 = substation2.newVoltageLevel()
        .setId("VL3")
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    Bus& vl3Bus2 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS2")
        .add();

    Bus& vl3Bus3 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS3")
        .add();

    vl3.getBusBreakerView().newSwitch()
        .setId("SW1")
        .setName("SW1_NAME")
        .setBus1(vl3Bus1.getId())
        .setBus2(vl3Bus2.getId())
        .add();

    vl3.getBusBreakerView().newSwitch()
        .setId("SW2")
        .setName("SW2_NAME")
        .setBus1(vl3Bus2.getId())
        .setBus2(vl3Bus3.getId())
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    Bus& vl4Bus1 = vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    network.newLine()
        .setId("VL2_VL4")
        .setVoltageLevel1(vl2.getId())
        .setBus1(vl2Bus1.getId())
        .setConnectableBus1(vl2Bus1.getId())
        .setVoltageLevel2(vl4.getId())
        .setBus2(vl4Bus1.getId())
        .setConnectableBus2(vl4Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    network.newTieLine()
        .setId("TL_VL1_VL3")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl3.getId())
        .setBus2(vl3Bus1.getId())
        .setConnectableBus2(vl3Bus1.getId())
        .setUcteXnodeCode("UcteXnodeCode")
        .line1()
        .setId("H1_TL_VL1_VL3")
        .setXnodeP(1)
        .setXnodeQ(2)
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .line2()
        .setId("H2_TL_VL1_VL3")
        .setXnodeP(3)
        .setXnodeQ(4)
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add();

    network.newSubstation()
        .setId("S3")
        .setName("S3_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    vl1.newDanglingLine()
        .setId("DL1")
        .setName("DL1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setB(1.0)
        .setG(2.0)
        .setP0(3.0)
        .setQ0(4.0)
        .setR(5.0)
        .setX(6.0)
        .setUcteXnodeCode("ucteXnodeCodeTest")
        .add();

    return network;
}

TEST(Network, constructor) {
    EXPECT_THROW(Network("", ""), PowsyblException);
    EXPECT_THROW(Network("id", ""), ValidationException);

    Network network("id", "sourceFormat");
    EXPECT_EQ("id", network.getId());
    EXPECT_EQ("id", network.getName());
    EXPECT_EQ("sourceFormat", network.getSourceFormat());
    EXPECT_EQ(0, network.getForecastDistance());
}

TEST(Network, forecastDistance) {
    Network network("id", "test");

    EXPECT_THROW(network.setForecastDistance(-1), ValidationException);
    EXPECT_TRUE(stdcxx::areSame(network, network.setForecastDistance(1)));
    EXPECT_EQ(1, network.getForecastDistance());
}

TEST(Network, country) {
    const Network& network = createTestNetwork();

    ASSERT_EQ(3, network.getSubstationCount());
    ASSERT_EQ(2, network.getCountryCount());
}

TEST(Network, branch) {
    const Network& network = createTestNetwork();

    ASSERT_EQ(3, network.getBranchCount());
    POWSYBL_ASSERT_THROW(network.getBranch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network.getBranch("DL1"), PowsyblException, "Identifiable 'DL1' is not a powsybl::iidm::Branch");
    ASSERT_NO_THROW(network.getBranch("TL_VL1_VL3"));

    unsigned long lineCount = network.getLineCount();
    unsigned long branchCount = network.getBranchCount();
    ASSERT_EQ(lineCount + network.getTwoWindingsTransformerCount(), branchCount);

    unsigned long lineLoopCount = 0ul;
    unsigned long branchLoopCount = 0ul;
    for (auto it = network.cbegin<Line>(); it != network.cend<Line>(); ++it) {
        lineLoopCount++;
    }
    for (auto it = network.cbegin<Branch>(); it != network.cend<Branch>(); ++it) {
        branchLoopCount++;
    }
    ASSERT_EQ(lineLoopCount, lineCount);
    ASSERT_EQ(branchLoopCount, branchCount);
}

TEST(Network, switch) {
    //BusBreaker
    Network network1 = createTestNetwork();
    ASSERT_EQ(2, network1.getSwitchCount());
    POWSYBL_ASSERT_THROW(network1.getSwitch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network1.getSwitch("DL1"), PowsyblException, "Identifiable 'DL1' is not a powsybl::iidm::Switch");
    ASSERT_NO_THROW(network1.getSwitch("SW1"));

    //NodeBreaker
    Network network2("test2", "test2");
    Substation& substation = network2.newSubstation()
        .setId("S4")
        .setName("S4_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setId("VL5")
        .setName("VL5_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    voltageLevel.getNodeBreakerView().setNodeCount(5);
    voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setName("BBS1_NAME")
        .setNode(0)
        .add();

    voltageLevel.getNodeBreakerView().newBreaker()
        .setId("BK")
        .setName("BK_NAME")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    voltageLevel.getNodeBreakerView().newInternalConnection()
        .setId("IC_1")
        .setNode1(1)
        .setNode2(2)
        .add();

    voltageLevel.getNodeBreakerView().newDisconnector()
        .setId("DIS1")
        .setName("DIS1_NAME")
        .setNode1(2)
        .setNode2(3)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    voltageLevel.getNodeBreakerView().newSwitch()
        .setId("LBS1")
        .setName("LBS1_NAME")
        .setKind(SwitchKind::LOAD_BREAK_SWITCH)
        .setNode1(3)
        .setNode2(4)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    ASSERT_EQ(3, network2.getSwitchCount());
    POWSYBL_ASSERT_THROW(network2.getSwitch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network2.getSwitch("BBS1"), PowsyblException, "Identifiable 'BBS1' is not a powsybl::iidm::Switch");
    ASSERT_NO_THROW(network2.getSwitch("BK"));
}

}  // namespace iidm

}  // namespace powsybl
