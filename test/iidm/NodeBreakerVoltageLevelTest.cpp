/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
#include <powsybl/iidm/Switch.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(NodeBreakerVoltageLevel, BusbarSection) {
    const Network& network = createNetwork();

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
    ASSERT_TRUE(stdcxx::areSame(voltageLevel, bbs.getTerminal().getVoltageLevel()));

    BusbarSection& bbs2 = network.getBusbarSection("BBS");
    ASSERT_TRUE(stdcxx::areSame(bbs, bbs2));

    BusbarSectionAdder adder = voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("DUPLICATE")
        .setNode(0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Busbar section 'DUPLICATE': An equipment (BBS) is already connected to the node 0 of voltage level VL2");
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
    voltageLevel.getNodeBreakerView().setNodeCount(2);
    ASSERT_EQ(2, voltageLevel.getNodeBreakerView().getNodeCount());

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

    // Remove a busbar section
    bbs1.remove();
    // TODO(mathbagu): ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getBusbarSectionCount());
    bbs2.remove();
    ASSERT_EQ(0, voltageLevel.getNodeBreakerView().getBusbarSectionCount());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView().removeSwitch("UNKNOWN"), PowsyblException, "Switch 'UNKNOWN' not found in voltage level 'VL2'");
    ASSERT_EQ(1, voltageLevel.getNodeBreakerView().getSwitchCount());
    voltageLevel.getNodeBreakerView().removeSwitch("BK");
    ASSERT_EQ(0, voltageLevel.getNodeBreakerView().getSwitchCount());
}

TEST(NodeBreakerVoltageLevel, CalculatedBusTopology) {
    // TODO(mathbagu): Need to implement at least one kind of branch
}

}  // namespace iidm

}  // namespace powsybl
