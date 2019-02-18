/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/BusView.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(NodeBreakerVoltageLevel, BusbarSection) {
    const Network& network = createNetwork();
    ASSERT_EQ(0, network.getBusbarSectionCount());

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    ASSERT_EQ(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getNodeBreakerView().setNodeCount(1);

    BusbarSection& bbs = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    ASSERT_EQ("BBS", bbs.getId());
    ASSERT_EQ("BBS_NAME", bbs.getName());
    ASSERT_EQ(ConnectableType::BUSBAR_SECTION, bbs.getType());
    ASSERT_TRUE(stdcxx::areSame(voltageLevel, bbs.getTerminal().getVoltageLevel()));

    BusbarSection& bbs2 = network.getBusbarSection("BBS");
    ASSERT_TRUE(stdcxx::areSame(bbs, bbs2));
    ASSERT_TRUE(std::isnan(bbs.getAngle()));
    ASSERT_TRUE(std::isnan(bbs.getV()));

    BusbarSectionAdder adder = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("DUPLICATE")
        .setNode(0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Busbar section 'DUPLICATE': An equipment (BBS) is already connected to the node 0 of voltage level VL2");

    ASSERT_EQ(2, network.getBusbarSectionCount());
}

TEST(NodeBreakerVoltageLevel, Switch) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    ASSERT_EQ(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());

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

    ASSERT_EQ("BK", breaker.getId());
    ASSERT_EQ("BK_NAME", breaker.getName());
    ASSERT_EQ(SwitchKind::BREAKER, breaker.getKind());
    ASSERT_FALSE(breaker.isOpen());
    ASSERT_TRUE(breaker.isRetained());
    ASSERT_FALSE(breaker.isFictitious());
    ASSERT_TRUE(stdcxx::areSame(voltageLevel, breaker.getVoltageLevel()));

    breaker.setOpen(true)
        .setRetained(false)
        .setFictitious(true);
    ASSERT_TRUE(breaker.isOpen());
    ASSERT_FALSE(breaker.isRetained());
    ASSERT_TRUE(breaker.isFictitious());
}

TEST(NodeBreakerVoltageLevel, NodeBreakerView) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL2");
    voltageLevel.getNodeBreakerView().setNodeCount(3);
    ASSERT_EQ(3, voltageLevel.getNodeBreakerView().getNodeCount());

    BusbarSection& bbs1 = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setNode(0)
        .add();

    BusbarSection& bbs2 = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setNode(1)
        .add();

    ASSERT_EQ(0, voltageLevel.getNodeBreakerView().getSwitchCount());
    Switch& breaker = voltageLevel.getNodeBreakerView().newBreaker()
        .setId("BK")
        .setName("BK_NAME")
        .setNode1(0)
        .setNode2(1)
        .add();
    ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getSwitchCount());

    auto swAdder = voltageLevel.getNodeBreakerView().newSwitch()
        .setId("BK2")
        .setName("BK2_NAME")
        .setNode1(1)
        .setNode2(2);
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'BK2': Kind is not set");
    ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getSwitchCount());

    // Get a busbar section
    // TODO(mathbagu): need to refactor VoltageLevel::getConnectable to return a Reference<T> instead of a T&
    // const auto& refBusbarSection = voltageLevel.getNodeBreakerView().getBusbarSection("BBS");
    // ASSERT_TRUE(refBusbarSection);
    // ASSERT_TRUE(stdcxx::areSame(bbs, refBusbarSection.get()));

    const auto& refTerminal = voltageLevel.getNodeBreakerView().getTerminal(0);
    ASSERT_TRUE(refTerminal);
    ASSERT_TRUE(stdcxx::areSame(bbs1, refTerminal.get().getConnectable().get()));

    // Get an unknown busbar section
    ASSERT_FALSE(voltageLevel.getNodeBreakerView().getBusbarSection("UNKNOWN"));

    // Get a switch
    const auto& refBreaker = voltageLevel.getNodeBreakerView().getSwitch("BK");
    ASSERT_TRUE(refBreaker);
    ASSERT_TRUE(stdcxx::areSame(breaker, refBreaker.get()));
    // Get an unknown switch
    ASSERT_FALSE(voltageLevel.getNodeBreakerView().getSwitch("UNKNOWN"));

    unsigned long node1 = voltageLevel.getNodeBreakerView().getNode1("BK");
    ASSERT_EQ(0, node1);
    const auto& terminal1 = voltageLevel.getNodeBreakerView().getTerminal1("BK");
    ASSERT_TRUE(stdcxx::areSame(bbs1, terminal1.get().getConnectable().get()));

    unsigned long node2 = voltageLevel.getNodeBreakerView().getNode2("BK");
    ASSERT_EQ(1, node2);
    const auto& terminal2 = voltageLevel.getNodeBreakerView().getTerminal2("BK");
    ASSERT_TRUE(stdcxx::areSame(bbs2, terminal2.get().getConnectable().get()));

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().getNode1("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in the voltage level 'VL2'");

    // Remove a busbar section
    bbs1.remove();
    // TODO(mathbagu): ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getBusbarSectionCount());
    bbs2.remove();
    ASSERT_EQ(0, voltageLevel.getNodeBreakerView().getBusbarSectionCount());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().removeSwitch("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in voltage level 'VL2'");
    ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    voltageLevel.getNodeBreakerView().removeSwitch("BK");
    ASSERT_EQ(0, voltageLevel.getNodeBreakerView().getSwitchCount());

    // test const versions
    const VoltageLevel& vlTest = network.getVoltageLevel("VL2");
    const NodeBreakerView& view = vlTest.getNodeBreakerView();
    ASSERT_EQ(0, view.getBusbarSectionCount());
}

TEST(NodeBreakerVoltageLevel, CalculatedBusBreakerTopology) {
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
    ASSERT_TRUE(stdcxx::areSame(busBreakerView, cBusBreakerView));
    ASSERT_TRUE(busBreakerView.getBus("VL_0"));
    ASSERT_TRUE(busBreakerView.getBus("VL_1"));
    ASSERT_TRUE(busBreakerView.getBus("VL_2"));
    ASSERT_FALSE(busBreakerView.getBus("VL_3"));
    ASSERT_TRUE(busBreakerView.getBus1("SW1"));
    ASSERT_TRUE(busBreakerView.getBus2("SW1"));
    ASSERT_TRUE(busBreakerView.getSwitch("SW1"));
    POWSYBL_ASSERT_THROW(busBreakerView.getSwitch("UNKNOWN"), PowsyblException, "Switch UNKNOWN not found");
    POWSYBL_ASSERT_THROW(busBreakerView.newBus(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.newSwitch(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllBuses(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeAllSwitches(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeBus(""), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(busBreakerView.removeSwitch(""), AssertionError, "Not implemented");

    auto& testBus = busBreakerView.getBus("VL_0").get();
    ASSERT_DOUBLE_EQ(7.7, testBus.setAngle(7.7).setV(8.8).getAngle());
    ASSERT_DOUBLE_EQ(8.8, testBus.getV());
    ASSERT_EQ(2ul, testBus.getConnectedTerminalCount());
    std::vector<std::reference_wrapper<Terminal> > terminals = testBus.getConnectedTerminals();
    ASSERT_EQ(terminals.size(), testBus.getConnectedTerminalCount());
    ASSERT_TRUE(stdcxx::areSame(vl, testBus.getVoltageLevel()));

    sw.setOpen(false);
    ASSERT_FALSE(busBreakerView.getBus("VL_0"));
    ASSERT_FALSE(busBreakerView.getBus("VL_1"));
    ASSERT_FALSE(busBreakerView.getBus("VL_2"));
    ASSERT_TRUE(busBreakerView.getBus("VL_3"));
    ASSERT_TRUE(busBreakerView.getBus("VL_4"));
    ASSERT_TRUE(busBreakerView.getBus("VL_5"));
    ASSERT_FALSE(busBreakerView.getBus("VL_6"));

    sw.setRetained(false);
    ASSERT_TRUE(busBreakerView.getBus("VL_6"));
    ASSERT_TRUE(busBreakerView.getBus("VL_7"));
    ASSERT_FALSE(busBreakerView.getBus("VL_8"));
}

TEST(NodeBreakerVoltageLevel, CalculatedBusTopology) {
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
    ASSERT_EQ(1ul, vl.getBusView().getBuses().size());
    ASSERT_TRUE(busView.getBus("VL_0"));
    // TODO(thiebarr): ASSERT_TRUE(busView.getMergedBus("BBS"));
    sw.setOpen(true);
    ASSERT_EQ(2ul, busView.getBuses().size());
    ASSERT_TRUE(busView.getBus("VL_1"));
    ASSERT_TRUE(busView.getBus("VL_2"));
    sw.setOpen(false);
    ASSERT_EQ(1ul, vl.getBusView().getBuses().size());
    ASSERT_TRUE(busView.getBus("VL_3"));
    sw.setRetained(true);
    ASSERT_EQ(1ul, vl.getBusView().getBuses().size());
}

TEST(NodeBreakerVoltageLevel, Terminal) {
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
    ASSERT_TRUE(std::isnan(terminal.getV()));
    ASSERT_TRUE(std::isnan(terminal.getAngle()));
    ASSERT_TRUE(std::isnan(terminal.getI()));

    ASSERT_TRUE(terminal.isConnected());
    ASSERT_TRUE(vl.disconnect(terminal));
    ASSERT_FALSE(terminal.isConnected());
    // TODO(thiebarr): ASSERT_FALSE(vl.disconnect(terminal));
    // TODO(thiebarr): ASSERT_FALSE(terminal.isConnected());
    ASSERT_TRUE(vl.connect(terminal));
    ASSERT_TRUE(terminal.isConnected());
    ASSERT_FALSE(vl.connect(terminal));
    ASSERT_TRUE(terminal.isConnected());
    ASSERT_TRUE(terminal.disconnect());
    ASSERT_FALSE(terminal.isConnected());
    ASSERT_TRUE(terminal.connect());
    ASSERT_TRUE(terminal.isConnected());

    POWSYBL_ASSERT_THROW(vl3.attach(l1.getTerminal(), true), ValidationException, "Load 'LOAD1': Voltage level 'VL3' has a bus/breaker topology, a bus connection should be specified instead of a node connection");
    POWSYBL_ASSERT_THROW(vl.attach(l3.getTerminal(), true), ValidationException, "Load 'LOAD3': Voltage level VL has a node/breaker topology, a node connection should be specified instead of a bus connection");

    const Terminal& cTerminal = l1.getTerminal();
    auto& busBreakerView = terminal.getBusBreakerView();
    const auto& cBusBreakerView = cTerminal.getBusBreakerView();
    ASSERT_TRUE(stdcxx::areSame(busBreakerView, cBusBreakerView));
    ASSERT_TRUE(stdcxx::areSame(busBreakerView.getBus().get(), busBreakerView.getConnectableBus().get()));

    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus("BUS1"), AssertionError, "Not implemented");

    auto& busView = terminal.getBusView();
    const auto& cBusView = cTerminal.getBusView();
    ASSERT_TRUE(stdcxx::areSame(busView, cBusView));
    ASSERT_TRUE(stdcxx::areSame(busView.getBus().get(), busView.getConnectableBus().get()));

    ASSERT_TRUE(stdcxx::areSame(terminal.getNodeBreakerView(), cTerminal.getNodeBreakerView()));
    ASSERT_EQ(2ul, terminal.getNodeBreakerView().getNode());

    Terminal& terminal2 = l2.getTerminal();
    ASSERT_FALSE(terminal2.disconnect());

    Terminal& terminal3 = line.getTerminal1();
    Terminal& terminal4 = line.getTerminal2();
    ASSERT_FALSE(stdcxx::areSame(terminal3, terminal4));
    POWSYBL_ASSERT_THROW(line.getTerminal(static_cast<iidm::Branch::Side>(3u)), AssertionError, "Unexpected side value: 3");
    ASSERT_FALSE(terminal4.disconnect());

    BusbarSection& bbs = dynamic_cast<BusbarSection&>(network.getConnectable("BBS"));
    ASSERT_DOUBLE_EQ(0, bbs.getTerminal().getI());
    POWSYBL_ASSERT_THROW(bbs.getTerminal().setP(1.0), ValidationException, "Busbar section 'BBS': cannot set active power on a busbar section");
    POWSYBL_ASSERT_THROW(bbs.getTerminal().setQ(2.0), ValidationException, "Busbar section 'BBS': cannot set reactive power on a busbar section");
}

}  // namespace iidm

}  // namespace powsybl
