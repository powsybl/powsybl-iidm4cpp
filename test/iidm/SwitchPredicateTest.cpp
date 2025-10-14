/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createSwitchPredicateTestNetwork() {
    Network network("test", "test");
    Substation &s1 = network.newSubstation()
                         .setId("S1")
                         .setCountry(Country::FR)
                         .add();
    VoltageLevel &vl = s1.newVoltageLevel()
                           .setId("VL1")
                           .setNominalV(1.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();

    // Busbar sections
    vl.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS11")
        .setNode(0)
        .add();
    vl.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS21")
        .setNode(1)
        .add();
    vl.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS12")
        .setNode(2)
        .add();
    vl.getNodeBreakerView()
        .newBusbarSection()
        .setId("BBS22")
        .setNode(3)
        .add();

    // Disconnectors for coupling
    vl.getNodeBreakerView().newDisconnector().setId("D_BBS11_BBS12").setNode1(0).setNode2(2).setOpen(true).add();
    vl.getNodeBreakerView().newDisconnector().setId("D_BBS21_BBS22").setNode1(1).setNode2(3).setOpen(false).add();

    // Generators and loads
    vl.newLoad()
        .setId("L1")
        .setNode(4)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.newGenerator()
        .setId("G1")
        .setNode(5)
        .setMaxP(100)
        .setMinP(50)
        .setTargetP(100)
        .setTargetV(400)
        .setVoltageRegulatorOn(true)
        .add();
    vl.newGenerator()
        .setId("G2")
        .setNode(6)
        .setMaxP(100)
        .setMinP(50)
        .setTargetP(100)
        .setTargetV(400)
        .setVoltageRegulatorOn(true)
        .add();

    // Breakers
    vl.getNodeBreakerView().newBreaker().setId("B_L1_1").setNode1(4).setNode2(7).setOpen(false).setFictitious(true).add();
    vl.getNodeBreakerView().newBreaker().setId("B_L1_2").setNode1(4).setNode2(7).setOpen(false).add();
    vl.getNodeBreakerView().newBreaker().setId("B_G1").setNode1(5).setNode2(8).setOpen(true).add();
    vl.getNodeBreakerView().newBreaker().setId("B_G2").setNode1(6).setNode2(9).setOpen(true).setFictitious(true).add();
    vl.getNodeBreakerView().newBreaker().setId("B0").setNode1(7).setNode2(17).setOpen(false).add();
    vl.getNodeBreakerView().newBreaker().setId("B1").setNode1(8).setNode2(11).setOpen(true).add();
    vl.getNodeBreakerView().newBreaker().setId("B2").setNode1(9).setNode2(12).setOpen(false).setFictitious(true).add();
    vl.getNodeBreakerView().newBreaker().setId("B3").setNode1(7).setNode2(8).setOpen(false).add();
    vl.getNodeBreakerView().newSwitch().setId("B4").setNode1(8).setNode2(9).setOpen(false).setKind(SwitchKind::LOAD_BREAK_SWITCH).add();
    vl.getNodeBreakerView().newBreaker().setId("B5").setNode1(17).setNode2(10).setOpen(false).add();

    // Disconnectors
    vl.getNodeBreakerView().newDisconnector().setId("D0").setNode1(0).setNode2(10).setOpen(true).add();
    vl.getNodeBreakerView().newDisconnector().setId("D1").setNode1(1).setNode2(10).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D2").setNode1(0).setNode2(11).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D3").setNode1(1).setNode2(11).setOpen(true).add();
    vl.getNodeBreakerView().newDisconnector().setId("D4").setNode1(2).setNode2(12).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D5").setNode1(3).setNode2(12).setOpen(true).add();
    return network;
}

BOOST_AUTO_TEST_SUITE(SwitchPredicateTestSuite)

BOOST_AUTO_TEST_CASE(NonFictionalClosedBreakersTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_NONFICTIONAL_CLOSED_BREAKER();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B1"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B_L1_1"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12").get()));
    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B1").get()));
    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B_L1_1").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5").get()));
}

BOOST_AUTO_TEST_CASE(NonFictionalBreakersTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_NONFICTIONAL_BREAKER();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B_L1_1"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12").get()));
    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B_L1_1").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5").get()));
}

BOOST_AUTO_TEST_CASE(ClosedBreakersTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_CLOSED_BREAKER();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B1"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12").get()));
    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B1").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5").get()));
}

BOOST_AUTO_TEST_CASE(BreakerOrDisconnectorTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_BREAKER_OR_DISCONNECTOR();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B4"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B4").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D_BBS11_BBS12").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B5").get()));
}

BOOST_AUTO_TEST_CASE(OpenDisconnectorTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_OPEN_DISCONNECTOR();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B4"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D4"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("B4").get()));
    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D4").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D5").get()));
}

BOOST_AUTO_TEST_CASE(OpenTest) {
    Network network = createSwitchPredicateTestNetwork();
    stdcxx::Predicate<Switch> predicate = SwitchPredicate::IS_OPEN();

    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D4"));
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D5"));

    BOOST_CHECK(!predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D4").get()));
    BOOST_CHECK(predicate(network.getVoltageLevel("VL1").getNodeBreakerView().getSwitch("D5").get()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
