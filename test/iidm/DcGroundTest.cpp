/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcGroundTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(1).add();

    DcGround& dcGround1 = network.newDcGround()
                .setId("dcGround1Id")
                .setDcNode(dcNode.getId())
                .setConnected(true)
                .setR(0.1)
                .add();

    BOOST_CHECK_EQUAL(IdentifiableType::DC_GROUND, dcGround1.getType());
    BOOST_CHECK_EQUAL("dcGround1Id", dcGround1.getId());
    BOOST_CHECK(dcGround1.getDcTerminal().isConnected());
    BOOST_CHECK_EQUAL(TwoSides::UNDEFINED ,dcGround1.getDcTerminal().getSide());
    BOOST_CHECK_EQUAL(1, dcGround1.getDcTerminals().size());
    BOOST_CHECK(stdcxx::areSame(dcGround1.getDcTerminals().at(0).get(), dcGround1.getDcTerminal()));
    BOOST_CHECK(stdcxx::areSame(dcGround1, dcGround1.getDcTerminal().getDcConnectable().get()));
    BOOST_CHECK(stdcxx::areSame(dcNode, dcGround1.getDcTerminal().getDcNode()));
    BOOST_CHECK_EQUAL(1, network.getDcGroundCount());
    BOOST_CHECK_CLOSE(0.1, dcGround1.getR(), std::numeric_limits<double>::epsilon());

    DcGround& dcGround2 = network.newDcGround()
                .setId("dcGround2Id")
                .setDcNode(dcNode.getId())
                .setConnected(false)
                .setR(0.2)
                .add();
    BOOST_CHECK_EQUAL("dcGround2Id", dcGround2.getId());
    BOOST_CHECK_CLOSE(0.2, dcGround2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!dcGround2.getDcTerminal().isConnected());
    BOOST_CHECK(stdcxx::areSame(dcNode, dcGround2.getDcTerminal().getDcNode()));

    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    POWSYBL_ASSERT_REF_TRUE(network.find("dcGround1Id"));
    POWSYBL_ASSERT_REF_TRUE(network.find("dcGround2Id"));

}


BOOST_AUTO_TEST_CASE(getterSetterTest) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(1).add();
    
    DcGround& dcGround = network.newDcGround()
                .setId("dcGround")
                .setDcNode(dcNode.getId())
                .setConnected(true)
                .setR(0.1)
                .add();
    BOOST_CHECK_CLOSE(0.1, dcGround.getR(), std::numeric_limits<double>::epsilon());
    dcGround.setR(0.2);
    BOOST_CHECK_CLOSE(0.2, dcGround.getR(), std::numeric_limits<double>::epsilon());
    dcGround.setR(0.0);
    BOOST_CHECK_EQUAL(0.0, dcGround.getR());

    POWSYBL_ASSERT_THROW(dcGround.setR(stdcxx::nan()), ValidationException, "DC Ground 'dcGround': r is invalid");
    POWSYBL_ASSERT_THROW(dcGround.setR(-1.0), ValidationException, "DC Ground 'dcGround': r is invalid");
}

BOOST_AUTO_TEST_CASE(testCreateDuplicate) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(1).add();
    
    network.newDcGround()
                .setId("dcGround")
                .setDcNode(dcNode.getId())
                .setConnected(true)
                .setR(0.1)
                .add();
    DcGroundAdder adder = network.newDcGround()
                .setId("dcGround")
                .setDcNode(dcNode.getId())
                .setConnected(true)
                .setR(0.1);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'DcGround' with the id 'dcGround'");
}

BOOST_AUTO_TEST_CASE(testRemove) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(1).add();
    
    DcGround& dcGround1 = network.newDcGround()
                .setId("dcGround1Id")
                .setDcNode(dcNode.getId())
                .setR(1.1)
                .add();
    DcGround& dcGround2 = network.newDcGround()
                .setId("dcGround2Id")
                .setDcNode(dcNode.getId())
                .setR(1.2)
                .add();
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    dcGround1.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcGround1Id"));
    BOOST_CHECK_EQUAL(1, network.getDcGroundCount());
    dcGround2.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcGround2Id"));
    BOOST_CHECK_EQUAL(0, network.getDcGroundCount());
}

BOOST_AUTO_TEST_CASE(testCreationError) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(1).add();
    DcGroundAdder adder = network.newDcGround();

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "DC Ground id is not set");
    adder.setId("dcGround");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Ground 'dcGround': dcNode is not set");
    adder.setDcNode("notExists");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Ground 'dcGround': DcNode 'notExists' not found");
    adder.setDcNode(dcNode.getId());
    
    adder.setR(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Ground 'dcGround': r is invalid");
    adder.setR(-1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Ground 'dcGround': r is invalid");
}

BOOST_AUTO_TEST_CASE(testNotAcrossNetworkSubnetworks) {
    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnetwork1").setNominalV(1.).add();
    DcNode& dcNodeRootNetwork = netWithSubnet.newDcNode().setId("dcNodeRootNetwork").setNominalV(1.).add();

    // test cannot create DcGround in netWithSubnet referencing nodes of subnetwork1
    DcGroundAdder adder1 = netWithSubnet.newDcGround().setId("dcGround1")
                .setDcNode(dcNode1Subnet1.getId());
    POWSYBL_ASSERT_THROW(adder1.add(), ValidationException, "DC Ground 'dcGround1': DC Node 'dcNode1Subnetwork1' is in network 'subnetwork1' but DC Equipment is in 'test'");

    // test cannot create DcGround in subnetwork1 referencing nodes of netWithSubnet
    DcGroundAdder adder2 = subnetwork1.newDcGround().setId("dcGround2")
                .setDcNode(dcNodeRootNetwork.getId());
    POWSYBL_ASSERT_THROW(adder2.add(), ValidationException, "DC Ground 'dcGround2': DC Node 'dcNodeRootNetwork' is in network 'test' but DC Equipment is in 'subnetwork1'");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
