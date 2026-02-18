/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createOperationalLimitsOnLineNetwork() {
    Network network = Network("test", "test");
    Substation& s1 = network.newSubstation().setId("S1").setCountry(Country::FR).add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
                           .setId("VL1")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl1.getBusBreakerView().newBus().setId("B1").add();
    Substation& s2 = network.newSubstation().setId("S2").setCountry(Country::FR).add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
                           .setId("VL2")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl2.getBusBreakerView().newBus().setId("B2").add();
    Line& l = network.newLine()
        .setId("L")
        .setVoltageLevel1("VL1")
        .setConnectableBus1("B1")
        .setBus1("B1")
        .setVoltageLevel2("VL2")
        .setConnectableBus2("B2")
        .setBus2("B2")
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setG2(0.0)
        .setB1(0.0)
        .setB2(0.0)
        .add();

    l.newOperationalLimitsGroup1("1")
                .newCurrentLimits()
                .setPermanentLimit(900.0)
                .beginTemporaryLimit()
                .setName("20'")
                .setAcceptableDuration(20 * 60)
                .setValue(1100.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("5'")
                .setAcceptableDuration(5 * 60)
                .setValue(1300.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(1500.0)
                .endTemporaryLimit()
                .add();
    l.newOperationalLimitsGroup1("2")
                .newActivePowerLimits()
                .setPermanentLimit(750.0)
                .beginTemporaryLimit()
                .setName("25'")
                .setAcceptableDuration(25 * 60)
                .setValue(1250.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("7'")
                .setAcceptableDuration(7 * 60)
                .setValue(1350.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("2'")
                .setAcceptableDuration(2 * 60)
                .setValue(1550.0)
                .endTemporaryLimit()
                .add();

    OperationalLimitsGroup& olg1_2 = l.getOperationalLimitsGroup1("2").get();
    olg1_2.newCurrentLimits()
                .setPermanentLimit(1000.0)
                .beginTemporaryLimit()
                .setName("20'")
                .setAcceptableDuration(20 * 60)
                .setValue(1200.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("5'")
                .setAcceptableDuration(5 * 60)
                .setValue(1400.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(1600.0)
                .endTemporaryLimit()
                .add();
    olg1_2.newApparentPowerLimits()
                .setPermanentLimit(890.0)
                .add();

    l.newOperationalLimitsGroup1("3").newApparentPowerLimits().setPermanentLimit(850.0).add();
    l.getOperationalLimitsGroup1("3").get().newCurrentLimits().setPermanentLimit(1000.0) .add();
    l.newOperationalLimitsGroup2("1").newCurrentLimits().setPermanentLimit(850.0).add();
    l.getOperationalLimitsGroup2("1").get().getCurrentLimits().get().setPermanentLimit(800.0);
    l.setSelectedOperationalLimitsGroup2("1");
    l.getOperationalLimitsGroup2("1").get().newApparentPowerLimits().setPermanentLimit(1100.0).add();
    l.getOperationalLimitsGroup2("1").get().getApparentPowerLimits().get().setPermanentLimit(900.0);

    return network;
}

Network createOperationalLimitsOnTWTNetwork() {
    Network network = Network("test_3wt", "test");
    Substation& s1 = network.newSubstation()
                        .setId("S1")
                        .setCountry(Country::FR)
                        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
                           .setId("VL1")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl1.getBusBreakerView().newBus().setId("B1").add();
    VoltageLevel& vl2 = s1.newVoltageLevel()
                           .setId("VL2")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl2.getBusBreakerView().newBus().setId("B2").add();
    VoltageLevel& vl3 = s1.newVoltageLevel()
                           .setId("VL3")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl3.getBusBreakerView().newBus().setId("B3").add();
    s1.newThreeWindingsTransformer()
        .setId("3WT")
        .setRatedU0(132.0)
        .newLeg1()
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(132.0)
        .setVoltageLevel("VL1")
        .setBus("B1")
        .add()
        .newLeg2()
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(132.0)
        .setVoltageLevel("VL2")
        .setBus("B2")
        .add()
        .newLeg3()
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(132.0)
        .setVoltageLevel("VL3")
        .setBus("B3")
        .add()
        .add();

    ThreeWindingsTransformer::Leg& leg = network.getThreeWindingsTransformer("3WT").getLeg3();
    leg.newOperationalLimitsGroup("1")
                .newCurrentLimits()
                .setPermanentLimit(900.0)
                .beginTemporaryLimit()
                .setName("20'")
                .setAcceptableDuration(20 * 60)
                .setValue(1100.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("5'")
                .setAcceptableDuration(5 * 60)
                .setValue(1300.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(1500.0)
                .endTemporaryLimit()
                .add();
    leg.newOperationalLimitsGroup("2")
                .newActivePowerLimits()
                .setPermanentLimit(750.0)
                .beginTemporaryLimit()
                .setName("25'")
                .setAcceptableDuration(25 * 60)
                .setValue(1250.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("7'")
                .setAcceptableDuration(7 * 60)
                .setValue(1350.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("2'")
                .setAcceptableDuration(2 * 60)
                .setValue(1550.0)
                .endTemporaryLimit()
                .add();
    leg.getOperationalLimitsGroup("2").get()
                .newCurrentLimits()
                .setPermanentLimit(1000.0)
                .beginTemporaryLimit()
                .setName("20'")
                .setAcceptableDuration(20 * 60)
                .setValue(1200.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("5'")
                .setAcceptableDuration(5 * 60)
                .setValue(1400.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(1600.0)
                .endTemporaryLimit()
                .add();
    leg.getOperationalLimitsGroup("2").get()
                .newApparentPowerLimits()
                .setPermanentLimit(890.0)
                .add();
    leg.newOperationalLimitsGroup("3").newApparentPowerLimits().setPermanentLimit(850.0).add();
    leg.getOperationalLimitsGroup("3").get().newCurrentLimits().setPermanentLimit(1000.0).add();

    return network;
}

BOOST_AUTO_TEST_SUITE(OperationalLimitsGroupsTestSuite)

BOOST_AUTO_TEST_CASE(OperationalLimitsGroupsOnLineTest) {
    Network network = createOperationalLimitsOnLineNetwork();
    Line& l = network.getLine("L");
    // Set some values for limits on side 1
    l.getTerminal1().getBusBreakerView().getBus().get().setV(390.0);
    l.getTerminal1().setP(800.0).setQ(400.0); // i = 1324.0969
    BOOST_CHECK(!std::isnan(l.getTerminal1().getI()));

    // No limits group by default, so no limits, so everything is false
    BOOST_CHECK(!l.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), l.getOverloadDuration());
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::APPARENT_POWER));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::APPARENT_POWER));

    // Set a default limits group, so we have an active limits
    l.setSelectedOperationalLimitsGroup1("2");
    BOOST_CHECK(l.isOverloaded());
    BOOST_CHECK_EQUAL(5 * 60L, l.getOverloadDuration());
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(l.checkTemporaryLimits1(LimitType::CURRENT));
    BOOST_CHECK_EQUAL(5 * 60L, l.checkTemporaryLimits1(LimitType::CURRENT)->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_CLOSE(1200.0, l.checkTemporaryLimits1(LimitType::CURRENT)->getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(l.checkTemporaryLimits1(LimitType::ACTIVE_POWER));
    BOOST_CHECK_EQUAL(25 * 60L, l.checkTemporaryLimits1(LimitType::ACTIVE_POWER)->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_CLOSE(750.0, l.checkTemporaryLimits1(LimitType::ACTIVE_POWER)->getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::APPARENT_POWER));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::APPARENT_POWER));

    // Change default limits group, so active limits is changed, this one have no temporary limits
    l.setSelectedOperationalLimitsGroup1("3");
    BOOST_CHECK(l.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), l.getOverloadDuration());
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::APPARENT_POWER));
    BOOST_CHECK(!l.checkTemporaryLimits1(LimitType::APPARENT_POWER));

    // Remove limits inside non selected limits group, so no impact expected
    l.getOperationalLimitsGroup1("2").get().removeCurrentLimits();
    BOOST_CHECK_EQUAL("3", *l.getSelectedOperationalLimitsGroupId1());
    BOOST_CHECK(l.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), l.getOverloadDuration());
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::APPARENT_POWER));

    // Remove limits inside selected limits group, impact expected, no more current limits
    l.getOperationalLimitsGroup1("3").get().removeCurrentLimits();
    BOOST_CHECK(!l.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), l.getOverloadDuration());
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(l.checkPermanentLimit1(LimitType::APPARENT_POWER));
    BOOST_CHECK(!l.getOperationalLimitsGroup1("3").get().isEmpty());

    // Remove default limits group, impact expected, no more limits
    l.removeOperationalLimitsGroup1("3");
    BOOST_CHECK(!l.getSelectedOperationalLimitsGroupId1());
    POWSYBL_ASSERT_REF_FALSE(l.getOperationalLimitsGroup1("3"));
    BOOST_CHECK(!l.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), l.getOverloadDuration());
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!l.checkPermanentLimit1(LimitType::APPARENT_POWER));

    // Set some values for limits on side 2
    l.getTerminal2().getBusBreakerView().getBus().get().setV(390.0);
    l.getTerminal2().setP(900.0).setQ(500.0); // i = 1524.1499
    BOOST_CHECK(!std::isnan(l.getTerminal2().getI()));

    // A group by default is defined and fill, but no active power defined
    BOOST_CHECK(l.checkPermanentLimit2(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit2(LimitType::ACTIVE_POWER));
    BOOST_CHECK(l.checkPermanentLimit2(LimitType::APPARENT_POWER));

    // cancel default limits, no group is the default limits, impact expected, no more limits
    l.cancelSelectedOperationalLimitsGroup2();
    BOOST_CHECK(!l.getSelectedOperationalLimitsGroupId2());
    POWSYBL_ASSERT_REF_FALSE(l.getSelectedOperationalLimitsGroup2());
    POWSYBL_ASSERT_REF_TRUE(l.getOperationalLimitsGroup2("1"));
    POWSYBL_ASSERT_REF_FALSE(l.getOperationalLimitsGroup2("1").get().getActivePowerLimits());
    POWSYBL_ASSERT_REF_TRUE(l.getOperationalLimitsGroup2("1").get().getApparentPowerLimits());
    POWSYBL_ASSERT_REF_TRUE(l.getOperationalLimitsGroup2("1").get().getCurrentLimits());
    BOOST_CHECK(!l.checkPermanentLimit2(LimitType::CURRENT));
    BOOST_CHECK(!l.checkPermanentLimit2(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!l.checkPermanentLimit2(LimitType::APPARENT_POWER));

}

BOOST_AUTO_TEST_CASE(OperationalLimitsGroupsOnTWTTest) {
    Network network = createOperationalLimitsOnTWTNetwork();
    ThreeWindingsTransformer& t = network.getThreeWindingsTransformer("3WT");
    ThreeWindingsTransformer::Leg& l = t.getLeg3();

    // Set some values for limits
    l.getTerminal().getBusBreakerView().getBus().get().setV(390.0);
    l.getTerminal().setP(800.0).setQ(400.0); // i = 1324.0969
    BOOST_CHECK(!std::isnan(l.getTerminal().getI()));

    // No limits group by default, so no limits, so everything is false
    BOOST_CHECK(!t.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), t.getOverloadDuration());
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::APPARENT_POWER));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::APPARENT_POWER));

    // Set a default limits group, so we have an active limits
    l.setSelectedOperationalLimitsGroup("2");
    BOOST_CHECK(t.isOverloaded());
    BOOST_CHECK_EQUAL(5 * 60L, t.getOverloadDuration());
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::CURRENT));
    BOOST_CHECK(t.checkTemporaryLimits3(LimitType::CURRENT));
    BOOST_CHECK_EQUAL(5 * 60L, t.checkTemporaryLimits3(LimitType::CURRENT)->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_CLOSE(1200.0, t.checkTemporaryLimits3(LimitType::CURRENT)->getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(t.checkTemporaryLimits3(LimitType::ACTIVE_POWER));
    BOOST_CHECK_EQUAL(25 * 60L, t.checkTemporaryLimits3(LimitType::ACTIVE_POWER)->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_CLOSE(750.0, t.checkTemporaryLimits3(LimitType::ACTIVE_POWER)->getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::APPARENT_POWER));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::APPARENT_POWER));

    // Change default limits group, so active limits is changed, this one have no temporary limits
    l.setSelectedOperationalLimitsGroup("3");
    BOOST_CHECK(t.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), t.getOverloadDuration());
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::APPARENT_POWER));
    BOOST_CHECK(!t.checkTemporaryLimits3(LimitType::APPARENT_POWER));

    // Remove limits inside non default limits group, so no impact expected
    l.getOperationalLimitsGroup("2").get().removeCurrentLimits();
    BOOST_CHECK_EQUAL("3", *l.getSelectedOperationalLimitsGroupId());
    BOOST_CHECK(t.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), t.getOverloadDuration());
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::APPARENT_POWER));

    // Remove limits inside default limits group, impact expected, no more current limits
    l.getOperationalLimitsGroup("3").get().removeCurrentLimits();
    BOOST_CHECK(!t.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), t.getOverloadDuration());
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::CURRENT));
    BOOST_CHECK(!t.checkPermanentLimit3(LimitType::ACTIVE_POWER));
    BOOST_CHECK(t.checkPermanentLimit3(LimitType::APPARENT_POWER));
    BOOST_CHECK(!l.getOperationalLimitsGroup("3").get().isEmpty());

    // Remove default limits group, impact expected, no more limits
    l.removeOperationalLimitsGroup("3");
    BOOST_CHECK(!l.getSelectedOperationalLimitsGroupId());
    POWSYBL_ASSERT_REF_FALSE(l.getOperationalLimitsGroup("3"));
    BOOST_CHECK(!t.isOverloaded());
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), t.getOverloadDuration());
    BOOST_CHECK(!t.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_CHECK(!t.checkPermanentLimit1(LimitType::ACTIVE_POWER));
    BOOST_CHECK(!t.checkPermanentLimit1(LimitType::APPARENT_POWER));
}

BOOST_AUTO_TEST_CASE(testGetOrCreateDefault) {
    Network network = createOperationalLimitsOnLineNetwork();
    Line& l = network.getLine("L");
 
    POWSYBL_ASSERT_REF_FALSE(l.getSelectedOperationalLimitsGroup1());
 
    l.newApparentPowerLimits1().setPermanentLimit(850.0).add(); //create default and set selected
 
    POWSYBL_ASSERT_REF_TRUE(l.getSelectedOperationalLimitsGroup1());
    BOOST_CHECK_EQUAL(l.getSelectedOperationalLimitsGroup1().get().getId(), "DEFAULT");
 
    l.cancelSelectedOperationalLimitsGroup1(); //unselect, but default group still exist
    POWSYBL_ASSERT_REF_FALSE(l.getSelectedOperationalLimitsGroup1());
    POWSYBL_ASSERT_REF_TRUE(l.getOperationalLimitsGroup1("DEFAULT"));

    l.newApparentPowerLimits1().setPermanentLimit(9000.0).add(); //retrieve default group and reset it as selected
    POWSYBL_ASSERT_REF_TRUE(l.getSelectedOperationalLimitsGroup1());
    BOOST_CHECK_EQUAL(l.getSelectedOperationalLimitsGroup1().get().getId(), "DEFAULT");
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
