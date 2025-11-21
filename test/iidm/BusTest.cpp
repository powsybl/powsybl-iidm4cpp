/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <set>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/AbstractTerminalTopologyVisitor.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
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
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network create() {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .add();

    VoltageLevel& vl1 = s.newVoltageLevel()
        .setId("VL1")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl1.getBusBreakerView().newBus()
        .setId("Bus1")
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("Bus2")
        .add();

    VoltageLevel& vl3 = s.newVoltageLevel()
        .setId("VL3")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    vl3.getBusBreakerView().newBus()
        .setId("Bus3")
        .add();

    return network;
}

Network createConnectedTerminalNetwork() {
    Network network("test", "test");
    Substation& s = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .setTso("TSO1")
        .setGeographicalTags({"region1"})
        .add();
    VoltageLevel& vl1 = s.newVoltageLevel()
        .setId("VL1")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B2")
        .add();
    vl1.getBusBreakerView().newSwitch()
        .setId("BR1")
        .setBus1("B1")
        .setBus2("B2")
        .setOpen(false)
        .add();

    vl1.newGenerator()
        .setId("G")
        .setBus("B1")
        .setMaxP(100.0)
        .setMinP(50.0)
        .setTargetP(100.0)
        .setTargetV(400.0)
        .setVoltageRegulatorOn(true)
        .add();
    vl1.newLoad()
        .setId("LD1")
        .setConnectableBus("B1")
        .setBus("B1")
        .setP0(1.0)
        .setQ0(1.0)
        .add();

    vl1.newLoad()
        .setId("LD2")
        .setConnectableBus("B2")
        .setBus("B2")
        .setP0(1.0)
        .setQ0(1.0)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getBusBreakerView().newBus()
        .setId("B21")
        .add();
    vl2.newGenerator()
        .setId("G2")
        .setBus("B21")
        .setMaxP(100.0)
        .setMinP(50.0)
        .setTargetP(100.0)
        .setTargetV(400.0)
        .setVoltageRegulatorOn(true)
        .add();

    network.newLine()
        .setId("L1")
        .setVoltageLevel1("VL1")
        .setBus1("B1")
        .setVoltageLevel2("VL2")
        .setBus2("B21")
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(BusTestSuite)

BOOST_AUTO_TEST_CASE(testSetterGetter) {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setCountry(Country::AF)
        .setTso("tso")
        .setName("sub")
        .setId("subId")
        .add();
    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setId("bbVL")
        .setName("bbVL_name")
        .setNominalV(200.0)
        .add();
    // ConfiguredBus
    Bus& bus = voltageLevel.getBusBreakerView()
        .newBus()
        .setName("bus1Name")
        .setId("bus1")
        .add();

    BOOST_CHECK_EQUAL("bus1", network.getBusBreakerView().getBus("bus1").get().getId());
    BOOST_CHECK_EQUAL("bus1", bus.getId());
    BOOST_CHECK_EQUAL("bus1Name", bus.getNameOrId());

    LccConverterStation& lccConverterStation = voltageLevel.newLccConverterStation()
        .setId("lcc")
        .setName("lcc")
        .setBus("bus1")
        .setLossFactor(0.011)
        .setPowerFactor(0.5)
        .setConnectableBus("bus1")
        .add();
    VscConverterStation& vscConverterStation = voltageLevel.newVscConverterStation()
        .setId("vsc")
        .setName("vsc")
        .setBus("bus1")
        .setLossFactor(0.011)
        .setVoltageRegulatorOn(false)
        .setReactivePowerSetpoint(1.0)
        .setConnectableBus("bus1")
        .add();
    BOOST_CHECK_EQUAL(HvdcConverterStation::HvdcType::LCC, lccConverterStation.getHvdcType());
    BOOST_CHECK_EQUAL(HvdcConverterStation::HvdcType::VSC, vscConverterStation.getHvdcType());
    double p1 = 1.0;
    double q1 = 2.0;
    double p2 = 10.0;
    double q2 = 20.0;
    lccConverterStation.getTerminal().setP(p1);
    lccConverterStation.getTerminal().setQ(q1);
    vscConverterStation.getTerminal().setP(p2);
    vscConverterStation.getTerminal().setQ(q2);

    BOOST_CHECK(stdcxx::areSame(voltageLevel, bus.getVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(network, bus.getNetwork()));
    POWSYBL_ASSERT_THROW(bus.setV(-1.0), PowsyblException, "Bus 'bus1': voltage cannot be < 0");
    bus.setV(200.0);
    BOOST_CHECK_CLOSE(200.0, bus.getV(), std::numeric_limits<double>::epsilon());
    bus.setAngle(30.0);
    BOOST_CHECK_CLOSE(30.0, bus.getAngle(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(p1 + p2, bus.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(q1 + q2, bus.getQ(), std::numeric_limits<double>::epsilon());

    
    BOOST_CHECK(std::isnan(bus.getFictitiousP0()));
    BOOST_CHECK(std::isnan(bus.getFictitiousQ0()));
    bus.setFictitiousP0(1.0).setFictitiousQ0(2.0);
    BOOST_CHECK_CLOSE(1.0, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());
    const auto& terminals = bus.getConnectedTerminals();
    for (auto& term : terminals) {
        stdcxx::Reference<Bus> refBus = term.getBusView().getBus();
        if(refBus) {
            BOOST_CHECK_CLOSE(1.0, refBus.get().getFictitiousP0(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(2.0, refBus.get().getFictitiousQ0(), std::numeric_limits<double>::epsilon());
            refBus.get().setFictitiousP0(3.0).setFictitiousQ0(4.0);
            break;
        }
    }
    BOOST_CHECK_CLOSE(3.0, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_CASE(range_batteries) {
    Network network = create();
    auto& bus = network.get<Bus>("Bus1");
    const auto& cBus = bus;

    bus.setFictitious(true);
    BOOST_CHECK(bus.isFictitious());
    bus.setFictitious(false);
    BOOST_CHECK(!bus.isFictitious());

    VoltageLevel& vl = network.getVoltageLevel("VL1");
    vl.newBattery()
        .setId("B")
        .setBus("Bus1")
        .setTargetP(0.0)
        .setTargetQ(0.0)
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
        .setSectionCount(1)
        .newLinearModel()
        .setBPerSection(0.25)
        .setGPerSection(0.75)
        .setMaximumSectionCount(3)
        .add()
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

class TerminalTopologyVisitor : public AbstractTerminalTopologyVisitor {
public:
    TerminalTopologyVisitor() = default;

    const std::map<IdentifiableType, std::set<std::string>>& getConnectables() const {
        return m_equipments;
    }

    void visitTerminal(const Terminal& terminal) override {
        m_equipments[terminal.getConnectable().get().getType()].emplace(terminal.getConnectable().get().getId());
    }

private:
    std::map<IdentifiableType, std::set<std::string>> m_equipments;
};

BOOST_AUTO_TEST_CASE(TerminalVisitorEurostag) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    TerminalTopologyVisitor connectableEquipmentsVisitor;

    network.get<Bus>("NHV2").visitConnectedOrConnectableEquipments(connectableEquipmentsVisitor);
    BOOST_CHECK_EQUAL(2, boost::size(connectableEquipmentsVisitor.getConnectables()));

    std::set<std::string> connectedLines_NHV2 = { "NHV1_NHV2_1", "NHV1_NHV2_2" };
    BOOST_CHECK(connectedLines_NHV2 == connectableEquipmentsVisitor.getConnectables().find(IdentifiableType::LINE)->second);

    std::set<std::string> connected2WT_NHV2 = { "NHV2_NLOAD" };
    BOOST_CHECK(connected2WT_NHV2 == connectableEquipmentsVisitor.getConnectables().find(IdentifiableType::TWO_WINDINGS_TRANSFORMER)->second);

    TerminalTopologyVisitor connectedEquipmentsVisitor;
    Line& line = network.getLine("NHV1_NHV2_1");
    line.getTerminal1().disconnect();

    network.get<Bus>("NHV1").visitConnectedEquipments(connectedEquipmentsVisitor);
    BOOST_CHECK_EQUAL(2, boost::size(connectedEquipmentsVisitor.getConnectables()));

    std::set<std::string> connectedLines_NHV1 = { "NHV1_NHV2_2" };
    BOOST_CHECK(connectedLines_NHV1 == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::LINE)->second);

    std::set<std::string> connected2WT_NHV1 = { "NGEN_NHV1" };
    BOOST_CHECK(connected2WT_NHV1 == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::TWO_WINDINGS_TRANSFORMER)->second);
}

BOOST_AUTO_TEST_CASE(TerminalVisitorAllBbk) {
    Network network = create();

    VoltageLevel& vl = network.getVoltageLevel("VL1");

    vl.newBattery()
        .setId("BATTERY")
        .setBus("Bus1")
        .setTargetP(0.0)
        .setTargetQ(0.0)
        .setMinP(0.0)
        .setMaxP(100.0)
        .add();

    vl.newStaticVarCompensator()
        .setId("SVC")
        .setBus("Bus1")
        .setBmin(0.0)
        .setBmax(10.0)
        .setRegulationMode(StaticVarCompensator::RegulationMode::OFF)
        .add();

    vl.newVscConverterStation()
        .setId("VSC_S")
        .setBus("Bus1")
        .setLossFactor(0.6)
        .setVoltageRegulatorOn(false)
        .setReactivePowerSetpoint(50.0)
        .add();

    vl.newShuntCompensator()
        .setId("SHUNT")
        .setBus("Bus1")
        .setSectionCount(1)
        .newLinearModel()
        .setBPerSection(0.25)
        .setGPerSection(0.75)
        .setMaximumSectionCount(3)
        .add()
        .add();

    vl.newLoad()
        .setId("LOAD")
        .setBus("Bus1")
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    network.newLine()
        .setId("LINE")
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

    vl.newLccConverterStation()
        .setId("LCC_S")
        .setBus("Bus1")
        .setLossFactor(0.6)
        .setPowerFactor(1.0)
        .add();

    vl.newGenerator()
        .setId("GENERATOR")
        .setBus("Bus1")
        .setMinP(0)
        .setMaxP(100)
        .setTargetP(90)
        .setTargetQ(35)
        .setVoltageRegulatorOn(false)
        .add();

    vl.newDanglingLine()
        .setId("DANGLINE_LINE")
        .setBus("Bus1")
        .setP0(0.0)
        .setQ0(0.0)
        .setR(1.0)
        .setX(0.1)
        .setG(0.00005)
        .setB(0.00005)
        .add();

    network.getSubstation("S").newTwoWindingsTransformer()
        .setId("2WT")
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

    network.getSubstation("S").newThreeWindingsTransformer()
        .setId("3WT")
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

    TerminalTopologyVisitor connectedEquipmentsVisitor;

    network.get<Bus>("Bus1").visitConnectedEquipments(connectedEquipmentsVisitor);
    BOOST_CHECK_EQUAL(10, boost::size(connectedEquipmentsVisitor.getConnectables()));

    std::set<std::string> connectedLines = { "LINE" };
    BOOST_CHECK(connectedLines == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::LINE)->second);

    std::set<std::string> connected2WT = { "2WT" };
    BOOST_CHECK(connected2WT == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::TWO_WINDINGS_TRANSFORMER)->second);

    std::set<std::string> connected3WT = { "3WT" };
    BOOST_CHECK(connected3WT == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::THREE_WINDINGS_TRANSFORMER)->second);

    std::set<std::string> connectedGenerators = { "GENERATOR" };
    BOOST_CHECK(connectedGenerators == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::GENERATOR)->second);

    std::set<std::string> connectedBatteries = { "BATTERY" };
    BOOST_CHECK(connectedBatteries == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::BATTERY)->second);

    std::set<std::string> connectedLoads = { "LOAD" };
    BOOST_CHECK(connectedLoads == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::LOAD)->second);

    std::set<std::string> connectedShunts = { "SHUNT" };
    BOOST_CHECK(connectedShunts == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::SHUNT_COMPENSATOR)->second);

    std::set<std::string> connectedDanglingLines = { "DANGLINE_LINE" };
    BOOST_CHECK(connectedDanglingLines == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::DANGLING_LINE)->second);

    std::set<std::string> connectedSvc = { "SVC" };
    BOOST_CHECK(connectedSvc == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::STATIC_VAR_COMPENSATOR)->second);

    std::set<std::string> connectedHvdc = { "VSC_S", "LCC_S" };
    BOOST_CHECK(connectedHvdc == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::HVDC_CONVERTER_STATION)->second);

    // visit Bus2
    TerminalTopologyVisitor bus2Visitor;
    network.get<Bus>("Bus2").visitConnectedEquipments(bus2Visitor);
    std::set<std::string> connected3WT_Buses12 = { "3WT" };
    BOOST_CHECK(connected3WT_Buses12 == bus2Visitor.getConnectables().find(IdentifiableType::THREE_WINDINGS_TRANSFORMER)->second);

    // visit Bus3
    TerminalTopologyVisitor bus3Visitor;
    network.get<Bus>("Bus3").visitConnectedEquipments(bus3Visitor);
    std::set<std::string> connected3WT_Buses123 = { "3WT" };
    BOOST_CHECK(connected3WT_Buses123 == bus3Visitor.getConnectables().find(IdentifiableType::THREE_WINDINGS_TRANSFORMER)->second);

    // visit VoltageLevel
    TerminalTopologyVisitor voltageLevelVisitor;
    vl.visitEquipments(voltageLevelVisitor);
    BOOST_CHECK_EQUAL(10, boost::size(voltageLevelVisitor.getConnectables()));
}

BOOST_FIXTURE_TEST_CASE(TerminalVisitorBusbarSection, test::ResourceFixture) {
    Network network = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("fictitiousSwitchRef.xml", converter::xml::IidmXmlVersion::V1_4()));

    TerminalTopologyVisitor connectedEquipmentsVisitor;

    network.getBusbarSection("D").getTerminal().getBusBreakerView().getBus().get().visitConnectedOrConnectableEquipments(connectedEquipmentsVisitor);
    BOOST_CHECK_EQUAL(1, boost::size(connectedEquipmentsVisitor.getConnectables()));

    std::set<std::string> connectedBbs = { "D" };
    BOOST_CHECK(connectedBbs == connectedEquipmentsVisitor.getConnectables().find(IdentifiableType::BUSBAR_SECTION)->second);
}

BOOST_AUTO_TEST_CASE(testConnectedTerminals) {
    Network network = createConnectedTerminalNetwork();
    const VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    const Terminal& gt = network.getGenerator("G").getTerminal();
    const Terminal& ld1t = network.getLoad("LD1").getTerminal();
    const Terminal& ld2t = network.getLoad("LD2").getTerminal();
    const Terminal& l1t = network.getLine("L1").getTerminal1();

    BOOST_CHECK(gt.isConnected());
    BOOST_CHECK(ld1t.isConnected());
    BOOST_CHECK(ld2t.isConnected());
    BOOST_CHECK(l1t.isConnected());

    BOOST_CHECK_EQUAL(1, boost::size(vl1.getBusView().getBuses()));

    BOOST_CHECK(stdcxx::areSame(gt.getBusView().getBus().get(), gt.getBusView().getConnectableBus().get()));
    BOOST_CHECK(stdcxx::areSame(ld1t.getBusView().getBus().get(), ld1t.getBusView().getConnectableBus().get()));
    BOOST_CHECK(stdcxx::areSame(ld2t.getBusView().getBus().get(), ld2t.getBusView().getConnectableBus().get()));
    BOOST_CHECK(stdcxx::areSame(l1t.getBusView().getBus().get(), l1t.getBusView().getConnectableBus().get()));

    BOOST_CHECK_EQUAL(4, vl1.getBusView().getBus("VL1_0").get().getConnectedTerminalCount());
    BOOST_CHECK_EQUAL(4, boost::size(vl1.getBusView().getBus("VL1_0").get().getConnectedTerminals()));
}

BOOST_AUTO_TEST_CASE(testDisconnectConnect) {
    Network network = createConnectedTerminalNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    Terminal& gt = network.getGenerator("G").getTerminal();
    Terminal& l1t = network.getLine("L1").getTerminal1();

    BOOST_CHECK(gt.disconnect());
    BOOST_CHECK(!gt.isConnected());
    BOOST_CHECK(!gt.getBusView().getBus());
    BOOST_CHECK(gt.getBusView().getConnectableBus());
    BOOST_CHECK(stdcxx::areSame(vl1.getBusView().getBus("VL1_0").get(), gt.getBusView().getConnectableBus().get()));
    BOOST_CHECK_EQUAL(3, vl1.getBusView().getBus("VL1_0").get().getConnectedTerminalCount());
    BOOST_CHECK_EQUAL(3, boost::size(vl1.getBusView().getBus("VL1_0").get().getConnectedTerminals()));

    BOOST_CHECK(l1t.disconnect());
    BOOST_CHECK(!l1t.isConnected());
    BOOST_CHECK_EQUAL(0, boost::size(vl1.getBusView().getBuses())); // Because no line in the VL
    BOOST_CHECK(!l1t.getBusView().getBus());
    BOOST_CHECK(!l1t.getBusView().getConnectableBus()); // Because no buses
    BOOST_CHECK(!l1t.getBusView().getConnectableBus() && !vl1.getBusView().getBus("VL1_0"));
    BOOST_CHECK(l1t.connect());
    BOOST_CHECK(l1t.isConnected());

    BOOST_CHECK(gt.connect());
    BOOST_CHECK(gt.isConnected());
    BOOST_CHECK(stdcxx::areSame(vl1.getBusView().getBus("VL1_0").get(), gt.getBusView().getConnectableBus().get()));

    BOOST_CHECK_EQUAL(4, vl1.getBusView().getBus("VL1_0").get().getConnectedTerminalCount());
    BOOST_CHECK_EQUAL(4, boost::size(vl1.getBusView().getBus("VL1_0").get().getConnectedTerminals()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
