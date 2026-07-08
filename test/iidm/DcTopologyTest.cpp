/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/DcSwitchAdder.hpp>
#include <powsybl/iidm/DcTopologyVisitor.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>


#include <powsybl/network/DcDetailedNetworkFactory.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcTopologyTestSuite)


void assertDcBusesAre(const Network& network, const std::set<std::string>& expectedDcBusesIds) {
    BOOST_CHECK_EQUAL(network.getDcBusCount(), expectedDcBusesIds.size());

    std::set<std::string> dcBusesIds;
    auto dcBuses = network.getDcBuses();
    for (const auto& dcBus : dcBuses) {
        dcBusesIds.emplace(dcBus.getId());
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(expectedDcBusesIds.cbegin(), expectedDcBusesIds.cend(), dcBusesIds.cbegin(), dcBusesIds.cend());
}

void assertDcBusContainsDcNodes(const DcBus& dcBus, const std::set<std::string>& expectedDcNodesIds) {
    auto dcNodes = dcBus.getDcNodes();
    BOOST_CHECK_EQUAL(boost::size(dcNodes), expectedDcNodesIds.size());

    std::set<std::string> dcNodesIds;
    for (const auto& dcNode : dcNodes) {
        dcNodesIds.emplace(dcNode.getId());
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(expectedDcNodesIds.cbegin(), expectedDcNodesIds.cend(), dcNodesIds.cbegin(), dcNodesIds.cend());
}

void assertComponent(const Component& component, unsigned long expectedNum, const std::set<std::string>& expectedAcBusesIds, const std::set<std::string>& expectedDcBusesIds) {

    BOOST_CHECK_EQUAL(expectedNum, component.getNum());
    BOOST_CHECK_EQUAL(expectedAcBusesIds.size() + expectedDcBusesIds.size(), component.getSize());

    auto acBuses = component.getBuses();
    auto dcBuses = component.getDcBuses();
    std::set<std::string> acBusesIds;
    for (const auto& acBus : acBuses) {
        acBusesIds.emplace(acBus.getId());
    }
    std::set<std::string> dcBusesIds;
    for (const auto& dcBus : dcBuses) {
        dcBusesIds.emplace(dcBus.getId());
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAcBusesIds.cbegin(), expectedAcBusesIds.cend(), acBusesIds.cbegin(), acBusesIds.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedDcBusesIds.cbegin(), expectedDcBusesIds.cend(), dcBusesIds.cbegin(), dcBusesIds.cend());

}

BOOST_AUTO_TEST_CASE(testBasicDcBusTopology) {
    Network net1("n1", "test");

    DcNode& n11 = net1.newDcNode().setId("n11").setNominalV(500.).add();
    DcGround& n11g = net1.newDcGround().setId("n11g").setDcNode(n11.getId()).add();
    assertDcBusesAre(net1, {"n11_dcBus"});
    BOOST_CHECK_EQUAL(IdentifiableType::DC_BUS, net1.getDcBus("n11_dcBus").get().getType());
    std::ostringstream oss;
    oss << net1.getDcBus("n11_dcBus").get().getType();
    BOOST_CHECK_EQUAL("DC_BUS", oss.str());
    BOOST_CHECK(n11.getDcBus() ==  net1.getDcBus("n11_dcBus"));
    assertDcBusContainsDcNodes(net1.getDcBus("n11_dcBus"), {"n11"});

    DcNode& n12 = net1.newDcNode().setId("n12").setNominalV(500.).add();
    net1.newDcGround().setId("n12g").setDcNode(n12.getId()).add();
    assertDcBusesAre(net1, {"n11_dcBus", "n12_dcBus"});
    assertDcBusContainsDcNodes(net1.getDcBus("n11_dcBus"), {"n11"});
    assertDcBusContainsDcNodes(net1.getDcBus("n12_dcBus"), {"n12"});

    DcSwitch& s1112 = net1.newDcSwitch().setId("s11-12").setKind(DcSwitchKind::BREAKER).setOpen(true).setDcNode1(n11.getId()).setDcNode2(n12.getId()).add();
    assertDcBusesAre(net1, {"n11_dcBus", "n12_dcBus"});

    s1112.setOpen(false); //Connect
    assertDcBusesAre(net1, {"n11_dcBus"});
    BOOST_CHECK(n11.getDcBus() == net1.getDcBus("n11_dcBus"));
    BOOST_CHECK(n12.getDcBus() == net1.getDcBus("n11_dcBus"));
    assertDcBusContainsDcNodes(net1.getDcBus("n11_dcBus"), {"n11", "n12"});

    s1112.remove(); //Disconnect
    assertDcBusesAre(net1, {"n11_dcBus", "n12_dcBus"});
    BOOST_CHECK(n11.getDcBus() == net1.getDcBus("n11_dcBus"));
    BOOST_CHECK(n12.getDcBus() == net1.getDcBus("n12_dcBus"));

    n11g.remove();
    n11.remove();
    assertDcBusesAre(net1, {"n12_dcBus"});
}

BOOST_AUTO_TEST_CASE(testMultiVariantDcBusV) {
    Network net1("n1", "test");
    auto& variantManager = net1.getVariantManager();

    DcNode& n11 = net1.newDcNode().setId("n11").setNominalV(500.).add();
    net1.newDcGround().setId("n11g").setDcNode(n11.getId()).add();
    DcNode& n12 = net1.newDcNode().setId("n12").setNominalV(500.).add();
    net1.newDcGround().setId("n12g").setDcNode(n12.getId()).add();
    DcSwitch& s1112 = net1.newDcSwitch().setId("s11-12")
                .setKind(DcSwitchKind::BREAKER)
                .setOpen(true)
                .setDcNode1(n11.getId()).setDcNode2(n12.getId())
                .add();
    BOOST_CHECK_EQUAL(2, net1.getDcBusCount());
    DcBus& n11DcBus = net1.getDcBus("n11_dcBus").get();
    DcBus& n12DcBus = net1.getDcBus("n12_dcBus").get();
    assertDcBusContainsDcNodes(n11DcBus, {"n11"});
    assertDcBusContainsDcNodes(n12DcBus, {"n12"});
    BOOST_CHECK(std::isnan(n11DcBus.getV()));
    BOOST_CHECK(std::isnan(n11.getV()));
    BOOST_CHECK(std::isnan(n12DcBus.getV()));
    BOOST_CHECK(std::isnan(n12.getV()));
    n11DcBus.setV(501.);
    n12.setV(-502.);
    BOOST_CHECK_CLOSE(501., n11DcBus.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(501., n11.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12DcBus.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12.getV(), std::numeric_limits<double>::epsilon());

    variantManager.cloneVariant(variantManager.getInitialVariantId(), "v1");
    variantManager.setWorkingVariant("v1");
    DcBus& n11DcBus_v1 = net1.getDcBus("n11_dcBus").get();
    DcBus& n12DcBus_v1 = net1.getDcBus("n12_dcBus").get();
    BOOST_CHECK(s1112.isOpen());
    BOOST_CHECK_CLOSE(501., n11DcBus_v1.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(501., n11.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12DcBus_v1.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12.getV(), std::numeric_limits<double>::epsilon());

    s1112.setOpen(false); //Should only impact Dc Topology of Variant v1:
    BOOST_CHECK_EQUAL(1, net1.getDcBusCount());
    DcBus& n11DcBus_v1_bis = net1.getDcBus("n11_dcBus").get();
    n11DcBus_v1_bis.setV(504.);
    BOOST_CHECK_CLOSE(504., n11DcBus_v1_bis.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(504., n11.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(504., n12.getV(), std::numeric_limits<double>::epsilon());

    // back to original variant
    variantManager.setWorkingVariant(variantManager.getInitialVariantId()); //should not have been modified (cache not even invalidated, thus DcBus ref should still be valid)
    DcBus& n11DcBus_init = net1.getDcBus("n11_dcBus");
    DcBus& n12DcBus_init = net1.getDcBus("n12_dcBus");
    BOOST_CHECK(stdcxx::areSame(n11DcBus_init, n11DcBus));
    BOOST_CHECK(stdcxx::areSame(n12DcBus_init, n12DcBus));
    BOOST_CHECK(s1112.isOpen());
    BOOST_CHECK_CLOSE(501., n11DcBus_init.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(501., n11.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12DcBus_init.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-502., n12.getV(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_CASE(testDcBusInvalidation) {
    Network net1("n1", "test");

    DcNode& n11 = net1.newDcNode().setId("n11").setNominalV(500.).add();
    net1.newDcGround().setId("n11g").setDcNode(n11.getId()).add();
    DcNode& n12 = net1.newDcNode().setId("n12").setNominalV(500.).add();
    net1.newDcGround().setId("n12g").setDcNode(n12.getId()).add();
    DcSwitch& s1112 = net1.newDcSwitch().setId("s11-12").setKind(DcSwitchKind::BREAKER).setOpen(true).setDcNode1(n11.getId()).setDcNode2(n12.getId()).add();

    assertDcBusesAre(net1, {"n11_dcBus", "n12_dcBus"});
    auto dcBus = net1.getDcBus("n11_dcBus");
    POWSYBL_ASSERT_REF_TRUE(dcBus);
    s1112.setOpen(false); // closing the switch, this invalidates the topology, DcBuses are invalidated and cache emptied :
    //DcBus pointer has even been deleted -> dcBus is a dangling reference and should not been used anymore
    POWSYBL_ASSERT_THROW(dcBus.get().getV(), PowsyblException, "DcBus has been invalidated");
}


BOOST_AUTO_TEST_CASE(testSubnetworksTopology) {
    Network network("rootNetwork", "test");
    
    DcNode& n0 = network.newDcNode().setId("n0").setNominalV(500.).add(); 
    //Node without terminal -> no DcBus
    BOOST_CHECK_EQUAL(0, network.getDcBusCount());
    POWSYBL_ASSERT_REF_FALSE(n0.getDcBus());
    std::set<std::string> networkDcBuses;

    Network& net1 = network.newSubnetwork("n1", "test");
    DcNode& n11 = net1.newDcNode().setId("n11").setNominalV(500.).add();
    net1.newDcGround().setId("n11g").setDcNode(n11.getId()).add();
    networkDcBuses.emplace("n11_dcBus");
    DcNode& n12 = net1.newDcNode().setId("n12").setNominalV(500.).add();
    net1.newDcGround().setId("n12g").setDcNode(n12.getId()).add();
    networkDcBuses.emplace("n12_dcBus");

    assertDcBusesAre(net1, {"n11_dcBus", "n12_dcBus"});
    assertDcBusesAre(network, networkDcBuses);
    
    net1.newDcSwitch().setId("s11-12")
                .setKind(DcSwitchKind::BREAKER)
                .setOpen(false)
                .setDcNode1(n11.getId()).setDcNode2(n12.getId())
                .add();
    networkDcBuses.erase("n12_dcBus");

    assertDcBusesAre(net1, {"n11_dcBus"});
    assertDcBusesAre(network, networkDcBuses);

    Network& net2 = network.newSubnetwork("n2", "test");
    DcNode& n21 = net2.newDcNode().setId("n21").setNominalV(500.).add();
    net2.newDcGround().setId("n21g").setDcNode(n21.getId()).add();
    networkDcBuses.emplace("n21_dcBus");
    DcNode& n22 = net2.newDcNode().setId("n22").setNominalV(500.).add();
    net2.newDcGround().setId("n22g").setDcNode(n22.getId()).add();
    networkDcBuses.emplace("n22_dcBus");
    assertDcBusesAre(net2, {"n21_dcBus", "n22_dcBus"});
    assertDcBusesAre(network, networkDcBuses);
    net2.newDcSwitch().setId("s21-22")
                .setKind(DcSwitchKind::BREAKER)
                .setOpen(true)
                .setDcNode1(n21.getId()).setDcNode2(n22.getId())
                .add();
    assertDcBusesAre(net2, {"n21_dcBus", "n22_dcBus"});
    assertDcBusesAre(network, networkDcBuses);
}

BOOST_AUTO_TEST_CASE(moveTopology) {
    Network n1("id", "test");
    n1.newDcNode().setId("dcNode").setNominalV(500.).add();
    n1.newDcGround().setId("dcGround").setDcNode("dcNode").add();

    Network& sub = n1.newSubnetwork("sub", "test");
    DcNode& dcNodeSub = sub.newDcNode().setId("dcNode_sub").setNominalV(550.).add();
    sub.newDcGround().setId("dcGround_sub").setDcNode("dcNode_sub").add();

    BOOST_CHECK_EQUAL(2, n1.getDcBusCount());
    stdcxx::Reference<DcBus> dcBus1 = n1.getDcBus("dcNode_dcBus");
    POWSYBL_ASSERT_REF_TRUE(dcBus1);
    BOOST_CHECK_EQUAL(1, sub.getDcBusCount());
    stdcxx::Reference<DcBus> dcBusSub = n1.getDcBus("dcNode_sub_dcBus");
    POWSYBL_ASSERT_REF_TRUE(dcBusSub);

    n1.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "variant2");
    n1.getVariantManager().setWorkingVariant("variant2");
    
    dcBusSub.get().setV(600.0);

    Network n2 = std::move(n1);
    BOOST_TEST(stdcxx::areSame(n2, sub.getParentNetwork()));
    BOOST_TEST(2, n2.getDcBusCount());
    DcBus& dcsubbus2_v2 = n2.getDcBus("dcNode_sub_dcBus");
    BOOST_CHECK_CLOSE(600.0, dcsubbus2_v2.getV(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(2, n2.getVariantManager().getVariantArraySize());
    BOOST_TEST("variant2", n2.getVariantManager().getWorkingVariantId()); 

    n2.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    DcBus& dcsubbus2 = n2.getDcBus("dcNode_sub_dcBus");
    BOOST_CHECK(std::isnan(dcsubbus2_v2.getV()));

    BOOST_TEST(stdcxx::areSame(dcsubbus2, dcBusSub.get()));
    BOOST_TEST(1, dcsubbus2.getConnectedDcTerminalCount());
    for(auto& node : dcsubbus2.getDcNodes()) {
        BOOST_TEST(stdcxx::areSame(n2, node.getNetwork()));
    }

    BOOST_TEST(stdcxx::areSame(n2, dcNodeSub.getNetwork()));
    BOOST_TEST(stdcxx::areSame(sub, dcNodeSub.getParentNetwork()));
    BOOST_TEST(stdcxx::areSame(sub, n2.getSubNetwork("sub").get()));
    BOOST_TEST(stdcxx::areSame(n2, dcsubbus2.getNetwork()));
    BOOST_TEST(stdcxx::areSame(sub, dcsubbus2.getParentNetwork()));
}

BOOST_AUTO_TEST_CASE(testOneDcComponent) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    auto dcs = network.getDcComponents();
    auto ccs = network.getBusView().getConnectedComponents();
    auto scs = network.getBusView().getSynchronousComponents();

    BOOST_CHECK_EQUAL(1, boost::size(dcs));
    BOOST_CHECK_EQUAL(1, boost::size(ccs));
    BOOST_CHECK_EQUAL(2, boost::size(scs));

    Component& dc0 = dcs.front();
    Component& cc0 = ccs.front();
    Component& sc0 = scs.front();
    scs.pop_front();
    Component& sc1 = scs.front();

    std::set<std::string> sc0expectedAcBuses = {"VL-FRANCE_0", "VLDC-FRANCE-xNodeDc1fr-150_0", "VLDC-FRANCE-xNodeDc1fr-150_1", "VLDC-FRANCE-xNodeDc1fr-400_0", "VLDC-FRANCE-xNodeDc1fr-400_1"};
    std::set<std::string> sc1expectedAcBuses = {"VL-UNITED KINGDOM_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_1", "VLDC-UNITED KINGDOM-xNodeDc1gb-400_0"};
    std::set<std::string> cc0expectedAcBuses = {"VL-FRANCE_0", "VLDC-FRANCE-xNodeDc1fr-150_0", "VLDC-FRANCE-xNodeDc1fr-150_1", "VLDC-FRANCE-xNodeDc1fr-400_0", "VLDC-FRANCE-xNodeDc1fr-400_1",
                                                "VL-UNITED KINGDOM_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_1", "VLDC-UNITED KINGDOM-xNodeDc1gb-400_0"};
    std::set<std::string> expectedDcBuses = {"dcNodeFrNeg_dcBus", "dcNodeFrPos_dcBus", "dcNodeGbNeg_dcBus", "dcNodeGbPos_dcBus"};

    assertComponent(cc0, 0, cc0expectedAcBuses, expectedDcBuses);
    assertComponent(dc0, 0, {}, expectedDcBuses);
    assertComponent(sc0, 0, sc0expectedAcBuses, {});
    assertComponent(sc1, 1, sc1expectedAcBuses, {});

    for (const auto& dcBus : cc0.getDcBuses()) {
        BOOST_CHECK(dcBus.isInMainConnectedComponent());
    }
}

BOOST_AUTO_TEST_CASE(tetDcBusesInSubnetwork) {
    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    Network& dcSubnet = network.getSubNetwork("LccMonopoleGroundReturn");

    BOOST_CHECK_EQUAL(4, dcSubnet.getDcBusCount());
    auto dcComponents = dcSubnet.getDcComponents();

    std::set<std::string> expectedDcBuses = {"dcNodeFrNeg_dcBus", "dcNodeFrPos_dcBus", "dcNodeGbNeg_dcBus", "dcNodeGbPos_dcBus"};

    for (const auto& dcBusId : expectedDcBuses) {
        POWSYBL_ASSERT_REF_TRUE(dcSubnet.getDcBus(dcBusId));
    }

    assertComponent(dcComponents.front(), 0, {}, expectedDcBuses);
}

BOOST_AUTO_TEST_CASE(testTwoDcComponents) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    // run a first topology processing so that cache invalidation gets triggered
    network.getDcComponents();
    network.getBusView().getConnectedComponents();
    network.getBusView().getSynchronousComponents();

    DcLine& dcLine = network.getDcLine(powsybl::network::DcDetailedNetworkFactory::DC_LINE1);
    dcLine.getDcTerminal1().setConnected(false);

    auto dcs = network.getDcComponents();
    auto ccs = network.getBusView().getConnectedComponents();
    auto scs = network.getBusView().getSynchronousComponents();

    BOOST_CHECK_EQUAL(2, boost::size(dcs));
    BOOST_CHECK_EQUAL(2, boost::size(ccs));
    BOOST_CHECK_EQUAL(2, boost::size(scs));

    Component& dc0 = dcs.front();
    Component& cc0 = ccs.front();
    Component& sc0 = scs.front();
    dcs.pop_front();
    ccs.pop_front();
    scs.pop_front();
    Component& dc1 = dcs.front();
    Component& cc1 = ccs.front();
    Component& sc1 = scs.front();

    std::set<std::string> sc0expectedAcBuses = {"VL-FRANCE_0", "VLDC-FRANCE-xNodeDc1fr-150_0", "VLDC-FRANCE-xNodeDc1fr-150_1", "VLDC-FRANCE-xNodeDc1fr-400_0", "VLDC-FRANCE-xNodeDc1fr-400_1"};
    std::set<std::string> sc1expectedAcBuses = {"VL-UNITED KINGDOM_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_0", "VLDC-UNITED KINGDOM-xNodeDc1gb-150_1", "VLDC-UNITED KINGDOM-xNodeDc1gb-400_0"};
    std::set<std::string> dc0expectedDcBuses = {"dcNodeFrNeg_dcBus", "dcNodeFrPos_dcBus"};
    std::set<std::string> dc1expectedDcBuses = {"dcNodeGbNeg_dcBus", "dcNodeGbPos_dcBus"};

    assertComponent(cc0, 0, sc0expectedAcBuses, dc0expectedDcBuses);
    assertComponent(cc1, 1, sc1expectedAcBuses, dc1expectedDcBuses);
    assertComponent(dc0, 0, {}, dc0expectedDcBuses);
    assertComponent(dc1, 1, {}, dc1expectedDcBuses);
    assertComponent(sc0, 0, sc0expectedAcBuses, {});
    assertComponent(sc1, 1, sc1expectedAcBuses, {});

    for (const auto& dcBus0 : cc0.getDcBuses()) {
        BOOST_CHECK(dcBus0.isInMainConnectedComponent());
    }
    for (const auto& dcBus1 : cc1.getDcBuses()) {
        BOOST_CHECK(!dcBus1.isInMainConnectedComponent());
    }
}

BOOST_AUTO_TEST_CASE(testDcNodeTerminals) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    DcNode& dcNodeFrPos = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS);
    DcLine& dcLine = network.getDcLine(powsybl::network::DcDetailedNetworkFactory::DC_LINE1);
    LineCommutatedConverter& lccFr = network.getLineCommutatedConverter("LccFr");

    BOOST_CHECK_EQUAL(2, dcNodeFrPos.getDcTerminalCount());
    BOOST_CHECK_EQUAL(2, dcNodeFrPos.getConnectedDcTerminalCount());

    auto dcTerminals = dcNodeFrPos.getDcTerminals();
    auto dcConnectedTerminals = dcNodeFrPos.getConnectedDcTerminals();

    BOOST_CHECK(stdcxx::areSame(dcTerminals.front(), dcLine.getDcTerminal1()));
    BOOST_CHECK(stdcxx::areSame(dcConnectedTerminals.front(), dcLine.getDcTerminal1()));
    dcTerminals.pop_front();
    dcConnectedTerminals.pop_front();
    BOOST_CHECK(stdcxx::areSame(dcTerminals.front(), lccFr.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(dcConnectedTerminals.front(), lccFr.getDcTerminal2()));

    dcLine.getDcTerminal1().setConnected(false);

    auto dcTerminals2 = dcNodeFrPos.getDcTerminals();
    auto dcConnectedTerminals2 = dcNodeFrPos.getConnectedDcTerminals();

    BOOST_CHECK_EQUAL(2, dcNodeFrPos.getDcTerminalCount());
    BOOST_CHECK_EQUAL(1, dcNodeFrPos.getConnectedDcTerminalCount());
    BOOST_CHECK(stdcxx::areSame(dcTerminals2.front(), dcLine.getDcTerminal1()));
    dcTerminals2.pop_front();
    BOOST_CHECK(stdcxx::areSame(dcTerminals2.front(), lccFr.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(dcConnectedTerminals2.front(), lccFr.getDcTerminal2()));

}

BOOST_AUTO_TEST_CASE(testDcBusTerminals) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    DcNode& dcNodeFrPos = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS);
    DcLine& dcLine = network.getDcLine(powsybl::network::DcDetailedNetworkFactory::DC_LINE1);
    LineCommutatedConverter& lccFr = network.getLineCommutatedConverter("LccFr");

    POWSYBL_ASSERT_REF_TRUE(dcNodeFrPos.getDcBus());
    DcBus& dcBusFrPos = dcNodeFrPos.getDcBus();

    BOOST_CHECK_EQUAL(2, dcBusFrPos.getDcTerminalCount());
    BOOST_CHECK_EQUAL(2, dcBusFrPos.getConnectedDcTerminalCount());
    auto dcTerminals1 = dcBusFrPos.getDcTerminals();
    auto connectedDcTerminals1 = dcBusFrPos.getConnectedDcTerminals();

    BOOST_CHECK(stdcxx::areSame(dcTerminals1.front(), dcLine.getDcTerminal1()));
    BOOST_CHECK(stdcxx::areSame(connectedDcTerminals1.front(), dcLine.getDcTerminal1()));
    dcTerminals1.pop_front();
    connectedDcTerminals1.pop_front();
    BOOST_CHECK(stdcxx::areSame(dcTerminals1.front(), lccFr.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(connectedDcTerminals1.front(), lccFr.getDcTerminal2()));

    dcLine.getDcTerminal1().setConnected(false);
    DcBus& dcBusFrPos2 = dcNodeFrPos.getDcBus(); // refresh because old DcBus was invalidated by topology processing
    dcBusFrPos.getDcTerminals();
    BOOST_CHECK_EQUAL(2, dcBusFrPos2.getDcTerminalCount());
    BOOST_CHECK_EQUAL(1, dcBusFrPos2.getConnectedDcTerminalCount());
    auto dcTerminals2 = dcBusFrPos2.getDcTerminals();
    auto connectedDcTerminals2 = dcBusFrPos2.getConnectedDcTerminals();
    BOOST_CHECK(stdcxx::areSame(dcTerminals2.front(), dcLine.getDcTerminal1()));
    dcTerminals2.pop_front();
    BOOST_CHECK(stdcxx::areSame(dcTerminals2.front(), lccFr.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(connectedDcTerminals2.front(), lccFr.getDcTerminal2()));
}

BOOST_AUTO_TEST_CASE(testVisitDcNodeEquipments) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccBipoleGroundReturnWithDcLineSegments();
    DcNode& dcNodeFrPos = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS);
    DcNode& dcNodeFrMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_MID);
    DcNode& dcNodePosA1 = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_POS_A1);
    DcNode& dcNodeGbMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_MID);

    // DC grounds
    BOOST_CHECK_EQUAL(0, boost::size(dcNodeFrPos.getDcGrounds()));
    BOOST_CHECK_EQUAL(1, boost::size(dcNodeFrMid.getDcGrounds()));
    std::set<std::string> dcGroundIds;
    for (const auto& dcGround : dcNodeFrMid.getDcGrounds()) {
        dcGroundIds.emplace(dcGround.getId());
    }
    std::set<std::string> expectedDcGroundsIds = {std::string(powsybl::network::DcDetailedNetworkFactory::DC_GROUND_FR)};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGroundIds.cbegin(), dcGroundIds.cend(), expectedDcGroundsIds.cbegin(), expectedDcGroundsIds.cend());
    BOOST_CHECK_EQUAL(0, boost::size(dcNodeGbMid.getDcGrounds())); // because DC ground not connected

    // DC Lines
    BOOST_CHECK_EQUAL(2, boost::size(dcNodePosA1.getDcLines()));
    std::set<std::string> dcLineIds;
    for (const auto& dcLine : dcNodePosA1.getDcLines()) {
        dcLineIds.emplace(dcLine.getId());
    }
    std::set<std::string> expectedDcLinesIds = {"dcLineSegmentFrPosA", "dcLineSegmentPosA"};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLineIds.cbegin(), dcLineIds.cend(), expectedDcLinesIds.cbegin(), expectedDcLinesIds.cend());
    BOOST_CHECK_EQUAL(0, boost::size(dcNodeFrMid.getDcLines()));

    // AC/DC Converters
    BOOST_CHECK_EQUAL(1, boost::size(dcNodeFrPos.getLineCommutatedConverters()));
    std::set<std::string> dcLccIds;
    for (const auto& lcc : dcNodeFrPos.getLineCommutatedConverters()) {
        dcLccIds.emplace(lcc.getId());
    }
    std::set<std::string> expectedLccIds = {"LccFrPos"};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLccIds.cbegin(), dcLccIds.cend(), expectedLccIds.cbegin(), expectedLccIds.cend());
    BOOST_CHECK_EQUAL(0, boost::size(dcNodeFrPos.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(0, boost::size(dcNodePosA1.getLineCommutatedConverters()));

}

BOOST_AUTO_TEST_CASE(testVisitDcBusEquipments) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccBipoleGroundReturnWithDcLineSegments();
    DcNode& dcNodeFrPos = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS);
    DcNode& dcNodeFrMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_MID);
    DcNode& dcNodePosA1 = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_POS_A1);
    DcNode& dcNodeGbMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_MID);
    DcBus& dcBusFrPos = dcNodeFrPos.getDcBus();
    DcBus& dcBusFrMid = dcNodeFrMid.getDcBus();
    DcBus& dcBusGbMid = dcNodeGbMid.getDcBus();
    DcBus& dcBusPosA1 = dcNodePosA1.getDcBus();
   
    // DC grounds
    BOOST_CHECK_EQUAL(0, boost::size(dcBusFrPos.getDcGrounds()));
    BOOST_CHECK_EQUAL(1, boost::size(dcBusFrMid.getDcGrounds()));
    std::set<std::string> dcGroundIds;
    for (const auto& dcGround : dcBusFrMid.getDcGrounds()) {
        dcGroundIds.emplace(dcGround.getId());
    }
    std::set<std::string> expectedDcGroundsIds = {std::string(powsybl::network::DcDetailedNetworkFactory::DC_GROUND_FR)};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcGroundIds.cbegin(), dcGroundIds.cend(), expectedDcGroundsIds.cbegin(), expectedDcGroundsIds.cend());
    BOOST_CHECK_EQUAL(0, boost::size(dcBusGbMid.getDcGrounds())); // because DC ground not connected

    // DC Lines
    BOOST_CHECK_EQUAL(2, boost::size(dcBusPosA1.getDcLines()));
    std::set<std::string> dcLineIds;
    for (const auto& dcLine : dcBusPosA1.getDcLines()) {
        dcLineIds.emplace(dcLine.getId());
    }
    std::set<std::string> expectedDcLinesIds = {"dcLineSegmentFrPosA", "dcLineSegmentPosA"};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLineIds.cbegin(), dcLineIds.cend(), expectedDcLinesIds.cbegin(), expectedDcLinesIds.cend());
    BOOST_CHECK_EQUAL(2, boost::size(dcBusFrPos.getDcLines()));
    dcLineIds.clear();
    for (const auto& dcLine : dcBusFrPos.getDcLines()) {
        dcLineIds.emplace(dcLine.getId());
    }
    expectedDcLinesIds = {"dcLineSegmentFrPosA", "dcLineSegmentFrPosB"};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLineIds.cbegin(), dcLineIds.cend(), expectedDcLinesIds.cbegin(), expectedDcLinesIds.cend());        
    BOOST_CHECK_EQUAL(0, boost::size(dcBusFrMid.getDcLines()));

    // AC/DC Converters
    BOOST_CHECK_EQUAL(1, boost::size(dcBusFrPos.getLineCommutatedConverters()));
    std::set<std::string> dcLccIds;
    for (const auto& lcc : dcBusFrPos.getLineCommutatedConverters()) {
        dcLccIds.emplace(lcc.getId());
    }
    std::set<std::string> expectedLccIds = {"LccFrPos"};
    BOOST_CHECK_EQUAL_COLLECTIONS(dcLccIds.cbegin(), dcLccIds.cend(), expectedLccIds.cbegin(), expectedLccIds.cend());
    BOOST_CHECK_EQUAL(0, boost::size(dcBusFrPos.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(0, boost::size(dcBusPosA1.getLineCommutatedConverters()));

}

class TestDcTopologyVisitor : public DcTopologyVisitor {
public:
    TestDcTopologyVisitor() = default;

    void reset() {
        m_visited.clear();
    }

    const std::map<iidm::IdentifiableType, std::vector<std::string>>& getVisited() const {
        return m_visited;
    }

    std::set<std::string> getVisitedAsList() const {
        std::set<std::string> visitedSet;
        for (const auto& pair: m_visited) {
            for (const auto& id : pair.second) {
                visitedSet.emplace(id);
            }
        }
        return visitedSet;
    }

    void visitAcDcConverter(const AcDcConverter& converter, const TerminalNumber& terminalNumber) override {
        m_visited[converter.getType()].push_back( stdcxx::format("%1%/%2%", converter.getId(), terminalNumber));
    }

    void visitDcGround(const DcGround& dcGround) override {
        m_visited[dcGround.getType()].push_back(dcGround.getId());
    }

    void visitDcLine(const DcLine& dcLine, const TwoSides& side) override {
        m_visited[dcLine.getType()].push_back( stdcxx::format("%1%/%2%", dcLine.getId(), side));
    }


private:
    std::map<iidm::IdentifiableType, std::vector<std::string>> m_visited;
};


BOOST_AUTO_TEST_CASE(testDcNodeEquipmentsVisitor) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccBipoleGroundReturnWithDcLineSegments();
    DcNode& dcNodeFrMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_MID);
    DcNode& dcNodePosA1 = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_POS_A1);
    DcNode& dcNodeGbMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_MID);
    network.getDcLine("dcLineSegmentPosA").getDcTerminal1().setConnected(false);

    TestDcTopologyVisitor visitorConnected;
    TestDcTopologyVisitor visitorAll;

    dcNodeFrMid.visitConnectedEquipments(visitorConnected);
    dcNodeFrMid.visitConnectedOrConnectableEquipments(visitorAll);
    std::set<std::string> expectedConnected = {"LccFrNeg/TWO", "LccFrPos/ONE", "dcGroundFr"};
    std::set<std::string> expectedAll = {"LccFrNeg/TWO", "LccFrPos/ONE", "dcGroundFr"};

    auto connectedVisited = visitorConnected.getVisitedAsList();
    auto allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

    visitorConnected.reset();
    visitorAll.reset();
    dcNodeGbMid.visitConnectedEquipments(visitorConnected);
    dcNodeGbMid.visitConnectedOrConnectableEquipments(visitorAll);
    expectedConnected = {"LccGbNeg/TWO", "LccGbPos/ONE"};
    expectedAll = {"LccGbNeg/TWO", "LccGbPos/ONE", "dcGroundGb"};
    connectedVisited = visitorConnected.getVisitedAsList();
    allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

    visitorConnected.reset();
    visitorAll.reset();
    dcNodePosA1.visitConnectedEquipments(visitorConnected);
    dcNodePosA1.visitConnectedOrConnectableEquipments(visitorAll);
    expectedConnected = {"dcLineSegmentFrPosA/TWO"};
    expectedAll = {"dcLineSegmentFrPosA/TWO", "dcLineSegmentPosA/ONE"};
    connectedVisited = visitorConnected.getVisitedAsList();
    allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());
}

BOOST_AUTO_TEST_CASE(testDcBusEquipmentsVisitor) {

    Network network = powsybl::network::DcDetailedNetworkFactory::createLccBipoleGroundReturnWithDcLineSegments();
    network.getDcLine("dcLineSegmentPosA").getDcTerminal1().setConnected(false);

    DcBus &dcBusFrPos = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS).getDcBus();
    DcBus &dcBusFrMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_MID).getDcBus();
    DcBus &dcBusPosA1 = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_POS_A1).getDcBus();
    DcBus &dcBusGbMid = network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_MID).getDcBus();

    TestDcTopologyVisitor visitorConnected;
    TestDcTopologyVisitor visitorAll;

    dcBusFrMid.visitConnectedEquipments(visitorConnected);
    dcBusFrMid.visitConnectedOrConnectableEquipments(visitorAll);
    std::set<std::string> expectedConnected = {"LccFrNeg/TWO", "LccFrPos/ONE", "dcGroundFr"};
    std::set<std::string> expectedAll = {"LccFrNeg/TWO", "LccFrPos/ONE", "dcGroundFr"};

    auto connectedVisited = visitorConnected.getVisitedAsList();
    auto allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

    visitorConnected.reset();
    visitorAll.reset();
    dcBusGbMid.visitConnectedEquipments(visitorConnected);
    dcBusGbMid.visitConnectedOrConnectableEquipments(visitorAll);
    expectedConnected = {"LccGbNeg/TWO", "LccGbPos/ONE"};
    expectedAll = {"LccGbNeg/TWO", "LccGbPos/ONE", "dcGroundGb"};
    connectedVisited = visitorConnected.getVisitedAsList();
    allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

    visitorConnected.reset();
    visitorAll.reset();
    dcBusPosA1.visitConnectedEquipments(visitorConnected);
    dcBusPosA1.visitConnectedOrConnectableEquipments(visitorAll);
    expectedConnected = {"dcLineSegmentFrPosA/TWO"};
    expectedAll = {"dcLineSegmentFrPosA/TWO", "dcLineSegmentPosA/ONE"};
    connectedVisited = visitorConnected.getVisitedAsList();
    allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

    visitorConnected.reset();
    visitorAll.reset();
    dcBusFrPos.visitConnectedEquipments(visitorConnected);
    dcBusFrPos.visitConnectedOrConnectableEquipments(visitorAll);
    expectedConnected = {"LccFrPos/TWO", "dcLineSegmentFrPosA/ONE", "dcLineSegmentFrPosB/ONE"};
    expectedAll = {"LccFrPos/TWO", "dcLineSegmentFrPosA/ONE", "dcLineSegmentFrPosB/ONE"};
    connectedVisited = visitorConnected.getVisitedAsList();
    allVisited = visitorAll.getVisitedAsList();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnected.cbegin(), expectedConnected.cend(), connectedVisited.cbegin(), connectedVisited.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedAll.cbegin(), expectedAll.cend(), allVisited.cbegin(), allVisited.cend());

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
