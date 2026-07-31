/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/util/BusbarSectionFinderTraverser.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(BusbarSectionFinderTraverserTestSuite)

void createSwitch(VoltageLevel& vl, const std::string& switchId, const std::string& switchName, const SwitchKind& kind, bool retained, bool open, bool fictitious, unsigned long node1, unsigned long node2) {
    vl.getNodeBreakerView().newSwitch().setId(switchId).setName(switchName).setKind(kind).setRetained(retained).setOpen(open).setFictitious(fictitious).setNode1(node1).setNode2(node2).add();
}

Network createNetwork() {
    Network network("network", "test");
    Substation& s1 = network.newSubstation().setId("S1").add();
    VoltageLevel& vl1 = s1.newVoltageLevel().setId("VL1").setNominalV(24.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl1.getNodeBreakerView().newBusbarSection().setId("BBS1").setName("BBS1").setNode(0).add();

    VoltageLevel& vl2 = s1.newVoltageLevel().setId("VL2").setNominalV(24.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl2.getNodeBreakerView().newBusbarSection().setId("BBS1_1").setName("BBS1_1").setNode(0).add();
    vl2.getNodeBreakerView().newBusbarSection().setId("BBS1_2").setName("BBS1_2").setNode(1).add();
    vl2.getNodeBreakerView().newBusbarSection().setId("BBS2_1").setName("BBS2_1").setNode(3).add();
    vl2.getNodeBreakerView().newBusbarSection().setId("BBS2_2").setName("BBS2_2").setNode(4).add();

    // VL1 switches
    createSwitch(vl1, "DISC_LINE_1_1_VL1", "DISC_LINE_1_1_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 1);
    createSwitch(vl1, "DISC_LINE_1_2_VL1", "DISC_LINE_1_2_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 2);
    createSwitch(vl1, "DISC_LINE_2_1_VL1", "DISC_LINE_2_1_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 3);
    createSwitch(vl1, "DISC_LINE_2_2_VL1", "DISC_LINE_2_2_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 4);
    createSwitch(vl1, "BRK_LINE_1_1_VL1", "BRK_LINE_1_1_VL1", SwitchKind::BREAKER, false, false, false, 1, 5);
    createSwitch(vl1, "BRK_LINE_1_2_VL1", "BRK_LINE_1_2_VL1", SwitchKind::BREAKER, false, false, false, 2, 6);
    createSwitch(vl1, "BRK_LINE_2_1_VL1", "BRK_LINE_2_1_VL1", SwitchKind::BREAKER, false, false, false, 3, 7);
    createSwitch(vl1, "BRK_LINE_2_2_VL1", "BRK_LINE_2_2_VL1", SwitchKind::BREAKER, false, false, false, 4, 8);

    // Fork topology
    createSwitch(vl2, "DISC_BBS1.1_BBS1.2", "DISC_BBS1.1_BBS1.2", SwitchKind::DISCONNECTOR, false, false, false, 0, 1);
    createSwitch(vl2, "DISC_BBS2.1_BBS2.2", "DISC_BBS2.1_BBS2.2", SwitchKind::DISCONNECTOR, false, true, false, 3, 4);
    createSwitch(vl2, "DISC_BBS1_2", "DISC_BBS1_2", SwitchKind::DISCONNECTOR, false, true, false, 1, 6);
    createSwitch(vl2, "DISC_BBS2_2", "DISC_BBS2_2", SwitchKind::DISCONNECTOR, false, true, false, 4, 6);
    createSwitch(vl2, "BRK_FORK", "BRK_FORK", SwitchKind::BREAKER, false, true, false, 6, 7);
    createSwitch(vl2, "DISC_LINE_1_2", "DISC_LINE_1_2", SwitchKind::DISCONNECTOR, false, false, false, 7, 8);
    createSwitch(vl2, "DISC_LINE_2_2", "DISC_LINE_2_2", SwitchKind::DISCONNECTOR, false, false, false, 7, 9);
    createSwitch(vl2, "BRK_LINE_1_2", "BRK_LINE_1_2", SwitchKind::BREAKER, false, false, false, 8, 10);
    createSwitch(vl2, "BRK_LINE_2_2", "BRK_LINE_2_2", SwitchKind::BREAKER, false, false, false, 9, 11);

    // Internal Connection
    createSwitch(vl2, "DISC_BUS1_2_TD1", "DISC_BUS1_2_TD1", SwitchKind::DISCONNECTOR, false, false, false, 1, 12);
    createSwitch(vl2, "DISC_BUS2_2_TD1", "DISC_BUS2_2_TD1", SwitchKind::DISCONNECTOR, false, false, false, 4, 13);

    // BYPASS topology
    createSwitch(vl2, "DISC_BBS1_1", "DISC_BBS1_1", SwitchKind::DISCONNECTOR, false, true, false, 0, 19);
    createSwitch(vl2, "DISC_BBS2_1", "DISC_BBS2_1", SwitchKind::DISCONNECTOR, false, true, false, 3, 22);
    createSwitch(vl2, "DISC_BYPASS", "DISC_BYPASS", SwitchKind::DISCONNECTOR, false, true, false, 19, 22);
    createSwitch(vl2, "DISC_LINE_1_1", "DISC_LINE_1_1", SwitchKind::DISCONNECTOR, false, true, false, 19, 16);
    createSwitch(vl2, "DISC_LINE_2_1", "DISC_LINE_2_1", SwitchKind::DISCONNECTOR, false, true, false, 22, 25);
    createSwitch(vl2, "BRK_LINE_1_1", "BRK_LINE_1_1", SwitchKind::BREAKER, false, true, false, 16, 17);
    createSwitch(vl2, "BRK_LINE_2_1", "BRK_LINE_2_1", SwitchKind::BREAKER, false, true, false, 25, 26);

    network.newLine().setId("LINE_1_2").setName("LINE_1_2").setVoltageLevel1("VL2").setNode1(10).setVoltageLevel2("VL1").setNode2(6).setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();

    network.newLine().setId("LINE_2_2").setName("LINE_2_2").setVoltageLevel1("VL2").setNode1(11).setVoltageLevel2("VL1").setNode2(8).setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();
    network.newLine().setId("LINE_1_1").setName("LINE_1_1").setVoltageLevel1("VL2").setNode1(17).setVoltageLevel2("VL1").setNode2(5).setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();

    network.newLine().setId("LINE_2_1").setName("LINE_2_1").setVoltageLevel1("VL2").setNode1(26).setVoltageLevel2("VL1").setNode2(7).setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();

    s1.newTwoWindingsTransformer().setId("TD1").setName("TD1").setVoltageLevel1("VL2").setNode1(12).setVoltageLevel2("VL2").setNode2(13).setR(0.24).setX(2.4).setG(0.0).setB(0.0).add();
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    return network;
}

void assertBusbarSectionFinderResult(Terminal& startTerminal, const std::string& expectedBBSId, unsigned long expectedDepth, const std::string & expectedLastSwitchId, bool expectedLastSwitchState, bool expectedAllClosedSwitch) {
    auto result = BusbarSectionFinderTraverser::getBusbarSectionResult(startTerminal);
    BOOST_CHECK_EQUAL(result.busbarSectionId, expectedBBSId); 
    BOOST_CHECK_EQUAL(result.depth, expectedDepth); 
    BOOST_CHECK_EQUAL(result.lastSwitch.switchId, expectedLastSwitchId); 
    BOOST_CHECK_EQUAL(result.lastSwitch.isOpen, expectedLastSwitchState); 
    BOOST_CHECK_EQUAL(result.allClosedSwitch, expectedAllClosedSwitch);
}

BOOST_AUTO_TEST_CASE(testWithOpenSwitch) {
    Network n = createNetwork();
    Line& line12 = n.getLine("LINE_1_2");
    Line& line22 = n.getLine("LINE_2_2");

    std::string bbsFound = BusbarSectionFinderTraverser::findBusbarSectionId(line12.getTerminal1());
    BOOST_CHECK_EQUAL(bbsFound, "BBS1_2");

    assertBusbarSectionFinderResult(line12.getTerminal1(), "BBS1_2", 4, "DISC_BBS1_2", true, false);
    assertBusbarSectionFinderResult(line22.getTerminal1(), "BBS1_2", 4, "DISC_BBS1_2", true, false);
}

BOOST_AUTO_TEST_CASE(testWithClosedLastSwitch) {
    Network n = createNetwork();

    n.getSwitch("DISC_BBS1_1").setOpen(false);
    n.getSwitch("DISC_BBS2_2").setOpen(false);

    Line& line11 = n.getLine("LINE_1_1");
    Line& line12 = n.getLine("LINE_1_2");
    Line& line21 = n.getLine("LINE_2_1");

    assertBusbarSectionFinderResult(line11.getTerminal1(), "BBS1_1", 3, "DISC_BBS1_1", false, false);
    assertBusbarSectionFinderResult(line21.getTerminal1(), "BBS1_1", 4, "DISC_BBS1_1", false, false);
    assertBusbarSectionFinderResult(line12.getTerminal1(), "BBS2_2", 4, "DISC_BBS2_2", false, false);
}

BOOST_AUTO_TEST_CASE(testWithAllClosedSwitch) {
    Network n = createNetwork();

    n.getSwitch("BRK_LINE_1_1").setOpen(false);
    n.getSwitch("DISC_LINE_1_1").setOpen(false);
    n.getSwitch("DISC_BBS1_1").setOpen(false);
    n.getSwitch("DISC_BBS2_1").setOpen(false);
    n.getSwitch("BRK_LINE_2_1").setOpen(false);
    n.getSwitch("DISC_LINE_2_1").setOpen(false);

    Line& line11 = n.getLine("LINE_1_1");
    Line& line21 = n.getLine("LINE_2_1");

    assertBusbarSectionFinderResult(line11.getTerminal1(), "BBS1_1", 3, "DISC_BBS1_1", false, true);
    assertBusbarSectionFinderResult(line21.getTerminal1(), "BBS2_1", 3, "DISC_BBS2_1", false, true);
}

BOOST_AUTO_TEST_CASE(testWithInternalConnections) {
    Network n = createNetwork();
    auto& td1 = n.getTwoWindingsTransformer("TD1");

    assertBusbarSectionFinderResult(td1.getTerminal1(), "BBS1_2", 1, "DISC_BUS1_2_TD1", false, true);
    assertBusbarSectionFinderResult(td1.getTerminal2(), "BBS2_2", 1, "DISC_BUS2_2_TD1", false, true);
}

BOOST_AUTO_TEST_CASE(testWithNoBusbarSection) {
    Network n = createNetwork();
    //Add voltageLevel with no BusbarSection
    n.newVoltageLevel().setId("teePoint").setName("teePoint").setFictitious(true).setNominalV(200).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    createSwitch(n.getVoltageLevel("VL1"), "DISC_LINE_TEE_POINT_VL1", "DISC_LINE_TEE_POINT_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 9);
    auto& teePointLine = n.newLine().setId("LINE_TEE_POINT").setName("LINE_TEE_POINT").setVoltageLevel1("VL1").setNode1(9).setVoltageLevel2("teePoint").setNode2(0).setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();

    //Empty result :
    BOOST_CHECK(BusbarSectionFinderTraverser::getBusbarSectionResult(teePointLine.getTerminal2()).busbarSectionId.empty());
    //No BusbarSection at all in this voltageLevel:
    BOOST_CHECK(BusbarSectionFinderTraverser::findBusbarSectionId(teePointLine.getTerminal2()).empty());
}

BOOST_AUTO_TEST_CASE(testWithBusBreakerVoltageLevel) {
    Network n = createNetwork();
    n.newVoltageLevel().setId("busBreakerVL").setName("busBreakerVL").setFictitious(true).setNominalV(200).setTopologyKind(TopologyKind::BUS_BREAKER).add();
    n.getVoltageLevel("busBreakerVL").getBusBreakerView().newBus().setId("B1").add();
    createSwitch(n.getVoltageLevel("VL1"), "DISC_LINE_TEE_POINT_VL1", "DISC_LINE_TEE_POINT_VL1", SwitchKind::DISCONNECTOR, false, false, false, 0, 9);
    Line& busBreakerLine = n.newLine().setId("busBreakerLine").setName("busBreakerLine").setVoltageLevel1("VL1").setNode1(9).setVoltageLevel2("busBreakerVL").setConnectableBus2("B1").setR(2.0).setX(25.0).setG1(0.0).setB1(300E-6 / 2).setG2(0.0).setB2(300E-6 / 2).add();
    
    Terminal& busTerminal = busBreakerLine.getTerminal2();

    POWSYBL_ASSERT_THROW(BusbarSectionFinderTraverser::getBusbarSectionResult(busTerminal), PowsyblException, "BusbarSectionFinderTraverser only works with Node Breaker view and voltage level busBreakerVL is not in this topology kind");
    POWSYBL_ASSERT_THROW(BusbarSectionFinderTraverser::findBusbarSectionId(busTerminal), PowsyblException, "BusbarSectionFinderTraverser only works with Node Breaker view and voltage level busBreakerVL is not in this topology kind");
}

BOOST_AUTO_TEST_CASE(testWithBypassTopology) {
    Network n = createNetwork();
    VoltageLevel& vl3 = n.newVoltageLevel().setId("VL3").setNominalV(24.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl3.getNodeBreakerView().newBusbarSection().setId("VL3_BBS1").setName("VL3_BBS1").setNode(0).add();
    vl3.getNodeBreakerView().newBusbarSection().setId("VL3_BBS2").setName("VL3_BBS2").setNode(1).add();

    createSwitch(vl3, "VL3_DISC_BBS1_1", "VL3_DISC_BBS1_1", SwitchKind::DISCONNECTOR, false, false, false, 0, 2);
    createSwitch(vl3, "VL3_DISC_BBS1_2", "VL3_DISC_BBS1_2", SwitchKind::DISCONNECTOR, false, true, false, 2, 3);

    createSwitch(vl3, "VL3_DISC_BBS2_1", "VL3_DISC_BBS2_1", SwitchKind::DISCONNECTOR, false, false, false, 1, 4);
    createSwitch(vl3, "VL3_DISC_BBS2_2", "VL3_DISC_BBS2_2", SwitchKind::DISCONNECTOR, false, false, false, 4, 3);

    createSwitch(vl3, "VL3_DISC_BYPASS", "VL3_DISC_BYPASS", SwitchKind::DISCONNECTOR, false, false, false, 4, 5);

    Load& load = vl3.newLoad().setId("LOAD_BYPASS").setP0(10).setQ0(10).setNode(5).add();

    assertBusbarSectionFinderResult(load.getTerminal(), "VL3_BBS2", 2, "VL3_DISC_BBS2_1", false, true);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
