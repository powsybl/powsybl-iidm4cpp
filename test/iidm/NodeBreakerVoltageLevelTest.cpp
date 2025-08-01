/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/InternalConnection.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createCalculatedBusSwitchTestNetwork() {
    Network network("test", "test");

    Substation& s1 = network.newSubstation()
        .setId("A")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s1.newVoltageLevel()
        .setId("B")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(0.0)
        .setHighVoltageLimit(250.0)
        .add();

    vl.getNodeBreakerView().newBusbarSection()
        .setId("C")
        .setNode(0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("D")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("E")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    Substation& s2 = network.newSubstation()
        .setId("F")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("G")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(0.0)
        .setHighVoltageLimit(250.0)
        .add();

    vl2.getNodeBreakerView().newBusbarSection()
        .setId("H")
        .setNode(0)
        .add();

    vl2.getNodeBreakerView().newBusbarSection()
        .setId("I")
        .setNode(1)
        .add();

    vl2.getNodeBreakerView().newDisconnector()
        .setId("J")
        .setNode1(0)
        .setNode2(2)
        .setRetained(true)
        .setOpen(false)
        .add();

    vl2.getNodeBreakerView().newDisconnector()
        .setId("K")
        .setNode1(1)
        .setNode2(3)
        .setRetained(true)
        .setOpen(false)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("L")
        .setNode1(2)
        .setNode2(3)
        .setRetained(true)
        .setOpen(false)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("M")
        .setNode1(0)
        .setNode2(4)
        .setRetained(false)
        .setOpen(false)
        .add();

    network.newLine()
        .setId("N")
        .setVoltageLevel1(vl.getId())
        .setNode1(2)
        .setVoltageLevel2(vl2.getId())
        .setNode2(4)
        .setR(0.001)
        .setX(0.1)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();

    Substation& s3 = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl3 = s3.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl3.getNodeBreakerView().newBreaker()
        .setId("SW1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(true)
        .setOpen(true)
        .add();

    vl3.getNodeBreakerView().newInternalConnection()
        .setNode1(1)
        .setNode2(2)
        .add();

    vl3.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(100.0)
        .setQ0(50.0)
        .add();

    return network;
}

void addInternalConnection( VoltageLevel::NodeBreakerView& topo, int node1, int node2) {
    topo.newInternalConnection()
        .setNode1(node1)
        .setNode2(node2)
        .add();
}

void createNetwork(Network& network) {
    const std::string S5_10K_V = "S5 10kV";
    Substation& s = network.newSubstation()
        .setId("S5")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId(S5_10K_V)
        .setNominalV(10.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("NODE13")
        .setNode(0)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("NODE14")
        .setNode(3)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("NODE15")
        .setNode(2)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("NODE16")
        .setNode(1)
        .add();
    vl.getNodeBreakerView().newSwitch()
        .setId("BREAKER4")
        .setNode1(4)
        .setNode2(5)
        .setKind(SwitchKind::BREAKER)
    .add();
    vl.getNodeBreakerView().newSwitch()
        .setId("DISCONNECTOR7")
        .setNode1(6)
        .setNode2(7)
        .setKind(SwitchKind::DISCONNECTOR)
        .add();
    vl.getNodeBreakerView().newSwitch()
        .setId("DISCONNECTOR8")
        .setNode1(8)
        .setNode2(9)
        .setKind(SwitchKind::DISCONNECTOR)
        .add();
    vl.newLoad()
        .setId("M3")
        .setNode(11)
        .setP0(5)
        .setQ0(3)
        .add();
    vl.newLoad()
        .setId("M2a")
        .setNode(12)
        .setP0(2)
        .setQ0(1)
        .add();
    vl.newLoad()
        .setId("M2b")
        .setNode(13)
        .setP0(2)
        .setQ0(1)
        .add();
    VoltageLevel::NodeBreakerView& topo = vl.getNodeBreakerView();
    addInternalConnection(topo, 7, 0);
    addInternalConnection(topo, 6, 3);
    addInternalConnection(topo, 4, 3);
    addInternalConnection(topo, 5, 2);
    addInternalConnection(topo, 9, 2);
    addInternalConnection(topo, 8, 1);
    Substation& s4 = network.newSubstation()
        .setId("S4")
        .setCountry(Country::FR)
        .add();
    s4.newVoltageLevel()
    .setId("S4 10kV")
    .setNominalV(10.0)
    .setTopologyKind(TopologyKind::NODE_BREAKER)
    .add();
    network.newLine()
    .setId("L6")
    .setVoltageLevel1("S4 10kV")
    .setNode1(0)
    .setVoltageLevel2(S5_10K_V)
    .setNode2(10)
    .setR(0.082)
    .setX(0.086)
    .setG1(0)
    .setB1(0)
    .setG2(0)
    .setB2(0)
    .add();
}

Network createIsolatedLoadNetwork() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation()
        .setId("S")
        .add();

    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL")
        .setNominalV(1.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();

    VoltageLevel& vl2 = s1.newVoltageLevel()
        .setId("VL2")
        .setNominalV(1.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();

    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("B0")
        .setNode(0)
        .add();

    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("B1")
        .setNode(1)
        .add();

    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("B2")
        .setNode(2)
        .add();

    vl1.newLoad()
        .setId("L0")
        .setNode(6)
        .setP0(0)
        .setQ0(0)
        .add();

    vl1.newLoad()
        .setId("L1")
        .setNode(3)
        .setP0(0)
        .setQ0(0)
        .add();

    vl1.newLoad()
        .setId("L2")
        .setNode(4)
        .setP0(0)
        .setQ0(0)
        .add();

    vl1.newLoad()
        .setId("L3")
        .setNode(5)
        .setP0(0)
        .setQ0(0)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L0-node")
        .setOpen(false)
        .setNode1(0)
        .setNode2(6)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L1-node")
        .setOpen(true)
        .setNode1(4)
        .setNode2(10)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L0-B0")
        .setOpen(false)
        .setNode1(3)
        .setNode2(1)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("B0-node")
        .setOpen(true)
        .setNode1(1)
        .setNode2(10)
        .setRetained(true)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("node-B1")
        .setOpen(false)
        .setNode1(10)
        .setNode2(2)
        .setRetained(true)
        .add();

    vl2.newLoad()
        .setId("L4")
        .setNode(0)
        .setP0(0)
        .setQ0(0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(NodeBreakerVoltageLevelTestSuite)

BOOST_AUTO_TEST_CASE(busbarSection) {
    Network network = createNetwork();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(0, network.getBusbarSectionCount());

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    BOOST_CHECK_EQUAL(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

    BusbarSection& bbs = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    BOOST_CHECK_EQUAL("BBS", bbs.getId());
    BOOST_CHECK_EQUAL("BBS_NAME", bbs.getOptionalName());
    BOOST_CHECK_EQUAL(IdentifiableType::BUSBAR_SECTION, bbs.getType());
    std::ostringstream oss;
    oss << bbs.getType();
    BOOST_CHECK_EQUAL("BUSBAR_SECTION", oss.str());
    BOOST_TEST(stdcxx::areSame(voltageLevel, bbs.getTerminal().getVoltageLevel()));

    BusbarSection& bbs2 = network.getBusbarSection("BBS");
    BOOST_TEST(stdcxx::areSame(bbs, bbs2));
    BOOST_TEST(stdcxx::areSame(bbs, cNetwork.getBusbarSection("BBS")));
    BOOST_TEST(std::isnan(bbs.getAngle()));
    BOOST_TEST(std::isnan(bbs.getV()));

    BusbarSectionAdder adder = voltageLevel.getNodeBreakerView().newBusbarSection().setId("DUPLICATE");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Busbar section 'DUPLICATE': Node is not set");
    adder.setNode(0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Busbar section 'DUPLICATE': An equipment (BBS) is already connected to the node 0 of voltage level VL2");

    BOOST_CHECK_EQUAL(2UL, network.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getBusbarSections()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cNetwork.getBusbarSections()));

    const VoltageLevel& cVoltageLevel = voltageLevel;
    BOOST_CHECK_EQUAL(1UL, boost::size(voltageLevel.getNodeBreakerView().getBusbarSections()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cVoltageLevel.getNodeBreakerView().getBusbarSections()));
}

BOOST_AUTO_TEST_CASE(switches) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    BOOST_CHECK_EQUAL(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setName("BBS1_NAME")
        .setNode(0)
        .add();

    voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setName("BBS2_NAME")
        .setNode(1)
        .add();

    Switch& breaker = voltageLevel.getNodeBreakerView().newBreaker()
        .setId("BK")
        .setName("BK_NAME")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    BOOST_CHECK_EQUAL("BK", breaker.getId());
    BOOST_CHECK_EQUAL("BK_NAME", breaker.getOptionalName());
    POWSYBL_ASSERT_ENUM_EQ(SwitchKind::BREAKER, breaker.getKind());
    BOOST_TEST(!breaker.isOpen());
    BOOST_TEST(breaker.isRetained());
    BOOST_TEST(!breaker.isFictitious());
    BOOST_TEST(stdcxx::areSame(voltageLevel, breaker.getVoltageLevel()));

    breaker.setOpen(true)
        .setRetained(false)
        .setFictitious(true);
    BOOST_TEST(breaker.isOpen());
    BOOST_TEST(!breaker.isRetained());
    BOOST_TEST(breaker.isFictitious());

    const Network& cNetwork = network;
    BOOST_TEST(stdcxx::areSame(breaker, cNetwork.getSwitch("BK")));

    const auto& cBreaker = breaker;
    BOOST_TEST(stdcxx::areSame(network, breaker.getNetwork()));
    BOOST_TEST(stdcxx::areSame(cNetwork, cBreaker.getNetwork()));

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getSwitches()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cNetwork.getSwitches()));

    const VoltageLevel& cVoltageLevel = voltageLevel;
    BOOST_CHECK_EQUAL(1UL, cVoltageLevel.getSwitchCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(voltageLevel.getNodeBreakerView().getSwitches()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cVoltageLevel.getNodeBreakerView().getSwitches()));
    BOOST_CHECK_EQUAL(1UL, boost::size(voltageLevel.getSwitches()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cVoltageLevel.getSwitches()));

    BOOST_CHECK(voltageLevel.getNodeBreakerView().getOptionalTerminal(0));
    BOOST_CHECK(voltageLevel.getNodeBreakerView().getOptionalTerminal(1));
    BOOST_CHECK(voltageLevel.getNodeBreakerView().hasAttachedEquipment(0));
    BOOST_CHECK(voltageLevel.getNodeBreakerView().hasAttachedEquipment(1));
    voltageLevel.getNodeBreakerView().removeSwitch(breaker.getId());

    voltageLevel.getNodeBreakerView().newBreaker()
        .setId("SWITCH")
        .setName("SWITCH")
        .setNode1(4)
        .setNode2(5)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    // check non-const versions
    BOOST_CHECK(!voltageLevel.getNodeBreakerView().getOptionalTerminal(2));
    BOOST_CHECK(!voltageLevel.getNodeBreakerView().getOptionalTerminal(3));
    BOOST_CHECK(!voltageLevel.getNodeBreakerView().hasAttachedEquipment(2));
    BOOST_CHECK(!voltageLevel.getNodeBreakerView().hasAttachedEquipment(3));

    BOOST_CHECK(!voltageLevel.getNodeBreakerView().getOptionalTerminal(4));
    BOOST_CHECK(!voltageLevel.getNodeBreakerView().getOptionalTerminal(5));
    BOOST_CHECK(voltageLevel.getNodeBreakerView().hasAttachedEquipment(4));
    BOOST_CHECK(voltageLevel.getNodeBreakerView().hasAttachedEquipment(5));

    BOOST_CHECK(!voltageLevel.getNodeBreakerView().getOptionalTerminal(6));

    // check const versions
    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().getOptionalTerminal(2));
    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().getOptionalTerminal(3));
    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().hasAttachedEquipment(2));
    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().hasAttachedEquipment(3));

    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().getOptionalTerminal(4));
    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().getOptionalTerminal(5));
    BOOST_CHECK(cVoltageLevel.getNodeBreakerView().hasAttachedEquipment(4));
    BOOST_CHECK(cVoltageLevel.getNodeBreakerView().hasAttachedEquipment(5));

    BOOST_CHECK(!cVoltageLevel.getNodeBreakerView().getOptionalTerminal(6));
}

BOOST_AUTO_TEST_CASE(NodeBreakerViewTest) {
    Network network = createNetwork();
    const unsigned long NODE_COUNT = 3;

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    const auto& cVoltageLevel = voltageLevel;

    BusbarSection& bbs1 = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setNode(0)
        .add();

    BusbarSection& bbs2 = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setNode(1)
        .add();

    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());
    Switch& breaker = voltageLevel.getNodeBreakerView().newBreaker()
        .setId("BK")
        .setName("BK_NAME")
        .setNode1(0)
        .setNode2(1)
        .add();
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());

    auto swAdder = voltageLevel.getNodeBreakerView().newSwitch()
        .setId("BK2")
        .setName("BK2_NAME");
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'BK2': First connection node is not set");
    swAdder.setNode1(0);
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'BK2': Second connection node is not set");
    swAdder.setNode2(1);
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'BK2': Kind is not set");
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());

    // Get a busbar section
    const auto& refBusbarSection = voltageLevel.getNodeBreakerView().getBusbarSection("BBS1");
    POWSYBL_ASSERT_REF_TRUE(refBusbarSection);
    BOOST_TEST(stdcxx::areSame(bbs1, refBusbarSection.get()));

    const auto& refTerminal = voltageLevel.getNodeBreakerView().getTerminal(0);
    BOOST_TEST(refTerminal);
    BOOST_TEST(stdcxx::areSame(bbs1, refTerminal.get().getConnectable().get()));

    // Get an unknown busbar section
    BOOST_TEST(!voltageLevel.getNodeBreakerView().getBusbarSection("UNKNOWN"));

    // Get a switch
    const auto& refBreaker = voltageLevel.getNodeBreakerView().getSwitch("BK");
    POWSYBL_ASSERT_REF_TRUE(refBreaker);
    BOOST_TEST(stdcxx::areSame(breaker, refBreaker.get()));
    const auto& cRefBreaker = cVoltageLevel.getNodeBreakerView().getSwitch("BK");
    POWSYBL_ASSERT_REF_TRUE(cRefBreaker);
    BOOST_TEST(stdcxx::areSame(breaker, cRefBreaker.get()));
    // Get an unknown switch
    BOOST_TEST(!voltageLevel.getNodeBreakerView().getSwitch("UNKNOWN"));
    BOOST_TEST(!cVoltageLevel.getNodeBreakerView().getSwitch("UNKNOWN"));

    unsigned long node1 = voltageLevel.getNodeBreakerView().getNode1("BK");
    BOOST_CHECK_EQUAL(0, node1);
    const auto& terminal1 = voltageLevel.getNodeBreakerView().getTerminal1("BK");
    const auto& cTerminal1 = cVoltageLevel.getNodeBreakerView().getTerminal1("BK");
    BOOST_TEST(stdcxx::areSame(bbs1, terminal1.get().getConnectable().get()));
    BOOST_TEST(stdcxx::areSame(cTerminal1.get(), terminal1.get()));

    unsigned long node2 = voltageLevel.getNodeBreakerView().getNode2("BK");
    BOOST_CHECK_EQUAL(1, node2);
    const auto& terminal2 = voltageLevel.getNodeBreakerView().getTerminal2("BK");
    const auto& cTerminal2 = cVoltageLevel.getNodeBreakerView().getTerminal2("BK");
    BOOST_TEST(stdcxx::areSame(bbs2, terminal2.get().getConnectable().get()));
    BOOST_TEST(stdcxx::areSame(cTerminal2.get(), terminal2.get()));

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().getNode1("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in the voltage level 'VL2'");

    VoltageLevel::NodeBreakerView::TopologyTraverser traverser = [=](unsigned long /*node1*/, const stdcxx::Reference<Switch>& /*sw*/, unsigned long node2) {
        return (node2 < (NODE_COUNT - 1)) ? math::TraverseResult::CONTINUE : math::TraverseResult::TERMINATE_TRAVERSER;
    };
    voltageLevel.getNodeBreakerView().traverse(0, traverser);

    // Remove a busbar section
    bbs1.remove();
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getBusbarSectionCount());
    BOOST_CHECK_EQUAL(1, boost::size(voltageLevel.getNodeBreakerView().getBusbarSections()));
    bbs2.remove();
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getBusbarSectionCount());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().removeSwitch("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in voltage level 'VL2'");
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());
    voltageLevel.getNodeBreakerView().removeSwitch("BK");
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());

    // test const versions
    VoltageLevel& vlTest = network.getVoltageLevel("VL2");
    auto& view = vlTest.getNodeBreakerView();
    const auto& cView = view;
    BOOST_CHECK_EQUAL(0, view.getBusbarSectionCount());
    BOOST_TEST(boost::empty(view.getBusbarSections()));
    BOOST_TEST(boost::empty(cView.getBusbarSections()));

    //test internal connections
    BOOST_CHECK_EQUAL(0, view.getSwitchCount());
    BOOST_CHECK_EQUAL(0, view.getInternalConnectionCount());
    auto internalConnectionAdder = voltageLevel.getNodeBreakerView().newInternalConnection();

    POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "Voltage level 'VL2': first connection node is not set");
    internalConnectionAdder.setNode1(0);

    POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "Voltage level 'VL2': second connection node is not set");
    internalConnectionAdder.setNode2(0);
    // TODO(thiebarr):  POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "InternalConnection 'IC_1': connection nodes must be different");
    internalConnectionAdder.setNode2(1);

    BOOST_CHECK_NO_THROW(internalConnectionAdder.add());
    BOOST_CHECK_EQUAL(0, view.getSwitchCount());
    BOOST_CHECK_EQUAL(1, view.getInternalConnectionCount());
    BOOST_CHECK_EQUAL(1, boost::size(view.getInternalConnections()));
    BOOST_CHECK_EQUAL(1, boost::size(cView.getInternalConnections()));

    POWSYBL_ASSERT_THROW(view.removeInternalConnections(0, 2), PowsyblException, "Internal connection not found between 0 and 2");
    view.removeInternalConnections(0, 1);
    BOOST_CHECK_EQUAL(0, boost::size(view.getInternalConnections()));
    BOOST_CHECK_EQUAL(0, boost::size(cView.getInternalConnections()));
}

BOOST_AUTO_TEST_CASE(calculatedBusBreakerTopology) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("BK1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    Switch& sw = vl.getNodeBreakerView().newDisconnector()
        .setId("SW1")
        .setNode1(1)
        .setNode2(2)
        .setRetained(true)
        .setOpen(true)
        .add();

    vl.newLoad()
        .setId("LOAD2")
        .setNode(2)
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("BK2")
        .setNode1(2)
        .setNode2(3)
        .setRetained(true)
        .setOpen(true)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    network.newLine()
        .setId("VL1_VL2")
        .setVoltageLevel1(vl.getId())
        .setNode1(1)
        .setVoltageLevel2(vl2.getId())
        .setNode2(0)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    network.newLine()
        .setId("VL1_VL2_2")
        .setVoltageLevel1(vl.getId())
        .setNode1(3)
        .setVoltageLevel2(vl2.getId())
        .setNode2(1)
        .setR(4.0)
        .setX(44.0)
        .setG1(5.0)
        .setB1(0.3)
        .setG2(6.0)
        .setB2(0.5)
        .add();

    auto& busBreakerView = vl.getBusBreakerView();
    const auto& cVl = vl;
    const auto& cBusBreakerView = cVl.getBusBreakerView();
    BOOST_TEST(stdcxx::areSame(busBreakerView, cBusBreakerView));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_2"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_3"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus1("SW1"));
    POWSYBL_ASSERT_REF_TRUE(cBusBreakerView.getBus1("SW1"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus2("SW1"));
    POWSYBL_ASSERT_REF_TRUE(cBusBreakerView.getBus2("SW1"));
    BOOST_CHECK_EQUAL(3, boost::size(busBreakerView.getBuses()));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getSwitch("SW1"));
    POWSYBL_ASSERT_REF_TRUE(cBusBreakerView.getSwitch("SW1"));
    BOOST_CHECK_EQUAL(2UL, busBreakerView.getSwitchCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(busBreakerView.getSwitches()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cBusBreakerView.getSwitches()));
    POWSYBL_ASSERT_THROW(busBreakerView.getSwitch("UNKNOWN"), PowsyblException, "Switch UNKNOWN not found");
    POWSYBL_ASSERT_THROW(cBusBreakerView.getSwitch("UNKNOWN"), PowsyblException, "Switch UNKNOWN not found");
    POWSYBL_ASSERT_THROW(busBreakerView.newBus(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.newSwitch(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllBuses(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllSwitches(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeBus(""), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeSwitch(""), AssertionError, "Not implemented");

    auto& testBus = busBreakerView.getBus("VL_0").get();
    const auto& cTestBus = testBus;
    BOOST_CHECK_EQUAL("VL_0", testBus.getId());
    BOOST_CHECK(testBus.getOptionalName().empty());
    BOOST_CHECK_EQUAL("VL_0", testBus.getNameOrId());
    BOOST_CHECK_CLOSE(7.7, testBus.setAngle(7.7).setV(8.8).getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(8.8, testBus.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, testBus.getConnectedTerminalCount());
    const auto& terminals = testBus.getConnectedTerminals();
    BOOST_CHECK_EQUAL(boost::size(terminals), testBus.getConnectedTerminalCount());
    const auto& cTerminals = cTestBus.getConnectedTerminals();
    BOOST_CHECK_EQUAL(boost::size(cTerminals), cTestBus.getConnectedTerminalCount());
    BOOST_TEST(stdcxx::areSame(vl, testBus.getVoltageLevel()));

    sw.setOpen(false);
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_2"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_3"));

    sw.setRetained(false);
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_2"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_3"));
}

BOOST_AUTO_TEST_CASE(calculatedBusTraverser) {
    Network network = createCalculatedBusSwitchTestNetwork();
    Load& l1 = network.getLoad("LOAD1");
    VoltageLevel& vlG = network.getVoltageLevel("G");

    auto& bus = l1.getTerminal().getBusBreakerView().getBus().get();
    BOOST_CHECK_EQUAL("VL_0", bus.getId());
    BOOST_TEST(std::isnan(bus.getAngle()));
    BOOST_TEST(std::isnan(bus.getV()));
    bus.setAngle(10.0).setV(20.0);
    BOOST_CHECK_CLOSE(10.0, bus.getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, bus.getV(), std::numeric_limits<double>::epsilon());

    auto& bus2 = vlG.getBusBreakerView().getBus("G_0").get();
    BOOST_CHECK_EQUAL("G_0", bus2.getId());
    BOOST_TEST(std::isnan(bus2.getAngle()));
    BOOST_TEST(std::isnan(bus2.getV()));
    bus2.setAngle(30.0).setV(40.0);
    BOOST_CHECK_CLOSE(30.0, bus2.getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40.0, bus2.getV(), std::numeric_limits<double>::epsilon());

    auto& bus3 = vlG.getBusBreakerView().getBus("G_1").get();
    BOOST_CHECK_EQUAL("G_1", bus3.getId());
    BOOST_TEST(std::isnan(bus3.getAngle()));
    BOOST_TEST(std::isnan(bus3.getV()));
    bus3.setAngle(50.0).setV(60.0);
    BOOST_CHECK_CLOSE(50.0, bus3.getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, bus3.getV(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(CalculatedBusTopology) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    vl.newLoad()
        .setId("LOAD1")
        .setNode(1)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("BK1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    Switch& sw = vl.getNodeBreakerView().newDisconnector()
        .setId("SW1")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("BK2")
        .setNode1(2)
        .setNode2(3)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    network.newLine()
        .setId("VL1_VL2")
        .setVoltageLevel1(vl.getId())
        .setNode1(2)
        .setVoltageLevel2(vl2.getId())
        .setNode2(0)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    network.newLine()
        .setId("VL1_VL2_2")
        .setVoltageLevel1(vl.getId())
        .setNode1(3)
        .setVoltageLevel2(vl2.getId())
        .setNode2(1)
        .setR(4.0)
        .setX(44.0)
        .setG1(5.0)
        .setB1(0.3)
        .setG2(6.0)
        .setB2(0.5)
        .add();

    VoltageLevel& vlTest = vl;
    auto& busView = vlTest.getBusView();
    const auto& cBusView = vlTest.getBusView();
    BOOST_CHECK_EQUAL(1UL, boost::size(busView.getBuses()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cBusView.getBuses()));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_TRUE(busView.getMergedBus("BBS"));
    POWSYBL_ASSERT_REF_TRUE(cBusView.getMergedBus("BBS"));
    const auto& calculatedBus = busView.getBus("VL_0").get();
    BOOST_CHECK_EQUAL("VL_0", calculatedBus.getId());
    BOOST_CHECK(calculatedBus.getOptionalName().empty());

    sw.setOpen(true);
    BOOST_CHECK_EQUAL(2UL, boost::size(busView.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cBusView.getBuses()));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_2"));
    sw.setOpen(false);
    BOOST_CHECK_EQUAL(1UL, boost::size(busView.getBuses()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cBusView.getBuses()));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    sw.setRetained(true);
    BOOST_CHECK_EQUAL(1UL, boost::size(busView.getBuses()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cBusView.getBuses()));
}

BOOST_AUTO_TEST_CASE(CalculatedBusTopology2) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .add();

    auto& l1 = vl.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    POWSYBL_ASSERT_REF_FALSE(l1.getTerminal().getBusBreakerView().getConnectableBus());
}

BOOST_AUTO_TEST_CASE(CalculatedBusTopology3) {
    Network network("test", "test");

    Substation& s1 = network.newSubstation()
        .setId("S")
        .add();

    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(1.0)
        .add();

    vl1.getNodeBreakerView().newBusbarSection()
        .setId("B0")
        .setNode(0)
        .add();

    vl1.getNodeBreakerView().newBusbarSection()
        .setId("B1")
        .setNode(1)
        .add();

    vl1.getNodeBreakerView().newBusbarSection()
        .setId("B2")
        .setNode(2)
        .add();

    auto& l0 = vl1.newLoad()
        .setId("L0")
        .setNode(6)
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    auto& l1 = vl1.newLoad()
        .setId("L1")
        .setNode(3)
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    auto& l2 = vl1.newLoad()
        .setId("L2")
        .setNode(4)
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    auto& l3 = vl1.newLoad()
        .setId("L3")
        .setNode(5)
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L0-node")
        .setNode1(0)
        .setNode2(6)
        .setOpen(false)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L1-node")
        .setNode1(4)
        .setNode2(10)
        .setOpen(true)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("L0-B0")
        .setNode1(3)
        .setNode2(1)
        .setOpen(false)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("B0-node")
        .setNode1(1)
        .setNode2(10)
        .setRetained(true)
        .setOpen(true)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("node-B1")
        .setNode1(10)
        .setNode2(2)
        .setRetained(true)
        .setOpen(false)
        .add();

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getBuses()));

    // load "L0" is connected to bus "VL_0"
    POWSYBL_ASSERT_REF_TRUE(l0.getTerminal().getBusView().getBus());
    BOOST_CHECK_EQUAL("VL_0", l0.getTerminal().getBusView().getConnectableBus().get().getId());

    // load "L1" is connected to bus "VL_1"
    POWSYBL_ASSERT_REF_TRUE(l1.getTerminal().getBusView().getBus());
    BOOST_CHECK_EQUAL("VL_1", l1.getTerminal().getBusView().getConnectableBus().get().getId());

    // load "L2" is not connected but is connectable to bus "VL_1"
    POWSYBL_ASSERT_REF_FALSE(l2.getTerminal().getBusView().getBus());
    BOOST_CHECK_EQUAL("VL_1", l2.getTerminal().getBusView().getConnectableBus().get().getId());

    // load "L3" is not connected but is connectable to bus "VL_0"
    POWSYBL_ASSERT_REF_FALSE(l3.getTerminal().getBusView().getBus());
    BOOST_CHECK_EQUAL("VL_0", l3.getTerminal().getBusView().getConnectableBus().get().getId());
}

BOOST_AUTO_TEST_CASE(TerminalTest) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setName("VL_name")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB1")
        .setNode1(1)
        .setNode2(99)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newInternalConnection()
        .setNode1(99)
        .setNode2(2)
        .add();

    Load& l1 = vl.newLoad()
        .setId("LOAD1")
        .setNode(2)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD2")
        .setNode1(0)
        .setNode2(3)
        .setRetained(false)
        .setOpen(false)
        .add();

    Load& l2 = vl.newLoad()
        .setId("LOAD2")
        .setNode(3)
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB2")
        .setNode1(3)
        .setNode2(4)
        .setRetained(false)
        .setOpen(false)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    Line& line = network.newLine()
        .setId("VL1_VL2")
        .setVoltageLevel1(vl.getId())
        .setNode1(4)
        .setVoltageLevel2(vl2.getId())
        .setNode2(0)
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    VoltageLevel& vl3 = s.newVoltageLevel()
        .setId("VL3")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    Load& l3 = vl3.newLoad()
        .setId("LOAD3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .setName("LOAD3_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    Terminal& terminal = l1.getTerminal();
    BOOST_TEST(std::isnan(terminal.getV()));
    BOOST_TEST(std::isnan(terminal.getAngle()));
    BOOST_TEST(std::isnan(terminal.getI()));

    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(vl.disconnect(terminal));
    BOOST_TEST(!terminal.isConnected());
    // TODO(thiebarr): BOOST_TEST(!vl.disconnect(terminal));
    // TODO(thiebarr): BOOST_TEST(!terminal.isConnected());
    BOOST_TEST(vl.connect(terminal));
    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(!vl.connect(terminal));
    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(terminal.disconnect());
    BOOST_TEST(!terminal.isConnected());
    BOOST_TEST(terminal.connect());
    BOOST_TEST(terminal.isConnected());

    POWSYBL_ASSERT_THROW(vl3.attach(l1.getTerminal(), true), ValidationException, "Load 'LOAD1': Voltage level 'VL3' has a bus/breaker topology, a bus connection should be specified instead of a node connection");
    POWSYBL_ASSERT_THROW(vl.attach(l3.getTerminal(), true), ValidationException, "Load 'LOAD3': Voltage level VL has a node/breaker topology, a node connection should be specified instead of a bus connection");

    const Terminal& cTerminal = l1.getTerminal();
    auto& busBreakerView = terminal.getBusBreakerView();
    const auto& cBusBreakerView = cTerminal.getBusBreakerView();
    BOOST_TEST(stdcxx::areSame(busBreakerView, cBusBreakerView));
    BOOST_TEST(stdcxx::areSame(busBreakerView.getBus().get(), busBreakerView.getConnectableBus().get()));
    BOOST_TEST(stdcxx::areSame(cBusBreakerView.getBus().get(), cBusBreakerView.getConnectableBus().get()));
    const auto& calculatedBus = busBreakerView.getBus().get();
    BOOST_CHECK_EQUAL("VL_0", calculatedBus.getId());
    BOOST_CHECK_EQUAL("VL_name_0", calculatedBus.getOptionalName());
    BOOST_CHECK_EQUAL("VL_name_0", calculatedBus.getNameOrId());


    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus("BUS1"), AssertionError, "Not implemented");

    auto& busView = terminal.getBusView();
    const auto& cBusView = cTerminal.getBusView();
    BOOST_TEST(stdcxx::areSame(busView, cBusView));
    BOOST_TEST(stdcxx::areSame(busView.getBus().get(), busView.getConnectableBus().get()));
    BOOST_TEST(stdcxx::areSame(cBusView.getBus().get(), cBusView.getConnectableBus().get()));
    const auto& calculatedBus2 = busView.getBus().get();
    BOOST_CHECK_EQUAL("VL_0", calculatedBus2.getId());
    BOOST_CHECK_EQUAL("VL_name_0", calculatedBus2.getOptionalName());
    BOOST_CHECK_EQUAL("VL_name_0", calculatedBus.getNameOrId());

    BOOST_TEST(stdcxx::areSame(terminal.getNodeBreakerView(), cTerminal.getNodeBreakerView()));
    BOOST_CHECK_EQUAL(2UL, terminal.getNodeBreakerView().getNode());

    Terminal& terminal2 = l2.getTerminal();
    BOOST_TEST(!terminal2.disconnect());

    Terminal& terminal3 = line.getTerminal1();
    Terminal& terminal4 = line.getTerminal2();
    BOOST_TEST(!stdcxx::areSame(terminal3, terminal4));
    POWSYBL_ASSERT_THROW(line.getTerminal(static_cast<iidm::Branch::Side>(3U)), AssertionError, "Unexpected Side value: 3");
    BOOST_TEST(!terminal4.disconnect());

    BusbarSection& bbs = vl.getNodeBreakerView().getBusbarSection("BBS");
    BOOST_CHECK_CLOSE(0, bbs.getTerminal().getI(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(bbs.getTerminal().setP(1.0), ValidationException, "Busbar section 'BBS': cannot set active power on a busbar section");
    POWSYBL_ASSERT_THROW(bbs.getTerminal().setQ(2.0), ValidationException, "Busbar section 'BBS': cannot set reactive power on a busbar section");

    //test const version
    const auto& cVl = vl;
    const BusbarSection& cBbs = cVl.getNodeBreakerView().getBusbarSection("BBS");
    BOOST_CHECK_CLOSE(0, cBbs.getTerminal().getI(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(TestRemoveVoltageLevelWithInternalConnectionsIssue) {
    Network network("test", "test");

    createNetwork(network);
    network.getLine("L6").remove(); // needed to be allowed to remove the voltage level
    network.remove(network.getVoltageLevel("S5 10kV"));
    POWSYBL_ASSERT_THROW(network.getVoltageLevel("S5 10kV"), PowsyblException, "Unable to find to the identifiable 'S5 10kV'");
}

BOOST_AUTO_TEST_CASE(issue318_invalidateCache) {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();
    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(90.0)
        .setLowVoltageLimit(88.0)
        .setHighVoltageLimit(96.0)
        .add();

    BusbarSection& bbs1 = vl1.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setName("BBS1_NAME")
        .setNode(0)
        .add();

    // In that case, the bus cache is initialized, but there is no bus for node 0
    BOOST_CHECK(!bbs1.getTerminal().getBusView().getBus());

    BusbarSection& bbs2 = vl1.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setName("BBS2_NAME")
        .setNode(1)
        .add();

    // Without the fix, this call throws an exception (ArrayIndexOutOfBoundsException), but an invalid reference is expected
    BOOST_CHECK(!bbs2.getTerminal().getBusView().getBus());
}

BOOST_AUTO_TEST_CASE(NbkComprehensiveErrorMessage) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    BusbarSection& busbarSection = network.getBusbarSection("S1VL2_BBS1");
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.getPhaseTapChanger().setRegulationTerminal(stdcxx::ref(busbarSection.getTerminal()));

    stdcxx::Properties properties;
    properties.set(converter::ExportOptions::TOPOLOGY_LEVEL, "NODE_BREAKER");

    std::stringstream ss;

    // make sure no error is thrown while exporting to NODE_BREAKER topology
    BOOST_CHECK_NO_THROW({
         Network::writeXml("network.xiidm", ss, network, converter::ExportOptions(properties));
         Network::readXml("network.xiidm", ss);
    });

    // make sure an error is thrown when exporting to BUS_BREAKER topology (see #295)
    properties.set(converter::ExportOptions::TOPOLOGY_LEVEL, "BUS_BREAKER");
    POWSYBL_ASSERT_THROW(Network::writeXml("network.xiidm", ss, network, converter::ExportOptions(properties)), PowsyblException, "Terminal ref should not point to a busbar section (here S1VL2_BBS1). Try to export in node-breaker or delete this terminal ref.");
}

BOOST_AUTO_TEST_CASE(getMaximumNodeIndexTest) {
    Network network("test", "test");

    Substation& substation = network.newSubstation()
        .setId("S1")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(380.0)
        .add();

    vl1.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl1.getNodeBreakerView().newBreaker()
        .setId("SWB1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();
    BOOST_CHECK_EQUAL(1, vl1.getNodeBreakerView().getMaximumNodeIndex());

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(225.0)
        .add();

    vl2.newLoad()
        .setId("LOAD2")
        .setNode(0)
        .setP0(60.0)
        .setQ0(70.0)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("SWB2")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl2.getNodeBreakerView().newBreaker()
        .setId("SWB3")
        .setNode1(1)
        .setNode2(12)
        .setRetained(false)
        .setOpen(false)
        .add();

    BOOST_CHECK_EQUAL(12, vl2.getNodeBreakerView().getMaximumNodeIndex());

    VoltageLevel& vl3 = substation.newVoltageLevel()
        .setId("VL3")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(225.0)
        .add();
    BOOST_CHECK_EQUAL(-1, vl3.getNodeBreakerView().getMaximumNodeIndex());
}

BOOST_AUTO_TEST_CASE(testCalculatedBus) {
    Network network = createIsolatedLoadNetwork();

    const auto& busL0 = network.getLoad("L0").getTerminal().getBusBreakerView().getBus();
    BOOST_CHECK(busL0);
    BOOST_CHECK_EQUAL("VL", busL0.get().getVoltageLevel().getId());
    BOOST_CHECK_EQUAL("VL_0", busL0.get().getId());

    BOOST_CHECK(!network.getBusBreakerView().getBus("unknownBus"));


    network.getVoltageLevel("VL").getNodeBreakerView().newBusbarSection() 
        .setId("VL_0")
        .setNode(10)
        .add();
    const auto& busL0bis = network.getLoad("L0").getTerminal().getBusBreakerView().getBus();
    BOOST_CHECK_EQUAL("VL_0#0", busL0bis.get().getId());
}

BOOST_AUTO_TEST_CASE(testRemove) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    Substation& sub = network.getSubstation("S1");

    // Disconnect the substation from the network
    network.getHvdcLine("HVDC1").remove();
    network.getHvdcLine("HVDC2").remove();
    sub.getVoltageLevels();

    for (auto& voltageLevel : sub.getVoltageLevels()) {
        for (auto& vscConverter : voltageLevel.getVscConverterStations()) {
            vscConverter.remove();
        }
    }

    // Remove 1 switch
    VoltageLevel& vl1 = network.getVoltageLevel("S1VL1");
    vl1.getNodeBreakerView().removeSwitch("S1VL1_LD1_BREAKER");
    BOOST_CHECK(!vl1.getNodeBreakerView().getSwitch("S1VL1_LD1_BREAKER"));

    // Remove substation
    sub.remove();
    POWSYBL_ASSERT_THROW(network.getSubstation("S1"), PowsyblException, "Unable to find to the identifiable 'S1'");
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
