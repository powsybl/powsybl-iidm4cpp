/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkFactory.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

namespace powsybl {

namespace iidm {

static Network network2("test2", "test2");

Network createHvdcConverterStationTestNetwork() {
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
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newLccConverterStation()
        .setId("LCC1")
        .setName("LCC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(1.0)
        .setPowerFactor(1.0)
        .add();

    vl1.newVscConverterStation()
        .setId("VSC1")
        .setName("VSC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(3.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(4.0)
        .setReactivePowerSetpoint(5.0)
        .add();

    return network;
}

Network createMixedTopolyKindNetwork() {
    Network network("test", "test");

    Substation& substation = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = substation.newVoltageLevel()
        .setId("VL")
        .setName("VL_name")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB1")
        .setNode1(1)
        .setNode2(99)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newInternalConnection()
        .setNode1(99)
        .setNode2(2)
        .add();

    vl.newLoad()
        .setId("LOAD1")
        .setNode(2)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD2")
        .setNode1(0)
        .setNode2(3)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.newLoad()
        .setId("LOAD2")
        .setNode(3)
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB2")
        .setNode1(3)
        .setNode2(4)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    network.newLine()
        .setId("VL1_VL2")
        .setVoltageLevel1(vl.getId())
        .setNode1(4)
        .setVoltageLevel2(vl2.getId())
        .setNode2(0)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    VoltageLevel& vl3 = substation.newVoltageLevel()
        .setId("VL3")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    vl3.newLoad()
        .setId("LOAD3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .setName("LOAD3_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    return network;
}

Network createNetwork() {
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
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newLoad()
        .setId("LOAD1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    return network;
}

Terminal& getTerminalFromNetwork2() {
    if(network2.getSubstationCount() == 0) {
        Substation& s = network2.newSubstation()
            .setId("S")
            .setCountry(Country::FR)
            .add();

        VoltageLevel& vl = s.newVoltageLevel()
            .setId("VL")
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .setNominalV(400.0)
            .setLowVoltageLimit(380.0)
            .setHighVoltageLimit(420.0)
            .add();

        vl.newLoad()
            .setId("LOAD1")
            .setNode(0)
            .setName("LOAD1_NAME")
            .setLoadType(LoadType::UNDEFINED)
            .setP0(50.0)
            .setQ0(40.0)
            .add();
    }

    return network2.getLoad("LOAD1").getTerminal();
}

Network createComponentsTestNetworkBB() {
    Network network("testBB", "testBB");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(90.0)
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(380.0)
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
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL4_VL6")
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(90.0)
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(380.0)
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
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL4_VL6")
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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
        .setNominalV(380.0)
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
        .setNominalV(225.0)
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

}  // namespace iidm

}  // namespace powsybl
