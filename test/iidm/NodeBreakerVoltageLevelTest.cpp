/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/BusView.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>

#include "AssertionUtils.hpp"
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
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(0.0)
        .setHighVoltageLimit(250.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(3);

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
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(0.0)
        .setHighVoltageLimit(250.0)
        .add();

    vl2.getNodeBreakerView().setNodeCount(5);

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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl3.getNodeBreakerView().setNodeCount(3);

    vl3.getNodeBreakerView().newBreaker()
        .setId("SW1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(true)
        .setOpen(true)
        .add();

    vl3.getNodeBreakerView().newInternalConnection()
        .setId("IC")
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

BOOST_AUTO_TEST_SUITE(NodeBreakerVoltageLevelTestSuite)

BOOST_AUTO_TEST_CASE(busbarSection) {
    Network network = createNetwork();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(0, network.getBusbarSectionCount());

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    BOOST_CHECK_EQUAL(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getNodeBreakerView().setNodeCount(1);

    BusbarSection& bbs = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    BOOST_CHECK_EQUAL("BBS", bbs.getId());
    BOOST_CHECK_EQUAL("BBS_NAME", bbs.getName());
    BOOST_CHECK_EQUAL(ConnectableType::BUSBAR_SECTION, bbs.getType());
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

    BOOST_CHECK_EQUAL(2, network.getBusbarSectionCount());
}

BOOST_AUTO_TEST_CASE(switches) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    BOOST_CHECK_EQUAL(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getNodeBreakerView().setNodeCount(2);
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
    BOOST_CHECK_EQUAL("BK_NAME", breaker.getName());
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
}

BOOST_AUTO_TEST_CASE(NodeBreakerViewTest) {
    Network network = createNetwork();
    const unsigned long NODE_COUNT = 3;

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    voltageLevel.getNodeBreakerView().setNodeCount(NODE_COUNT);
    BOOST_CHECK_EQUAL(3, voltageLevel.getNodeBreakerView().getNodeCount());

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
    // Get an unknown switch
    BOOST_TEST(!voltageLevel.getNodeBreakerView().getSwitch("UNKNOWN"));

    unsigned long node1 = voltageLevel.getNodeBreakerView().getNode1("BK");
    BOOST_CHECK_EQUAL(0, node1);
    const auto& terminal1 = voltageLevel.getNodeBreakerView().getTerminal1("BK");
    BOOST_TEST(stdcxx::areSame(bbs1, terminal1.get().getConnectable().get()));

    unsigned long node2 = voltageLevel.getNodeBreakerView().getNode2("BK");
    BOOST_CHECK_EQUAL(1, node2);
    const auto& terminal2 = voltageLevel.getNodeBreakerView().getTerminal2("BK");
    BOOST_TEST(stdcxx::areSame(bbs2, terminal2.get().getConnectable().get()));

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().getNode1("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in the voltage level 'VL2'");

    NodeBreakerView::Traverser traverser = [](unsigned long /*node1*/, const stdcxx::Reference<Switch>& /*sw*/, unsigned long node2) {
        return (node2 < (NODE_COUNT - 1));
    };
    voltageLevel.getNodeBreakerView().traverse(0, traverser);

    // Remove a busbar section
    bbs1.remove();
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getBusbarSectionCount());
    bbs2.remove();
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getBusbarSectionCount());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().removeSwitch("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in voltage level 'VL2'");
    BOOST_CHECK_EQUAL(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());
    voltageLevel.getNodeBreakerView().removeSwitch("BK");
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getSwitchCount());
    BOOST_CHECK_EQUAL(0, voltageLevel.getNodeBreakerView().getInternalConnectionCount());

    // test const versions
    const VoltageLevel& vlTest = network.getVoltageLevel("VL2");
    const NodeBreakerView& view = vlTest.getNodeBreakerView();
    BOOST_CHECK_EQUAL(0, view.getBusbarSectionCount());

    //test internal connections
    BOOST_CHECK_EQUAL(0, view.getSwitchCount());
    BOOST_CHECK_EQUAL(0, view.getInternalConnectionCount());
    auto internalConnectionAdder = voltageLevel.getNodeBreakerView().newInternalConnection();
    internalConnectionAdder.setId("IC_1");

    POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "InternalConnection 'IC_1': first connection node is not set");
    internalConnectionAdder.setNode1(0);

    POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "InternalConnection 'IC_1': second connection node is not set");
    internalConnectionAdder.setNode2(0);
    // TODO(thiebarr):  POWSYBL_ASSERT_THROW(internalConnectionAdder.add(), ValidationException, "InternalConnection 'IC_1': connection nodes must be different");
    internalConnectionAdder.setNode2(1);

    BOOST_CHECK_NO_THROW(internalConnectionAdder.add());
    BOOST_CHECK_EQUAL(0, view.getSwitchCount());
    BOOST_CHECK_EQUAL(1, view.getInternalConnectionCount());
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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(4);

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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl2.getNodeBreakerView().setNodeCount(2);

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
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_2"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_3"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus1("SW1"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus2("SW1"));
    BOOST_CHECK_EQUAL(3, busBreakerView.getBuses().size());
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getSwitch("SW1"));
    BOOST_CHECK_EQUAL(3ul, busBreakerView.getSwitchCount());
    POWSYBL_ASSERT_THROW(busBreakerView.getSwitch("UNKNOWN"), PowsyblException, "Switch UNKNOWN not found");
    POWSYBL_ASSERT_THROW(busBreakerView.newBus(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.newSwitch(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllBuses(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllSwitches(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeBus(""), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeSwitch(""), AssertionError, "Not implemented");

    auto& testBus = busBreakerView.getBus("VL_0").get();
    BOOST_CHECK_EQUAL("VL_0", testBus.getId());
    BOOST_CHECK_EQUAL("VL_0", testBus.getName());
    BOOST_CHECK_CLOSE(7.7, testBus.setAngle(7.7).setV(8.8).getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(8.8, testBus.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2ul, testBus.getConnectedTerminalCount());
    std::vector<std::reference_wrapper<Terminal> > terminals = testBus.getConnectedTerminals();
    BOOST_CHECK_EQUAL(terminals.size(), testBus.getConnectedTerminalCount());
    BOOST_TEST(stdcxx::areSame(vl, testBus.getVoltageLevel()));

    sw.setOpen(false);
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_2"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_3"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_4"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_5"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_6"));

    sw.setRetained(false);
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_6"));
    POWSYBL_ASSERT_REF_TRUE(busBreakerView.getBus("VL_7"));
    POWSYBL_ASSERT_REF_FALSE(busBreakerView.getBus("VL_8"));
}

BOOST_AUTO_TEST_CASE(calculatedBusTraverser) {
    Network network = createCalculatedBusSwitchTestNetwork();
    const Load& l1 = network.getLoad("LOAD1");
    const VoltageLevel& vlG = network.getVoltageLevel("G");

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
    BOOST_TEST(std::isnan(bus3.getAngle()));
    BOOST_TEST(std::isnan(bus3.getV()));
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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(5);

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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl2.getNodeBreakerView().setNodeCount(2);

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

    const VoltageLevel& vlTest = vl;
    const auto& busView = vlTest.getBusView();
    BOOST_CHECK_EQUAL(1ul, vl.getBusView().getBuses().size());
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_TRUE(busView.getMergedBus("BBS"));
    const auto& calculatedBus = busView.getBus("VL_0").get();
    BOOST_CHECK_EQUAL("VL_0", calculatedBus.getId());
    BOOST_CHECK_EQUAL("VL_0", calculatedBus.getName());

    sw.setOpen(true);
    BOOST_CHECK_EQUAL(2ul, busView.getBuses().size());
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_1"));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_2"));
    sw.setOpen(false);
    BOOST_CHECK_EQUAL(1ul, vl.getBusView().getBuses().size());
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_3"));
    sw.setRetained(true);
    BOOST_CHECK_EQUAL(1ul, vl.getBusView().getBuses().size());
}

BOOST_AUTO_TEST_CASE(TerminalTest) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(5);

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
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
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
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl2.getNodeBreakerView().setNodeCount(1);

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
        .setNominalVoltage(400.0)
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
    const auto& calculatedBus = busBreakerView.getBus().get();
    BOOST_CHECK_EQUAL("VL_7", calculatedBus.getId());
    BOOST_CHECK_EQUAL("VL_7", calculatedBus.getName());


    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus("BUS1"), AssertionError, "Not implemented");

    auto& busView = terminal.getBusView();
    const auto& cBusView = cTerminal.getBusView();
    BOOST_TEST(stdcxx::areSame(busView, cBusView));
    BOOST_TEST(stdcxx::areSame(busView.getBus().get(), busView.getConnectableBus().get()));
    const auto& calculatedBus2 = busView.getBus().get();
    BOOST_CHECK_EQUAL("VL_6", calculatedBus2.getId());
    BOOST_CHECK_EQUAL("VL_6", calculatedBus2.getName());

    BOOST_TEST(stdcxx::areSame(terminal.getNodeBreakerView(), cTerminal.getNodeBreakerView()));
    BOOST_CHECK_EQUAL(2ul, terminal.getNodeBreakerView().getNode());

    Terminal& terminal2 = l2.getTerminal();
    BOOST_TEST(!terminal2.disconnect());

    Terminal& terminal3 = line.getTerminal1();
    Terminal& terminal4 = line.getTerminal2();
    BOOST_TEST(!stdcxx::areSame(terminal3, terminal4));
    POWSYBL_ASSERT_THROW(line.getTerminal(static_cast<iidm::Branch::Side>(3u)), AssertionError, "Unexpected side value: 3");
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

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
