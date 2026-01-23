/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/AreaAdder.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Subnetwork.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

std::string id(const std::string& id, const std::string& networkId){
    return "n" + networkId + "_" + id;
}

void CreateSubnetworkExploreTest(Network& network, const std::string& nid, Country otherSubstationCountry) {
    Substation &substation1 = network.newSubstation()
                                  .setId(id("substation1", nid))
                                  .setCountry(Country::FR)
                                  .setTso(id("TSO1", nid))
                                  .setGeographicalTags({id("region1", nid)})
                                  .add();
    VoltageLevel &voltageLevel1 = substation1.newVoltageLevel()
                                      .setId(id("voltageLevel1", nid))
                                      .setNominalV(400)
                                      .setTopologyKind(TopologyKind::NODE_BREAKER)
                                      .add();
    VoltageLevel::NodeBreakerView &topology1 = voltageLevel1.getNodeBreakerView();
    BusbarSection &voltageLevel1BusbarSection1 = topology1.newBusbarSection()
                                                     .setId(id("voltageLevel1BusbarSection1", nid))
                                                     .setNode(0)
                                                     .add();
    BusbarSection &voltageLevel1BusbarSection2 = topology1.newBusbarSection()
                                                     .setId(id("voltageLevel1BusbarSection2", nid))
                                                     .setNode(1)
                                                     .add();
    topology1.newBreaker()
        .setId(id("voltageLevel1Breaker1", nid))
        .setRetained(true)
        .setOpen(false)
        .setNode1(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    Load &load1 = voltageLevel1.newLoad()
                      .setId(id("load1", nid))
                      .setNode(2)
                      .setP0(10)
                      .setQ0(3)
                      .add();
    topology1.newDisconnector()
        .setId(id("load1Disconnector1", nid))
        .setOpen(false)
        .setNode1(load1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(3)
        .add();
    topology1.newDisconnector()
        .setId(id("load1Breaker1", nid))
        .setOpen(false)
        .setNode1(3)
        .setNode2(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .add();
    Generator &generator1 = voltageLevel1.newGenerator()
                                .setId(id("generator1", nid))
                                .setEnergySource(EnergySource::NUCLEAR)
                                .setMinP(200.0)
                                .setMaxP(900.0)
                                .setVoltageRegulatorOn(true)
                                .setTargetP(900.0)
                                .setTargetV(380.0)
                                .setNode(5)
                                .add();
    generator1.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(200.0)
        .setMinQ(300.0)
        .setMaxQ(500.0)
        .endPoint()
        .beginPoint()
        .setP(900.0)
        .setMinQ(300.0)
        .setMaxQ(500.0)
        .endPoint()
        .add();
    topology1.newDisconnector()
        .setId(id("generator1Disconnector1", nid))
        .setOpen(false)
        .setNode1(generator1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(6)
        .add();
    topology1.newDisconnector()
        .setId(id("generator1Breaker1", nid))
        .setOpen(false)
        .setNode1(6)
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();

    voltageLevel1.newBattery()
        .setId(id("battery1", nid))
        .setMaxP(20.0)
        .setMinP(10.0)
        .setTargetP(15.0)
        .setTargetQ(10.0)
        .setNode(4)
        .add();
    voltageLevel1.newShuntCompensator()
        .setId(id("shuntCompensator1", nid))
        .setNode(7)
        .setSectionCount(0)
        .newLinearModel()
        .setBPerSection(1e-5)
        .setMaximumSectionCount(1)
        .add()
        .add();
    voltageLevel1.newStaticVarCompensator()
        .setId(id("svc1", nid))
        .setNode(12)
        .setBmin(-5e-2)
        .setBmax(5e-2)
        .setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE)
        .setVoltageSetpoint(400)
        .add();
    voltageLevel1.newLccConverterStation()
        .setId(id("lcc1", nid))
        .setNode(8)
        .setPowerFactor(0.95f)
        .setLossFactor(0.99f)
        .add();
    voltageLevel1.newVscConverterStation()
        .setId(id("vsc1", nid))
        .setNode(10)
        .setLossFactor(1.1f)
        .setVoltageSetpoint(405.0)
        .setVoltageRegulatorOn(true)
        .add();

    Substation &substation2 = network.newSubstation()
                                  .setId(id("substation2", nid))
                                  .setCountry(otherSubstationCountry)
                                  .setTso(id("TSO2", nid))
                                  .setGeographicalTags({id("region2", nid)})
                                  .add();
    VoltageLevel &voltageLevel2 = substation2.newVoltageLevel()
                                     .setId(id("voltageLevel2", nid))
                                     .setNominalV(400)
                                     .setTopologyKind(TopologyKind::NODE_BREAKER)
                                     .add();
    voltageLevel2.newLccConverterStation()
        .setId(id("lcc2", nid))
        .setNode(9)
        .setPowerFactor(0.95f)
        .setLossFactor(0.99f)
        .add();
    voltageLevel2.newVscConverterStation()
        .setId(id("vsc2", nid))
        .setNode(11)
        .setLossFactor(1.1f)
        .setReactivePowerSetpoint(123)
        .setVoltageRegulatorOn(false)
        .add();

    network.newHvdcLine()
        .setId(id("hvdcLine1", nid))
        .setR(1)
        .setNominalV(400)
        .setConverterStationId1(id("lcc1", nid))
        .setConverterStationId2(id("lcc2", nid))
        .setMaxP(2000)
        .setActivePowerSetpoint(50)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .add();
    network.newHvdcLine()
        .setId(id("hvdcLine2", nid))
        .setR(5.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
        .setNominalV(440.0)
        .setMaxP(50.0)
        .setActivePowerSetpoint(20.0)
        .setConverterStationId1(id("vsc1", nid))
        .setConverterStationId2(id("vsc2", nid))
        .add();

    Substation &substation3 = network.newSubstation()
                                 .setId(id("substation3", nid))
                                 .setCountry(Country::DE)
                                 .setTso(id("TSO3", nid))
                                 .add();
    VoltageLevel &voltageLevel3 = substation3.newVoltageLevel()
                                     .setId(id("voltageLevel3", nid))
                                     .setNominalV(400)
                                     .setTopologyKind(TopologyKind::NODE_BREAKER)
                                     .add();
    substation3.newVoltageLevel()
        .setId(id("voltageLevel4", nid))
        .setNominalV(225)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    substation3.newVoltageLevel()
        .setId(id("voltageLevel5", nid))
        .setNominalV(90)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    ThreeWindingsTransformerAdder threeWindingsTransformerAdder1 = substation3.newThreeWindingsTransformer()
                                                                       .setId(id("threeWindingsTransformer1", nid))
                                                                       .setRatedU0(400);
    threeWindingsTransformerAdder1.newLeg1()
        .setNode(1)
        .setR(0.001)
        .setX(0.000001)
        .setB(0)
        .setG(0)
        .setRatedU(400)
        .setVoltageLevel(id("voltageLevel3", nid))
        .add();
    threeWindingsTransformerAdder1.newLeg2()
        .setNode(1)
        .setR(0.1)
        .setX(0.00001)
        .setB(0)
        .setG(0)
        .setRatedU(225)
        .setVoltageLevel(id("voltageLevel4", nid))
        .add();
    threeWindingsTransformerAdder1.newLeg3()
        .setNode(1)
        .setR(0.01)
        .setX(0.0001)
        .setB(0)
        .setG(0)
        .setRatedU(90)
        .setVoltageLevel(id("voltageLevel5", nid))
        .add();
    threeWindingsTransformerAdder1.add();
    substation3.newTwoWindingsTransformer()
        .setId(id("twoWindingsTransformer1", nid))
        .setVoltageLevel1(id("voltageLevel3", nid))
        .setNode1(2)
        .setRatedU1(400)
        .setVoltageLevel2(id("voltageLevel4", nid))
        .setNode2(2)
        .setRatedU2(225)
        .setR(0.24 / 1300 * (38 * 38))
        .setX(sqrt(10 * 10 - 0.24 * 0.24) / 1300 * (38 * 38))
        .setG(0.0)
        .setB(0.0)
        .add();
    std::string line1Id = id("line1", nid);
    network.newLine()
        .setId(line1Id)
        .setVoltageLevel1(id("voltageLevel1", nid))
        .setNode1(13)
        .setVoltageLevel2(id("voltageLevel2", nid))
        .setNode2(14)
        .setR(1)
        .setX(1)
        .setG1(0)
        .setG2(0)
        .setB1(0)
        .setB2(0)
        .add();
    voltageLevel1.newDanglingLine()
        .setId(id("danglingLine1", nid))
        .setNode(15)
        .setR(1.0)
        .setX(0.1)
        .setG(0.0)
        .setB(0.001)
        .setP0(10)
        .setQ0(1)
        .add();
    voltageLevel2.newDanglingLine()
        .setId(id("danglingLine2", nid))
        .setNode(16)
        .setR(1.0)
        .setX(0.1)
        .setG(0.0)
        .setB(0.001)
        .setP0(10)
        .setQ0(1)
        .add();
    network.newTieLine()
        .setId(id("tieLine1", nid))
        .setDanglingLine1(id("danglingLine1", nid))
        .setDanglingLine2(id("danglingLine2", nid))
        .add();
    voltageLevel3.newDanglingLine()
        .setId(id("danglingLine3", nid))
        .setNode(17)
        .setR(1.0)
        .setX(0.1)
        .setG(0.0)
        .setB(0.001)
        .setP0(10)
        .setQ0(1)
        .setPairingKey("mergingKey")
        .add();

    substation3.newOverloadManagementSystem()
        .setId(id("overloadManagementSystem", nid))
        .setEnabled(true)
        .setMonitoredElementId(line1Id)
        .setMonitoredElementSide(ThreeSides::ONE)
        .newBranchTripping()
            ->setBranchToOperateId(line1Id)
            .setSideToOperate(TwoSides::ONE)
            .setKey("branchTripping")
            .setCurrentLimit(80.)
            .setOpenAction(true)
            .add()
        .add();

    Area& area1 = network.newArea()
                            .setId(id("area1", nid))
                            .setAreaType(id("areaType1", nid))
                            .setName("AREA")
                            .add();

    voltageLevel1.addArea(area1);
    return;
}

template <typename T>
void assertCollection(const std::set<T>& expectedSet, const std::set<T>& resultSet) {
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedSet.begin(), expectedSet.end(), resultSet.begin(), resultSet.end());
}

BOOST_AUTO_TEST_SUITE(SubnetworkExplorationTestSuite)

BOOST_AUTO_TEST_CASE(SubnetworkExplorationTest) {
    Network network("Root", "Root");
    Network& subnetwork1 = network.newSubnetwork("n1_network", "n1_format");
    Network& subnetwork2 = network.newSubnetwork("n2_network", "n2_format");

    CreateSubnetworkExploreTest(subnetwork1, "1", Country::ES);
    CreateSubnetworkExploreTest(subnetwork2, "2", Country::BE);

    //Create Tie line on dangling lines from each subnetworks
    network.newTieLine()
        .setId(id("tieLine3", "0"))
        .setDanglingLine1(id("danglingLine3", "1"))
        .setDanglingLine2(id("danglingLine3", "2"))
        .add();

    // Explore subnetworks
    BOOST_CHECK_EQUAL(2, network.getSubNetworksCount());
    BOOST_CHECK_EQUAL(0, subnetwork1.getSubNetworksCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getSubNetworksCount());
    BOOST_CHECK(!subnetwork1.getSubNetwork("Root"));
    BOOST_CHECK(!subnetwork1.getSubNetwork(id("network", "1")));
    BOOST_CHECK(!subnetwork1.getSubNetwork(id("network", "2")));
    BOOST_CHECK(!subnetwork2.getSubNetwork("Root"));
    BOOST_CHECK(!subnetwork2.getSubNetwork(id("network", "1")));
    BOOST_CHECK(!subnetwork2.getSubNetwork(id("network", "2")));

    // Explore networks
    BOOST_CHECK(stdcxx::areSame(network, network.getRootNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork1.getRootNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork2.getRootNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, network.getNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork1.getNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork2.getNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, network.getParentNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork1.getParentNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork2.getParentNetwork()));
    BOOST_CHECK(!network.getParentNetworkRef());
    BOOST_CHECK(subnetwork1.getParentNetworkRef());
    BOOST_CHECK(subnetwork2.getParentNetworkRef());
    BOOST_CHECK(stdcxx::areSame(network, subnetwork1.getParentNetworkRef().get()));
    BOOST_CHECK(stdcxx::areSame(network, subnetwork2.getParentNetworkRef().get()));

    // Explore VariantManager
    BOOST_CHECK(stdcxx::areSame(network.getVariantManager(), subnetwork1.getVariantManager()));
    BOOST_CHECK(stdcxx::areSame(network.getVariantManager(), subnetwork2.getVariantManager()));

    // Explore countries
    BOOST_CHECK_EQUAL(4, network.getCountryCount());
    BOOST_CHECK_EQUAL(3, subnetwork1.getCountryCount());
    BOOST_CHECK_EQUAL(3, subnetwork2.getCountryCount());
    assertCollection({Country::FR, Country::ES, Country::BE, Country::DE}, network.getCountries());
    assertCollection({Country::FR, Country::ES, Country::DE}, subnetwork1.getCountries());
    assertCollection({Country::FR, Country::BE, Country::DE}, subnetwork2.getCountries());

    //Explore substations
    auto expectedSubstations1 = {id("substation1", "1"), id("substation2", "1"), id("substation3", "1")};
    auto expectedSubstations2 = {id("substation1", "2"), id("substation2", "2"), id("substation3", "2")};
    BOOST_CHECK_EQUAL(expectedSubstations1.size(), subnetwork1.getSubstationCount());
    BOOST_CHECK_EQUAL(expectedSubstations2.size(), subnetwork2.getSubstationCount());
    BOOST_CHECK_EQUAL(expectedSubstations1.size(), boost::size(subnetwork1.getSubstations()));
    BOOST_CHECK_EQUAL(expectedSubstations2.size(), boost::size(subnetwork2.getSubstations()));
    for (auto& id : expectedSubstations1) {
        subnetwork1.getSubstation(id);
    }
    for (auto& id : expectedSubstations2) {
        subnetwork2.getSubstation(id);
    }

    // Voltage levels
    auto expectedVls1 = {id("voltageLevel1", "1"),
                                            id("voltageLevel2", "1"), id("voltageLevel3", "1"),
                                            id("voltageLevel4", "1"), id("voltageLevel5", "1")};
    auto expectedVls2 = {id("voltageLevel1", "2"),
                                            id("voltageLevel2", "2"), id("voltageLevel3", "2"),
                                            id("voltageLevel4", "2"), id("voltageLevel5", "2")};

    BOOST_CHECK_EQUAL(expectedVls1.size(), subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(expectedVls2.size(), subnetwork2.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(expectedVls1.size(), boost::size(subnetwork1.getVoltageLevels()));
    BOOST_CHECK_EQUAL(expectedVls2.size(), boost::size(subnetwork2.getVoltageLevels()));
    for (auto& id : expectedVls1) {
        subnetwork1.getVoltageLevel(id);
    }
    for (auto& id : expectedVls2) {
        subnetwork2.getVoltageLevel(id);
    }

    // Generators
    auto expectedGens1 = {id("generator1", "1")};
    auto expectedGens2 = {id("generator1", "2")};

    BOOST_CHECK_EQUAL(expectedGens1.size(), subnetwork1.getGeneratorCount());
    BOOST_CHECK_EQUAL(expectedGens2.size(), subnetwork2.getGeneratorCount());
    BOOST_CHECK_EQUAL(expectedGens1.size(), boost::size(subnetwork1.getGenerators()));
    BOOST_CHECK_EQUAL(expectedGens2.size(), boost::size(subnetwork2.getGenerators()));
    for (auto& id : expectedGens1) {
        subnetwork1.getGenerator(id);
    }
    for (auto& id : expectedGens2) {
        subnetwork2.getGenerator(id);
    }

    // Loads
    auto expectedLoads1 = {id("load1", "1")};
    auto expectedLoads2 = {id("load1", "2")};

    BOOST_CHECK_EQUAL(expectedLoads1.size(), subnetwork1.getLoadCount());
    BOOST_CHECK_EQUAL(expectedLoads2.size(), subnetwork2.getLoadCount());
    BOOST_CHECK_EQUAL(expectedLoads1.size(), boost::size(subnetwork1.getLoads()));
    BOOST_CHECK_EQUAL(expectedLoads2.size(), boost::size(subnetwork2.getLoads()));
    for (auto& id : expectedLoads1) {
        subnetwork1.getLoad(id);
    }
    for (auto& id : expectedLoads2) {
        subnetwork2.getLoad(id);
    }

    // Batteries
    auto expectedIdsForSubnetwork1 = {id("battery1", "1")};
    auto expectedIdsForSubnetwork2 = {id("battery1", "2")};

    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork1.size(), subnetwork1.getBatteryCount());
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork2.size(), subnetwork2.getBatteryCount());
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork1.size(), boost::size(subnetwork1.getBatteries()));
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork2.size(), boost::size(subnetwork2.getBatteries()));
    for (auto& id : expectedIdsForSubnetwork1) {
        subnetwork1.getBattery(id);
    }
    for (auto& id : expectedIdsForSubnetwork2) {
        subnetwork2.getBattery(id);
    }

    // ShuntCompensators
    auto expectedShunts1 = {id("shuntCompensator1", "1")};
    auto expectedShunts2 = {id("shuntCompensator1", "2")};

    BOOST_CHECK_EQUAL(expectedShunts1.size(), subnetwork1.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(expectedShunts2.size(), subnetwork2.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(expectedShunts1.size(), boost::size(subnetwork1.getShuntCompensators()));
    BOOST_CHECK_EQUAL(expectedShunts2.size(), boost::size(subnetwork2.getShuntCompensators()));
    for (auto& id : expectedShunts1) {
        subnetwork1.getShuntCompensator(id);
    }
    for (auto& id : expectedShunts2) {
        subnetwork2.getShuntCompensator(id);
    }

    // StaticVarCompensators
    auto expectedSvc1 = {id("svc1", "1")};
    auto expectedSvc2 = {id("svc1", "2")};

    BOOST_CHECK_EQUAL(expectedSvc1.size(), subnetwork1.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(expectedSvc2.size(), subnetwork2.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(expectedSvc1.size(), boost::size(subnetwork1.getStaticVarCompensators()));
    BOOST_CHECK_EQUAL(expectedSvc2.size(), boost::size(subnetwork2.getStaticVarCompensators()));
    for (auto& id : expectedSvc1) {
        subnetwork1.getStaticVarCompensator(id);
    }
    for (auto& id : expectedSvc2) {
        subnetwork2.getStaticVarCompensator(id);
    }

    // BusbarSections
    auto expectedBbs1 = {id("voltageLevel1BusbarSection1", "1"), id("voltageLevel1BusbarSection2", "1")};
    auto expectedBbs2 = {id("voltageLevel1BusbarSection1", "2"), id("voltageLevel1BusbarSection2", "2")};

    BOOST_CHECK_EQUAL(expectedBbs1.size(), subnetwork1.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(expectedBbs2.size(), subnetwork2.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(expectedBbs1.size(), boost::size(subnetwork1.getBusbarSections()));
    BOOST_CHECK_EQUAL(expectedBbs2.size(), boost::size(subnetwork2.getBusbarSections()));
    for (auto& id : expectedBbs1) {
        subnetwork1.getBusbarSection(id);
    }
    for (auto& id : expectedBbs2) {
        subnetwork2.getBusbarSection(id);
    }

    // Switches
    auto expectedSwitches1 = {id("voltageLevel1Breaker1", "1"),
                                            id("load1Disconnector1", "1"),
                                            id("load1Breaker1", "1"),
                                            id("generator1Disconnector1", "1"),
                                            id("generator1Breaker1", "1")};
    auto expectedSwitches2 = {id("voltageLevel1Breaker1", "2"),
                                            id("load1Disconnector1", "2"),
                                            id("load1Breaker1", "2"),
                                            id("generator1Disconnector1", "2"),
                                            id("generator1Breaker1", "2")};

    BOOST_CHECK_EQUAL(expectedSwitches1.size(), subnetwork1.getSwitchCount());
    BOOST_CHECK_EQUAL(expectedSwitches2.size(), subnetwork2.getSwitchCount());
    BOOST_CHECK_EQUAL(expectedSwitches1.size(), boost::size(subnetwork1.getSwitches()));
    BOOST_CHECK_EQUAL(expectedSwitches2.size(), boost::size(subnetwork2.getSwitches()));
    for (auto& id : expectedSwitches1) {
        subnetwork1.getSwitch(id);
    }
    for (auto& id : expectedSwitches2) {
        subnetwork2.getSwitch(id);
    }

    // HVDC converter stations
    auto lccIdsForSubnetwork1 = {id("lcc1", "1"), id("lcc2", "1")};
    auto lccIdsForSubnetwork2 = {id("lcc1", "2"), id("lcc2", "2")};
    BOOST_CHECK_EQUAL(lccIdsForSubnetwork1.size(), subnetwork1.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(lccIdsForSubnetwork2.size(), subnetwork2.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(lccIdsForSubnetwork1.size(), boost::size(subnetwork1.getLccConverterStations()));
    BOOST_CHECK_EQUAL(lccIdsForSubnetwork2.size(), boost::size(subnetwork2.getLccConverterStations()));
    for (auto& id : lccIdsForSubnetwork1) {
        subnetwork1.getLccConverterStation(id);
    }
    for (auto& id : lccIdsForSubnetwork2) {
        subnetwork2.getLccConverterStation(id);
    }

    auto vscIdsForSubnetwork1 = {id("vsc1", "1"), id("vsc2", "1")};
    auto vscIdsForSubnetwork2 = {id("vsc1", "2"), id("vsc2", "2")};
    BOOST_CHECK_EQUAL(vscIdsForSubnetwork1.size(), subnetwork1.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(vscIdsForSubnetwork2.size(), subnetwork2.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(vscIdsForSubnetwork1.size(), boost::size(subnetwork1.getVscConverterStations()));
    BOOST_CHECK_EQUAL(vscIdsForSubnetwork2.size(), boost::size(subnetwork2.getVscConverterStations()));
    for (auto& id : vscIdsForSubnetwork1) {
        subnetwork1.getVscConverterStation(id);
    }
    for (auto& id : vscIdsForSubnetwork2) {
        subnetwork2.getVscConverterStation(id);
    }

    auto hvdcConvertersForSubnetwork1 = {id("lcc1", "1"), id("lcc2", "1"), id("vsc1", "1"), id("vsc2", "1")};
    auto hvdcConvertersForSubnetwork2 = {id("lcc1", "2"), id("lcc2", "2"), id("vsc1", "2"), id("vsc2", "2")};
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork1.size(), subnetwork1.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork2.size(), subnetwork2.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork1.size(), boost::size(subnetwork1.getHvdcConverterStations()));
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork2.size(), boost::size(subnetwork2.getHvdcConverterStations()));
    for (auto& id : hvdcConvertersForSubnetwork1) {
        subnetwork1.getHvdcConverterStation(id);
    }
    for (auto& id : hvdcConvertersForSubnetwork2) {
        subnetwork2.getHvdcConverterStation(id);
    }

    // HVDC Lines
    auto expectedhvdclines1 = {id("hvdcLine1", "1"), id("hvdcLine2", "1")};
    auto expectedhvdclines2 = {id("hvdcLine1", "2"), id("hvdcLine2", "2")};
    BOOST_CHECK_EQUAL(expectedhvdclines1.size(), subnetwork1.getHvdcLineCount());
    BOOST_CHECK_EQUAL(expectedhvdclines2.size(), subnetwork2.getHvdcLineCount());
    BOOST_CHECK_EQUAL(expectedhvdclines1.size(), boost::size(subnetwork1.getHvdcLines()));
    BOOST_CHECK_EQUAL(expectedhvdclines2.size(), boost::size(subnetwork2.getHvdcLines()));
    for (auto& id : expectedhvdclines1) {
        subnetwork1.getHvdcLine(id);
    }
    for (auto& id : expectedhvdclines2) {
        subnetwork2.getHvdcLine(id);
    }

    HvdcConverterStation& converterlcc1 = network.getLccConverterStation(id("lcc1", "1"));
    BOOST_CHECK_EQUAL(id("hvdcLine1", "1"), network.getHvdcLine(converterlcc1).getId());
    BOOST_CHECK_EQUAL(id("hvdcLine1", "1"), subnetwork1.getHvdcLine(converterlcc1).getId());
    BOOST_CHECK(!subnetwork2.findHvdcLine(converterlcc1));

    HvdcConverterStation& convertervsc2 = network.getVscConverterStation(id("vsc2", "1"));
    BOOST_CHECK_EQUAL(id("hvdcLine2", "1"), network.getHvdcLine(convertervsc2).getId());
    BOOST_CHECK_EQUAL(id("hvdcLine2", "1"), subnetwork1.getHvdcLine(convertervsc2).getId());
    BOOST_CHECK(!subnetwork2.findHvdcLine(convertervsc2));

    HvdcConverterStation& convertervsc1 = network.getVscConverterStation(id("vsc1", "2"));
    BOOST_CHECK_EQUAL(id("hvdcLine2", "2"), network.getHvdcLine(convertervsc1).getId());
    BOOST_CHECK_EQUAL(id("hvdcLine2", "2"), subnetwork2.getHvdcLine(convertervsc1).getId());
    BOOST_CHECK(!subnetwork1.findHvdcLine(convertervsc1));

    HvdcConverterStation& converterlcc2 = network.getLccConverterStation(id("lcc2", "2"));
    BOOST_CHECK_EQUAL(id("hvdcLine1", "2"), network.getHvdcLine(converterlcc2).getId());
    BOOST_CHECK_EQUAL(id("hvdcLine1", "2"), subnetwork2.getHvdcLine(converterlcc2).getId());
    BOOST_CHECK(!subnetwork1.findHvdcLine(converterlcc2));

    // 3WT
    auto expected3wt1 = {id("threeWindingsTransformer1", "1")};
    auto expected3wt2 = {id("threeWindingsTransformer1", "2")};
    BOOST_CHECK_EQUAL(expected3wt1.size(), subnetwork1.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected3wt2.size(), subnetwork2.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected3wt1.size(), boost::size(subnetwork1.getThreeWindingsTransformers()));
    BOOST_CHECK_EQUAL(expected3wt2.size(), boost::size(subnetwork2.getThreeWindingsTransformers()));
    for (auto& id : expected3wt1) {
        subnetwork1.getThreeWindingsTransformer(id);
    }
    for (auto& id : expected3wt2) {
        subnetwork2.getThreeWindingsTransformer(id);
    }

    // 2WT
    auto expected2wt1 = {id("twoWindingsTransformer1", "1")};
    auto expected2wt2 = {id("twoWindingsTransformer1", "2")};
    BOOST_CHECK_EQUAL(expected2wt1.size(), subnetwork1.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected2wt2.size(), subnetwork2.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected2wt1.size(), boost::size(subnetwork1.getTwoWindingsTransformers()));
    BOOST_CHECK_EQUAL(expected2wt2.size(), boost::size(subnetwork2.getTwoWindingsTransformers()));
    for (auto& id : expected2wt1) {
        subnetwork1.getTwoWindingsTransformer(id);
    }
    for (auto& id : expected2wt2) {
        subnetwork2.getTwoWindingsTransformer(id);
    }

    // Lines
    auto expectedLines1 = {id("line1", "1")};
    auto expectedLines2 = {id("line1", "2")};
    BOOST_CHECK_EQUAL(expectedLines1.size(), subnetwork1.getLineCount());
    BOOST_CHECK_EQUAL(expectedLines2.size(), subnetwork2.getLineCount());
    BOOST_CHECK_EQUAL(expectedLines1.size(), boost::size(subnetwork1.getLines()));
    BOOST_CHECK_EQUAL(expectedLines2.size(), boost::size(subnetwork2.getLines()));
    for (auto& id : expectedLines1) {
        subnetwork1.getLine(id);
    }
    for (auto& id : expectedLines2) {
        subnetwork2.getLine(id);
    }

    // DanglingLines
    auto expectedDLines1 = {id("danglingLine1", "1"), id("danglingLine2", "1"), id("danglingLine3", "1")};
    auto expectedDLines2 = {id("danglingLine1", "2"), id("danglingLine2", "2"), id("danglingLine3", "2")};
    BOOST_CHECK_EQUAL(expectedDLines1.size(), subnetwork1.getDanglingLineCount());
    BOOST_CHECK_EQUAL(expectedDLines2.size(), subnetwork2.getDanglingLineCount());
    BOOST_CHECK_EQUAL(expectedDLines1.size(), boost::size(subnetwork1.getDanglingLines()));
    BOOST_CHECK_EQUAL(expectedDLines2.size(), boost::size(subnetwork2.getDanglingLines()));
    for (auto& id : expectedDLines1) {
        subnetwork1.getDanglingLine(id);
    }
    for (auto& id : expectedDLines2) {
        subnetwork2.getDanglingLine(id);
    }

    // TieLines
    auto expectedTieLine0= {id("tieLine3", "0"), id("tieLine1", "1"), id("tieLine1", "2")};
    auto expectedTieLine1 = {id("tieLine1", "1")};
    auto expectedTieLine2 = {id("tieLine1", "2")};
    BOOST_CHECK_EQUAL(expectedTieLine0.size(), network.getTieLineCount());
    BOOST_CHECK_EQUAL(expectedTieLine1.size(), subnetwork1.getTieLineCount());
    BOOST_CHECK_EQUAL(expectedTieLine2.size(), subnetwork2.getTieLineCount());
    BOOST_CHECK_EQUAL(expectedTieLine0.size(), boost::size(network.getTieLines()));
    BOOST_CHECK_EQUAL(expectedTieLine1.size(), boost::size(subnetwork1.getTieLines()));
    BOOST_CHECK_EQUAL(expectedTieLine2.size(), boost::size(subnetwork2.getTieLines()));
    for (auto& id : expectedTieLine0) {
        network.getTieLine(id);
    }
    for (auto& id : expectedTieLine1) {
        subnetwork1.getTieLine(id);
    }
    for (auto& id : expectedTieLine2) {
        subnetwork2.getTieLine(id);
    }

    // Branches
    auto expectedBranches0 = {id("tieLine3", "0"), 
                                id("line1", "1"), id("twoWindingsTransformer1", "1"), id("tieLine1", "1"),
                                id("line1", "2"), id("twoWindingsTransformer1", "2"), id("tieLine1", "2")};
    auto expectedBranches1 = {id("line1", "1"), id("twoWindingsTransformer1", "1"), id("tieLine1", "1")};
    auto expectedBranches2 = {id("line1", "2"), id("twoWindingsTransformer1", "2"), id("tieLine1", "2")};
    BOOST_CHECK_EQUAL(expectedBranches0.size(), network.getBranchCount());
    BOOST_CHECK_EQUAL(expectedBranches1.size(), subnetwork1.getBranchCount());
    BOOST_CHECK_EQUAL(expectedBranches2.size(), subnetwork2.getBranchCount());
    BOOST_CHECK_EQUAL(expectedBranches0.size(), boost::size(network.getBranches()));
    BOOST_CHECK_EQUAL(expectedBranches1.size(), boost::size(subnetwork1.getBranches()));
    BOOST_CHECK_EQUAL(expectedBranches2.size(), boost::size(subnetwork2.getBranches()));
    for (auto& id : expectedBranches0) {
        network.getBranch(id);
    }
    for (auto& id : expectedBranches1) {
        subnetwork1.getBranch(id);
    }
    for (auto& id : expectedBranches2) {
        subnetwork2.getBranch(id);
    }

    // OverloadManagementSystem
    auto expectedOMS0 = {id("overloadManagementSystem", "1"), id("overloadManagementSystem", "2")};
    auto expectedOMS1 = {id("overloadManagementSystem", "1")};
    auto expectedOMS2 = {id("overloadManagementSystem", "2")};
    BOOST_CHECK_EQUAL(expectedOMS0.size(), network.getOverloadManagementSystemCount());
    BOOST_CHECK_EQUAL(expectedOMS1.size(), subnetwork1.getOverloadManagementSystemCount());
    BOOST_CHECK_EQUAL(expectedOMS2.size(), subnetwork2.getOverloadManagementSystemCount());
    BOOST_CHECK_EQUAL(expectedOMS0.size(), boost::size(network.getOverloadManagementSystems()));
    BOOST_CHECK_EQUAL(expectedOMS1.size(), boost::size(subnetwork1.getOverloadManagementSystems()));
    BOOST_CHECK_EQUAL(expectedOMS2.size(), boost::size(subnetwork2.getOverloadManagementSystems()));
    for (auto& id : expectedOMS0) {
        network.getOverloadManagementSystem(id);
    }
    for (auto& id : expectedOMS1) {
        subnetwork1.getOverloadManagementSystem(id);
    }
    for (auto& id : expectedOMS2) {
        subnetwork2.getOverloadManagementSystem(id);
    }

    //Areas
    auto expectedAreas0 = {id("area1", "1"), id("area1", "2")};
    auto expectedAreas1 = {id("area1", "1")};
    auto expectedAreas2 = {id("area1", "2")};
    BOOST_CHECK_EQUAL(expectedAreas0.size(), network.getAreaCount());
    BOOST_CHECK_EQUAL(expectedAreas1.size(), subnetwork1.getAreaCount());
    BOOST_CHECK_EQUAL(expectedAreas2.size(), subnetwork2.getAreaCount());
    BOOST_CHECK_EQUAL(expectedAreas0.size(), boost::size(network.getAreas()));
    BOOST_CHECK_EQUAL(expectedAreas1.size(), boost::size(subnetwork1.getAreas()));
    BOOST_CHECK_EQUAL(expectedAreas2.size(), boost::size(subnetwork2.getAreas()));
    
    for (auto& id : expectedAreas0) {
        network.getArea(id);
    }
    for (auto& id : expectedAreas1) {
        subnetwork1.getArea(id);
    }
    for (auto& id : expectedAreas2) {
        subnetwork2.getArea(id);
    }

    //AreaTypes
    auto expectedAreaTypes0 = {id("areaType1", "1"), id("areaType1", "2")};
    auto expectedAreaTypes1 = {id("areaType1", "1")};
    auto expectedAreaTypes2 = {id("areaType1", "2")};
    BOOST_CHECK_EQUAL(expectedAreaTypes0.size(), network.getAreaTypeCount());
    BOOST_CHECK_EQUAL(expectedAreaTypes1.size(), subnetwork1.getAreaTypeCount());
    BOOST_CHECK_EQUAL(expectedAreaTypes2.size(), subnetwork2.getAreaTypeCount());
    BOOST_CHECK_EQUAL(expectedAreaTypes0.size(), boost::size(network.getAreaTypes()));
    BOOST_CHECK_EQUAL(expectedAreaTypes1.size(), boost::size(subnetwork1.getAreaTypes()));
    BOOST_CHECK_EQUAL(expectedAreaTypes2.size(), boost::size(subnetwork2.getAreaTypes()));

    // Connectables are retrieved from the root network even when called from a subnetwork
    std::set<std::string> expectedConnectables = {id("battery1", "1"), 
                                            id("voltageLevel1BusbarSection1", "1"), id("voltageLevel1BusbarSection2", "1"),
                                            id("generator1", "1"),
                                            id("lcc1", "1"), id("lcc2", "1"),
                                            id("line1", "1"),
                                            id("load1", "1"),
                                            id("shuntCompensator1", "1"),
                                            id("svc1", "1"),
                                            id("vsc1", "1"), id("vsc2", "1"),
                                            id("threeWindingsTransformer1", "1"),
                                            id("twoWindingsTransformer1", "1"),
                                            id("danglingLine1", "1"), id("danglingLine2", "1"), id("danglingLine3", "1"),
                                id("battery1", "2"),
                                            id("voltageLevel1BusbarSection1", "2"), id("voltageLevel1BusbarSection2", "2"),
                                            id("generator1", "2"),
                                            id("lcc1", "2"), id("lcc2", "2"),
                                            id("line1", "2"),
                                            id("load1", "2"),
                                            id("shuntCompensator1", "2"),
                                            id("svc1", "2"),
                                            id("vsc1", "2"), id("vsc2", "2"),
                                            id("threeWindingsTransformer1", "2"),
                                            id("twoWindingsTransformer1", "2"),
                                            id("danglingLine1", "2"), id("danglingLine2", "2"), id("danglingLine3", "2")};

    BOOST_CHECK_EQUAL(expectedConnectables.size(), subnetwork1.getConnectableCount());
    BOOST_CHECK_EQUAL(expectedConnectables.size(), subnetwork2.getConnectableCount());
    BOOST_CHECK_EQUAL(expectedConnectables.size(), boost::size(subnetwork1.getConnectables()));
    BOOST_CHECK_EQUAL(expectedConnectables.size(), boost::size(subnetwork2.getConnectables()));
    std::set<std::string> connectablesId1;
    for (auto& c : subnetwork1.getConnectables()){
        connectablesId1.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectables.begin(), expectedConnectables.end(), connectablesId1.begin(), connectablesId1.end());
    std::set<std::string> connectablesId2;
    for (auto& c : subnetwork2.getConnectables()){
        connectablesId2.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectables.begin(), expectedConnectables.end(), connectablesId2.begin(), connectablesId2.end());


    std::set<std::string> expectedConnectableBattery = {id("battery1", "1"), id("battery1", "2")};
    BOOST_CHECK_EQUAL(expectedConnectableBattery.size(), subnetwork1.getConnectableCount<Battery>());
    BOOST_CHECK_EQUAL(expectedConnectableBattery.size(), subnetwork2.getConnectableCount<Battery>());
    BOOST_CHECK_EQUAL(expectedConnectableBattery.size(), boost::size(subnetwork1.getConnectables<Battery>()));
    BOOST_CHECK_EQUAL(expectedConnectableBattery.size(), boost::size(subnetwork2.getConnectables<Battery>()));
    std::set<std::string> connectablesBatId1;
    for (auto& c : subnetwork1.getConnectables<Battery>()){
        connectablesBatId1.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectableBattery.begin(), expectedConnectableBattery.end(), connectablesBatId1.begin(), connectablesBatId1.end());
    std::set<std::string> connectablesBatId2;
    for (auto& c : subnetwork2.getConnectables<Battery>()){
        connectablesBatId2.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectableBattery.begin(), expectedConnectableBattery.end(), connectablesBatId2.begin(), connectablesBatId2.end());

    // Identifiables
    std::set<std::string> expectedIdentifiables1 = {"n1_area1","n1_battery1","n1_danglingLine1","n1_danglingLine2","n1_danglingLine3",
        "n1_generator1","n1_generator1Breaker1","n1_generator1Disconnector1","n1_hvdcLine1","n1_hvdcLine2",
        "n1_lcc1","n1_lcc2","n1_line1","n1_load1","n1_load1Breaker1","n1_load1Disconnector1",
        "n1_network","n1_overloadManagementSystem","n1_shuntCompensator1","n1_substation1","n1_substation2","n1_substation3",
        "n1_svc1","n1_threeWindingsTransformer1","n1_tieLine1","n1_twoWindingsTransformer1",
        "n1_voltageLevel1","n1_voltageLevel1Breaker1","n1_voltageLevel1BusbarSection1","n1_voltageLevel1BusbarSection2",
        "n1_voltageLevel2","n1_voltageLevel3","n1_voltageLevel4","n1_voltageLevel5","n1_vsc1","n1_vsc2"};
    std::set<std::string> expectedIdentifiables2 = {"n2_area1","n2_battery1","n2_danglingLine1","n2_danglingLine2","n2_danglingLine3",
        "n2_generator1","n2_generator1Breaker1","n2_generator1Disconnector1","n2_hvdcLine1","n2_hvdcLine2",
        "n2_lcc1","n2_lcc2","n2_line1","n2_load1","n2_load1Breaker1","n2_load1Disconnector1",
        "n2_network","n2_overloadManagementSystem","n2_shuntCompensator1","n2_substation1","n2_substation2","n2_substation3",
        "n2_svc1","n2_threeWindingsTransformer1","n2_tieLine1","n2_twoWindingsTransformer1",
        "n2_voltageLevel1","n2_voltageLevel1Breaker1","n2_voltageLevel1BusbarSection1","n2_voltageLevel1BusbarSection2",
        "n2_voltageLevel2","n2_voltageLevel3","n2_voltageLevel4","n2_voltageLevel5","n2_vsc1","n2_vsc2"};
    std::set<std::string> expectedIdentifiables0 = {"Root", "n0_tieLine3"};
    for (auto& i : expectedIdentifiables1){
        expectedIdentifiables0.emplace(i);
    }
    for (auto& i : expectedIdentifiables2){
        expectedIdentifiables0.emplace(i);
    }

    BOOST_CHECK_EQUAL(expectedIdentifiables0.size(), boost::size(network.getIdentifiables()));
    BOOST_CHECK_EQUAL(expectedIdentifiables1.size(), boost::size(subnetwork1.getIdentifiables()));
    BOOST_CHECK_EQUAL(expectedIdentifiables2.size(), boost::size(subnetwork2.getIdentifiables()));
    for (auto& i : expectedIdentifiables0){
        network.getIdentifiable(i);
    }
    for (auto& i : expectedIdentifiables1){
        subnetwork1.getIdentifiable(i);
    }
    for (auto& i : expectedIdentifiables2){
        subnetwork2.getIdentifiable(i);
    }
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
