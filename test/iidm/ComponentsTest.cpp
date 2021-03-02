/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/util/ConnectedComponents.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createComponentsTestNetworkBB() {
    Network network("testBB", "testBB");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newLoad()
        .setId("LOAD1")
        .setBus(vl1Bus1.getId())
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    vl2.newLoad()
        .setId("LOAD2")
        .setBus(vl2Bus1.getId())
        .setP0(60.0)
        .setQ0(70.0)
        .add();

    VoltageLevel& vl3 = substation.newVoltageLevel()
        .setId("VL3")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(90.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    vl3.newLoad()
        .setId("LOAD3")
        .setBus(vl3Bus1.getId())
        .setP0(80.0)
        .setQ0(90.0)
        .add();

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL1_VL2_VL3")
        .newLeg1()
            .setR(1.3)
            .setX(1.4)
            .setG(1.6)
            .setB(1.7)
            .setRatedU(1.1)
            .setVoltageLevel(vl1.getId())
            .setBus(vl1Bus1.getId())
            .add()
        .newLeg2()
            .setR(2.3)
            .setX(2.4)
            .setRatedU(2.1)
            .setVoltageLevel(vl2.getId())
            .setBus(vl2Bus1.getId())
            .add()
        .newLeg3()
            .setR(3.3)
            .setX(3.4)
            .setRatedU(3.1)
            .setVoltageLevel(vl3.getId())
            .setBus(vl3Bus1.getId())
            .add()
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    Bus& vl4Bus1 = vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    vl4.newLoad()
        .setId("LOAD4")
        .setBus(vl4Bus1.getId())
        .setP0(100.0)
        .setQ0(110.0)
        .add();

    VoltageLevel& vl5 = substation2.newVoltageLevel()
        .setId("VL5")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    Bus& vl5Bus1 = vl5.getBusBreakerView().newBus()
        .setId("VL5_BUS1")
        .add();

    vl5.newLoad()
        .setId("LOAD5")
        .setBus(vl5Bus1.getId())
        .setP0(130.0)
        .setQ0(120.0)
        .add();

    substation2.newTwoWindingsTransformer()
        .setId("2WT_VL4_VL5")
        .setVoltageLevel1(vl4.getId())
        .setBus1(vl4Bus1.getId())
        .setVoltageLevel2(vl5.getId())
        .setBus2(vl5Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    network.newLine()
        .setId("VL2_VL5")
        .setVoltageLevel1(vl2.getId())
        .setBus1(vl2Bus1.getId())
        .setVoltageLevel2(vl5.getId())
        .setBus2(vl5Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    Substation& substation3 = network.newSubstation()
        .setId("S3")
        .add();

    VoltageLevel& vl6 = substation3.newVoltageLevel()
        .setId("VL6")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    Bus& vl6Bus1 = vl6.getBusBreakerView().newBus()
        .setId("VL6_BUS1")
        .add();

    vl6.newLoad()
        .setId("LOAD6")
        .setBus(vl6Bus1.getId())
        .setP0(150.0)
        .setQ0(140.0)
        .add();

    vl6.newLccConverterStation()
        .setId("LCC1")
        .setBus(vl6Bus1.getId())
        .setLossFactor(1.0)
        .setPowerFactor(0.2)
        .add();

    network.newTieLine()
        .setId("TL_VL4_VL6")
        .setVoltageLevel1(vl4.getId())
        .setBus1(vl4Bus1.getId())
        .setVoltageLevel2(vl6.getId())
        .setBus2(vl6Bus1.getId())
        .setUcteXnodeCode("UcteXnodeCode")
        .newHalfLine1()
        .setId("H1_TL_VL4_VL6")
        .setXnodeP(1)
        .setXnodeQ(2)
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL4_VL6")
        .setXnodeP(3)
        .setXnodeQ(4)
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add()
        .add();

    Substation& substation4 = network.newSubstation()
        .setId("S4")
        .add();

    VoltageLevel& vl7 = substation4.newVoltageLevel()
        .setId("VL7")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    Bus& vl7Bus1 = vl7.getBusBreakerView().newBus()
        .setId("VL7_BUS1")
        .add();

    vl7.newLoad()
        .setId("LOAD7")
        .setBus(vl7Bus1.getId())
        .setP0(170.0)
        .setQ0(160.0)
        .add();

    vl7.newLccConverterStation()
        .setId("LCC2")
        .setBus(vl7Bus1.getId())
        .setLossFactor(3.0)
        .setPowerFactor(0.4)
        .add();

    network.newHvdcLine()
        .setId("HVDC1")
        .setActivePowerSetpoint(11.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .setConverterStationId1("LCC1")
        .setConverterStationId2("LCC2")
        .setMaxP(12.0)
        .setNominalV(380.0)
        .setR(14.0)
        .add();

    VoltageLevel& vl8 = substation4.newVoltageLevel()
        .setId("VL8")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    Bus& vl8Bus1 = vl8.getBusBreakerView().newBus()
        .setId("VL8_BUS1")
        .add();

    vl8.newLoad()
        .setId("LOAD8")
        .setBus(vl8Bus1.getId())
        .setP0(190.0)
        .setQ0(180.0)
        .add();

    Substation& substation5 = network.newSubstation()
        .setId("S5")
        .add();

    VoltageLevel& vl9 = substation5.newVoltageLevel()
        .setId("VL9")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    Bus& vl9Bus1 = vl9.getBusBreakerView().newBus()
        .setId("VL9_BUS1")
        .add();

    vl9.newLoad()
        .setId("LOAD9")
        .setBus(vl9Bus1.getId())
        .setP0(200.0)
        .setQ0(210.0)
        .add();

    network.newLine()
        .setId("VL7_VL9")
        .setVoltageLevel1(vl7.getId())
        .setBus1(vl7Bus1.getId())
        .setVoltageLevel2(vl9.getId())
        .setBus2(vl9Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    VoltageLevel& vl10 = substation5.newVoltageLevel()
        .setId("VL10")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    Bus& vl10Bus1 = vl10.getBusBreakerView().newBus()
        .setId("VL10_BUS1")
        .add();

    vl10.newLoad()
        .setId("LOAD10")
        .setBus(vl10Bus1.getId())
        .setP0(230.0)
        .setQ0(220.0)
        .add();

    network.newLine()
        .setId("VL8_VL10")
        .setVoltageLevel1(vl8.getId())
        .setBus1(vl8Bus1.getId())
        .setVoltageLevel2(vl10.getId())
        .setBus2(vl10Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    return network;
}

Network createComponentsTestNetworkNB() {
    Network network("testNB", "testNB");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

     vl1.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("SWB1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl2.newLoad()
        .setId("LOAD2")
        .setNode(0)
        .setP0(60.0)
        .setQ0(70.0)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("SWB2")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("SWB3")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl3 = substation.newVoltageLevel()
        .setId("VL3")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(90.0)
        .add();

    vl3.newLoad()
        .setId("LOAD3")
        .setNode(0)
        .setP0(80.0)
        .setQ0(90.0)
        .add();

    vl3.getNodeBreakerView().newBreaker()
        .setId("SWB4")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL1_VL2_VL3")
        .setName("3WT_VL1_VL2_VL3_NAME")
        .newLeg1()
            .setR(1.3)
            .setX(1.4)
            .setG(1.6)
            .setB(1.7)
            .setRatedU(1.1)
            .setVoltageLevel(vl1.getId())
            .setNode(1)
            .add()
        .newLeg2()
            .setR(2.3)
            .setX(2.4)
            .setRatedU(2.1)
            .setVoltageLevel(vl2.getId())
            .setNode(1)
            .add()
        .newLeg3()
            .setR(3.3)
            .setX(3.4)
            .setRatedU(3.1)
            .setVoltageLevel(vl3.getId())
            .setNode(1)
            .add()
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl4.newLoad()
        .setId("LOAD4")
        .setNode(0)
        .setP0(100.0)
        .setQ0(110.0)
        .add();

    vl4.getNodeBreakerView().newBreaker()
        .setId("SWB5")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl4.getNodeBreakerView().newBreaker()
        .setId("SWB6")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl5 = substation2.newVoltageLevel()
        .setId("VL5")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl5.newLoad()
        .setId("LOAD5")
        .setNode(0)
        .setP0(130.0)
        .setQ0(120.0)
        .add();

    vl5.getNodeBreakerView().newBreaker()
        .setId("SWB7")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl5.getNodeBreakerView().newBreaker()
        .setId("SWB8")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    substation2.newTwoWindingsTransformer()
        .setId("2WT_VL4_VL5")
        .setVoltageLevel1(vl4.getId())
        .setNode1(1)
        .setVoltageLevel2(vl5.getId())
        .setNode2(1)
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    network.newLine()
        .setId("VL2_VL5")
        .setVoltageLevel1(vl2.getId())
        .setNode1(2)
        .setVoltageLevel2(vl5.getId())
        .setNode2(2)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    Substation& substation3 = network.newSubstation()
        .setId("S3")
        .add();

    VoltageLevel& vl6 = substation3.newVoltageLevel()
        .setId("VL6")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl6.newLoad()
        .setId("LOAD6")
        .setNode(0)
        .setP0(150.0)
        .setQ0(140.0)
        .add();

    vl6.getNodeBreakerView().newBreaker()
        .setId("SWB9")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl6.newVscConverterStation()
        .setId("VSC1")
        .setNode(1)
        .setLossFactor(3.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(4.0)
        .setReactivePowerSetpoint(5.0)
        .add();

    vl6.getNodeBreakerView().newBreaker()
        .setId("SWB10")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    network.newTieLine()
        .setId("TL_VL4_VL6")
        .setVoltageLevel1(vl4.getId())
        .setNode1(2)
        .setVoltageLevel2(vl6.getId())
        .setNode2(2)
        .setUcteXnodeCode("UcteXnodeCode")
        .newHalfLine1()
        .setId("H1_TL_VL4_VL6")
        .setXnodeP(1)
        .setXnodeQ(2)
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL4_VL6")
        .setXnodeP(3)
        .setXnodeQ(4)
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add()
        .add();

    Substation& substation4 = network.newSubstation()
        .setId("S4")
        .add();

    VoltageLevel& vl7 = substation4.newVoltageLevel()
        .setId("VL7")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl7.newLoad()
        .setId("LOAD7")
        .setNode(0)
        .setP0(170.0)
        .setQ0(160.0)
        .add();

    vl7.getNodeBreakerView().newBreaker()
        .setId("SWB11")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl7.newVscConverterStation()
        .setId("VSC2")
        .setNode(1)
        .setLossFactor(6.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(7.0)
        .setReactivePowerSetpoint(8.0)
        .add();

    vl7.getNodeBreakerView().newBreaker()
        .setId("SWB12")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    network.newHvdcLine()
        .setId("HVDC1")
        .setActivePowerSetpoint(11.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .setConverterStationId1("VSC1")
        .setConverterStationId2("VSC2")
        .setMaxP(12.0)
        .setNominalV(380.0)
        .setR(14.0)
        .add();

    VoltageLevel& vl8 = substation4.newVoltageLevel()
        .setId("VL8")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl8.newLoad()
        .setId("LOAD8")
        .setNode(0)
        .setP0(190.0)
        .setQ0(180.0)
        .add();

    vl8.getNodeBreakerView().newBreaker()
        .setId("SWB13")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    Substation& substation5 = network.newSubstation()
        .setId("S5")
        .add();

    VoltageLevel& vl9 = substation5.newVoltageLevel()
        .setId("VL9")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl9.newLoad()
        .setId("LOAD9")
        .setNode(0)
        .setP0(200.0)
        .setQ0(210.0)
        .add();

    vl9.getNodeBreakerView().newBreaker()
        .setId("SWB14")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    network.newLine()
        .setId("VL7_VL9")
        .setVoltageLevel1(vl7.getId())
        .setNode1(2)
        .setVoltageLevel2(vl9.getId())
        .setNode2(1)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    VoltageLevel& vl10 = substation5.newVoltageLevel()
        .setId("VL10")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl10.newLoad()
        .setId("LOAD10")
        .setNode(0)
        .setP0(230.0)
        .setQ0(220.0)
        .add();

    vl10.getNodeBreakerView().newBreaker()
        .setId("SWB15")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    network.newLine()
        .setId("VL8_VL10")
        .setVoltageLevel1(vl8.getId())
        .setNode1(1)
        .setVoltageLevel2(vl10.getId())
        .setNode2(1)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    // Force the initialization of the components
    network.getBusView().getBus("VL1_0").get().getConnectedComponent();

    return network;
}

Network createMultiConnectedComponentsNetwork() {
    Network network("test_mcc", "test");

    Substation& s1 = network.newSubstation()
            .setId("A")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
            .setId("B")
            .setNominalVoltage(225.0)
            .setLowVoltageLimit(0.0)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl1.getNodeBreakerView().newBusbarSection()
            .setId("C")
            .setNode(0)
            .add();
    vl1.getNodeBreakerView().newSwitch()
            .setId("D")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(false)
            .setOpen(false)
            .setNode1(0)
            .setNode2(1)
            .add();
    vl1.getNodeBreakerView().newSwitch()
            .setId("E")
            .setKind(SwitchKind::BREAKER)
            .setRetained(false)
            .setOpen(false)
            .setNode1(1)
            .setNode2(2)
            .add();

    Substation& s2 = network.newSubstation()
            .setId("F")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
            .setId("G")
            .setNominalVoltage(225.0)
            .setLowVoltageLimit(0.0)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("H")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("I")
            .setNode(1)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("J")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(true)
            .setOpen(false)
            .setNode1(0)
            .setNode2(2)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("K")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(true)
            .setOpen(false)
            .setNode1(1)
            .setNode2(3)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("L")
            .setKind(SwitchKind::BREAKER)
            .setRetained(true)
            .setOpen(false)
            .setNode1(2)
            .setNode2(3)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("M")
            .setKind(SwitchKind::BREAKER)
            .setRetained(false)
            .setOpen(false)
            .setNode1(0)
            .setNode2(4)
            .add();

    network.newLine()
            .setId("N")
            .setR(0.001)
            .setX(0.1)
            .setG1(0.0)
            .setB1(0.0)
            .setG2(0.0)
            .setB2(0.0)
            .setVoltageLevel1("B")
            .setNode1(2)
            .setVoltageLevel2("G")
            .setNode2(4)
            .add();
    return network;
}

Network createNetworkWithHvdcLine() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation()
            .setId("S1")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
            .setId("VL1")
            .setNominalVoltage(400)
            .setTopologyKind(TopologyKind::BUS_BREAKER)
            .add();
    vl1.getBusBreakerView().newBus()
            .setId("B1")
            .add();
    Substation& s2 = network.newSubstation()
            .setId("S2")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
            .setId("VL2")
            .setNominalVoltage(400)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("BBS1")
            .setName("BusbarSection")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView().newDisconnector()
            .setId("DISC_BBS1_BK1")
            .setName("Disconnector")
            .setNode1(0)
            .setNode2(1)
            .setOpen(false)
            .setRetained(false)
            .add();
    vl2.getNodeBreakerView().newBreaker()
            .setId("BK1")
            .setName("Breaker")
            .setNode1(1)
            .setNode2(2)
            .setOpen(false)
            .setRetained(true)
            .add();
    VscConverterStation& cs1 = vl1.newVscConverterStation()
            .setId("C1")
            .setName("Converter1")
            .setConnectableBus("B1")
            .setBus("B1")
            .setLossFactor(1.1)
            .setVoltageSetpoint(405.0)
            .setVoltageRegulatorOn(true)
            .add();
    cs1.getTerminal()
            .setP(100.0)
            .setQ(50.0);
    cs1.newReactiveCapabilityCurve()
            .beginPoint()
            .setP(5.0)
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .endPoint()
            .beginPoint()
            .setP(10.0)
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .endPoint()
            .add();
    VscConverterStation& cs2 = vl2.newVscConverterStation()
            .setId("C2")
            .setName("Converter2")
            .setNode(2)
            .setLossFactor(1.1)
            .setReactivePowerSetpoint(123)
            .setVoltageRegulatorOn(false)
            .add();
    cs2.newMinMaxReactiveLimits()
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .add();
    network.newHvdcLine()
            .setId("L")
            .setName("HVDC")
            .setConverterStationId1("C1")
            .setConverterStationId2("C2")
            .setR(1)
            .setNominalV(400)
            .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
            .setMaxP(300.0)
            .setActivePowerSetpoint(280)
            .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(ComponentsTestSuite)

BOOST_AUTO_TEST_CASE(NodeBreakerTest) {
    Network network = createComponentsTestNetworkNB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    Bus& bus = network.getBusBreakerView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusBreakerView().getBus("VL10_0");
    const Bus& cBus10 = cNetwork.getBusBreakerView().getBus("VL10_0");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(BusBreakerTest) {
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    Bus& bus = network.getBusBreakerView().getBus("VL1_BUS1");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_BUS1");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusBreakerView().getBus("VL10_BUS1");
    const Bus& cBus10 = cNetwork.getBusBreakerView().getBus("VL10_BUS1");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_BUS1")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_BUS1")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(BusViewTest) {
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(2UL, boost::size(cNetwork.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusView().getBuses()));

    Bus& bus = network.getBusView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());
    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL1_0")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusView().getBus("VL10_0");
    const Bus& cBus10 = cNetwork.getBusView().getBus("VL10_0");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(MainConnectedComponentWithSwitchTest) {
    Network network = createMultiConnectedComponentsNetwork();

    const VoltageLevel& vl1 = network.getVoltageLevel("B");

    for (Bus& b : network.getBusView().getBuses()) {
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            b.setV(230.0).setAngle(0.5);
        } else {
            b.setV(220.0).setAngle(0.7);
        }
    }

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getBuses()));
    for (Bus& b : network.getBusView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
    }

    BOOST_CHECK_EQUAL(5, boost::size(network.getBusBreakerView().getBuses()));
    for (Bus& b : network.getBusBreakerView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            BOOST_CHECK_CLOSE(230.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.5, b.getAngle(), std::numeric_limits<double>::epsilon());
        } else {
            BOOST_CHECK_CLOSE(220.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.7, b.getAngle(), std::numeric_limits<double>::epsilon());
        }
    }
}

BOOST_AUTO_TEST_CASE(ComponentsWithHvdcLine) {
    Network network = createNetworkWithHvdcLine();
    VscConverterStation& cs1 = network.getVscConverterStation("C1");
    VscConverterStation& cs2 = network.getVscConverterStation("C2");

    const auto& bus1 = cs1.getTerminal().getBusView().getBus().get();
    const auto& cComp1 = bus1.getConnectedComponent().get();
    const auto& sComp1 = bus1.getSynchronousComponent().get();

    const auto& bus2 = cs2.getTerminal().getBusView().getBus().get();
    const auto& cComp2 = bus2.getConnectedComponent().get();
    const auto& sComp2 = bus2.getSynchronousComponent().get();

    // Same connected component
    BOOST_CHECK(bus1.isInMainConnectedComponent());
    BOOST_CHECK(bus2.isInMainConnectedComponent());
    BOOST_CHECK(stdcxx::areSame(cComp1, cComp2));

    // Not the same synchronous component
    BOOST_CHECK(bus1.isInMainSynchronousComponent());
    BOOST_CHECK(!bus2.isInMainSynchronousComponent());
    BOOST_CHECK(!stdcxx::areSame(sComp1, sComp2));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
