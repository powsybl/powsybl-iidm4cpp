/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Subnetwork.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network CreateSubnetworksNetworkTest() {
    Network network("Root", "format0");
    network.newSubnetwork("Sub1", "format1");
    network.newSubnetwork("Sub2", "format2");

    return network;
}

void assertNetworks(Network& expectedRootNetwork, Network& expectedParentNetwork, Identifiable& identifiable) {
    Network& root = identifiable.getNetwork();
    Network& parent = identifiable.getParentNetwork();
    BOOST_CHECK(stdcxx::areSame(expectedRootNetwork, root));
    BOOST_CHECK(stdcxx::areSame(expectedParentNetwork, parent));
}

void assertValidationLevels(ValidationLevel expected, Network& rootnetwork) {
    // The validation level must be the same between the root network and its subnetworks
    BOOST_CHECK_EQUAL(expected, rootnetwork.getValidationLevel());
    BOOST_CHECK_EQUAL(expected, rootnetwork.getSubNetwork("Sub1").get().getValidationLevel());
    BOOST_CHECK_EQUAL(expected, rootnetwork.getSubNetwork("Sub2").get().getValidationLevel());
}
void assertMinValidationLevels(ValidationLevel expected, Network& rootnetwork) {
    // The validation level must be the same between the root network and its subnetworks
    BOOST_CHECK_EQUAL(expected, rootnetwork.getMinimumValidationLevel());
    BOOST_CHECK_EQUAL(expected, rootnetwork.getSubNetwork("Sub1").get().getMinimumValidationLevel());
    BOOST_CHECK_EQUAL(expected, rootnetwork.getSubNetwork("Sub2").get().getMinimumValidationLevel());
}

std::string getBusId(std::string vlId3) {
    return "bus_" + vlId3;
}

Substation& addSubstation(Network& network, const std::string& substationId) {
    return network.newSubstation()
                .setId(substationId)
                .setCountry(Country::AQ)
                .add();
}

VoltageLevel& addVoltageLevel(VoltageLevelAdder adder, double nominalV, const std::string& id) {
    VoltageLevel& voltageLevel = adder.setId(id)
                .setNominalV(nominalV)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();
    voltageLevel.getBusBreakerView().newBus()
                .setId(getBusId(id))
                .add();
    return voltageLevel;
}
VoltageLevel& addVoltageLevel(VoltageLevelAdder adder, const std::string& id) {
    return addVoltageLevel(adder, 380.0, id);
}

Line& addLine(Network& network, const std::string& id, const std::string& vl1, const std::string& vl2) {
    return network.newLine()
                .setId(id)
                .setVoltageLevel1(vl1).setBus1(getBusId(vl1))
                .setVoltageLevel2(vl2).setBus2(getBusId(vl2))
                .setR(1.0).setX(1.0).setG1(0.0).setB1(0.0).setG2(0.0).setB2(0.0)
                .add();
}

TwoWindingsTransformer& addTwoWindingsTransformer(Substation& substation, const std::string& id, const std::string& vlId1, double nominalV1,
                                                             const std::string& vlId2, double nominalV2) {
    return substation.newTwoWindingsTransformer()
                .setId(id)
                .setR(0)
                .setX(0)
                .setG(0)
                .setB(0)
                .setConnectableBus1(getBusId(vlId1))
                .setBus1(getBusId(vlId1))
                .setConnectableBus2(getBusId(vlId2))
                .setBus2(getBusId(vlId2))
                .setVoltageLevel1(vlId1)
                .setVoltageLevel2(vlId2)
                .setRatedU1(nominalV1)
                .setRatedU2(nominalV2)
                .add();
}

ThreeWindingsTransformer& addThreeWindingsTransformer(Substation& substation, const std::string& id, const std::string& vlId1, double nominalV1,
                                                             const std::string& vlId2, double nominalV2, const std::string& vlId3, double nominalV3) {
    return substation.newThreeWindingsTransformer()
                .setId(id)
                .newLeg1()
                .setRatedU(nominalV1)
                .setR(0)
                .setX(0)
                .setG(0)
                .setB(0)
                .setConnectableBus(getBusId(vlId1))
                .setBus(getBusId(vlId1))
                .setVoltageLevel(vlId1)
                .add()
                .newLeg2()
                .setRatedU(nominalV2)
                .setR(0)
                .setX(0)
                .setG(0)
                .setB(0)
                .setConnectableBus(getBusId(vlId2))
                .setBus(getBusId(vlId2))
                .setVoltageLevel(vlId2)
                .add()
                .newLeg3()
                .setRatedU(nominalV3)
                .setR(0)
                .setX(0)
                .setG(0)
                .setB(0)
                .setConnectableBus(getBusId(vlId3))
                .setBus(getBusId(vlId3))
                .setVoltageLevel(vlId3)
                .add()
                .add();
}

BOOST_AUTO_TEST_SUITE(SubnetworkCreationTestSuite)

BOOST_AUTO_TEST_CASE(SubnetworksCreationTest) {
    Network network = CreateSubnetworksNetworkTest();

    BOOST_CHECK_EQUAL(2, network.getSubNetworksCount());
    BOOST_CHECK_EQUAL(3, boost::size(network.getIdentifiables()));

    stdcxx::Reference<Network> sub1 = network.getSubNetwork("Sub1");
    stdcxx::Reference<Network> sub2 = network.getSubNetwork("Sub2");

    BOOST_CHECK(sub1);
    BOOST_CHECK(sub2);
    BOOST_CHECK(stdcxx::areSame(network, sub1.get().getRootNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, sub2.get().getRootNetwork()));

    BOOST_CHECK_EQUAL(0, sub1.get().getSubNetworksCount());
    BOOST_CHECK_EQUAL(0, boost::size(sub2.get().getSubNetworks()));

    POWSYBL_ASSERT_THROW(network.newSubnetwork("Sub1","format1"), PowsyblException, "The network 'Root' already contains a subnetwork 'Sub1'");
    POWSYBL_ASSERT_THROW(sub1.get().newSubnetwork("Sub3", "format3"), PowsyblException, "Unsupported operation, Inner subnetworks are not supported");

}

BOOST_AUTO_TEST_CASE(SubstationCreationTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    // On root network level
    addSubstation(network, "s0");
    BOOST_CHECK_EQUAL(1, network.getSubstationCount());
    BOOST_CHECK_EQUAL(0, subnetwork1.getSubstationCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getSubstationCount());

    BOOST_CHECK_NO_THROW(network.getSubstation("s0"));
    POWSYBL_ASSERT_THROW(subnetwork1.getSubstation("s0"),PowsyblException, "Substation 's0' does not belong to the subnetwork 'Sub1'");
    POWSYBL_ASSERT_THROW(subnetwork2.getSubstation("s0"),PowsyblException, "Substation 's0' does not belong to the subnetwork 'Sub2'");

    // On subnetwork level
    addSubstation(subnetwork1, "s1");
    BOOST_CHECK_EQUAL(2, network.getSubstationCount());
    BOOST_CHECK_EQUAL(1, subnetwork1.getSubstationCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getSubstationCount());


    BOOST_CHECK_NO_THROW(network.getSubstation("s1"));
    BOOST_CHECK_NO_THROW(subnetwork1.getSubstation("s1"));
    POWSYBL_ASSERT_THROW(subnetwork2.getSubstation("s1"),PowsyblException, "Substation 's1' does not belong to the subnetwork 'Sub2'");

    Substation& s1_2 = subnetwork2.newSubstation()
                .setId("s1")
                .setCountry(Country::AQ)
                .setEnsureIdUnicity(true)
                .add();

    BOOST_CHECK_EQUAL("s1#0", s1_2.getId());
}

BOOST_AUTO_TEST_CASE(VoltageLevelCreationTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    addSubstation(network, "s0");
    addSubstation(subnetwork1, "s1");
    addSubstation(subnetwork2, "s2");
    BOOST_CHECK_EQUAL(3, network.getSubstationCount());
    BOOST_CHECK_EQUAL(1, subnetwork1.getSubstationCount());
    BOOST_CHECK_EQUAL(1, subnetwork2.getSubstationCount());

    // On a substation at root network level
    addVoltageLevel(network.getSubstation("s0").newVoltageLevel(), "vl0_0");
    BOOST_CHECK_EQUAL(1, network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(0, subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getVoltageLevelCount());
    assertNetworks(network, network, network.getVoltageLevel("vl0_0"));

    // On a substation at subnetwork level
    addVoltageLevel(network.getSubstation("s1").newVoltageLevel(), "vl1_0");
    BOOST_CHECK_EQUAL(2, network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(1, subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getVoltageLevelCount());
    assertNetworks(network, subnetwork1, subnetwork1.getVoltageLevel("vl1_0"));

    // On network: root network level
    addVoltageLevel(network.newVoltageLevel(), "vl0_1");
    BOOST_CHECK_EQUAL(3, network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(1, subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(0, subnetwork2.getVoltageLevelCount());
    assertNetworks(network, network, network.getVoltageLevel("vl0_1"));

    // On network: subnetwork level
    addVoltageLevel(subnetwork2.newVoltageLevel(), "vl2_0");
    BOOST_CHECK_EQUAL(4, network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(1, subnetwork1.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(1, subnetwork2.getVoltageLevelCount());
    assertNetworks(network, subnetwork2, subnetwork2.getVoltageLevel("vl2_0"));
}

BOOST_AUTO_TEST_CASE(LineCreationTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    addSubstation(network, "s0");
    addSubstation(subnetwork1, "s1");
    addSubstation(subnetwork2, "s2");

    addVoltageLevel(network.getSubstation("s0").newVoltageLevel(), "vl0_0");
    addVoltageLevel(network.newVoltageLevel(), "vl0_1");
    addVoltageLevel(network.getSubstation("s1").newVoltageLevel(), "vl1_0");
    addVoltageLevel(subnetwork1.newVoltageLevel(), "vl1_1");
    addVoltageLevel(network.getSubstation("s2").newVoltageLevel(), "vl2_0");
    addVoltageLevel(subnetwork2.newVoltageLevel(), "vl2_1");

    // On root network, voltage levels both in root network
    Line& l0 = addLine(network, "l0", "vl0_0", "vl0_1");

    // On root network, voltage levels both in subnetwork1
    Line& l1 = addLine(network, "l1", "vl1_0", "vl1_1");

    // On subnetwork2, voltage levels both in subnetwork2
    Line& l2 = addLine(subnetwork2, "l2", "vl2_0", "vl2_1");

    // On root network, voltage levels in different subnetworks
    Line& l3 = addLine(network, "l3", "vl1_0", "vl2_0");

    // On root network, voltage levels in root network and subnetwork2
    Line& l4 = addLine(network, "l4", "vl0_0", "vl2_0");

    assertNetworks(network, network, l0);
    assertNetworks(network, subnetwork1, l1);
    assertNetworks(network, subnetwork2, l2);
    assertNetworks(network, network, l3);
    assertNetworks(network, network, l4);

    //On subnetwork1, voltage levels in root network and subnetwork1
    POWSYBL_ASSERT_THROW(addLine(subnetwork1, "l5", "vl0_0", "vl1_1"), ValidationException, 
    "AC line 'l5': The involved voltage levels are not in the subnetwork 'Sub1'. Create this line from the parent network 'Root'");
    //On subnetwork1, voltage levels both in subnetwork2
    POWSYBL_ASSERT_THROW(addLine(subnetwork1, "l6", "vl2_0", "vl2_1"), ValidationException, 
    "AC line 'l6': The involved voltage levels are not in the subnetwork 'Sub1'. Create this line from the parent network 'Root'");

}

BOOST_AUTO_TEST_CASE(TwoWindingsCreationTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    Substation& substation0 = addSubstation(network, "s0");
    Substation& substation1 = addSubstation(subnetwork1, "s1");
    Substation& substation2 = addSubstation(subnetwork2, "s2");
    addVoltageLevel(substation0.newVoltageLevel(), "vl0_0");
    addVoltageLevel(substation0.newVoltageLevel(), 90, "vl0_1");
    addVoltageLevel(substation1.newVoltageLevel(), "vl1_0");
    addVoltageLevel(substation1.newVoltageLevel(), 90, "vl1_1");
    addVoltageLevel(substation2.newVoltageLevel(), "vl2_0");
    addVoltageLevel(substation2.newVoltageLevel(), 90, "vl2_1");

    // On root network
    TwoWindingsTransformer& t0 = addTwoWindingsTransformer(substation0, "twt0", "vl0_0", 380, "vl0_1", 90);

    // On subnetwork1
    TwoWindingsTransformer& t1 = addTwoWindingsTransformer(substation1, "twt1", "vl1_0", 380, "vl1_1", 90);

    // On subnetwork2
    TwoWindingsTransformer& t2 = addTwoWindingsTransformer(substation2, "twt2", "vl2_0", 380, "vl2_1", 90);

    assertNetworks(network, network, t0);
    assertNetworks(network, subnetwork1, t1);
    assertNetworks(network, subnetwork2, t2);
}

BOOST_AUTO_TEST_CASE(ThreeWindingsCreationTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    Substation& substation0 = addSubstation(network, "s0");
    Substation& substation1 = addSubstation(subnetwork1, "s1");
    Substation& substation2 = addSubstation(subnetwork2, "s2");
    addVoltageLevel(substation0.newVoltageLevel(), "vl0_0");
    addVoltageLevel(substation0.newVoltageLevel(), 225, "vl0_1");
    addVoltageLevel(substation0.newVoltageLevel(), 90, "vl0_2");
    addVoltageLevel(substation1.newVoltageLevel(), "vl1_0");
    addVoltageLevel(substation1.newVoltageLevel(), 225, "vl1_1");
    addVoltageLevel(substation1.newVoltageLevel(), 90, "vl1_2");
    addVoltageLevel(substation2.newVoltageLevel(), "vl2_0");
    addVoltageLevel(substation2.newVoltageLevel(), 225, "vl2_1");
    addVoltageLevel(substation2.newVoltageLevel(), 90, "vl2_2");

    // On root network
    ThreeWindingsTransformer& t0 = addThreeWindingsTransformer(substation0, "twt0", "vl0_0", 380, "vl0_1", 225, "vl0_2", 90);

    // On subnetwork1
    ThreeWindingsTransformer& t1 = addThreeWindingsTransformer(substation1, "twt1", "vl1_0", 380, "vl1_1", 225, "vl1_2", 90);

    // On subnetwork2
    ThreeWindingsTransformer& t2 = addThreeWindingsTransformer(substation2, "twt2", "vl2_0", 380, "vl2_1", 225, "vl2_2", 90);

    assertNetworks(network, network, t0);
    assertNetworks(network, subnetwork1, t1);
    assertNetworks(network, subnetwork2, t2);
}

BOOST_AUTO_TEST_CASE(ValidationLevelTest) {
    Network network = CreateSubnetworksNetworkTest();
    Network& subnetwork1 = network.getSubNetwork("Sub1").get();
    Network& subnetwork2 = network.getSubNetwork("Sub2").get();

    VoltageLevel& voltageLevel1 = addVoltageLevel(subnetwork1.newVoltageLevel().setTopologyKind(TopologyKind::BUS_BREAKER), "vl1");

    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);
    network.runValidationChecks();
    subnetwork1.runValidationChecks();
    subnetwork2.runValidationChecks();

    network.setMinimumAcceptableValidationLevel(ValidationLevel::EQUIPMENT);
    assertMinValidationLevels(ValidationLevel::EQUIPMENT, network);
    subnetwork1.setMinimumAcceptableValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS);
    assertMinValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);
    subnetwork1.setMinimumAcceptableValidationLevel(ValidationLevel::EQUIPMENT);
    assertMinValidationLevels(ValidationLevel::EQUIPMENT, network);

    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);

    voltageLevel1.newLoad()
                .setId("unchecked")
                .setP0(1.0).setQ0(1.0)
                .setBus(getBusId("vl1"))
                .setConnectableBus(getBusId("vl1"))
                .add();
    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);

    network.invalidateValidationLevel();
    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);
    subnetwork1.invalidateValidationLevel();
    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);
    subnetwork2.invalidateValidationLevel();
    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);

    Load& unchecked2 = voltageLevel1.newLoad()
                .setId("unchecked2")
                .setBus(getBusId("vl1"))
                .setConnectableBus(getBusId("vl1"))
                .add();

    assertValidationLevels(ValidationLevel::EQUIPMENT, network);
    POWSYBL_ASSERT_THROW(network.setMinimumAcceptableValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS), ValidationException, "Network 'Root': Network should be corrected in order to correspond to validation level STEADY_STATE_HYPOTHESIS");
    POWSYBL_ASSERT_THROW(subnetwork1.setMinimumAcceptableValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS), ValidationException, "Network 'Root': Network should be corrected in order to correspond to validation level STEADY_STATE_HYPOTHESIS");
    POWSYBL_ASSERT_THROW(subnetwork2.setMinimumAcceptableValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS), ValidationException, "Network 'Root': Network should be corrected in order to correspond to validation level STEADY_STATE_HYPOTHESIS");

    unchecked2.setP0(0.0).setQ0(0.0);
    assertValidationLevels(ValidationLevel::STEADY_STATE_HYPOTHESIS, network);
    subnetwork1.setMinimumAcceptableValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS);
    network.runValidationChecks();
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
