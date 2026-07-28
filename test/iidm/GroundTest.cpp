/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/GroundAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(GroundTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createTwoVoltageLevelNetwork();
    
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    VoltageLevel& vl2 = network.getVoltageLevel("VL2");

    vl1.getNodeBreakerView().newDisconnector()
            .setId("D_1_6")
            .setOpen(false)
            .setNode1(1)
            .setNode2(6)
            .add();

    Ground& groundNB = vl1.newGround()
            .setId("GroundNB")
            .setNode(6)
            .setEnsureIdUnicity(true)
            .add();

    Ground& groundBB = vl2.newGround()
        .setId("GroundBB")
        .setBus("BUS2")
        .setEnsureIdUnicity(true)
        .add();

    BOOST_CHECK_EQUAL(IdentifiableType::GROUND, groundNB.getType());
    BOOST_CHECK_EQUAL(IdentifiableType::GROUND, groundBB.getType());

    BOOST_CHECK(stdcxx::areSame(vl1, groundNB.getTerminal().getVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(vl2, groundBB.getTerminal().getVoltageLevel()));
    BOOST_CHECK_EQUAL("GroundNB", groundNB.getId());
    BOOST_CHECK_EQUAL("GroundBB", groundBB.getId());
    BOOST_CHECK(stdcxx::areSame(network, groundNB.getNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, groundBB.getNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, groundNB.getParentNetwork()));
    BOOST_CHECK(stdcxx::areSame(network, groundBB.getParentNetwork()));
    BOOST_CHECK_EQUAL(2, network.getGroundCount());
    BOOST_CHECK_EQUAL(2, boost::size(network.getGrounds()));
    BOOST_CHECK_EQUAL(2, boost::size(network.getIdentifiables(IdentifiableType::GROUND)));
    BOOST_CHECK_EQUAL(1, vl1.getGroundCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl1.getGrounds()));
    BOOST_CHECK_EQUAL(1, vl2.getGroundCount());
    BOOST_CHECK_EQUAL(1, boost::size(vl2.getGrounds()));

    BOOST_CHECK(stdcxx::areSame(groundNB, network.getGround("GroundNB")));
    POWSYBL_ASSERT_REF_FALSE(network.find<Ground>("GroundNB2"));
}

BOOST_AUTO_TEST_CASE(createOnSubnetwork) {
    Network network = createTwoVoltageLevelNetwork();

    Network& subnetwork1 = network.newSubnetwork("Sub1", "format1");
    Network &subnetwork2 = network.newSubnetwork("Sub2", "format2");
    Substation &substation1 = subnetwork1.newSubstation().setId("Sub1_S").setCountry(Country::FR).add();
    Substation &substation2 = subnetwork2.newSubstation().setId("Sub2_S").setCountry(Country::FR).add();
    VoltageLevel &vl1_1 = substation1.newVoltageLevel().setId("Sub1_VL1").setNominalV(400.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    VoltageLevel &vl1_2 = substation1.newVoltageLevel().setId("Sub1_VL2").setNominalV(220.0).setTopologyKind(TopologyKind::BUS_BREAKER).add();
    VoltageLevel &vl2_1 = substation2.newVoltageLevel().setId("Sub2_VL1").setNominalV(400.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    VoltageLevel &vl2_2 = substation2.newVoltageLevel().setId("Sub2_VL2").setNominalV(220.0).setTopologyKind(TopologyKind::BUS_BREAKER).add();
    vl1_1.getNodeBreakerView().newBusbarSection().setId("Sub1_BBS1").setNode(0).add();
    vl1_1.getNodeBreakerView().newBusbarSection().setId("Sub1_BBS2").setNode(1).add();
    vl1_2.getBusBreakerView().newBus().setId("Sub1_BUS1").add();
    vl1_2.getBusBreakerView().newBus().setId("Sub1_BUS2").add();
    vl2_1.getNodeBreakerView().newBusbarSection().setId("Sub2_BBS1").setNode(0).add();
    vl2_1.getNodeBreakerView().newBusbarSection().setId("Sub2_BBS2").setNode(1).add();
    vl2_2.getBusBreakerView().newBus().setId("Sub2_BUS1").add();
    vl2_2.getBusBreakerView().newBus().setId("Sub2_BUS2").add();

    vl1_1.getNodeBreakerView().newDisconnector().setId("D_1_6").setOpen(false).setNode1(1).setNode2(6).add();
    vl1_1.getNodeBreakerView().newDisconnector().setId("D_2_7").setOpen(false).setNode1(2).setNode2(7).add();

    Ground& groundNBSub16 = vl1_1.newGround()
        .setEnsureIdUnicity(true)
        .setId("GroundNBSub1_6")
        .setNode(6)
        .add();
    Ground& groundNBSub17 = vl1_1.newGround()
        .setEnsureIdUnicity(true)
        .setId("GroundNBSub1_7")
        .setNode(7)
        .add();
    vl1_2.newGround()
        .setEnsureIdUnicity(true)
        .setId("GroundBBSub1_2")
        .setBus("Sub1_BUS2")
        .add();

    BOOST_CHECK(stdcxx::areSame(groundNBSub16, subnetwork1.getGround("GroundNBSub1_6")));
    BOOST_CHECK(stdcxx::areSame(groundNBSub17, network.getGround("GroundNBSub1_7")));
    BOOST_CHECK_EQUAL(3, network.getGroundCount());
    BOOST_CHECK_EQUAL(3, boost::size(network.getGrounds()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getIdentifiables(IdentifiableType::GROUND)));
    BOOST_CHECK_EQUAL(3, subnetwork1.getGroundCount());
    BOOST_CHECK_EQUAL(3, boost::size(subnetwork1.getGrounds()));
    BOOST_CHECK_EQUAL(3, boost::size(subnetwork1.getIdentifiables(IdentifiableType::GROUND)));
    BOOST_CHECK_EQUAL(0, subnetwork2.getGroundCount());
    BOOST_CHECK(subnetwork2.getGrounds().empty());
    BOOST_CHECK(subnetwork2.getIdentifiables(IdentifiableType::GROUND).empty());
}

BOOST_AUTO_TEST_CASE(sameIds) {
    Network network = createTwoVoltageLevelNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    VoltageLevel& vl2 = network.getVoltageLevel("VL2");

    // Create first grounds
    vl1.getNodeBreakerView().newDisconnector().setId("D_1_6").setOpen(false).setNode1(1).setNode2(6).add();
    vl1.newGround().setId("Ground").setNode(6).add();
    vl2.newGround().setId("Ground1").setBus("BUS1").add();

    GroundAdder adder1 = vl2.newGround();
    adder1.setId("Ground").setBus("BUS1");
    POWSYBL_ASSERT_THROW(adder1.add(), PowsyblException, "The network test already contains an object 'Ground' with the id 'Ground'");
    GroundAdder adder2 = vl1.newGround();
    adder2.setId("Ground1").setNode(7);
    POWSYBL_ASSERT_THROW(adder2.add(), PowsyblException, "The network test already contains an object 'Ground' with the id 'Ground1'");
}

BOOST_AUTO_TEST_CASE(fictitiousGoround) {
    Network network = createTwoVoltageLevelNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    vl1.getNodeBreakerView().newDisconnector().setId("D_1_6").setOpen(false).setNode1(1).setNode2(6).add();
    Ground& ground = vl1.newGround().setId("Ground").setNode(6).add();

    BOOST_CHECK(!ground.isFictitious());
    ground.setFictitious(false);
    BOOST_CHECK(!ground.isFictitious());
    POWSYBL_ASSERT_THROW(ground.setFictitious(true), PowsyblException, "The ground cannot be fictitious.");
}

BOOST_AUTO_TEST_CASE(creationErrors) {
    Network network = createTwoVoltageLevelNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    VoltageLevel& vl2 = network.getVoltageLevel("VL2");

    vl1.getNodeBreakerView().newDisconnector().setId("D_1_6").setOpen(false).setNode1(1).setNode2(6).add();

    //in NB need a node
    GroundAdder groundAdder = vl1.newGround();
    groundAdder.setId("Ground").setEnsureIdUnicity(true);
    POWSYBL_ASSERT_THROW(groundAdder.add(), PowsyblException, "Ground 'Ground': connectable bus is not set");
    groundAdder.setNode(6).add();
    GroundAdder groundAdder1 = vl1.newGround();
    groundAdder1.setNode(6).setEnsureIdUnicity(true);
    POWSYBL_ASSERT_THROW(groundAdder1.add(), PowsyblException, "Ground id is not set");
    groundAdder1.setId("Ground");
    POWSYBL_ASSERT_THROW(groundAdder1.add(), ValidationException, "Ground 'Ground#1': An equipment (Ground#0) is already connected to the node 6 of voltage level VL1");

    //in BB need a bus
    GroundAdder groundAdder2 = vl2.newGround();
    groundAdder2.setId("Ground").setEnsureIdUnicity(true);
    POWSYBL_ASSERT_THROW(groundAdder2.add(), PowsyblException, "Ground 'Ground': connectable bus is not set");
    groundAdder2.setBus("BUS1").add();
    GroundAdder groundAdder3 = vl2.newGround();
    groundAdder3.setBus("BUS1").setEnsureIdUnicity(true);
    POWSYBL_ASSERT_THROW(groundAdder3.add(), PowsyblException, "Ground id is not set");

    //bus in NB or node in BB:
    GroundAdder groundAdder4 = vl1.newGround();
    groundAdder4.setId("Ground").setEnsureIdUnicity(true)
        .setBus("BUS1");
    POWSYBL_ASSERT_THROW(groundAdder4.add(), ValidationException, "Ground 'Ground#4': Voltage level VL1 has a node/breaker topology, a node connection should be specified instead of a bus connection");
    GroundAdder groundAdder5 = vl2.newGround();
    groundAdder5.setId("Ground").setEnsureIdUnicity(true)
        .setNode(7);
    POWSYBL_ASSERT_THROW(groundAdder5.add(), ValidationException, "Ground 'Ground#5': Voltage level 'VL2' has a bus/breaker topology, a bus connection should be specified instead of a node connection");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
