/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcLineAdder.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcLineTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();

    DcLine& dcLine1 = network.newDcLine()
                .setId("dcLine1Id")
                .setDcNode1(dcNode1.getId())
                .setConnected1(true)
                .setDcNode2(dcNode2.getId())
                .setConnected2(true)
                .setR(1.1)
                .add();

    BOOST_CHECK_EQUAL(IdentifiableType::DC_LINE, dcLine1.getType());
    BOOST_CHECK_EQUAL("dcLine1Id", dcLine1.getId());
    BOOST_CHECK(dcLine1.getDcTerminal1().isConnected());
    BOOST_CHECK(dcLine1.getDcTerminal2().isConnected());
    BOOST_CHECK_EQUAL(2 ,dcLine1.getDcTerminals().size());
    BOOST_CHECK_EQUAL(TwoSides::ONE ,dcLine1.getDcTerminal1().getSide());
    BOOST_CHECK_EQUAL(TwoSides::TWO ,dcLine1.getDcTerminal2().getSide());
    BOOST_CHECK_EQUAL(TerminalNumber::UNDEFINED ,dcLine1.getDcTerminal1().getTerminalNumber());
    BOOST_CHECK_EQUAL(TerminalNumber::UNDEFINED ,dcLine1.getDcTerminal2().getTerminalNumber());
    BOOST_CHECK(stdcxx::areSame(dcLine1.getDcTerminals().at(0).get(), dcLine1.getDcTerminal1()));
    BOOST_CHECK(stdcxx::areSame(dcLine1.getDcTerminals().at(1).get(), dcLine1.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(dcLine1, dcLine1.getDcTerminal1().getDcConnectable().get()));
    BOOST_CHECK(stdcxx::areSame(dcLine1, dcLine1.getDcTerminal2().getDcConnectable().get()));
    BOOST_CHECK(stdcxx::areSame(dcNode1, dcLine1.getDcTerminal1().getDcNode()));
    BOOST_CHECK(stdcxx::areSame(dcNode2, dcLine1.getDcTerminal2().getDcNode()));
    BOOST_CHECK_EQUAL(1, network.getDcLineCount());
    BOOST_CHECK_CLOSE(1.1, dcLine1.getR(), std::numeric_limits<double>::epsilon());

    DcLine& dcLine2 = network.newDcLine()
                .setId("dcLine2Id")
                .setDcNode1(dcNode1.getId())
                .setConnected1(false)
                .setDcNode2(dcNode2.getId())
                .setConnected2(false)
                .setR(1.2)
                .add();
    BOOST_CHECK_EQUAL("dcLine2Id", dcLine2.getId());
    BOOST_CHECK_CLOSE(1.2, dcLine2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!dcLine2.getDcTerminal1().isConnected());
    BOOST_CHECK(!dcLine2.getDcTerminal2().isConnected());
    BOOST_CHECK(stdcxx::areSame(dcNode1, dcLine2.getDcTerminal1().getDcNode()));
    BOOST_CHECK(stdcxx::areSame(dcNode2, dcLine2.getDcTerminal2().getDcNode()));

    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    POWSYBL_ASSERT_REF_TRUE(network.find("dcLine1Id"));
    POWSYBL_ASSERT_REF_TRUE(network.find("dcLine2Id"));
}


BOOST_AUTO_TEST_CASE(getterSetterTest) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    
    DcLine& dcLine = network.newDcLine()
                .setId("dcLine")
                .setDcNode1(dcNode1.getId())
                .setConnected1(true)
                .setDcNode2(dcNode2.getId())
                .setConnected2(true)
                .setR(1.1)
                .add();

    BOOST_CHECK_CLOSE(1.1, dcLine.getR(), std::numeric_limits<double>::epsilon());
    dcLine.setR(1.2);
    BOOST_CHECK_CLOSE(1.2, dcLine.getR(), std::numeric_limits<double>::epsilon());
    dcLine.setR(0.0);
    BOOST_CHECK_EQUAL(0.0, dcLine.getR());

    POWSYBL_ASSERT_THROW(dcLine.setR(stdcxx::nan()), ValidationException, "DC Line 'dcLine': r is invalid");
    POWSYBL_ASSERT_THROW(dcLine.setR(-1.0), ValidationException, "DC Line 'dcLine': r is invalid");
}

BOOST_AUTO_TEST_CASE(testCreateDuplicate) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    
    network.newDcLine()
                .setId("dcLine")
                .setDcNode1(dcNode1.getId())
                .setConnected1(true)
                .setDcNode2(dcNode2.getId())
                .setConnected2(true)
                .setR(1.1)
                .add();
    DcLineAdder adder = network.newDcLine()
                .setId("dcLine")
                .setDcNode1(dcNode1.getId())
                .setConnected1(false)
                .setDcNode2(dcNode2.getId())
                .setConnected2(false)
                .setR(1.2);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'DcLine' with the id 'dcLine'");
}

BOOST_AUTO_TEST_CASE(testRemove) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    
    DcLine& dcLine1 = network.newDcLine()
                .setId("dcLine1Id")
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setR(1.1)
                .add();
    DcLine& dcLine2 = network.newDcLine()
                .setId("dcLine2Id")
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setR(1.2)
                .add();
    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    dcLine1.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcLine1Id"));
    BOOST_CHECK_EQUAL(1, network.getDcLineCount());
    dcLine2.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcLine2Id"));
    BOOST_CHECK_EQUAL(0, network.getDcLineCount());
}

BOOST_AUTO_TEST_CASE(testCreationError) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    DcLineAdder adder = network.newDcLine();

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "DC Line id is not set");
    adder.setId("dcLine");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': dcNode1 is not set");
    adder.setDcNode1("notExists");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': DcNode 'notExists' not found");
    adder.setDcNode1(dcNode1.getId());

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': dcNode2 is not set");
    adder.setDcNode2("notExists");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': DcNode 'notExists' not found");
    adder.setDcNode2(dcNode2.getId());
    
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': r is invalid");
    adder.setR(-1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLine': r is invalid");
}

BOOST_AUTO_TEST_CASE(testNotAcrossNetworkSubnetworks) {
    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    Network& subnetwork2 = netWithSubnet.newSubnetwork("subnetwork2", "format2");
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnetwork1").setNominalV(500.).add();
    DcNode& dcNode2Subnet1 = subnetwork1.newDcNode().setId("dcNode2Subnetwork1").setNominalV(500.).add();
    DcNode& dcNode1Subnet2 = subnetwork2.newDcNode().setId("dcNode1Subnetwork2").setNominalV(500.).add();
    DcNode& dcNodeRootNetwork = netWithSubnet.newDcNode().setId("dcNodeRootNetwork").setNominalV(500.).add();

    // test cannot create DcLine across subnetwork1 & subnetwork2
    DcLineAdder adder = netWithSubnet.newDcLine().setId("dcLineAcrossSubnets")
                .setDcNode1(dcNode1Subnet1.getId())
                .setDcNode2(dcNode1Subnet2.getId())
                .setR(1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLineAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNode1Subnetwork2' are in different networks 'subnetwork1' and 'subnetwork2'");

    // test cannot create DcLine in netWithSubnet referencing both nodes of subnetwork1
    adder.setDcNode1(dcNode1Subnet1.getId()).setDcNode2(dcNode2Subnet1.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLineAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNode2Subnetwork1' are in network 'subnetwork1' but DC Equipment is in 'test'");

    // test cannot create DcLine across subnetwork1 & netWithSubnet
    adder.setDcNode2(dcNodeRootNetwork.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Line 'dcLineAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNodeRootNetwork' are in different networks 'subnetwork1' and 'test'");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
