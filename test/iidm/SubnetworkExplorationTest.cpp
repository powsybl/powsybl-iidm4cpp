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
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcLineAdder.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcSwitchAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
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
#include <powsybl/iidm/VoltageSourceConverter.hpp>
#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>
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
        .setRegulating(true)
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
    voltageLevel1.newBoundaryLine()
        .setId(id("boundaryLine1", nid))
        .setNode(15)
        .setR(1.0)
        .setX(0.1)
        .setG(0.0)
        .setB(0.001)
        .setP0(10)
        .setQ0(1)
        .add();
    voltageLevel2.newBoundaryLine()
        .setId(id("boundaryLine2", nid))
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
        .setBoundaryLine1(id("boundaryLine1", nid))
        .setBoundaryLine2(id("boundaryLine2", nid))
        .add();
    voltageLevel3.newBoundaryLine()
        .setId(id("boundaryLine3", nid))
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

    DcNode& dcNode1 = network.newDcNode()
                            .setId(id("dcNode1", nid))
                            .setNominalV(500.0)
                            .add();
    DcNode& dcNode2 = network.newDcNode()
                            .setId(id("dcNode2", nid))
                            .setNominalV(500.0)
                            .add();
    network.newDcSwitch()
                .setId(id("dcSwitch1", nid))
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setOpen(false)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .add();
    network.newDcGround()
                .setId(id("dcGround1", nid))
                .setDcNode(dcNode1.getId())
                .setR(0.01)
                .add();
    network.newDcLine()
                .setId(id("dcLine1", nid))
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setR(1.0)
                .add();
    voltageLevel1.newLineCommutatedConverter()
                .setId(id("lccDetailed1", nid))
                .setControlMode(AcDcConverter::ControlMode::V_DC)
                .setNode1(16)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setTargetP(0.)
                .setTargetVdc(500.)
                .add();
    voltageLevel1.newVoltageSourceConverter()
                .setId(id("vscDetailed1", nid))
                .setControlMode(AcDcConverter::ControlMode::V_DC)
                .setNode1(17)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setTargetP(0.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.)
                .add();

    return;
}

template <typename T>
void assertCollection(const std::set<T>& expectedSet, const std::set<T>& resultSet) {
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedSet.begin(), expectedSet.end(), resultSet.begin(), resultSet.end());
}

BOOST_AUTO_TEST_SUITE(SubnetworkExplorationTestSuite)

template <typename T>
std::string mapId(const T& identifiable) {
    return identifiable.getId();
}

BOOST_AUTO_TEST_CASE(SubnetworkExplorationTest) {
    Network network("Root", "Root");
    Network& subnetwork1 = network.newSubnetwork("n1_network", "n1_format");
    Network& subnetwork2 = network.newSubnetwork("n2_network", "n2_format");

    CreateSubnetworkExploreTest(subnetwork1, "1", Country::ES);
    CreateSubnetworkExploreTest(subnetwork2, "2", Country::BE);

    //Create Tie line on boundary lines from each subnetworks
    network.newTieLine()
        .setId(id("tieLine3", "0"))
        .setBoundaryLine1(id("boundaryLine3", "1"))
        .setBoundaryLine2(id("boundaryLine3", "2"))
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

    auto expectedNetworksRoot = {std::string("Root"), id("network", "1"), id("network", "2")};
    auto expectedNetworks1 = {id("network", "1")};
    auto expectedNetworks2 = {id("network", "2")};
    const auto& networksRoot = network.getIdentifiables(IdentifiableType::NETWORK) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& networks1 = subnetwork1.getIdentifiables(IdentifiableType::NETWORK) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& networks2 = subnetwork2.getIdentifiables(IdentifiableType::NETWORK) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL_COLLECTIONS(networksRoot.begin(), networksRoot.end(), expectedNetworksRoot.begin(), expectedNetworksRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(networks1.begin(), networks1.end(), expectedNetworks1.begin(), expectedNetworks1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(networks2.begin(), networks2.end(), expectedNetworks2.begin(), expectedNetworks2.end());

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
    auto expectedSubstations = {id("substation1", "1"), id("substation2", "1"), id("substation3", "1"),
                                id("substation1", "2"), id("substation2", "2"), id("substation3", "2")};
    auto expectedSubstations1 = {id("substation1", "1"), id("substation2", "1"), id("substation3", "1")};
    auto expectedSubstations2 = {id("substation1", "2"), id("substation2", "2"), id("substation3", "2")};
    BOOST_CHECK_EQUAL(expectedSubstations.size(), network.getSubstationCount());
    BOOST_CHECK_EQUAL(expectedSubstations1.size(), subnetwork1.getSubstationCount());
    BOOST_CHECK_EQUAL(expectedSubstations2.size(), subnetwork2.getSubstationCount());
    const auto& substationsRoot = network.getSubstations() | boost::adaptors::transformed(mapId<Substation>);
    const auto& substations1 = subnetwork1.getSubstations() | boost::adaptors::transformed(mapId<Substation>);
    const auto& substations2 = subnetwork2.getSubstations() | boost::adaptors::transformed(mapId<Substation>);
    const auto& substationsIdRoot = network.getIdentifiables(IdentifiableType::SUBSTATION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& substationsId1 = subnetwork1.getIdentifiables(IdentifiableType::SUBSTATION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& substationsId2 = subnetwork2.getIdentifiables(IdentifiableType::SUBSTATION) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedSubstations.size(), boost::size(substationsRoot));
    BOOST_CHECK_EQUAL(expectedSubstations1.size(), boost::size(substations1));
    BOOST_CHECK_EQUAL(expectedSubstations2.size(), boost::size(substations2));
    BOOST_CHECK_EQUAL_COLLECTIONS(substationsRoot.begin(), substationsRoot.end(), substationsIdRoot.begin(), substationsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(substationsRoot.begin(), substationsRoot.end(), expectedSubstations.begin(), expectedSubstations.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(substations1.begin(), substations1.end(), substationsId1.begin(), substationsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(substations1.begin(), substations1.end(), expectedSubstations1.begin(), expectedSubstations1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(substations2.begin(), substations2.end(), substationsId2.begin(), substationsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(substations2.begin(), substations2.end(), expectedSubstations2.begin(), expectedSubstations2.end());
    for (auto& id : expectedSubstations1) {
        subnetwork1.getSubstation(id);
    }
    for (auto& id : expectedSubstations2) {
        subnetwork2.getSubstation(id);
    }

    // Voltage levels
    auto expectedVlsRoot = {id("voltageLevel1", "1"), id("voltageLevel2", "1"), id("voltageLevel3", "1"), id("voltageLevel4", "1"), id("voltageLevel5", "1"),
                                  id("voltageLevel1", "2"), id("voltageLevel2", "2"), id("voltageLevel3", "2"), id("voltageLevel4", "2"), id("voltageLevel5", "2")};
    auto expectedVls1 = {id("voltageLevel1", "1"), id("voltageLevel2", "1"), id("voltageLevel3", "1"), id("voltageLevel4", "1"), id("voltageLevel5", "1")};
    auto expectedVls2 = {id("voltageLevel1", "2"), id("voltageLevel2", "2"), id("voltageLevel3", "2"), id("voltageLevel4", "2"), id("voltageLevel5", "2")};

    BOOST_CHECK_EQUAL(expectedVlsRoot.size(), network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(expectedVls1.size(), subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(expectedVls2.size(), subnetwork2.getVoltageLevelCount());
    const auto& vlsRoot = network.getVoltageLevels() | boost::adaptors::transformed(mapId<VoltageLevel>);
    const auto& vls1 = subnetwork1.getVoltageLevels() | boost::adaptors::transformed(mapId<VoltageLevel>);
    const auto& vls2 = subnetwork2.getVoltageLevels() | boost::adaptors::transformed(mapId<VoltageLevel>);
    const auto& vlsIdRoot = network.getIdentifiables(IdentifiableType::VOLTAGE_LEVEL) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& vlsId1 = subnetwork1.getIdentifiables(IdentifiableType::VOLTAGE_LEVEL) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& vlsId2 = subnetwork2.getIdentifiables(IdentifiableType::VOLTAGE_LEVEL) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedVlsRoot.size(), boost::size(vlsRoot));
    BOOST_CHECK_EQUAL(expectedVls1.size(), boost::size(vls1));
    BOOST_CHECK_EQUAL(expectedVls2.size(), boost::size(vls2));
    BOOST_CHECK_EQUAL_COLLECTIONS(vlsRoot.begin(), vlsRoot.end(), vlsIdRoot.begin(), vlsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vlsRoot.begin(), vlsRoot.end(), expectedVlsRoot.begin(), expectedVlsRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vls1.begin(), vls1.end(), vlsId1.begin(), vlsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vls1.begin(), vls1.end(), expectedVls1.begin(), expectedVls1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vls2.begin(), vls2.end(), vlsId2.begin(), vlsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vls2.begin(), vls2.end(), expectedVls2.begin(), expectedVls2.end());
    for (auto& id : expectedVls1) {
        subnetwork1.getVoltageLevel(id);
    }
    for (auto& id : expectedVls2) {
        subnetwork2.getVoltageLevel(id);
    }

    // Generators
    auto expectedGensRoot = {id("generator1", "1"), id("generator1", "2")};
    auto expectedGens1 = {id("generator1", "1")};
    auto expectedGens2 = {id("generator1", "2")};
    BOOST_CHECK_EQUAL(expectedGensRoot.size(), network.getGeneratorCount());
    BOOST_CHECK_EQUAL(expectedGens1.size(), subnetwork1.getGeneratorCount());
    BOOST_CHECK_EQUAL(expectedGens2.size(), subnetwork2.getGeneratorCount());
    const auto& gensRoot = network.getGenerators() | boost::adaptors::transformed(mapId<Generator>);
    const auto& gens1 = subnetwork1.getGenerators() | boost::adaptors::transformed(mapId<Generator>);
    const auto& gens2 = subnetwork2.getGenerators() | boost::adaptors::transformed(mapId<Generator>);
    const auto& gensIdRoot = network.getIdentifiables(IdentifiableType::GENERATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& gensId1 = subnetwork1.getIdentifiables(IdentifiableType::GENERATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& gensId2 = subnetwork2.getIdentifiables(IdentifiableType::GENERATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedGensRoot.size(), boost::size(gensRoot));
    BOOST_CHECK_EQUAL(expectedGens1.size(), boost::size(gens1));
    BOOST_CHECK_EQUAL(expectedGens2.size(), boost::size(gens2));
    BOOST_CHECK_EQUAL_COLLECTIONS(gensRoot.begin(), gensRoot.end(), gensIdRoot.begin(), gensIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(gensRoot.begin(), gensRoot.end(), expectedGensRoot.begin(), expectedGensRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(gens1.begin(), gens1.end(), gensId1.begin(), gensId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(gens1.begin(), gens1.end(), expectedGens1.begin(), expectedGens1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(gens2.begin(), gens2.end(), gensId2.begin(), gensId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(gens2.begin(), gens2.end(), expectedGens2.begin(), expectedGens2.end());
    for (auto& id : expectedGens1) {
        subnetwork1.getGenerator(id);
    }
    for (auto& id : expectedGens2) {
        subnetwork2.getGenerator(id);
    }

    // Loads
    auto expectedLoadsRoot = {id("load1", "1"), id("load1", "2")};
    auto expectedLoads1 = {id("load1", "1")};
    auto expectedLoads2 = {id("load1", "2")};
    BOOST_CHECK_EQUAL(expectedLoadsRoot.size(), network.getLoadCount());
    BOOST_CHECK_EQUAL(expectedLoads1.size(), subnetwork1.getLoadCount());
    BOOST_CHECK_EQUAL(expectedLoads2.size(), subnetwork2.getLoadCount());
    const auto& loadsRoot = network.getLoads() | boost::adaptors::transformed(mapId<Load>);
    const auto& loads1 = subnetwork1.getLoads() | boost::adaptors::transformed(mapId<Load>);
    const auto& loads2 = subnetwork2.getLoads() | boost::adaptors::transformed(mapId<Load>);
    const auto& loadsIdRoot = network.getIdentifiables(IdentifiableType::LOAD) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& loadsId1 = subnetwork1.getIdentifiables(IdentifiableType::LOAD) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& loadsId2 = subnetwork2.getIdentifiables(IdentifiableType::LOAD) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedLoadsRoot.size(), boost::size(loadsRoot));
    BOOST_CHECK_EQUAL(expectedLoads1.size(), boost::size(loads1));
    BOOST_CHECK_EQUAL(expectedLoads2.size(), boost::size(loads2));
    BOOST_CHECK_EQUAL_COLLECTIONS(loadsRoot.begin(), loadsRoot.end(), loadsIdRoot.begin(), loadsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(loadsRoot.begin(), loadsRoot.end(), expectedLoadsRoot.begin(), expectedLoadsRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(loads1.begin(), loads1.end(), loadsId1.begin(), loadsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(loads1.begin(), loads1.end(), expectedLoads1.begin(), expectedLoads1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(loads2.begin(), loads2.end(), loadsId2.begin(), loadsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(loads2.begin(), loads2.end(), expectedLoads2.begin(), expectedLoads2.end());
    for (auto& id : expectedLoads1) {
        subnetwork1.getLoad(id);
    }
    for (auto& id : expectedLoads2) {
        subnetwork2.getLoad(id);
    }

    // Batteries
    auto expectedIdsForRoot = {id("battery1", "1"), id("battery1", "2")};
    auto expectedIdsForSubnetwork1 = {id("battery1", "1")};
    auto expectedIdsForSubnetwork2 = {id("battery1", "2")};
    BOOST_CHECK_EQUAL(expectedIdsForRoot.size(), network.getBatteryCount());
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork1.size(), subnetwork1.getBatteryCount());
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork2.size(), subnetwork2.getBatteryCount());
    const auto& batteriesRoot = network.getBatteries() | boost::adaptors::transformed(mapId<Battery>);
    const auto& batteries1 = subnetwork1.getBatteries() | boost::adaptors::transformed(mapId<Battery>);
    const auto& batteries2 = subnetwork2.getBatteries() | boost::adaptors::transformed(mapId<Battery>);
    const auto& batteriesIdRoot = network.getIdentifiables(IdentifiableType::BATTERY) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& batteriesId1 = subnetwork1.getIdentifiables(IdentifiableType::BATTERY) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& batteriesId2 = subnetwork2.getIdentifiables(IdentifiableType::BATTERY) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedIdsForRoot.size(), boost::size(batteriesRoot));
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork1.size(), boost::size(batteries1));
    BOOST_CHECK_EQUAL(expectedIdsForSubnetwork2.size(), boost::size(batteries2));
    BOOST_CHECK_EQUAL_COLLECTIONS(batteriesRoot.begin(), batteriesRoot.end(), batteriesIdRoot.begin(), batteriesIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(batteriesRoot.begin(), batteriesRoot.end(), expectedIdsForRoot.begin(), expectedIdsForRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(batteries1.begin(), batteries1.end(), batteriesId1.begin(), batteriesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(batteries1.begin(), batteries1.end(), expectedIdsForSubnetwork1.begin(), expectedIdsForSubnetwork1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(batteries2.begin(), batteries2.end(), batteriesId2.begin(), batteriesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(batteries2.begin(), batteries2.end(), expectedIdsForSubnetwork2.begin(), expectedIdsForSubnetwork2.end());
    for (auto& id : expectedIdsForSubnetwork1) {
        subnetwork1.getBattery(id);
    }
    for (auto& id : expectedIdsForSubnetwork2) {
        subnetwork2.getBattery(id);
    }

    // ShuntCompensators
    auto expectedShuntsRoot = {id("shuntCompensator1", "1"), id("shuntCompensator1", "2")};
    auto expectedShunts1 = {id("shuntCompensator1", "1")};
    auto expectedShunts2 = {id("shuntCompensator1", "2")};
    BOOST_CHECK_EQUAL(expectedShuntsRoot.size(), network.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(expectedShunts1.size(), subnetwork1.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(expectedShunts2.size(), subnetwork2.getShuntCompensatorCount());
    const auto& shuntsRoot = network.getShuntCompensators() | boost::adaptors::transformed(mapId<ShuntCompensator>);
    const auto& shunts1 = subnetwork1.getShuntCompensators() | boost::adaptors::transformed(mapId<ShuntCompensator>);
    const auto& shunts2 = subnetwork2.getShuntCompensators() | boost::adaptors::transformed(mapId<ShuntCompensator>);
    const auto& shuntsIdRoot = network.getIdentifiables(IdentifiableType::SHUNT_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& shuntsId1 = subnetwork1.getIdentifiables(IdentifiableType::SHUNT_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& shuntsId2 = subnetwork2.getIdentifiables(IdentifiableType::SHUNT_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedShuntsRoot.size(), boost::size(shuntsRoot));
    BOOST_CHECK_EQUAL(expectedShunts1.size(), boost::size(shunts1));
    BOOST_CHECK_EQUAL(expectedShunts2.size(), boost::size(shunts2));
    BOOST_CHECK_EQUAL_COLLECTIONS(shuntsRoot.begin(), shuntsRoot.end(), shuntsIdRoot.begin(), shuntsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(shuntsRoot.begin(), shuntsRoot.end(), expectedShuntsRoot.begin(), expectedShuntsRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(shunts1.begin(), shunts1.end(), shuntsId1.begin(), shuntsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(shunts1.begin(), shunts1.end(), expectedShunts1.begin(), expectedShunts1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(shunts2.begin(), shunts2.end(), shuntsId2.begin(), shuntsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(shunts2.begin(), shunts2.end(), expectedShunts2.begin(), expectedShunts2.end());
    for (auto& id : expectedShunts1) {
        subnetwork1.getShuntCompensator(id);
    }
    for (auto& id : expectedShunts2) {
        subnetwork2.getShuntCompensator(id);
    }

    // StaticVarCompensators
    auto expectedSvcRoot = {id("svc1", "1"), id("svc1", "2")};
    auto expectedSvc1 = {id("svc1", "1")};
    auto expectedSvc2 = {id("svc1", "2")};

    BOOST_CHECK_EQUAL(expectedSvcRoot.size(), network.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(expectedSvc1.size(), subnetwork1.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(expectedSvc2.size(), subnetwork2.getStaticVarCompensatorCount());
    const auto& svcsRoot = network.getStaticVarCompensators() | boost::adaptors::transformed(mapId<StaticVarCompensator>);
    const auto& svcs1 = subnetwork1.getStaticVarCompensators() | boost::adaptors::transformed(mapId<StaticVarCompensator>);
    const auto& svcs2 = subnetwork2.getStaticVarCompensators() | boost::adaptors::transformed(mapId<StaticVarCompensator>);
    const auto& svcsIdRoot = network.getIdentifiables(IdentifiableType::STATIC_VAR_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& svcsId1 = subnetwork1.getIdentifiables(IdentifiableType::STATIC_VAR_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& svcsId2 = subnetwork2.getIdentifiables(IdentifiableType::STATIC_VAR_COMPENSATOR) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedSvcRoot.size(), boost::size(svcsRoot));
    BOOST_CHECK_EQUAL(expectedSvc1.size(), boost::size(svcs1));
    BOOST_CHECK_EQUAL(expectedSvc2.size(), boost::size(svcs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(svcsRoot.begin(), svcsRoot.end(), svcsIdRoot.begin(), svcsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(svcsRoot.begin(), svcsRoot.end(), expectedSvcRoot.begin(), expectedSvcRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(svcs1.begin(), svcs1.end(), svcsId1.begin(), svcsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(svcs1.begin(), svcs1.end(), expectedSvc1.begin(), expectedSvc1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(svcs2.begin(), svcs2.end(), svcsId2.begin(), svcsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(svcs2.begin(), svcs2.end(), expectedSvc2.begin(), expectedSvc2.end());
    for (auto& id : expectedSvc1) {
        subnetwork1.getStaticVarCompensator(id);
    }
    for (auto& id : expectedSvc2) {
        subnetwork2.getStaticVarCompensator(id);
    }

    // BusbarSections
    auto expectedBbsRoot = {id("voltageLevel1BusbarSection1", "1"), id("voltageLevel1BusbarSection2", "1"),
                            id("voltageLevel1BusbarSection1", "2"), id("voltageLevel1BusbarSection2", "2")};
    auto expectedBbs1 = {id("voltageLevel1BusbarSection1", "1"), id("voltageLevel1BusbarSection2", "1")};
    auto expectedBbs2 = {id("voltageLevel1BusbarSection1", "2"), id("voltageLevel1BusbarSection2", "2")};

    BOOST_CHECK_EQUAL(expectedBbsRoot.size(), network.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(expectedBbs1.size(), subnetwork1.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(expectedBbs2.size(), subnetwork2.getBusbarSectionCount());
    const auto& bbsRoot = network.getBusbarSections() | boost::adaptors::transformed(mapId<BusbarSection>);
    const auto& bbs1 = subnetwork1.getBusbarSections() | boost::adaptors::transformed(mapId<BusbarSection>);
    const auto& bbs2 = subnetwork2.getBusbarSections() | boost::adaptors::transformed(mapId<BusbarSection>);
    const auto& bbsIdRoot = network.getIdentifiables(IdentifiableType::BUSBAR_SECTION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& bbsId1 = subnetwork1.getIdentifiables(IdentifiableType::BUSBAR_SECTION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& bbsId2 = subnetwork2.getIdentifiables(IdentifiableType::BUSBAR_SECTION) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedBbsRoot.size(), boost::size(bbsRoot));
    BOOST_CHECK_EQUAL(expectedBbs1.size(), boost::size(bbs1));
    BOOST_CHECK_EQUAL(expectedBbs2.size(), boost::size(bbs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(bbsRoot.begin(), bbsRoot.end(), bbsIdRoot.begin(), bbsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(bbsRoot.begin(), bbsRoot.end(), expectedBbsRoot.begin(), expectedBbsRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(bbs1.begin(), bbs1.end(), bbsId1.begin(), bbsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(bbs1.begin(), bbs1.end(), expectedBbs1.begin(), expectedBbs1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(bbs2.begin(), bbs2.end(), bbsId2.begin(), bbsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(bbs2.begin(), bbs2.end(), expectedBbs2.begin(), expectedBbs2.end());
    for (auto& id : expectedBbs1) {
        subnetwork1.getBusbarSection(id);
    }
    for (auto& id : expectedBbs2) {
        subnetwork2.getBusbarSection(id);
    }

    // Switches
    auto expectedSwitchesRoot = {id("voltageLevel1Breaker1", "1"), id("load1Disconnector1", "1"), id("load1Breaker1", "1"), id("generator1Disconnector1", "1"), id("generator1Breaker1", "1"),
                                 id("voltageLevel1Breaker1", "2"), id("load1Disconnector1", "2"), id("load1Breaker1", "2"), id("generator1Disconnector1", "2"), id("generator1Breaker1", "2")};
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

    BOOST_CHECK_EQUAL(expectedSwitchesRoot.size(), network.getSwitchCount());
    BOOST_CHECK_EQUAL(expectedSwitches1.size(), subnetwork1.getSwitchCount());
    BOOST_CHECK_EQUAL(expectedSwitches2.size(), subnetwork2.getSwitchCount());
    const auto& switchesRoot = network.getSwitches() | boost::adaptors::transformed(mapId<Switch>);
    const auto& switches1 = subnetwork1.getSwitches() | boost::adaptors::transformed(mapId<Switch>);
    const auto& switches2 = subnetwork2.getSwitches() | boost::adaptors::transformed(mapId<Switch>);
    const auto& switchesIdRoot = network.getIdentifiables(IdentifiableType::SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& switchesId1 = subnetwork1.getIdentifiables(IdentifiableType::SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& switchesId2 = subnetwork2.getIdentifiables(IdentifiableType::SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedSwitchesRoot.size(), boost::size(switchesRoot));
    BOOST_CHECK_EQUAL(expectedSwitches1.size(), boost::size(switches1));
    BOOST_CHECK_EQUAL(expectedSwitches2.size(), boost::size(switches2));
    BOOST_CHECK_EQUAL_COLLECTIONS(switchesRoot.begin(), switchesRoot.end(), switchesIdRoot.begin(), switchesIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(switchesRoot.begin(), switchesRoot.end(), expectedSwitchesRoot.begin(), expectedSwitchesRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(switches1.begin(), switches1.end(), switchesId1.begin(), switchesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(switches1.begin(), switches1.end(), expectedSwitches1.begin(), expectedSwitches1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(switches2.begin(), switches2.end(), switchesId2.begin(), switchesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(switches2.begin(), switches2.end(), expectedSwitches2.begin(), expectedSwitches2.end());
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

    auto hvdcConvertersForRoot= {id("lcc1", "1"), id("lcc2", "1"), id("lcc1", "2"), id("lcc2", "2"),
                                 id("vsc1", "1"), id("vsc2", "1"), id("vsc1", "2"), id("vsc2", "2")}; //All lcc then all vsc
    auto hvdcConvertersForSubnetwork1 = {id("lcc1", "1"), id("lcc2", "1"), id("vsc1", "1"), id("vsc2", "1")};
    auto hvdcConvertersForSubnetwork2 = {id("lcc1", "2"), id("lcc2", "2"), id("vsc1", "2"), id("vsc2", "2")};
    BOOST_CHECK_EQUAL(hvdcConvertersForRoot.size(), network.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork1.size(), subnetwork1.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork2.size(), subnetwork2.getHvdcConverterStationCount());
    const auto& hvdcConvertersRoot = network.getHvdcConverterStations() | boost::adaptors::transformed(mapId<HvdcConverterStation>);
    const auto& hvdcConverters1 = subnetwork1.getHvdcConverterStations() | boost::adaptors::transformed(mapId<HvdcConverterStation>);
    const auto& hvdcConverters2 = subnetwork2.getHvdcConverterStations() | boost::adaptors::transformed(mapId<HvdcConverterStation>);
    const auto& hvdcConvertersIdRoot = network.getIdentifiables(IdentifiableType::HVDC_CONVERTER_STATION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& hvdcConvertersId1 = subnetwork1.getIdentifiables(IdentifiableType::HVDC_CONVERTER_STATION) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& hvdcConvertersId2 = subnetwork2.getIdentifiables(IdentifiableType::HVDC_CONVERTER_STATION) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(hvdcConvertersForRoot.size(), boost::size(hvdcConvertersRoot));
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork1.size(), boost::size(hvdcConverters1));
    BOOST_CHECK_EQUAL(hvdcConvertersForSubnetwork2.size(), boost::size(hvdcConverters2));
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConvertersRoot.begin(), hvdcConvertersRoot.end(), hvdcConvertersIdRoot.begin(), hvdcConvertersIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConvertersRoot.begin(), hvdcConvertersRoot.end(), hvdcConvertersForRoot.begin(), hvdcConvertersForRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConverters1.begin(), hvdcConverters1.end(), hvdcConvertersId1.begin(), hvdcConvertersId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConverters1.begin(), hvdcConverters1.end(), hvdcConvertersForSubnetwork1.begin(), hvdcConvertersForSubnetwork1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConverters2.begin(), hvdcConverters2.end(), hvdcConvertersId2.begin(), hvdcConvertersId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcConverters2.begin(), hvdcConverters2.end(), hvdcConvertersForSubnetwork2.begin(), hvdcConvertersForSubnetwork2.end());
    for (auto& id : hvdcConvertersForSubnetwork1) {
        subnetwork1.getHvdcConverterStation(id);
    }
    for (auto& id : hvdcConvertersForSubnetwork2) {
        subnetwork2.getHvdcConverterStation(id);
    }

    // HVDC Lines
    auto expectedhvdclinesRoot = {id("hvdcLine1", "1"), id("hvdcLine2", "1"), id("hvdcLine1", "2"), id("hvdcLine2", "2")};
    auto expectedhvdclines1 = {id("hvdcLine1", "1"), id("hvdcLine2", "1")};
    auto expectedhvdclines2 = {id("hvdcLine1", "2"), id("hvdcLine2", "2")};
    BOOST_CHECK_EQUAL(expectedhvdclinesRoot.size(), network.getHvdcLineCount());
    BOOST_CHECK_EQUAL(expectedhvdclines1.size(), subnetwork1.getHvdcLineCount());
    BOOST_CHECK_EQUAL(expectedhvdclines2.size(), subnetwork2.getHvdcLineCount());
    const auto& hvdcLinesRoot = network.getHvdcLines() | boost::adaptors::transformed(mapId<HvdcLine>);
    const auto& hvdcLines1 = subnetwork1.getHvdcLines() | boost::adaptors::transformed(mapId<HvdcLine>);
    const auto& hvdcLines2 = subnetwork2.getHvdcLines() | boost::adaptors::transformed(mapId<HvdcLine>);
    const auto& hvdcLinesIdRoot = network.getIdentifiables(IdentifiableType::HVDC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& hvdcLinesId1 = subnetwork1.getIdentifiables(IdentifiableType::HVDC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& hvdcLinesId2 = subnetwork2.getIdentifiables(IdentifiableType::HVDC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedhvdclinesRoot.size(), boost::size(hvdcLinesRoot));
    BOOST_CHECK_EQUAL(expectedhvdclines1.size(), boost::size(hvdcLines1));
    BOOST_CHECK_EQUAL(expectedhvdclines2.size(), boost::size(hvdcLines2));
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLinesRoot.begin(), hvdcLinesRoot.end(), hvdcLinesIdRoot.begin(), hvdcLinesIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLinesRoot.begin(), hvdcLinesRoot.end(), expectedhvdclinesRoot.begin(), expectedhvdclinesRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLines1.begin(), hvdcLines1.end(), hvdcLinesId1.begin(), hvdcLinesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLines1.begin(), hvdcLines1.end(), expectedhvdclines1.begin(), expectedhvdclines1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLines2.begin(), hvdcLines2.end(), hvdcLinesId2.begin(), hvdcLinesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(hvdcLines2.begin(), hvdcLines2.end(), expectedhvdclines2.begin(), expectedhvdclines2.end());
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
    auto expected3wtRoot = {id("threeWindingsTransformer1", "1"), id("threeWindingsTransformer1", "2")};
    auto expected3wt1 = {id("threeWindingsTransformer1", "1")};
    auto expected3wt2 = {id("threeWindingsTransformer1", "2")};
    BOOST_CHECK_EQUAL(expected3wtRoot.size(), network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected3wt1.size(), subnetwork1.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected3wt2.size(), subnetwork2.getThreeWindingsTransformerCount());
    const auto& threeWTsRoot = network.getThreeWindingsTransformers() | boost::adaptors::transformed(mapId<ThreeWindingsTransformer>);
    const auto& threeWTs1 = subnetwork1.getThreeWindingsTransformers() | boost::adaptors::transformed(mapId<ThreeWindingsTransformer>);
    const auto& threeWTs2 = subnetwork2.getThreeWindingsTransformers() | boost::adaptors::transformed(mapId<ThreeWindingsTransformer>);
    const auto& threeWTsIdRoot = network.getIdentifiables(IdentifiableType::THREE_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& threeWTsId1 = subnetwork1.getIdentifiables(IdentifiableType::THREE_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& threeWTsId2 = subnetwork2.getIdentifiables(IdentifiableType::THREE_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expected3wtRoot.size(), boost::size(threeWTsRoot));
    BOOST_CHECK_EQUAL(expected3wt1.size(), boost::size(threeWTs1));
    BOOST_CHECK_EQUAL(expected3wt2.size(), boost::size(threeWTs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTsRoot.begin(), threeWTsRoot.end(), threeWTsIdRoot.begin(), threeWTsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTsRoot.begin(), threeWTsRoot.end(), expected3wtRoot.begin(), expected3wtRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTs1.begin(), threeWTs1.end(), threeWTsId1.begin(), threeWTsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTs1.begin(), threeWTs1.end(), expected3wt1.begin(), expected3wt1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTs2.begin(), threeWTs2.end(), threeWTsId2.begin(), threeWTsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(threeWTs2.begin(), threeWTs2.end(), expected3wt2.begin(), expected3wt2.end());
    for (auto& id : expected3wt1) {
        subnetwork1.getThreeWindingsTransformer(id);
    }
    for (auto& id : expected3wt2) {
        subnetwork2.getThreeWindingsTransformer(id);
    }

    // 2WT
    auto expected2wtRoot = {id("twoWindingsTransformer1", "1"), id("twoWindingsTransformer1", "2")};
    auto expected2wt1 = {id("twoWindingsTransformer1", "1")};
    auto expected2wt2 = {id("twoWindingsTransformer1", "2")};
    BOOST_CHECK_EQUAL(expected2wtRoot.size(), network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected2wt1.size(), subnetwork1.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(expected2wt2.size(), subnetwork2.getTwoWindingsTransformerCount());
    const auto& twoWTsRoot = network.getTwoWindingsTransformers() | boost::adaptors::transformed(mapId<TwoWindingsTransformer>);
    const auto& twoWTs1 = subnetwork1.getTwoWindingsTransformers() | boost::adaptors::transformed(mapId<TwoWindingsTransformer>);
    const auto& twoWTs2 = subnetwork2.getTwoWindingsTransformers() | boost::adaptors::transformed(mapId<TwoWindingsTransformer>);
    const auto& twoWTsIdRoot = network.getIdentifiables(IdentifiableType::TWO_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& twoWTsId1 = subnetwork1.getIdentifiables(IdentifiableType::TWO_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& twoWTsId2 = subnetwork2.getIdentifiables(IdentifiableType::TWO_WINDINGS_TRANSFORMER) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expected2wtRoot.size(), boost::size(twoWTsRoot));
    BOOST_CHECK_EQUAL(expected2wt1.size(), boost::size(twoWTs1));
    BOOST_CHECK_EQUAL(expected2wt2.size(), boost::size(twoWTs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTsRoot.begin(), twoWTsRoot.end(), twoWTsIdRoot.begin(), twoWTsIdRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTsRoot.begin(), twoWTsRoot.end(), expected2wtRoot.begin(), expected2wtRoot.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTs1.begin(), twoWTs1.end(), twoWTsId1.begin(), twoWTsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTs1.begin(), twoWTs1.end(), expected2wt1.begin(), expected2wt1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTs2.begin(), twoWTs2.end(), twoWTsId2.begin(), twoWTsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(twoWTs2.begin(), twoWTs2.end(), expected2wt2.begin(), expected2wt2.end());
    for (auto& id : expected2wt1) {
        subnetwork1.getTwoWindingsTransformer(id);
    }
    for (auto& id : expected2wt2) {
        subnetwork2.getTwoWindingsTransformer(id);
    }

    // Lines
    auto expectedLines0 = {id("line1", "1"), id("line1", "2")};
    auto expectedLines1 = {id("line1", "1")};
    auto expectedLines2 = {id("line1", "2")};
    BOOST_CHECK_EQUAL(expectedLines0.size(), network.getLineCount());
    BOOST_CHECK_EQUAL(expectedLines1.size(), subnetwork1.getLineCount());
    BOOST_CHECK_EQUAL(expectedLines2.size(), subnetwork2.getLineCount());
    const auto& lines0 = network.getLines() | boost::adaptors::transformed(mapId<Line>);
    const auto& lines1 = subnetwork1.getLines() | boost::adaptors::transformed(mapId<Line>);
    const auto& lines2 = subnetwork2.getLines() | boost::adaptors::transformed(mapId<Line>);
    const auto& linesId0 = network.getIdentifiables(IdentifiableType::LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& linesId1 = subnetwork1.getIdentifiables(IdentifiableType::LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& linesId2 = subnetwork2.getIdentifiables(IdentifiableType::LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedLines0.size(), boost::size(lines0));
    BOOST_CHECK_EQUAL(expectedLines1.size(), boost::size(lines1));
    BOOST_CHECK_EQUAL(expectedLines2.size(), boost::size(lines2));
    BOOST_CHECK_EQUAL_COLLECTIONS(lines0.begin(), lines0.end(), linesId0.begin(), linesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lines0.begin(), lines0.end(), expectedLines0.begin(), expectedLines0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lines1.begin(), lines1.end(), linesId1.begin(), linesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lines1.begin(), lines1.end(), expectedLines1.begin(), expectedLines1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lines2.begin(), lines2.end(), linesId2.begin(), linesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lines2.begin(), lines2.end(), expectedLines2.begin(), expectedLines2.end());
    for (auto& id : expectedLines1) {
        subnetwork1.getLine(id);
    }
    for (auto& id : expectedLines2) {
        subnetwork2.getLine(id);
    }

    // BoundaryLines
    auto expectedBLines0 = {id("boundaryLine1", "1"), id("boundaryLine2", "1"), id("boundaryLine3", "1"), 
                               id("boundaryLine1", "2"), id("boundaryLine2", "2"), id("boundaryLine3", "2")};
    auto expectedBLines1 = {id("boundaryLine1", "1"), id("boundaryLine2", "1"), id("boundaryLine3", "1")};
    auto expectedBLines2 = {id("boundaryLine1", "2"), id("boundaryLine2", "2"), id("boundaryLine3", "2")};
    BOOST_CHECK_EQUAL(expectedBLines0.size(), network.getBoundaryLineCount());
    BOOST_CHECK_EQUAL(expectedBLines1.size(), subnetwork1.getBoundaryLineCount());
    BOOST_CHECK_EQUAL(expectedBLines2.size(), subnetwork2.getBoundaryLineCount());
    const auto& boundaryLines0 = network.getBoundaryLines() | boost::adaptors::transformed(mapId<BoundaryLine>);
    const auto& boundaryLines1 = subnetwork1.getBoundaryLines() | boost::adaptors::transformed(mapId<BoundaryLine>);
    const auto& boundaryLines2 = subnetwork2.getBoundaryLines() | boost::adaptors::transformed(mapId<BoundaryLine>);
    const auto& boundaryLinesId0 = network.getIdentifiables(IdentifiableType::BOUNDARY_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& boundaryLinesId1 = subnetwork1.getIdentifiables(IdentifiableType::BOUNDARY_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& boundaryLinesId2 = subnetwork2.getIdentifiables(IdentifiableType::BOUNDARY_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedBLines0.size(), boost::size(boundaryLines0));
    BOOST_CHECK_EQUAL(expectedBLines1.size(), boost::size(boundaryLines1));
    BOOST_CHECK_EQUAL(expectedBLines2.size(), boost::size(boundaryLines2));
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines0.begin(), boundaryLines0.end(), boundaryLinesId0.begin(), boundaryLinesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines0.begin(), boundaryLines0.end(), expectedBLines0.begin(), expectedBLines0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines1.begin(), boundaryLines1.end(), boundaryLinesId1.begin(), boundaryLinesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines1.begin(), boundaryLines1.end(), expectedBLines1.begin(), expectedBLines1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines2.begin(), boundaryLines2.end(), boundaryLinesId2.begin(), boundaryLinesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLines2.begin(), boundaryLines2.end(), expectedBLines2.begin(), expectedBLines2.end());
    for (auto& id : expectedBLines1) {
        subnetwork1.getBoundaryLine(id);
    }
    for (auto& id : expectedBLines2) {
        subnetwork2.getBoundaryLine(id);
    }

    // TieLines
    auto expectedTieLine0= {id("tieLine1", "1"), id("tieLine1", "2"), id("tieLine3", "0")};
    auto expectedTieLine1 = {id("tieLine1", "1")};
    auto expectedTieLine2 = {id("tieLine1", "2")};
    BOOST_CHECK_EQUAL(expectedTieLine0.size(), network.getTieLineCount());
    BOOST_CHECK_EQUAL(expectedTieLine1.size(), subnetwork1.getTieLineCount());
    BOOST_CHECK_EQUAL(expectedTieLine2.size(), subnetwork2.getTieLineCount());
    const auto& tieLines0 = network.getTieLines() | boost::adaptors::transformed(mapId<TieLine>);
    const auto& tieLines1 = subnetwork1.getTieLines() | boost::adaptors::transformed(mapId<TieLine>);
    const auto& tieLines2 = subnetwork2.getTieLines() | boost::adaptors::transformed(mapId<TieLine>);
    const auto& tieLinesId0 = network.getIdentifiables(IdentifiableType::TIE_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& tieLinesId1 = subnetwork1.getIdentifiables(IdentifiableType::TIE_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& tieLinesId2 = subnetwork2.getIdentifiables(IdentifiableType::TIE_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedTieLine0.size(), boost::size(tieLines0));
    BOOST_CHECK_EQUAL(expectedTieLine1.size(), boost::size(tieLines1));
    BOOST_CHECK_EQUAL(expectedTieLine2.size(), boost::size(tieLines2));
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines0.begin(), tieLines0.end(), tieLinesId0.begin(), tieLinesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines0.begin(), tieLines0.end(), expectedTieLine0.begin(), expectedTieLine0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines1.begin(), tieLines1.end(), tieLinesId1.begin(), tieLinesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines1.begin(), tieLines1.end(), expectedTieLine1.begin(), expectedTieLine1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines2.begin(), tieLines2.end(), tieLinesId2.begin(), tieLinesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(tieLines2.begin(), tieLines2.end(), expectedTieLine2.begin(), expectedTieLine2.end());
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
    const auto& oms0 = network.getOverloadManagementSystems() | boost::adaptors::transformed(mapId<OverloadManagementSystem>);
    const auto& oms1 = subnetwork1.getOverloadManagementSystems() | boost::adaptors::transformed(mapId<OverloadManagementSystem>);
    const auto& oms2 = subnetwork2.getOverloadManagementSystems() | boost::adaptors::transformed(mapId<OverloadManagementSystem>);
    const auto& omsId0 = network.getIdentifiables(IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& omsId1 = subnetwork1.getIdentifiables(IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& omsId2 = subnetwork2.getIdentifiables(IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedOMS0.size(), boost::size(oms0));
    BOOST_CHECK_EQUAL(expectedOMS1.size(), boost::size(oms1));
    BOOST_CHECK_EQUAL(expectedOMS2.size(), boost::size(oms2));
    BOOST_CHECK_EQUAL_COLLECTIONS(oms0.begin(), oms0.end(), omsId0.begin(), omsId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(oms0.begin(), oms0.end(), expectedOMS0.begin(), expectedOMS0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(oms1.begin(), oms1.end(), omsId1.begin(), omsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(oms1.begin(), oms1.end(), expectedOMS1.begin(), expectedOMS1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(oms2.begin(), oms2.end(), omsId2.begin(), omsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(oms2.begin(), oms2.end(), expectedOMS2.begin(), expectedOMS2.end());
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
    const auto& areas0 = network.getAreas() | boost::adaptors::transformed(mapId<Area>);
    const auto& areas1 = subnetwork1.getAreas() | boost::adaptors::transformed(mapId<Area>);
    const auto& areas2 = subnetwork2.getAreas() | boost::adaptors::transformed(mapId<Area>);
    const auto& areasId0 = network.getIdentifiables(IdentifiableType::AREA) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& areasId1 = subnetwork1.getIdentifiables(IdentifiableType::AREA) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& areasId2 = subnetwork2.getIdentifiables(IdentifiableType::AREA) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedAreas0.size(), boost::size(areas0));
    BOOST_CHECK_EQUAL(expectedAreas1.size(), boost::size(areas1));
    BOOST_CHECK_EQUAL(expectedAreas2.size(), boost::size(areas2));
    BOOST_CHECK_EQUAL_COLLECTIONS(areas0.begin(), areas0.end(), areasId0.begin(), areasId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(areas0.begin(), areas0.end(), expectedAreas0.begin(), expectedAreas0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(areas1.begin(), areas1.end(), areasId1.begin(), areasId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(areas1.begin(), areas1.end(), expectedAreas1.begin(), expectedAreas1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(areas2.begin(), areas2.end(), areasId2.begin(), areasId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(areas2.begin(), areas2.end(), expectedAreas2.begin(), expectedAreas2.end());
    
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
                                            id("boundaryLine1", "1"), id("boundaryLine2", "1"), id("boundaryLine3", "1"),
                                            id("lccDetailed1", "1"), id("vscDetailed1", "1"),
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
                                            id("boundaryLine1", "2"), id("boundaryLine2", "2"), id("boundaryLine3", "2"),
                                            id("lccDetailed1", "2"), id("vscDetailed1", "2")};

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

    //DcConnectables also are retrieved from root network
    std::set<std::string> expectedDcConnectables = {id("dcGround1", "1"), id("dcLine1", "1"), id("lccDetailed1", "1"), id("vscDetailed1", "1"),
                                                    id("dcGround1", "2"), id("dcLine1", "2"), id("lccDetailed1", "2"), id("vscDetailed1", "2")};
    BOOST_CHECK_EQUAL(expectedDcConnectables.size(), subnetwork1.getDcConnectableCount());
    BOOST_CHECK_EQUAL(expectedDcConnectables.size(), subnetwork2.getDcConnectableCount());
    BOOST_CHECK_EQUAL(expectedDcConnectables.size(), boost::size(subnetwork1.getDcConnectables()));
    BOOST_CHECK_EQUAL(expectedDcConnectables.size(), boost::size(subnetwork2.getDcConnectables()));
    BOOST_CHECK_EQUAL(2, subnetwork1.getDcConnectableCount<DcLine>());
    BOOST_CHECK_EQUAL(2, subnetwork2.getDcConnectableCount<DcLine>());
    BOOST_CHECK_EQUAL(4, subnetwork1.getDcConnectableCount<AcDcConverter>());
    BOOST_CHECK_EQUAL(4, subnetwork2.getDcConnectableCount<AcDcConverter>());
    std::set<std::string> dcConnectablesId1;
    for (auto& c : subnetwork1.getDcConnectables()){
        dcConnectablesId1.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedDcConnectables.begin(), expectedDcConnectables.end(), dcConnectablesId1.begin(), dcConnectablesId1.end());
    std::set<std::string> dcConnectablesId2;
    for (auto& c : subnetwork2.getDcConnectables()){
        dcConnectablesId2.emplace(c.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedDcConnectables.begin(), expectedDcConnectables.end(), dcConnectablesId2.begin(), dcConnectablesId2.end());

    //DcNodes
    auto expectedDcNodes0 = {id("dcNode1", "1"), id("dcNode2", "1"), id("dcNode1", "2"), id("dcNode2", "2")};
    auto expectedDcNodes1 = {id("dcNode1", "1"), id("dcNode2", "1"),};
    auto expectedDcNodes2 = {id("dcNode1", "2"), id("dcNode2", "2"),};
    BOOST_CHECK_EQUAL(expectedDcNodes0.size(), network.getDcNodeCount());
    BOOST_CHECK_EQUAL(expectedDcNodes1.size(), subnetwork1.getDcNodeCount());
    BOOST_CHECK_EQUAL(expectedDcNodes2.size(), subnetwork2.getDcNodeCount());
    const auto& dcNodes0 = network.getDcNodes() | boost::adaptors::transformed(mapId<DcNode>);
    const auto& dcNodes1 = subnetwork1.getDcNodes() | boost::adaptors::transformed(mapId<DcNode>);
    const auto& dcNodes2 = subnetwork2.getDcNodes() | boost::adaptors::transformed(mapId<DcNode>);
    const auto& dcNodesId0 = network.getIdentifiables(IdentifiableType::DC_NODE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcNodesId1 = subnetwork1.getIdentifiables(IdentifiableType::DC_NODE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcNodesId2 = subnetwork2.getIdentifiables(IdentifiableType::DC_NODE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedDcNodes0.size(), boost::size(dcNodes0));
    BOOST_CHECK_EQUAL(expectedDcNodes1.size(), boost::size(dcNodes1));
    BOOST_CHECK_EQUAL(expectedDcNodes2.size(), boost::size(dcNodes2));
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes0.begin(), dcNodes0.end(), dcNodesId0.begin(), dcNodesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes0.begin(), dcNodes0.end(), expectedDcNodes0.begin(), expectedDcNodes0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes1.begin(), dcNodes1.end(), dcNodesId1.begin(), dcNodesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes1.begin(), dcNodes1.end(), expectedDcNodes1.begin(), expectedDcNodes1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes2.begin(), dcNodes2.end(), dcNodesId2.begin(), dcNodesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcNodes2.begin(), dcNodes2.end(), expectedDcNodes2.begin(), expectedDcNodes2.end());
    for (auto& id : expectedDcNodes1) {
        subnetwork1.getDcNode(id);
    }
    for (auto& id : expectedDcNodes2) {
        subnetwork2.getDcNode(id);
    }

    //DcLines
    auto expectedDcLines0 = {id("dcLine1", "1"), id("dcLine1", "2")};
    auto expectedDcLines1 = {id("dcLine1", "1")};
    auto expectedDcLines2 = {id("dcLine1", "2")};
    BOOST_CHECK_EQUAL(expectedDcLines0.size(), network.getDcLineCount());
    BOOST_CHECK_EQUAL(expectedDcLines1.size(), subnetwork1.getDcLineCount());
    BOOST_CHECK_EQUAL(expectedDcLines2.size(), subnetwork2.getDcLineCount());
    const auto& dcLines0 = network.getDcLines() | boost::adaptors::transformed(mapId<DcLine>);
    const auto& dcLines1 = subnetwork1.getDcLines() | boost::adaptors::transformed(mapId<DcLine>);
    const auto& dcLines2 = subnetwork2.getDcLines() | boost::adaptors::transformed(mapId<DcLine>);
    const auto& dcLinesId0 = network.getIdentifiables(IdentifiableType::DC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcLinesId1 = subnetwork1.getIdentifiables(IdentifiableType::DC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcLinesId2 = subnetwork2.getIdentifiables(IdentifiableType::DC_LINE) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedDcLines0.size(), boost::size(dcLines0));
    BOOST_CHECK_EQUAL(expectedDcLines1.size(), boost::size(dcLines1));
    BOOST_CHECK_EQUAL(expectedDcLines2.size(), boost::size(dcLines2));
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines0.begin(), dcLines0.end(), dcLinesId0.begin(), dcLinesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines0.begin(), dcLines0.end(), expectedDcLines0.begin(), expectedDcLines0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines1.begin(), dcLines1.end(), dcLinesId1.begin(), dcLinesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines1.begin(), dcLines1.end(), expectedDcLines1.begin(), expectedDcLines1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines2.begin(), dcLines2.end(), dcLinesId2.begin(), dcLinesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLines2.begin(), dcLines2.end(), expectedDcLines2.begin(), expectedDcLines2.end());
    for (auto& id : expectedDcLines1) {
        subnetwork1.getDcLine(id);
    }
    for (auto& id : expectedDcLines2) {
        subnetwork2.getDcLine(id);
    }

    //DcGrounds
    auto expectedDcGrounds0 = {id("dcGround1", "1"), id("dcGround1", "2")};
    auto expectedDcGrounds1 = {id("dcGround1", "1")};
    auto expectedDcGrounds2 = {id("dcGround1", "2")};
    BOOST_CHECK_EQUAL(expectedDcGrounds0.size(), network.getDcGroundCount());
    BOOST_CHECK_EQUAL(expectedDcGrounds1.size(), subnetwork1.getDcGroundCount());
    BOOST_CHECK_EQUAL(expectedDcGrounds2.size(), subnetwork2.getDcGroundCount());
    const auto& dcGrounds0 = network.getDcGrounds() | boost::adaptors::transformed(mapId<DcGround>);
    const auto& dcGrounds1 = subnetwork1.getDcGrounds() | boost::adaptors::transformed(mapId<DcGround>);
    const auto& dcGrounds2 = subnetwork2.getDcGrounds() | boost::adaptors::transformed(mapId<DcGround>);
    const auto& dcGroundsId0 = network.getIdentifiables(IdentifiableType::DC_GROUND) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcGroundsId1 = subnetwork1.getIdentifiables(IdentifiableType::DC_GROUND) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcGroundsId2 = subnetwork2.getIdentifiables(IdentifiableType::DC_GROUND) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedDcGrounds0.size(), boost::size(dcGrounds0));
    BOOST_CHECK_EQUAL(expectedDcGrounds1.size(), boost::size(dcGrounds1));
    BOOST_CHECK_EQUAL(expectedDcGrounds2.size(), boost::size(dcGrounds2));
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds0.begin(), dcGrounds0.end(), dcGroundsId0.begin(), dcGroundsId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds0.begin(), dcGrounds0.end(), expectedDcGrounds0.begin(), expectedDcGrounds0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds1.begin(), dcGrounds1.end(), dcGroundsId1.begin(), dcGroundsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds1.begin(), dcGrounds1.end(), expectedDcGrounds1.begin(), expectedDcGrounds1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds2.begin(), dcGrounds2.end(), dcGroundsId2.begin(), dcGroundsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGrounds2.begin(), dcGrounds2.end(), expectedDcGrounds2.begin(), expectedDcGrounds2.end());
    for (auto& id : expectedDcGrounds1) {
        subnetwork1.getDcGround(id);
    }
    for (auto& id : expectedDcGrounds2) {
        subnetwork2.getDcGround(id);
    }

    //DcSwitches
    auto expectedDcSwitches0 = {id("dcSwitch1", "1"), id("dcSwitch1", "2")};
    auto expectedDcSwitches1 = {id("dcSwitch1", "1")};
    auto expectedDcSwitches2 = {id("dcSwitch1", "2")};
    BOOST_CHECK_EQUAL(expectedDcSwitches0.size(), network.getDcSwitchCount());
    BOOST_CHECK_EQUAL(expectedDcSwitches1.size(), subnetwork1.getDcSwitchCount());
    BOOST_CHECK_EQUAL(expectedDcSwitches2.size(), subnetwork2.getDcSwitchCount());
    const auto& dcSwitches0 = network.getDcSwitches() | boost::adaptors::transformed(mapId<DcSwitch>);
    const auto& dcSwitches1 = subnetwork1.getDcSwitches() | boost::adaptors::transformed(mapId<DcSwitch>);
    const auto& dcSwitches2 = subnetwork2.getDcSwitches() | boost::adaptors::transformed(mapId<DcSwitch>);
    const auto& dcSwitchesId0 = network.getIdentifiables(IdentifiableType::DC_SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcSwitchesId1 = subnetwork1.getIdentifiables(IdentifiableType::DC_SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcSwitchesId2 = subnetwork2.getIdentifiables(IdentifiableType::DC_SWITCH) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedDcSwitches0.size(), boost::size(dcSwitches0));
    BOOST_CHECK_EQUAL(expectedDcSwitches1.size(), boost::size(dcSwitches1));
    BOOST_CHECK_EQUAL(expectedDcSwitches2.size(), boost::size(dcSwitches2));
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches0.begin(), dcSwitches0.end(), dcSwitchesId0.begin(), dcSwitchesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches0.begin(), dcSwitches0.end(), expectedDcSwitches0.begin(), expectedDcSwitches0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches1.begin(), dcSwitches1.end(), dcSwitchesId1.begin(), dcSwitchesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches1.begin(), dcSwitches1.end(), expectedDcSwitches1.begin(), expectedDcSwitches1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches2.begin(), dcSwitches2.end(), dcSwitchesId2.begin(), dcSwitchesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcSwitches2.begin(), dcSwitches2.end(), expectedDcSwitches2.begin(), expectedDcSwitches2.end());
    for (auto& id : expectedDcSwitches1) {
        subnetwork1.getDcSwitch(id);
    }
    for (auto& id : expectedDcSwitches2) {
        subnetwork2.getDcSwitch(id);
    }

    //LineCommutatedConverters
    auto expectedLineCommutatedConverters0 = {id("lccDetailed1", "1"), id("lccDetailed1", "2")};
    auto expectedLineCommutatedConverters1 = {id("lccDetailed1", "1")};
    auto expectedLineCommutatedConverters2 = {id("lccDetailed1", "2")};
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters0.size(), network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters1.size(), subnetwork1.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters2.size(), subnetwork2.getLineCommutatedConverterCount());
    const auto& lccs0 = network.getLineCommutatedConverters() | boost::adaptors::transformed(mapId<LineCommutatedConverter>);
    const auto& lccs1 = subnetwork1.getLineCommutatedConverters() | boost::adaptors::transformed(mapId<LineCommutatedConverter>);
    const auto& lccs2 = subnetwork2.getLineCommutatedConverters() | boost::adaptors::transformed(mapId<LineCommutatedConverter>);
    const auto& lccsId0 = network.getIdentifiables(IdentifiableType::LINE_COMMUTATED_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& lccsId1 = subnetwork1.getIdentifiables(IdentifiableType::LINE_COMMUTATED_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& lccsId2 = subnetwork2.getIdentifiables(IdentifiableType::LINE_COMMUTATED_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters0.size(), boost::size(lccs0));
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters1.size(), boost::size(lccs1));
    BOOST_CHECK_EQUAL(expectedLineCommutatedConverters2.size(), boost::size(lccs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs0.begin(), lccs0.end(), lccsId0.begin(), lccsId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs0.begin(), lccs0.end(), expectedLineCommutatedConverters0.begin(), expectedLineCommutatedConverters0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs1.begin(), lccs1.end(), lccsId1.begin(), lccsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs1.begin(), lccs1.end(), expectedLineCommutatedConverters1.begin(), expectedLineCommutatedConverters1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs2.begin(), lccs2.end(), lccsId2.begin(), lccsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(lccs2.begin(), lccs2.end(), expectedLineCommutatedConverters2.begin(), expectedLineCommutatedConverters2.end());
    for (auto& id : expectedLineCommutatedConverters1) {
        subnetwork1.getLineCommutatedConverter(id);
    }
    for (auto& id : expectedLineCommutatedConverters2) {
        subnetwork2.getLineCommutatedConverter(id);
    }

    //VoltageSourceConverters
    auto expectedVoltageSourceConverters0 = {id("vscDetailed1", "1"), id("vscDetailed1", "2")};
    auto expectedVoltageSourceConverters1 = {id("vscDetailed1", "1")};
    auto expectedVoltageSourceConverters2 = {id("vscDetailed1", "2")};
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters0.size(), network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters1.size(), subnetwork1.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters2.size(), subnetwork2.getVoltageSourceConverterCount());
    const auto& vscs0 = network.getVoltageSourceConverters() | boost::adaptors::transformed(mapId<VoltageSourceConverter>);
    const auto& vscs1 = subnetwork1.getVoltageSourceConverters() | boost::adaptors::transformed(mapId<VoltageSourceConverter>);
    const auto& vscs2 = subnetwork2.getVoltageSourceConverters() | boost::adaptors::transformed(mapId<VoltageSourceConverter>);
    const auto& vscsId0 = network.getIdentifiables(IdentifiableType::VOLTAGE_SOURCE_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& vscsId1 = subnetwork1.getIdentifiables(IdentifiableType::VOLTAGE_SOURCE_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& vscsId2 = subnetwork2.getIdentifiables(IdentifiableType::VOLTAGE_SOURCE_CONVERTER) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters0.size(), boost::size(vscs0));
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters1.size(), boost::size(vscs1));
    BOOST_CHECK_EQUAL(expectedVoltageSourceConverters2.size(), boost::size(vscs2));
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs0.begin(), vscs0.end(), vscsId0.begin(), vscsId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs0.begin(), vscs0.end(), expectedVoltageSourceConverters0.begin(), expectedVoltageSourceConverters0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs1.begin(), vscs1.end(), vscsId1.begin(), vscsId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs1.begin(), vscs1.end(), expectedVoltageSourceConverters1.begin(), expectedVoltageSourceConverters1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs2.begin(), vscs2.end(), vscsId2.begin(), vscsId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(vscs2.begin(), vscs2.end(), expectedVoltageSourceConverters2.begin(), expectedVoltageSourceConverters2.end());
    for (auto& id : expectedVoltageSourceConverters1) {
        subnetwork1.getVoltageSourceConverter(id);
    }
    for (auto& id : expectedVoltageSourceConverters2) {
        subnetwork2.getVoltageSourceConverter(id);
    }

    // Identifiables
    std::set<std::string> expectedIdentifiables1 = {"n1_area1","n1_battery1","n1_boundaryLine1","n1_boundaryLine2","n1_boundaryLine3",
        "n1_dcGround1","n1_dcLine1","n1_dcNode1","n1_dcNode2","n1_dcSwitch1",
        "n1_generator1","n1_generator1Breaker1","n1_generator1Disconnector1","n1_hvdcLine1","n1_hvdcLine2",
        "n1_lcc1","n1_lcc2","n1_lccDetailed1","n1_line1","n1_load1","n1_load1Breaker1","n1_load1Disconnector1",
        "n1_network","n1_overloadManagementSystem","n1_shuntCompensator1","n1_substation1","n1_substation2","n1_substation3",
        "n1_svc1","n1_threeWindingsTransformer1","n1_tieLine1","n1_twoWindingsTransformer1",
        "n1_voltageLevel1","n1_voltageLevel1Breaker1","n1_voltageLevel1BusbarSection1","n1_voltageLevel1BusbarSection2",
        "n1_voltageLevel2","n1_voltageLevel3","n1_voltageLevel4","n1_voltageLevel5","n1_vsc1","n1_vsc2","n1_vscDetailed1"};
    std::set<std::string> expectedIdentifiables2 = {"n2_area1","n2_battery1","n2_boundaryLine1","n2_boundaryLine2","n2_boundaryLine3",
        "n2_dcGround1","n2_dcLine1","n2_dcNode1","n2_dcNode2","n2_dcSwitch1",
        "n2_generator1","n2_generator1Breaker1","n2_generator1Disconnector1","n2_hvdcLine1","n2_hvdcLine2",
        "n2_lcc1","n2_lcc2","n2_lccDetailed1","n2_line1","n2_load1","n2_load1Breaker1","n2_load1Disconnector1",
        "n2_network","n2_overloadManagementSystem","n2_shuntCompensator1","n2_substation1","n2_substation2","n2_substation3",
        "n2_svc1","n2_threeWindingsTransformer1","n2_tieLine1","n2_twoWindingsTransformer1",
        "n2_voltageLevel1","n2_voltageLevel1Breaker1","n2_voltageLevel1BusbarSection1","n2_voltageLevel1BusbarSection2",
        "n2_voltageLevel2","n2_voltageLevel3","n2_voltageLevel4","n2_voltageLevel5","n2_vsc1","n2_vsc2","n2_vscDetailed1"};
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

    //GROUND not tested since none present in the network built for this test, but covered in GroundTestSuite

    //BUS unsupported IdentifiableType from Network
    POWSYBL_ASSERT_THROW(network.getIdentifiables(IdentifiableType::BUS), AssertionError, "Can't get a range of BUS from a network");
    POWSYBL_ASSERT_THROW(subnetwork1.getIdentifiables(IdentifiableType::BUS), AssertionError, "Can't get a range of BUS from a network");
    POWSYBL_ASSERT_THROW(subnetwork2.getIdentifiables(IdentifiableType::BUS), AssertionError, "Can't get a range of BUS from a network");

    //DC_BUS acceptable though can be retrieved since they are hold by the network:
    std::list<std::string> expectedDcBuses0 = {id("dcNode1_dcBus", "1"), id("dcNode1_dcBus", "2")};
    auto expectedDcBuses1 = {id("dcNode1_dcBus", "1")};
    auto expectedDcBuses2 = {id("dcNode1_dcBus", "2")};
    BOOST_CHECK_EQUAL(expectedDcBuses0.size(), network.getDcBusCount());
    BOOST_CHECK_EQUAL(expectedDcBuses1.size(), subnetwork1.getDcBusCount());
    BOOST_CHECK_EQUAL(expectedDcBuses2.size(), subnetwork2.getDcBusCount());
    const auto& dcBuses0 = network.getDcBuses() | boost::adaptors::transformed(mapId<DcBus>);
    const auto& dcBuses1 = subnetwork1.getDcBuses() | boost::adaptors::transformed(mapId<DcBus>);
    const auto& dcBuses2 = subnetwork2.getDcBuses() | boost::adaptors::transformed(mapId<DcBus>);
    const auto& dcBusesId0 = network.getIdentifiables(IdentifiableType::DC_BUS) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcBusesId1 = subnetwork1.getIdentifiables(IdentifiableType::DC_BUS) | boost::adaptors::transformed(mapId<Identifiable>);
    const auto& dcBusesId2 = subnetwork2.getIdentifiables(IdentifiableType::DC_BUS) | boost::adaptors::transformed(mapId<Identifiable>);
    BOOST_CHECK_EQUAL(expectedDcBuses0.size(), boost::size(dcBuses0));
    BOOST_CHECK_EQUAL(expectedDcBuses1.size(), boost::size(dcBuses1));
    BOOST_CHECK_EQUAL(expectedDcBuses2.size(), boost::size(dcBuses2));
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses0.begin(), dcBuses0.end(), dcBusesId0.begin(), dcBusesId0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses0.begin(), dcBuses0.end(), expectedDcBuses0.begin(), expectedDcBuses0.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses1.begin(), dcBuses1.end(), dcBusesId1.begin(), dcBusesId1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses1.begin(), dcBuses1.end(), expectedDcBuses1.begin(), expectedDcBuses1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses2.begin(), dcBuses2.end(), dcBusesId2.begin(), dcBusesId2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(dcBuses2.begin(), dcBuses2.end(), expectedDcBuses2.begin(), expectedDcBuses2.end());
    for (auto& id : expectedDcBuses1) {
        subnetwork1.getDcBus(id);
    }
    for (auto& id : expectedDcBuses2) {
        subnetwork2.getDcBus(id);
    }

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
