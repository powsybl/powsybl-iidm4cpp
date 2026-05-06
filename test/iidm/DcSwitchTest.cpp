/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_SUITE(DcSwitchTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1Id").setNominalV(500.).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2Id").setNominalV(500.).add();

    DcSwitch& dcSwitch1 = network.newDcSwitch()
                .setId("dcSwitch1Id")
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setOpen(false)
                .add();
    BOOST_CHECK_EQUAL(IdentifiableType::DC_SWITCH, dcSwitch1.getType());
    BOOST_CHECK_EQUAL("dcSwitch1Id", dcSwitch1.getId());
    BOOST_CHECK(!dcSwitch1.isOpen());
    BOOST_CHECK_EQUAL(DcSwitchKind::DISCONNECTOR, dcSwitch1.getKind());
    BOOST_CHECK(stdcxx::areSame(dcNode1, dcSwitch1.getDcNode1()));
    BOOST_CHECK(stdcxx::areSame(dcNode2, dcSwitch1.getDcNode2()));
    BOOST_CHECK(stdcxx::areSame(network.getDcSwitch("dcSwitch1Id"), dcSwitch1));
    BOOST_CHECK_EQUAL(1, network.getDcSwitchCount());

    DcSwitch& dcSwitch2 = network.newDcSwitch()
                .setId("dcSwitch2Id")
                .setKind(DcSwitchKind::BREAKER)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setOpen(true)
                .add();
    BOOST_CHECK_EQUAL("dcSwitch2Id", dcSwitch2.getId());
    BOOST_CHECK(dcSwitch2.isOpen());
    BOOST_CHECK_EQUAL(DcSwitchKind::BREAKER, dcSwitch2.getKind());

    BOOST_CHECK_EQUAL(2, network.getDcSwitchCount());
    POWSYBL_ASSERT_REF_TRUE(network.find("dcSwitch1Id"));
    POWSYBL_ASSERT_REF_TRUE(network.find("dcSwitch2Id"));
}


BOOST_AUTO_TEST_CASE(getterSetterTest) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1Id").setNominalV(500.).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2Id").setNominalV(500.).add();

    DcSwitch& dcSwitch = network.newDcSwitch()
                .setId("dcSwitch")
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setOpen(true)
                .add();

    BOOST_CHECK(dcSwitch.isOpen());
    dcSwitch.setOpen(false);
    BOOST_CHECK(!dcSwitch.isOpen());
}

BOOST_AUTO_TEST_CASE(testCreateDuplicate) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1Id").setNominalV(500.).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2Id").setNominalV(500.).add();
    network.newDcSwitch()
        .setId("dcSwitch")
        .setKind(DcSwitchKind::DISCONNECTOR)
        .setDcNode1(dcNode1.getId())
        .setDcNode2(dcNode2.getId())
        .setOpen(false)
        .add();

    DcSwitchAdder adder = network.newDcSwitch()
                              .setId("dcSwitch")
                              .setKind(DcSwitchKind::DISCONNECTOR)
                              .setDcNode1(dcNode1.getId())
                              .setDcNode2(dcNode2.getId())
                              .setOpen(false);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'DcSwitch' with the id 'dcSwitch'");
}

BOOST_AUTO_TEST_CASE(testRemove) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();

    DcSwitch& dcSwitch1 = network.newDcSwitch()
                .setId("dcSwitch1Id")
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setOpen(false)
                .add();
    DcSwitch& dcSwitch2 = network.newDcSwitch()
                .setId("dcSwitch2Id")
                .setKind(DcSwitchKind::DISCONNECTOR)
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setOpen(true)
                .add();
    BOOST_CHECK_EQUAL(2, network.getDcSwitchCount());
    dcSwitch1.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcSwitch1Id"));
    BOOST_CHECK_EQUAL(1, network.getDcSwitchCount());
    dcSwitch2.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("dcSwitch2Id"));
    BOOST_CHECK_EQUAL(0, network.getDcSwitchCount());

}

BOOST_AUTO_TEST_CASE(testCreationError) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    DcSwitchAdder adder = network.newDcSwitch();

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "DC Switch id is not set");
    adder.setId("dcSwitch");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': dcNode1 is not set");
    adder.setDcNode1("notExists");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': DcNode 'notExists' not found");

    adder.setDcNode1(dcNode1.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': dcNode2 is not set");

    adder.setDcNode2("notExists");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': DcNode 'notExists' not found");

    adder.setDcNode2(dcNode2.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': kind is not set");

    adder.setKind(DcSwitchKind::DISCONNECTOR);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitch': open is not set");

}

BOOST_AUTO_TEST_CASE(testNotAcrossNetworkSubnetworks) {
    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    Network& subnetwork2 = netWithSubnet.newSubnetwork("subnetwork2", "format2");
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnetwork1").setNominalV(500.).add();
    DcNode& dcNode2Subnet1 = subnetwork1.newDcNode().setId("dcNode2Subnetwork1").setNominalV(500.).add();
    DcNode& dcNode1Subnet2 = subnetwork2.newDcNode().setId("dcNode1Subnetwork2").setNominalV(500.).add();
    DcNode& dcNodeRootNetwork = netWithSubnet.newDcNode().setId("dcNodeRootNetwork").setNominalV(500.).add();

    // test cannot create DcSwitch across subnetwork1 & subnetwork2
    DcSwitchAdder adder = netWithSubnet.newDcSwitch().setId("dcSwitchAcrossSubnets")
                .setDcNode1(dcNode1Subnet1.getId())
                .setDcNode2(dcNode1Subnet2.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitchAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNode1Subnetwork2' are in different networks 'subnetwork1' and 'subnetwork2'");

    // test cannot create DcLine in netWithSubnet referencing both nodes of subnetwork1
    adder.setDcNode1(dcNode1Subnet1.getId()).setDcNode2(dcNode2Subnet1.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitchAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNode2Subnetwork1' are in network 'subnetwork1' but DC Equipment is in 'test'");

    // test cannot create DcLine across subnetwork1 & netWithSubnet
    adder.setDcNode2(dcNodeRootNetwork.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "DC Switch 'dcSwitchAcrossSubnets': DC Nodes 'dcNode1Subnetwork1' and 'dcNodeRootNetwork' are in different networks 'subnetwork1' and 'test'");

}

BOOST_AUTO_TEST_CASE(testVariant) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500).add();
    DcSwitch &dcSwitch = network.newDcSwitch()
                             .setId("dcSwitch")
                             .setKind(DcSwitchKind::DISCONNECTOR)
                             .setDcNode1(dcNode1.getId())
                             .setDcNode2(dcNode2.getId())
                             .setOpen(false)
                             .add();

    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "s1";
    std::string variant2 = "s2";
    std::string variant3 = "s3";
    std::string variant4 = "s4";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2, variant3, variant4});

    variantManager.setWorkingVariant("s4");
    // check values cloned by extend
    BOOST_CHECK(!dcSwitch.isOpen());

    // change values in s4
    dcSwitch.setOpen(true);
    BOOST_CHECK(dcSwitch.isOpen());

    // remove s2
    variantManager.removeVariant("s2");

    variantManager.cloneVariant("s4", "s2b");
    variantManager.setWorkingVariant("s2b");
    // check values cloned by allocate
    BOOST_CHECK(dcSwitch.isOpen());

    // recheck initial variant value unchanged
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!dcSwitch.isOpen());

    // remove working variant s4
    variantManager.setWorkingVariant("s4");
    variantManager.removeVariant("s4");
    POWSYBL_ASSERT_THROW(dcSwitch.isOpen(), PowsyblException, "Variant index not set");

    // check we delete a single variant's values
    variantManager.setWorkingVariant("s3");
    BOOST_CHECK(!dcSwitch.isOpen());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
