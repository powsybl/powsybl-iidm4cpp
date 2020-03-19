/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

Network create() {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .add();

    VoltageLevel& vl1 = s.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl1.getBusBreakerView().newBus()
        .setId("Bus1")
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setNominalVoltage(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("Bus2")
        .add();

    VoltageLevel& vl3 = s.newVoltageLevel()
        .setId("VL3")
        .setNominalVoltage(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl3.getBusBreakerView().newBus()
        .setId("Bus3")
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(BusTestSuite)

BOOST_AUTO_TEST_CASE(range_batteries) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newBattery()
        .setId("B")
        .setBus("Bus1")
        .setP0(0.0)
        .setQ0(0.0)
        .setMinP(0.0)
        .setMaxP(100.0)
        .add();

    BOOST_CHECK_EQUAL(1, network.getBatteryCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getBatteries()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getBatteries()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getBatteryCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getBatteries()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getBatteries()));
}

BOOST_AUTO_TEST_CASE(range_danglingLines) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newDanglingLine()
        .setId("DL")
        .setBus("Bus1")
        .setP0(0.0)
        .setQ0(0.0)
        .setR(1.0)
        .setX(0.1)
        .setG(0.00005)
        .setB(0.00005)
        .add();

    BOOST_CHECK_EQUAL(1, network.getDanglingLineCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getDanglingLines()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getDanglingLines()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getDanglingLineCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getDanglingLines()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getDanglingLines()));
}

BOOST_AUTO_TEST_CASE(range_generators) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newGenerator()
        .setId("G")
        .setBus("Bus1")
        .setMinP(0)
        .setMaxP(100)
        .setTargetP(90)
        .setTargetQ(35)
        .setVoltageRegulatorOn(false)
        .add();

    BOOST_CHECK_EQUAL(1, network.getGeneratorCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getGenerators()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getGenerators()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getGeneratorCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getGenerators()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getGenerators()));
}

BOOST_AUTO_TEST_CASE(range_lccConverterStations) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newLccConverterStation()
        .setId("LCC_S")
        .setBus("Bus1")
        .setLossFactor(0.6)
        .setPowerFactor(1.0)
        .add();

    BOOST_CHECK_EQUAL(1, network.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getLccConverterStations()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getLccConverterStations()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getLccConverterStations()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getLccConverterStations()));
}

BOOST_AUTO_TEST_CASE(range_lines) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    network.newLine()
        .setId("L")
        .setVoltageLevel1("VL1")
        .setBus1("Bus1")
        .setVoltageLevel2("VL2")
        .setBus2("Bus2")
        .setR(1.0)
        .setX(0.1)
        .setB1(0.00005)
        .setB2(0.00005)
        .setG1(0.00005)
        .setG2(0.00005)
        .add();

    BOOST_CHECK_EQUAL(1, network.getLineCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getLines()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getLines()));
}

BOOST_AUTO_TEST_CASE(range_loads) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newLoad()
        .setId("L")
        .setBus("Bus1")
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    BOOST_CHECK_EQUAL(1, network.getLoadCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getLoads()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getLoads()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getLoadCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getLoads()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getLoads()));
}

BOOST_AUTO_TEST_CASE(range_shuntCompensators) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newShuntCompensator()
        .setId("Shunt")
        .setBus("Bus1")
        .setbPerSection(0.25)
        .setCurrentSectionCount(1)
        .setMaximumSectionCount(1)
        .add();

    BOOST_CHECK_EQUAL(1, network.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getShuntCompensators()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getShuntCompensators()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getShuntCompensators()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getShuntCompensators()));
}

BOOST_AUTO_TEST_CASE(range_staticVarCompensators) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newStaticVarCompensator()
        .setId("SVC")
        .setBus("Bus1")
        .setBmin(0.0)
        .setBmax(10.0)
        .setRegulationMode(StaticVarCompensator::RegulationMode::OFF)
        .add();

    BOOST_CHECK_EQUAL(1, network.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getStaticVarCompensators()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getStaticVarCompensators()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getStaticVarCompensators()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getStaticVarCompensators()));
}

BOOST_AUTO_TEST_CASE(range_threeWindingsTransformers) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    network.getSubstation("S").newThreeWindingsTransformer()
        .setId("T")
        .newLeg1()
            .setVoltageLevel("VL1")
            .setBus("Bus1")
            .setR(1.0)
            .setX(0.1)
            .setB(0.00005)
            .setG(0.00005)
            .setRatedU(1.0)
        .add()
        .newLeg2()
            .setVoltageLevel("VL2")
            .setBus("Bus2")
            .setR(1.1)
            .setX(0.11)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(1.1)
        .add()
        .newLeg3()
            .setVoltageLevel("VL3")
            .setBus("Bus3")
            .setR(1.2)
            .setX(0.12)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(1.2)
        .add()
        .add();

    BOOST_CHECK_EQUAL(1, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getThreeWindingsTransformers()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getThreeWindingsTransformers()));
}

BOOST_AUTO_TEST_CASE(range_twoWindingsTransformers) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    network.getSubstation("S").newTwoWindingsTransformer()
        .setId("T")
        .setVoltageLevel1("VL1")
        .setBus1("Bus1")
        .setVoltageLevel2("VL2")
        .setBus2("Bus2")
        .setR(1.0)
        .setX(0.1)
        .setB(0.00005)
        .setG(0.00005)
        .setRatedU1(1.0)
        .setRatedU2(1.0)
        .add();

    BOOST_CHECK_EQUAL(1, network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getTwoWindingsTransformers()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getTwoWindingsTransformers()));
}

BOOST_AUTO_TEST_CASE(range_vscConverterStations) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newVscConverterStation()
        .setId("VSC_S")
        .setBus("Bus1")
        .setLossFactor(0.6)
        .setVoltageRegulatorOn(false)
        .setReactivePowerSetpoint(50.0)
        .add();

    BOOST_CHECK_EQUAL(1, network.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(1, boost::size(bus.getVscConverterStations()));
    BOOST_CHECK_EQUAL(1, boost::size(cBus.getVscConverterStations()));

    const VoltageLevel& cVL = vl;
    BOOST_CHECK_EQUAL(1, vl.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl.getVscConverterStations()));
    BOOST_CHECK_EQUAL(1, boost::size(cVL.getVscConverterStations()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
