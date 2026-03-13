/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ForkConnectDisconnectTestSuite)

Network createForkTestNetwork() {
    Network network("test", "test");
    // Substations
    Substation& s1 = network.newSubstation()
                        .setId("S1")
                        .setCountry(Country::FR)
                        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
                           .setId("VL1")
                           .setNominalV(1.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();
    VoltageLevel& vl2 = s1.newVoltageLevel()
                           .setId("VL2")
                           .setNominalV(1.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();

    // Busbar sections
    vl1.getNodeBreakerView()
            .newBusbarSection()
            .setId("BBS11")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView()
            .newBusbarSection()
            .setId("BBS2A")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView()
            .newBusbarSection()
            .setId("BBS2B")
            .setNode(1)
            .add();

    //Lines
    network.newLine()
        .setId("L1")
        .setName("LINE1")
        .setR(1.0)
        .setX(2.0)
        .setG1(3.0)
        .setG2(3.5)
        .setB1(4.0)
        .setB2(4.5)
        .setVoltageLevel1("VL1")
        .setVoltageLevel2("VL2")
        .setNode1(4)
        .setNode2(3)
        .add();
    network.newLine()
        .setId("L2")
        .setName("LINE2")
        .setR(1.0)
        .setX(2.0)
        .setG1(3.0)
        .setG2(3.5)
        .setB1(4.0)
        .setB2(4.5)
        .setVoltageLevel1("VL1")
        .setVoltageLevel2("VL2")
        .setNode1(5)
        .setNode2(5)
        .add();

    // Disconnectors and breakers:
    // VL1
    vl1.getNodeBreakerView().newDisconnector().setId("D_L1").setNode1(4).setNode2(6).setOpen(false).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D_L2").setNode1(5).setNode2(6).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("B_L1_L2").setNode1(6).setNode2(7).setOpen(false).setFictitious(false).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D0").setNode1(7).setNode2(0).setOpen(false).add();
    // VL2
    vl2.getNodeBreakerView().newBreaker().setId("B_L1_VL2").setNode1(3).setNode2(2).setOpen(false).setFictitious(false).add();
    vl2.getNodeBreakerView().newDisconnector().setId("D_L1_BBS2A").setNode1(2).setNode2(0).setOpen(false).add();
    vl2.getNodeBreakerView().newDisconnector().setId("D_L1_BBS2B").setNode1(2).setNode2(1).setOpen(true).add();
    vl2.getNodeBreakerView().newBreaker().setId("B_L2_VL2").setNode1(5).setNode2(4).setOpen(false).setFictitious(false).add();
    vl2.getNodeBreakerView().newDisconnector().setId("D_L2_BBS2A").setNode1(4).setNode2(0).setOpen(true).add();
    vl2.getNodeBreakerView().newDisconnector().setId("D_L2_BBS2B").setNode1(4).setNode2(1).setOpen(false).add();
    return network;
}

BOOST_AUTO_TEST_CASE(fullyClosedTest) {
    Network network = createForkTestNetwork();

    VoltageLevel::NodeBreakerView& topo = network.getVoltageLevel("VL1").getNodeBreakerView();
    Line& line1 = network.getLine("L1");
    Line& line2 = network.getLine("L2");
    Switch& disconnector = network.getSwitch("D_L1");

    //Both lines are fully connected:
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }
    for (const auto& terminal : line2.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }

    // disconnect the line 1
    BOOST_CHECK(line1.disconnect(SwitchPredicate::TRUE()));

    // D_L1 should be open
    BOOST_CHECK(disconnector.isOpen());

    // check that L1 is disconnected while L2 is still connected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_FALSE(terminal.get().getBusView().getBus());
        BOOST_CHECK(!terminal.get().isConnected());
    }
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    for (const auto& terminal : line2.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }
}

BOOST_AUTO_TEST_CASE(forkDisconnectedTest) {
    Network network = createForkTestNetwork();

    VoltageLevel::NodeBreakerView& topo = network.getVoltageLevel("VL1").getNodeBreakerView();
    Line& line1 = network.getLine("L1");
    Line& line2 = network.getLine("L2");
    Switch& disconnectorL1 = network.getSwitch("D_L1");
    Switch& disconnectorL2 = network.getSwitch("D_L2");
    Switch& breaker = network.getSwitch("B_L1_L2");

    //open breaker
    breaker.setOpen(true);

    //Both lines are fully connected:
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }
    for (const auto& terminal : line2.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }

    // disconnect the line 1
    BOOST_CHECK(line1.disconnect(SwitchPredicate::TRUE()));

    // D_L1 should be open but not D_L2
    BOOST_CHECK(disconnectorL1.isOpen());
    BOOST_CHECK(!disconnectorL2.isOpen());

    // check that L1 and L2 are disconnected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_FALSE(terminal.get().getBusView().getBus());
        BOOST_CHECK(!terminal.get().isConnected());
    }
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    POWSYBL_ASSERT_REF_FALSE(line2.getTerminal1().getBusView().getBus());
    BOOST_CHECK(!line2.getTerminal1().isConnected());
    POWSYBL_ASSERT_REF_TRUE(line2.getTerminal2().getBusView().getBus());
    BOOST_CHECK(line2.getTerminal2().isConnected());
 
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
