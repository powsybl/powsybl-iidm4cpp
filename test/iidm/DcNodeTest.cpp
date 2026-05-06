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
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcSwitchAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcNodeTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1Id").setNominalV(500.).add();
    BOOST_CHECK_EQUAL(IdentifiableType::DC_NODE, dcNode1.getType());
    BOOST_CHECK_EQUAL("dcNode1Id", dcNode1.getId());
    BOOST_CHECK_CLOSE(500.0, dcNode1.getNominalV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(network.getDcNode("dcNode1Id"), dcNode1));
    BOOST_CHECK_EQUAL(1, network.getDcNodeCount());

    DcNode& dcNode2 = network.newDcNode().setId("dcNode2Id").setNominalV(510.).add();
    BOOST_CHECK_EQUAL(dcNode2.getId(), "dcNode2Id");
    BOOST_CHECK_CLOSE(510., dcNode2.getNominalV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(network.getDcNode("dcNode2Id"), dcNode2));

    BOOST_CHECK_EQUAL(2, network.getDcNodeCount());
    POWSYBL_ASSERT_REF_TRUE(network.find("dcNode1Id"));
    POWSYBL_ASSERT_REF_TRUE(network.find("dcNode2Id"));
}


BOOST_AUTO_TEST_CASE(getterSetterTest) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(500.).add();

    BOOST_CHECK_CLOSE(500, dcNode.getNominalV(), std::numeric_limits<double>::epsilon());
    dcNode.setNominalV(510);
    BOOST_CHECK_CLOSE(510, dcNode.getNominalV(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(dcNode.setNominalV(stdcxx::nan()), ValidationException, "DC Node 'dcNode': Nominal voltage is undefined");
    POWSYBL_ASSERT_THROW(dcNode.setNominalV(0.0), ValidationException, "DC Node 'dcNode': Nominal voltage is <= 0");
    POWSYBL_ASSERT_THROW(dcNode.setNominalV(-1.0), ValidationException, "DC Node 'dcNode': Nominal voltage is <= 0");
}

BOOST_AUTO_TEST_CASE(testCreateDuplicate) {
    Network network("test", "test");
    network.newDcNode().setId("dcNode").setNominalV(500).add();
    
    DcNodeAdder adder = network.newDcNode()
                .setId("dcNode")
                .setNominalV(1.2);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'DcNode' with the id 'dcNode'");
}

BOOST_AUTO_TEST_CASE(testRemove) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();

    BOOST_CHECK_EQUAL(2, network.getDcNodeCount());
    dcNode1.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcNode1"));
    BOOST_CHECK_EQUAL(1, network.getDcNodeCount());
    dcNode2.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcNode2"));
    BOOST_CHECK_EQUAL(0, network.getDcNodeCount());
}

BOOST_AUTO_TEST_CASE(testCreationError) {
    Network network("test", "test");
    DcNodeAdder adder = network.newDcNode();

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "DC Node id is not set");
    adder.setId("dcNode");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Node 'dcNode': Nominal voltage is undefined");
    adder.setNominalV(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Node 'dcNode': Nominal voltage is undefined");
    adder.setNominalV(0.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Node 'dcNode': Nominal voltage is <= 0");
    adder.setNominalV(-1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Node 'dcNode': Nominal voltage is <= 0");

}

BOOST_AUTO_TEST_CASE(testRemoveStillReferenced) {
    Network network("test", "test");
    DcNode& dcNode = network.newDcNode().setId("dcNode").setNominalV(500.).add();

    DcGround& dcGround = network.newDcGround().setId("dcGround").setDcNode(dcNode.getId()).add();
    DcSwitch& dcSwitch = network.newDcSwitch().setId("dcSwitch")
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setDcNode1(dcNode.getId())
                .setDcNode2(dcNode.getId())
                .setOpen(false)
                .add();

    POWSYBL_ASSERT_THROW(dcNode.remove(), PowsyblException, "Cannot remove DC Node 'dcNode' because DC Connectable 'dcGround' is connected to it");
    dcGround.remove();
    POWSYBL_ASSERT_THROW(dcNode.remove(), PowsyblException, "Cannot remove DC Node 'dcNode' because DC Switch 'dcSwitch' is connected to it");
    dcSwitch.remove();

    dcNode.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcNode"));
    BOOST_CHECK_EQUAL(0, network.getDcNodeCount());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
