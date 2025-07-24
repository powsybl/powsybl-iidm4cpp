/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkFactory.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
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

Network createNetworkTest1() {
    Network network("network", "test");
    Substation& substation1 = network.newSubstation()
        .setId("substation1")
        .setCountry(Country::FR)
        .setTso("TSO1")
        .setGeographicalTags({"region1"})
        .add();
    VoltageLevel& voltageLevel1 = substation1.newVoltageLevel()
        .setId("voltageLevel1")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    VoltageLevel::NodeBreakerView& topology1 = voltageLevel1.getNodeBreakerView();
    BusbarSection& voltageLevel1BusbarSection1 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection1")
        .setNode(0)
        .add();
    BusbarSection& voltageLevel1BusbarSection2 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection2")
        .setNode(1)
        .add();
    topology1.newBreaker()
        .setId("voltageLevel1Breaker1")
        .setRetained(true)
        .setOpen(false)
        .setNode1(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    Load& load1 = voltageLevel1.newLoad()
        .setId("load1")
        .setNode(2)
        .setP0(10)
        .setQ0(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Disconnector1")
        .setOpen(false)
        .setNode1(load1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Breaker1")
        .setOpen(false)
        .setNode1(3)
        .setNode2(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .add();
    Generator& generator1 = voltageLevel1.newGenerator()
        .setId("generator1")
        .setEnergySource(EnergySource::NUCLEAR)
        .setMinP(200.0)
        .setMaxP(900.0)
        .setVoltageRegulatorOn(true)
        .setTargetP(900.0)
        .setTargetV(380.0)
        .setNode(5)
        .add();
    generator1.newReactiveCapabilityCurve()
        .beginPoint().setP(200.0).setMinQ(300.0).setMaxQ(500.0).endPoint()
        .beginPoint().setP(900.0).setMinQ(300.0).setMaxQ(500.0).endPoint()
        .add();
    topology1.newDisconnector()
        .setId("generator1Disconnector1")
        .setOpen(false)
        .setNode1(generator1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(6)
        .add();
    topology1.newDisconnector()
        .setId("generator1Breaker1")
        .setOpen(false)
        .setNode1(6)
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    return network;
}

Network createMixedNodeBreakerBusBreakerNetwork() {
    Network network = createNodeBreakerNetwork();
    Substation& s3 = network.newSubstation()
        .setId("S3")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl3 = s3.newVoltageLevel()
        .setId("VL3")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl3.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    vl3.newLoad()
        .setId("LD2")
        .setConnectableBus("B1")
        .setBus("B1")
        .setP0(1.0)
        .setQ0(1.0)
        .add();
    network.getVoltageLevel("VL2").getNodeBreakerView().newBreaker()
        .setId("BR5")
        .setNode1(0)
        .setNode2(4)
        .setOpen(false)
        .add();
    network.newLine()
        .setId("L2")
        .setVoltageLevel1("VL2")
        .setNode1(4)
        .setVoltageLevel2("VL3")
        .setConnectableBus2("B1")
        .setBus2("B1")
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();
    return network;
}

Network createNodeBreakerNetwork() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl1.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setNode(0)
        .add();
    vl1.newGenerator()
        .setId("G")
        .setNode(4)
        .setMaxP(100.0)
        .setMinP(50.0)
        .setTargetP(100.0)
        .setTargetV(400.0)
        .setVoltageRegulatorOn(true)
        .add();
    vl1.getNodeBreakerView().newInternalConnection()
        .setNode1(1)
        .setNode2(4)
        .add();
    vl1.getNodeBreakerView().newDisconnector()
        .setId("BR1")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .add();
    vl1.getNodeBreakerView().newDisconnector()
        .setId("D1")
        .setNode1(0)
        .setNode2(2)
        .setOpen(false)
        .add();
    vl1.getNodeBreakerView().newBreaker()
        .setId("BR2")
        .setNode1(2)
        .setNode2(3)
        .setOpen(false)
        .add();

    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("VL2")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl2.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setNode(0)
        .add();
    vl2.newLoad()
        .setId("LD")
        .setNode(1)
        .setP0(1)
        .setQ0(1)
        .add();
    vl2.getNodeBreakerView().newDisconnector()
        .setId("BR3")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .add();
    vl2.getNodeBreakerView().newDisconnector()
        .setId("D2")
        .setNode1(0)
        .setNode2(2)
        .setOpen(false)
        .add();
    vl2.getNodeBreakerView().newBreaker()
        .setId("BR4")
        .setNode1(2)
        .setNode2(3)
        .setOpen(false)
        .add();
    network.newLine()
        .setId("L1")
        .setVoltageLevel1("VL1")
        .setNode1(3)
        .setVoltageLevel2("VL2")
        .setNode2(3)
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();
    return network;
}

Network createSwitchBBKNetwork() {
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

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
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
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    vl3.newLoad()
        .setId("LOAD1")
        .setBus(vl3Bus1.getId())
        .setConnectableBus(vl3Bus1.getId())
        .setP0(600.0)
        .setQ0(200.0)
        .add();

    Bus& vl3Bus2 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS2")
        .add();

    vl3.newLoad()
        .setId("LOAD2")
        .setBus(vl3Bus2.getId())
        .setConnectableBus(vl3Bus2.getId())
        .setP0(500.0)
        .setQ0(100.0)
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
        .setNominalV(225.0)
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
        .newHalfLine1()
            .setId("H1_TL_VL1_VL3")
            .setR(6.0)
            .setX(66.0)
            .setG1(0.2)
            .setB1(0.4)
            .setG2(0.3)
            .setB2(0.5)
        .add()
        .newHalfLine2()
            .setId("H2_TL_VL1_VL3")
            .setR(7.0)
            .setX(77.0)
            .setG1(0.6)
            .setB1(0.7)
            .setG2(0.9)
            .setB2(1.2)
            .add()
        .add();

    network.newSubstation()
        .setId("S3")
        .setName("S3_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    network.newSubstation()
        .setId("S4")
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

}  // namespace iidm

}  // namespace powsybl
