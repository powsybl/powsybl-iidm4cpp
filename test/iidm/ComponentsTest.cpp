/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/util/ConnectedComponents.hpp>
#include <powsybl/logging/ContainerLogger.hpp>
#include <powsybl/logging/LogMessage.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createComponentsTestNetworkBB() {
    Network network("testBB", "testBB");
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
         .add();

     Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
         .setId("VL1_BUS1")
         .add();

     vl1.newLoad()
         .setId("LOAD1")
         .setName("LOAD1_NAME")
         .setBus(vl1Bus1.getId())
         .setConnectableBus(vl1Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(50.0)
         .setQ0(40.0)
         .add();

     VoltageLevel& vl2 = substation.newVoltageLevel()
         .setId("VL2")
         .setName("VL2_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .add();

     Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
         .setId("VL2_BUS1")
         .add();

     vl2.newLoad()
         .setId("LOAD2")
         .setName("LOAD2_NAME")
         .setBus(vl2Bus1.getId())
         .setConnectableBus(vl2Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(60.0)
         .setQ0(70.0)
         .add();

     VoltageLevel& vl3 = substation.newVoltageLevel()
         .setId("VL3")
         .setName("VL3_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(90.0)
         .add();

     Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
         .setId("VL3_BUS1")
         .add();

     vl3.newLoad()
         .setId("LOAD3")
         .setName("LOAD3_NAME")
         .setBus(vl3Bus1.getId())
         .setConnectableBus(vl3Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(80.0)
         .setQ0(90.0)
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
         .setBus(vl1Bus1.getId())
         .setConnectableBus(vl1Bus1.getId())
         .add()
         .newLeg2()
         .setR(2.3)
         .setX(2.4)
         .setRatedU(2.1)
         .setVoltageLevel(vl2.getId())
         .setBus(vl2Bus1.getId())
         .setConnectableBus(vl2Bus1.getId())
         .add()
         .newLeg3()
         .setR(3.3)
         .setX(3.4)
         .setRatedU(3.1)
         .setVoltageLevel(vl3.getId())
         .setBus(vl3Bus1.getId())
         .setConnectableBus(vl3Bus1.getId())
         .add()
         .add();

     Substation& substation2 = network.newSubstation()
         .setId("S2")
         .setName("S2_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
         .add();

     VoltageLevel& vl4 = substation2.newVoltageLevel()
         .setId("VL4")
         .setName("VL4_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .add();

     Bus& vl4Bus1 = vl4.getBusBreakerView().newBus()
         .setId("VL4_BUS1")
         .add();

     vl4.newLoad()
         .setId("LOAD4")
         .setName("LOAD4_NAME")
         .setBus(vl4Bus1.getId())
         .setConnectableBus(vl4Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(100.0)
         .setQ0(110.0)
         .add();

     VoltageLevel& vl5 = substation2.newVoltageLevel()
         .setId("VL5")
         .setName("VL5_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .add();

     Bus& vl5Bus1 = vl5.getBusBreakerView().newBus()
         .setId("VL5_BUS1")
         .add();

     vl5.newLoad()
         .setId("LOAD5")
         .setName("LOAD5_NAME")
         .setBus(vl5Bus1.getId())
         .setConnectableBus(vl5Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(130.0)
         .setQ0(120.0)
         .add();

     substation2.newTwoWindingsTransformer()
         .setId("2WT_VL4_VL5")
         .setVoltageLevel1(vl4.getId())
         .setBus1(vl4Bus1.getId())
         .setConnectableBus1(vl4Bus1.getId())
         .setVoltageLevel2(vl5.getId())
         .setBus2(vl5Bus1.getId())
         .setConnectableBus2(vl5Bus1.getId())
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
         .setConnectableBus1(vl2Bus1.getId())
         .setVoltageLevel2(vl5.getId())
         .setBus2(vl5Bus1.getId())
         .setConnectableBus2(vl5Bus1.getId())
         .setR(3.0)
         .setX(33.0)
         .setG1(1.0)
         .setB1(0.2)
         .setG2(2.0)
         .setB2(0.4)
         .add();

     Substation& substation3 = network.newSubstation()
         .setId("S3")
         .setName("S3_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
         .add();

     VoltageLevel& vl6 = substation3.newVoltageLevel()
         .setId("VL6")
         .setName("VL6_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .add();

     Bus& vl6Bus1 = vl6.getBusBreakerView().newBus()
         .setId("VL6_BUS1")
         .add();

     vl6.newLoad()
         .setId("LOAD6")
         .setName("LOAD6_NAME")
         .setBus(vl6Bus1.getId())
         .setConnectableBus(vl6Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(150.0)
         .setQ0(140.0)
         .add();

     vl6.newLccConverterStation()
         .setId("LCC1")
         .setName("LCC1_NAME")
         .setBus(vl6Bus1.getId())
         .setConnectableBus(vl6Bus1.getId())
         .setLossFactor(1.0)
         .setPowerFactor(2.0)
         .add();

     network.newTieLine()
         .setId("TL_VL4_VL6")
         .setVoltageLevel1(vl4.getId())
         .setBus1(vl4Bus1.getId())
         .setConnectableBus1(vl4Bus1.getId())
         .setVoltageLevel2(vl6.getId())
         .setBus2(vl6Bus1.getId())
         .setConnectableBus2(vl6Bus1.getId())
         .setUcteXnodeCode("UcteXnodeCode")
         .line1()
         .setId("H1_TL_VL4_VL6")
         .setXnodeP(1)
         .setXnodeQ(2)
         .setR(6.0)
         .setX(66.0)
         .setG1(0.2)
         .setB1(0.4)
         .setG2(0.3)
         .setB2(0.5)
         .line2()
         .setId("H2_TL_VL4_VL6")
         .setXnodeP(3)
         .setXnodeQ(4)
         .setR(7.0)
         .setX(77.0)
         .setG1(0.6)
         .setB1(0.7)
         .setG2(0.9)
         .setB2(1.2)
         .add();

     Substation& substation4 = network.newSubstation()
         .setId("S4")
         .setName("S4_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
         .add();

     VoltageLevel& vl7 = substation4.newVoltageLevel()
         .setId("VL7")
         .setName("VL7_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .add();

     Bus& vl7Bus1 = vl7.getBusBreakerView().newBus()
         .setId("VL7_BUS1")
         .add();

     vl7.newLoad()
         .setId("LOAD7")
         .setName("LOAD7_NAME")
         .setBus(vl7Bus1.getId())
         .setConnectableBus(vl7Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(170.0)
         .setQ0(160.0)
         .add();

     vl7.newLccConverterStation()
         .setId("LCC2")
         .setName("LCC2_NAME")
         .setBus(vl7Bus1.getId())
         .setConnectableBus(vl7Bus1.getId())
         .setLossFactor(3.0)
         .setPowerFactor(4.0)
         .add();

     network.newHvdcLine()
         .setId("HVDC1")
         .setName("HVDC1_NAME")
         .setActivePowerSetpoint(11.0)
         .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
         .setConverterStationId1("LCC1")
         .setConverterStationId2("LCC2")
         .setMaxP(12.0)
         .setNominalVoltage(380.0)
         .setR(14.0)
         .add();

     VoltageLevel& vl8 = substation4.newVoltageLevel()
         .setId("VL8")
         .setName("VL8_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .add();

     Bus& vl8Bus1 = vl8.getBusBreakerView().newBus()
         .setId("VL8_BUS1")
         .add();

     vl8.newLoad()
         .setId("LOAD8")
         .setName("LOAD8_NAME")
         .setBus(vl8Bus1.getId())
         .setConnectableBus(vl8Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(190.0)
         .setQ0(180.0)
         .add();

     Substation& substation5 = network.newSubstation()
         .setId("S5")
         .setName("S5_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
         .add();

     VoltageLevel& vl9 = substation5.newVoltageLevel()
         .setId("VL9")
         .setName("VL9_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .add();

     Bus& vl9Bus1 = vl9.getBusBreakerView().newBus()
         .setId("VL9_BUS1")
         .add();

     vl9.newLoad()
         .setId("LOAD9")
         .setName("LOAD9_NAME")
         .setBus(vl9Bus1.getId())
         .setConnectableBus(vl9Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(200.0)
         .setQ0(210.0)
         .add();

     network.newLine()
         .setId("VL7_VL9")
         .setVoltageLevel1(vl7.getId())
         .setBus1(vl7Bus1.getId())
         .setConnectableBus1(vl7Bus1.getId())
         .setVoltageLevel2(vl9.getId())
         .setBus2(vl9Bus1.getId())
         .setConnectableBus2(vl9Bus1.getId())
         .setR(3.0)
         .setX(33.0)
         .setG1(1.0)
         .setB1(0.2)
         .setG2(2.0)
         .setB2(0.4)
         .add();

     VoltageLevel& vl10 = substation5.newVoltageLevel()
         .setId("VL10")
         .setName("VL10_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .add();

     Bus& vl10Bus1 = vl10.getBusBreakerView().newBus()
         .setId("VL10_BUS1")
         .add();

     vl10.newLoad()
         .setId("LOAD10")
         .setName("LOAD10_NAME")
         .setBus(vl10Bus1.getId())
         .setConnectableBus(vl10Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(230.0)
         .setQ0(220.0)
         .add();

     network.newLine()
         .setId("VL8_VL10")
         .setVoltageLevel1(vl8.getId())
         .setBus1(vl8Bus1.getId())
         .setConnectableBus1(vl8Bus1.getId())
         .setVoltageLevel2(vl10.getId())
         .setBus2(vl10Bus1.getId())
         .setConnectableBus2(vl10Bus1.getId())
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
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl1.getNodeBreakerView().setNodeCount(2);

    vl1.newLoad()
        .setId("LOAD1")
        .setName("LOAD1_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl2.getNodeBreakerView().setNodeCount(3);

    vl2.newLoad()
        .setId("LOAD2")
        .setName("LOAD2_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(90.0)
        .add();

    vl3.getNodeBreakerView().setNodeCount(2);

    vl3.newLoad()
        .setId("LOAD3")
        .setName("LOAD3_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl4.getNodeBreakerView().setNodeCount(3);

    vl4.newLoad()
        .setId("LOAD4")
        .setName("LOAD4_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VL5_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl5.getNodeBreakerView().setNodeCount(3);

    vl5.newLoad()
        .setId("LOAD5")
        .setName("LOAD5_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("S3_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl6 = substation3.newVoltageLevel()
        .setId("VL6")
        .setName("VL6_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl6.getNodeBreakerView().setNodeCount(3);

    vl6.newLoad()
        .setId("LOAD6")
        .setName("LOAD6_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VSC1_NAME")
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
        .line1()
        .setId("H1_TL_VL4_VL6")
        .setXnodeP(1)
        .setXnodeQ(2)
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .line2()
        .setId("H2_TL_VL4_VL6")
        .setXnodeP(3)
        .setXnodeQ(4)
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add();

    Substation& substation4 = network.newSubstation()
        .setId("S4")
        .setName("S4_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl7 = substation4.newVoltageLevel()
        .setId("VL7")
        .setName("VL7_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl7.getNodeBreakerView().setNodeCount(3);

    vl7.newLoad()
        .setId("LOAD7")
        .setName("LOAD7_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VSC2_NAME")
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
        .setName("HVDC1_NAME")
        .setActivePowerSetpoint(11.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .setConverterStationId1("VSC1")
        .setConverterStationId2("VSC2")
        .setMaxP(12.0)
        .setNominalVoltage(380.0)
        .setR(14.0)
        .add();

    VoltageLevel& vl8 = substation4.newVoltageLevel()
        .setId("VL8")
        .setName("VL8_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl8.getNodeBreakerView().setNodeCount(2);

    vl8.newLoad()
        .setId("LOAD8")
        .setName("LOAD8_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("S5_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl9 = substation5.newVoltageLevel()
        .setId("VL9")
        .setName("VL9_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(380.0)
        .add();

    vl9.getNodeBreakerView().setNodeCount(2);

    vl9.newLoad()
        .setId("LOAD9")
        .setName("LOAD9_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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
        .setName("VL10_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .add();

    vl10.getNodeBreakerView().setNodeCount(2);

    vl10.newLoad()
        .setId("LOAD10")
        .setName("LOAD10_NAME")
        .setNode(0)
        .setLoadType(LoadType::UNDEFINED)
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

    return network;
}

BOOST_AUTO_TEST_SUITE(ComponentsTestSuite)

BOOST_AUTO_TEST_CASE(NodeBreakerTest)
{
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm", stdcxx::make_unique<logging::ContainerLogger>());
    Network network = createComponentsTestNetworkNB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    auto& logger = dynamic_cast<logging::ContainerLogger&>(logging::LoggerFactory::getLogger("powsybl::iidm"));

    Bus& bus = network.getBusBreakerView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_CHECK_EQUAL(41UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(40UL).getLevel());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    BOOST_CHECK_EQUAL(42UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(41UL).getLevel());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::INVALID_NUM, ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

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

    BOOST_CHECK_EQUAL(42UL, logger.size());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());

    logging::LoggerFactory::getInstance().removeLogger("powsybl::iidm");
}

BOOST_AUTO_TEST_CASE(BusBreakerTest)
{
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm", stdcxx::make_unique<logging::ContainerLogger>());
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    auto& logger = dynamic_cast<logging::ContainerLogger&>(logging::LoggerFactory::getLogger("powsybl::iidm"));

    Bus& bus = network.getBusBreakerView().getBus("VL1_BUS1");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_BUS1");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_CHECK_EQUAL(1UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(0UL).getLevel());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    BOOST_CHECK_EQUAL(2UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(1UL).getLevel());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK_EQUAL(ComponentConstants::INVALID_NUM, ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

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

    BOOST_CHECK_EQUAL(2UL, logger.size());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_BUS1")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_BUS1")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());

    logging::LoggerFactory::getInstance().removeLogger("powsybl::iidm");
}

BOOST_AUTO_TEST_CASE(BusViewTest)
{
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm", stdcxx::make_unique<logging::ContainerLogger>());
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(2UL, boost::size(cNetwork.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusView().getBuses()));

    auto& logger = dynamic_cast<logging::ContainerLogger&>(logging::LoggerFactory::getLogger("powsybl::iidm"));
    BOOST_CHECK_EQUAL(1UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(0UL).getLevel());

    Bus& bus = network.getBusView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());
    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    BOOST_CHECK_EQUAL(2UL, logger.size());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::INFO, logger.getLogMessage(1UL).getLevel());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), ConnectedComponents::getCcNum(network.getBusView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::INVALID_NUM, ConnectedComponents::getCcNum(network.getBusView().getBus("INVALID")));

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

    BOOST_CHECK_EQUAL(2UL, logger.size());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), ConnectedComponents::getCcNum(network.getBusView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());

    logging::LoggerFactory::getInstance().removeLogger("powsybl::iidm");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
