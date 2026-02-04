/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createConnectableTestNetwork() {
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
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    VoltageLevel& vl3 = s1.newVoltageLevel()
                           .setId("VL3")
                           .setNominalV(1.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();

    // Busbar sections
    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS11")
        .setNode(0)
        .add();
    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS21")
        .setNode(1)
        .add();
    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS12")
        .setNode(2)
        .add();
    vl1.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS22")
        .setNode(3)
        .add();
    vl2.getBusBreakerView()
        .newBus()
        .setId("bus2A")
        .add();
    vl2.getBusBreakerView()
        .newBus()
        .setId("bus2B")
        .add();
    vl3.getBusBreakerView()
        .newBus()
        .setId("bus3A")
        .add();
    vl3.getBusBreakerView()
        .newBus()
        .setId("bus3B")
        .add();

    // Disconnectors for coupling
    vl1.getNodeBreakerView().newDisconnector().setId("D_BBS11_BBS12").setNode1(0).setNode2(2).setOpen(true).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D_BBS21_BBS22").setNode1(1).setNode2(3).setOpen(false).add();

    // Line and transformer
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
        .setBus2("bus2A")
        .setConnectableBus2("bus2A")
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
        .setVoltageLevel2("VL3")
        .setNode1(5)
        .setBus2("bus3A")
        .setConnectableBus2("bus3A")
        .add();
    s1.newThreeWindingsTransformer()
        .setId("twt")
        .setName("TWT_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setRatedS(1.2)
        .setVoltageLevel("VL1")
        .setNode(6)
        .add()
        .newLeg2()
        .setR(2.03)
        .setX(2.04)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.05)
        .setRatedS(2.06)
        .setVoltageLevel("VL2")
        .setBus("bus2B")
        .setConnectableBus("bus2B")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.5)
        .setRatedS(3.6)
        .setVoltageLevel("VL3")
        .setBus("bus3B")
        .setConnectableBus("bus3B")
        .add()
        .add();

    // Breakers
    vl1.getNodeBreakerView().newBreaker().setId("B_L1_1").setNode1(4).setNode2(7).setOpen(false).setFictitious(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B_L1_2").setNode1(4).setNode2(7).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("B_L2").setNode1(5).setNode2(8).setOpen(true).setFictitious(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B_TWT").setNode1(6).setNode2(9).setOpen(true).setFictitious(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B0").setNode1(7).setNode2(17).setOpen(false).setFictitious(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B1").setNode1(8).setNode2(11).setOpen(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B2").setNode1(9).setNode2(12).setOpen(false).setFictitious(true).add();
    vl1.getNodeBreakerView().newBreaker().setId("B3").setNode1(7).setNode2(8).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("B4").setNode1(8).setNode2(9).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("B5").setNode1(17).setNode2(10).setOpen(false).setFictitious(true).add();

    // Disconnectors
    vl1.getNodeBreakerView().newDisconnector().setId("D0").setNode1(0).setNode2(10).setOpen(true).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D1").setNode1(1).setNode2(10).setOpen(false).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D2").setNode1(0).setNode2(11).setOpen(false).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D3").setNode1(1).setNode2(11).setOpen(true).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D4").setNode1(2).setNode2(12).setOpen(false).add();
    vl1.getNodeBreakerView().newDisconnector().setId("D5").setNode1(3).setNode2(12).setOpen(true).add();
    return network;
}

BOOST_AUTO_TEST_SUITE(ConnectableTestSuite)

BOOST_AUTO_TEST_CASE(nominallyConnectedTest) {
    Network network = createConnectableTestNetwork();
    VoltageLevel::NodeBreakerView& topo = network.getVoltageLevel("VL1").getNodeBreakerView();
    Line& line1 = network.getLine("L1");

    // Line1 is fully connected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }

    // Failing disconnection
    BOOST_CHECK(!line1.disconnect(SwitchPredicate::IS_NONFICTIONAL_CLOSED_BREAKER()));

    // disconnect the line 1
    BOOST_CHECK(line1.disconnect());

    // check line 1 is disconnected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(4));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_FALSE(terminal.get().getBusView().getBus());
        BOOST_CHECK(!terminal.get().isConnected());
    }

    // disconnect the already fully disconnected line 1
    BOOST_CHECK(!line1.disconnect());

    // Reconnect the line 1
    BOOST_CHECK(line1.connect());

    // check line 1 is fully reconnected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    for (const auto& terminal : line1.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }
}

BOOST_AUTO_TEST_CASE(partiallyConnectedTest) {
    Network network = createConnectableTestNetwork();
    VoltageLevel::NodeBreakerView& topo = network.getVoltageLevel("VL1").getNodeBreakerView();
    Line& line2 = network.getLine("L2");
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("twt");

    // Line2 and twt are fully connected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(6));
    POWSYBL_ASSERT_REF_TRUE(line2.getTerminals().at(1).get().getBusView().getBus());
    POWSYBL_ASSERT_REF_FALSE(twt.getTerminals().at(0).get().getBusView().getBus());
    POWSYBL_ASSERT_REF_TRUE(twt.getTerminals().at(1).get().getBusView().getBus());
    POWSYBL_ASSERT_REF_TRUE(twt.getTerminals().at(2).get().getBusView().getBus());
    BOOST_CHECK(!line2.getTerminals().at(0).get().isConnected());
    BOOST_CHECK(line2.getTerminals().at(1).get().isConnected());
    BOOST_CHECK(!twt.getTerminals().at(0).get().isConnected());
    BOOST_CHECK(twt.getTerminals().at(1).get().isConnected());
    BOOST_CHECK(twt.getTerminals().at(2).get().isConnected());

    // Failing connection
    BOOST_CHECK(!line2.connect(SwitchPredicate::IS_NONFICTIONAL_BREAKER()));

    // connect the line 2
    BOOST_CHECK(line2.connect(SwitchPredicate::IS_BREAKER_OR_DISCONNECTOR()));

    // check line 2 is connected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(5));
    for (const auto& terminal : line2.getTerminals()) {
        POWSYBL_ASSERT_REF_TRUE(terminal.get().getBusView().getBus());
        BOOST_CHECK(terminal.get().isConnected());
    }

    // connect the already fully connected line 2
    BOOST_CHECK(!line2.connect());

    // Disconnect the twt
    BOOST_CHECK(twt.disconnect(SwitchPredicate::IS_CLOSED_BREAKER()));

    // check twt is disconnected
    POWSYBL_ASSERT_REF_TRUE(topo.getOptionalTerminal(6));
    for (const auto& terminal : twt.getTerminals()) {
        POWSYBL_ASSERT_REF_FALSE(terminal.get().getBusView().getBus());
        BOOST_CHECK(!terminal.get().isConnected());
    }
}

BOOST_AUTO_TEST_CASE(oneTerminalConnectedTest) {
    Network network = createConnectableTestNetwork();
    Line& line2 = network.getLine("L2");
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("twt");

    // Line and twt are connected
    BOOST_CHECK(!line2.getTerminals().at(0).get().isConnected());
    BOOST_CHECK(line2.getTerminals().at(1).get().isConnected());
    BOOST_CHECK(!twt.getTerminal(ThreeSides::ONE).isConnected());
    BOOST_CHECK(twt.getTerminal(ThreeSides::TWO).isConnected());
    BOOST_CHECK(twt.getTerminal(ThreeSides::THREE).isConnected());

    //Connect side ONE only :
    BOOST_CHECK(line2.connect(SwitchPredicate::IS_BREAKER(), ThreeSides::ONE));
    BOOST_CHECK(line2.getTerminals().at(0).get().isConnected());

    //disconnect twt side Three only :
    BOOST_CHECK(twt.disconnect(SwitchPredicate::IS_BREAKER_OR_DISCONNECTOR(), ThreeSides::THREE));
    BOOST_CHECK(!twt.getTerminal(ThreeSides::ONE).isConnected());
    BOOST_CHECK(twt.getTerminal(ThreeSides::TWO).isConnected());
    BOOST_CHECK(!twt.getTerminal(ThreeSides::THREE).isConnected());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
