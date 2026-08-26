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
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include <powsybl/iidm/util/LimitViolationUtils.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/stdcxx/exception.hpp>
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

    l.getOperationalLimitsGroup1("3").get().setProperty("propName1", "propValue1");
    l.getOperationalLimitsGroup2("1").get().setProperty("propName2", "propValue2");

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


BOOST_AUTO_TEST_CASE(propertiesHolderTest) {
    Network network = createOperationalLimitsOnLineNetwork();
    Line& l = network.getLine("L");

    OperationalLimitsGroup& group = l.getOperationalLimitsGroup1("1").get();
    BOOST_CHECK(!group.hasProperty());
    std::string property1 = "property_1";
    std::string property2 = "property_2";
    std::set<std::string> propertyNamesSet = {property1};
    group.setProperty(property1, "A");
    BOOST_CHECK(group.hasProperty());
    BOOST_CHECK(group.hasProperty(property1));
    BOOST_CHECK(!group.hasProperty(property2));
    BOOST_CHECK_EQUAL(1, boost::size(group.getPropertyNames()));
    BOOST_CHECK_EQUAL_COLLECTIONS(group.getPropertyNames().begin(), group.getPropertyNames().end(), propertyNamesSet.begin(), propertyNamesSet.end());
    BOOST_CHECK_EQUAL("A", group.getProperty(property1));
    POWSYBL_ASSERT_THROW(group.getProperty(property2), stdcxx::PropertyNotFoundException, "Property property_2 does not exist");
    BOOST_CHECK(group.getProperty(property2, "").empty());

    BOOST_CHECK(!group.removeProperty(property2));
    BOOST_CHECK_EQUAL(1, boost::size(group.getPropertyNames()));
    BOOST_CHECK_EQUAL_COLLECTIONS(group.getPropertyNames().begin(), group.getPropertyNames().end(), propertyNamesSet.begin(), propertyNamesSet.end());

    BOOST_CHECK(group.removeProperty(property1));
    BOOST_CHECK(!group.hasProperty());
    BOOST_CHECK(!group.hasProperty(property1));
    BOOST_CHECK(group.getPropertyNames().empty());
    POWSYBL_ASSERT_THROW(group.getProperty(property1), stdcxx::PropertyNotFoundException, "Property property_1 does not exist");
}

BOOST_AUTO_TEST_CASE(copyOperationalLimitsTest) {
    Network network = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();
    Line& line = network.getLine("NHV1_NHV2_1");
    Line& line2 = network.getLine("NHV1_NHV2_2");

    line.getOperationalLimitsGroup1("DEFAULT").get().setProperty("propName1", "propValue1");
    line.getOperationalLimitsGroup2("activated_2_1").get().setProperty("propName2", "propValue2");
    line.getOperationalLimitsGroup1("activated_1_2").get().setProperty("propName3", "propValue3");

    line2.copyOperationalLimits(line);

    //check properties are copied on l2: (on side 2, line 2 had also a "activated_2_2" and a "not_activated" group which is not destroyed by the copy)
    BOOST_CHECK_EQUAL(boost::size(line.getOperationalLimitsGroups1()), boost::size(line2.getOperationalLimitsGroups1()));
    BOOST_CHECK_EQUAL(boost::size(line.getOperationalLimitsGroups2()) + 2, boost::size(line2.getOperationalLimitsGroups2()));

    //properties are copied on groups:
    BOOST_CHECK_EQUAL(1, boost::size(line2.getOperationalLimitsGroup1("DEFAULT").get().getPropertyNames()));
    BOOST_CHECK_EQUAL("propValue1", line2.getOperationalLimitsGroup1("DEFAULT").get().getProperty("propName1"));
    BOOST_CHECK_EQUAL(1, boost::size(line2.getOperationalLimitsGroup2("activated_2_1").get().getPropertyNames()));
    BOOST_CHECK_EQUAL("propValue2", line2.getOperationalLimitsGroup2("activated_2_1").get().getProperty("propName2"));
    BOOST_CHECK_EQUAL(1, boost::size(line2.getOperationalLimitsGroup1("activated_1_2").get().getPropertyNames()));
    BOOST_CHECK_EQUAL("propValue3", line2.getOperationalLimitsGroup1("activated_1_2").get().getProperty("propName3"));

    //group selection is copied:
    std::list<std::string> selectedGroupIds_1_1 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    std::list<std::string> selectedGroupIds_1_2 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::TWO);
    std::list<std::string> selectedGroupIds_2_1 = line2.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    std::list<std::string> selectedGroupIds_2_2 = line2.getAllSelectedOperationalLimitsGroupIds(TwoSides::TWO);

    BOOST_CHECK_EQUAL_COLLECTIONS(selectedGroupIds_1_1.begin(), selectedGroupIds_1_1.end(), selectedGroupIds_2_1.begin(), selectedGroupIds_2_1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(selectedGroupIds_1_2.begin(), selectedGroupIds_1_2.end(), selectedGroupIds_2_2.begin(), selectedGroupIds_2_2.end());

}


BOOST_AUTO_TEST_CASE(operationalLimitsGroupSelectionOrdering) {
    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();

    std::list<std::string> expectedOrderedIdsBefore = {"DEFAULT",
            "activated_1_1",
            "activated_1_2"};

    Line& line = n.getLine("NHV1_NHV2_1");

    std::list<std::string> orderedGroupIdsBefore = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsBefore.begin(), expectedOrderedIdsBefore.end(), orderedGroupIdsBefore.begin(), orderedGroupIdsBefore.end());

    line.cancelSelectedOperationalLimitsGroup1();

    line.addSelectedOperationalLimitsGroups(TwoSides::ONE, {
            "activated_1_1",
            "activated_1_2",
            "activated_1_2",
            "activated_1_1",
            "not_activated",
            "DEFAULT" });

    line.deselectOperationalLimitsGroups(TwoSides::ONE, {"not_activated"});

    std::list<std::string> orderedGroupIds = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    std::list<std::string> expectedOrderedIds = {
            "activated_1_2",
            "activated_1_1",
            "DEFAULT"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIds.begin(), expectedOrderedIds.end(), orderedGroupIds.begin(), orderedGroupIds.end());
}

BOOST_AUTO_TEST_CASE(operationalLimitsGroupSelectionNotOrdered) {
    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();

    std::set<std::string> expectedIdsBefore = {"DEFAULT",
            "activated_1_1",
            "activated_1_2"};

    Line& line = n.getLine("NHV1_NHV2_1");

    std::set<std::string> selectedGroupIdsBefore;
    for (auto& olg : line.getAllSelectedOperationalLimitsGroups(TwoSides::ONE)) {
        selectedGroupIdsBefore.emplace(olg.getId());
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(expectedIdsBefore.begin(), expectedIdsBefore.end(), selectedGroupIdsBefore.begin(), selectedGroupIdsBefore.end());

    line.cancelSelectedOperationalLimitsGroup1();

    line.addSelectedOperationalLimitsGroups(TwoSides::ONE, {
            "activated_1_1",
            "activated_1_2",
            "activated_1_2",
            "activated_1_1",
            "not_activated",
            "DEFAULT" });

    line.deselectOperationalLimitsGroups(TwoSides::ONE, {"not_activated"});

    std::set<std::string> groupIds;
    for (auto& olg : line.getAllSelectedOperationalLimitsGroups(TwoSides::ONE)) {
        groupIds.emplace(olg.getId());
    }
    std::set<std::string> expectedGroupIds = {
            "activated_1_2",
            "activated_1_1",
            "DEFAULT"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedGroupIds.begin(), expectedGroupIds.end(), groupIds.begin(), groupIds.end());
}

BOOST_AUTO_TEST_CASE(getAllSelectedCurrentLimits) {
    //range from getAllSelectedCurrentLimits and getAllSelectedLoadingLimits are ordered by group "storage" order, not by selection order

    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();
    Line& line = n.getLine("NHV1_NHV2_2");

    auto currentLimits = line.getAllSelectedCurrentLimits(TwoSides::TWO);
    BOOST_CHECK_EQUAL(boost::size(currentLimits), 3);
    BOOST_CHECK_EQUAL(boost::size(currentLimits), boost::size(line.getAllSelectedLoadingLimits(LimitType::CURRENT,TwoSides::TWO)));

    CurrentLimits& cl1 = currentLimits.front();
    BOOST_CHECK_CLOSE(cl1.getPermanentLimit(), 500.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK(cl1.getTemporaryLimits().empty());
    currentLimits.drop_front();
    BOOST_CHECK_EQUAL(boost::size(currentLimits), 2);

    CurrentLimits& cl2 = currentLimits.front();
    BOOST_CHECK_CLOSE(cl2.getPermanentLimit(), 200.0, std::numeric_limits<double>::epsilon());
    auto cl2TemporaryLimits = cl2.getTemporaryLimits();
    BOOST_CHECK_EQUAL(boost::size(cl2TemporaryLimits), 2);
    auto& cl2tl1 = cl2TemporaryLimits.front();
    BOOST_CHECK_CLOSE(cl2tl1.getValue(), 600.0, std::numeric_limits<double>::epsilon());
    cl2TemporaryLimits.drop_front();
    auto& cl2tl2 = cl2TemporaryLimits.front();
    BOOST_CHECK_EQUAL(cl2tl2.getValue(), std::numeric_limits<double>::max());
    currentLimits.drop_front();
    BOOST_CHECK_EQUAL(boost::size(currentLimits), 1);

    CurrentLimits& cl3 = currentLimits.front();
    BOOST_CHECK_CLOSE(cl3.getPermanentLimit(), 300.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(boost::size(cl3.getTemporaryLimits()), 1);
    BOOST_CHECK_EQUAL(cl3.getTemporaryLimits().front().getValue(), std::numeric_limits<double>::max());
    currentLimits.drop_front();
    BOOST_CHECK(currentLimits.empty());

    BOOST_CHECK(stdcxx::areSame(line.getCurrentLimits(TwoSides::TWO).get(), cl3));

    line.cancelSelectedOperationalLimitsGroup2();
    line.addSelectedOperationalLimitsGroups(TwoSides::TWO, {"activated_2_2", "DEFAULT"});

    auto currentLoadingLimits = line.getAllSelectedLoadingLimits(LimitType::CURRENT, TwoSides::TWO);
    BOOST_CHECK_EQUAL(boost::size(currentLoadingLimits), 2);

    LoadingLimits& l1 = currentLoadingLimits.front();
    BOOST_CHECK(stdcxx::areSame(l1, cl1));
    BOOST_CHECK_CLOSE(l1.getPermanentLimit(), 500.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK(l1.getTemporaryLimits().empty());
    currentLoadingLimits.drop_front();
    BOOST_CHECK_EQUAL(boost::size(currentLoadingLimits), 1);

    LoadingLimits& l2 = currentLoadingLimits.front();
    BOOST_CHECK(stdcxx::areSame(l2, cl3));
    BOOST_CHECK_CLOSE(l2.getPermanentLimit(), 300.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(boost::size(l2.getTemporaryLimits()), 1);
    BOOST_CHECK_EQUAL(l2.getTemporaryLimits().front().getValue(), std::numeric_limits<double>::max());

    BOOST_CHECK(stdcxx::areSame(line.getCurrentLimits(TwoSides::TWO).get(), l1));
}

BOOST_AUTO_TEST_CASE(getAllSelectedActivePowerLimits) {
    //range from getAllSelectedActivePowerLimits and getAllSelectedLoadingLimits are ordered by group "storage" order, not by selection order

    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedActivePowerLimits();
    ThreeWindingsTransformer::Leg& legThree = n.getThreeWindingsTransformer("NGEN_V2_NHV1").getLeg3();

    auto activePowerLimits = legThree.getAllSelectedActivePowerLimits();
    BOOST_CHECK_EQUAL(boost::size(activePowerLimits), 2);
    BOOST_CHECK_EQUAL(boost::size(activePowerLimits), boost::size(legThree.getAllSelectedLoadingLimits(LimitType::ACTIVE_POWER)));

    ActivePowerLimits& apl1 = activePowerLimits.front();
    BOOST_CHECK_CLOSE(apl1.getPermanentLimit(), 250.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK(apl1.getTemporaryLimits().empty());
    activePowerLimits.drop_front();
    BOOST_CHECK_EQUAL(boost::size(activePowerLimits), 1);

    ActivePowerLimits& apl2 = activePowerLimits.front();
    BOOST_CHECK_CLOSE(apl2.getPermanentLimit(), 350.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(boost::size(apl2.getTemporaryLimits()), 1);
    BOOST_CHECK_CLOSE(apl2.getTemporaryLimits().front().getValue(), 400.0 ,std::numeric_limits<double>::epsilon());
    activePowerLimits.drop_front();
    BOOST_CHECK(activePowerLimits.empty());

    BOOST_CHECK(stdcxx::areSame(legThree.getActivePowerLimits().get(), apl2));

    //check that deselecting a non-selected group does nothing
    legThree.deselectOperationalLimitsGroups({"not_activated"});

    auto activePowerLoadingLimits = legThree.getAllSelectedLoadingLimits(LimitType::ACTIVE_POWER);
    BOOST_CHECK_EQUAL(boost::size(activePowerLoadingLimits), 2);

    legThree.deselectOperationalLimitsGroups({"activated_3_1", "DEFAULT"});
    activePowerLoadingLimits = legThree.getAllSelectedLoadingLimits(LimitType::ACTIVE_POWER);
    BOOST_CHECK(activePowerLoadingLimits.empty());

}

BOOST_AUTO_TEST_CASE(getAllSelectedApparentPowerLimits) {
    //range from getAllSelectedApparentPowerLimits and getAllSelectedLoadingLimits are ordered by group "storage" order, not by selection order

    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedApparentPowerLimits();
    TwoWindingsTransformer& twt = n.getTwoWindingsTransformer("NGEN_NHV1");

    auto apparentPowerLimits = twt.getAllSelectedApparentPowerLimits(TwoSides::TWO);
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLimits), 2);
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLimits), boost::size(twt.getAllSelectedLoadingLimits(LimitType::APPARENT_POWER, TwoSides::TWO)));

    ApparentPowerLimits& apl1 = apparentPowerLimits.front();
    BOOST_CHECK_CLOSE(apl1.getPermanentLimit(), 230.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(boost::size(apl1.getTemporaryLimits()), 1);
    BOOST_CHECK_CLOSE(apl1.getTemporaryLimits().front().getValue(), 240.0 ,std::numeric_limits<double>::epsilon());
    apparentPowerLimits.drop_front();
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLimits), 1);

    ApparentPowerLimits& apl2 = apparentPowerLimits.front();
    BOOST_CHECK_CLOSE(apl2.getPermanentLimit(), 240.0, std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(boost::size(apl2.getTemporaryLimits()), 1);
    BOOST_CHECK_CLOSE(apl2.getTemporaryLimits().front().getValue(), 250.0 ,std::numeric_limits<double>::epsilon());
    apparentPowerLimits.drop_front();
    BOOST_CHECK(apparentPowerLimits.empty());

    BOOST_CHECK(stdcxx::areSame(twt.getApparentPowerLimits(TwoSides::TWO).get(), apl2));

    //does not affect group 2
    twt.cancelSelectedOperationalLimitsGroup1();
    auto apparentPowerLoadingLimits = twt.getAllSelectedLoadingLimits(LimitType::APPARENT_POWER, TwoSides::TWO);
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLoadingLimits), 2);

    //does affect group 2
    twt.cancelSelectedOperationalLimitsGroup2();
    apparentPowerLoadingLimits = twt.getAllSelectedLoadingLimits(LimitType::APPARENT_POWER, TwoSides::TWO);
    BOOST_CHECK(apparentPowerLoadingLimits.empty());

}

void checkOverloadDuration(Network& n, const std::string& lineId, double p, double q, unsigned long expectedOverloadDuration) {
    Line& line = n.getLine(lineId);
    line.getTerminal1().setP(p).setQ(q);
    line.getTerminal2().setP(-p).setQ(-q);

    n.getBusBreakerView().getBus("NHV1").get().setV(400);
    n.getBusBreakerView().getBus("NHV2").get().setV(400);

    BOOST_CHECK_EQUAL(line.getOverloadDuration(), expectedOverloadDuration);
}

BOOST_AUTO_TEST_CASE(overloadDurationWithMultipleSelectedOperationalLimts) {

    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();

    checkOverloadDuration(n, "NHV1_NHV2_1", 200., 10., std::numeric_limits<unsigned long>::max());
    checkOverloadDuration(n, "NHV1_NHV2_1", 200., 60., 40 * 60);
    checkOverloadDuration(n, "NHV1_NHV2_1", 350., 60., 40 * 60);
    checkOverloadDuration(n, "NHV1_NHV2_1", 450., 60., 10 * 60);
    checkOverloadDuration(n, "NHV1_NHV2_1", 550., 100., 30);
    checkOverloadDuration(n, "NHV1_NHV2_1", 700., 100., 0);
    checkOverloadDuration(n, "NHV1_NHV2_1", 800., 200., 0);
    checkOverloadDuration(n, "NHV1_NHV2_1", 800., 250., 0);
    checkOverloadDuration(n, "NHV1_NHV2_1", 1000., 250., 0);
    checkOverloadDuration(n, "NHV1_NHV2_1", 1100., 250., 0);

}


BOOST_AUTO_TEST_CASE(checkAddWithPredicate) {
    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();
    Line& line = n.getLine("NHV1_NHV2_1");


    line.cancelSelectedOperationalLimitsGroup1();
    BOOST_CHECK(line.getAllSelectedOperationalLimitsGroups(TwoSides::ONE).empty());
    std::set<std::string> expectedIdsGroup1 = {"DEFAULT",
            "activated_1_1",
            "activated_1_2",
            "not_activated"};
    std::set<std::string> idsGroup1;
    for (const auto& group: line.getOperationalLimitsGroups1()) {
        idsGroup1.emplace(group.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedIdsGroup1.begin(), expectedIdsGroup1.end(), idsGroup1.begin(), idsGroup1.end());

    line.addSelectedOperationalLimitsGroupsByPredicate1([](const std::string& id){
        return id.find("activated_1") != std::string::npos;
    });
    std::list<std::string> orderedGroupIds1 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    std::list<std::string> expectedOrderedIdsGroup1 = {"activated_1_1", "activated_1_2"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup1.begin(), expectedOrderedIdsGroup1.end(), orderedGroupIds1.begin(), orderedGroupIds1.end());

    line.addSelectedOperationalLimitsGroupsByPredicate1([](const std::string& id){
        return id.find("activated") != std::string::npos;
    });
    orderedGroupIds1 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    expectedOrderedIdsGroup1 = {"activated_1_1", "activated_1_2", "not_activated"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup1.begin(), expectedOrderedIdsGroup1.end(), orderedGroupIds1.begin(), orderedGroupIds1.end());


    // adding with a predicate should not remove activated groups that do not match the predicate
    line.cancelSelectedOperationalLimitsGroup1();
    line.addSelectedOperationalLimitsGroups(TwoSides::ONE, {"activated_1_2"});
    line.addSelectedOperationalLimitsGroupsByPredicate(TwoSides::ONE, [](const std::string& id){
        return id == "DEFAULT";
    });
    orderedGroupIds1 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    expectedOrderedIdsGroup1 = {"activated_1_2", "DEFAULT"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup1.begin(), expectedOrderedIdsGroup1.end(), orderedGroupIds1.begin(), orderedGroupIds1.end());

    //also all groups can be activated -> byPredicate, will activate them in their "storage" order
    line.addSelectedOperationalLimitsGroupsByPredicate(TwoSides::ONE, [](const std::string&){
        return true;
    });
    orderedGroupIds1 = line.getAllSelectedOperationalLimitsGroupIds(TwoSides::ONE);
    expectedOrderedIdsGroup1 = {"DEFAULT", "activated_1_1", "activated_1_2", "not_activated"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup1.begin(), expectedOrderedIdsGroup1.end(), orderedGroupIds1.begin(), orderedGroupIds1.end());

}

BOOST_AUTO_TEST_CASE(doNotSelectGroupsIfAnyIsNullOrDoesNotExist) {
    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedActivePowerLimits();
    ThreeWindingsTransformer::Leg& leg = n.getThreeWindingsTransformer("NGEN_V2_NHV1").getLeg(ThreeSides::THREE);

    leg.deselectOperationalLimitsGroups({"activated_3_1"});
    std::list<std::string> orderedGroupIds = leg.getAllSelectedOperationalLimitsGroupIds();
    std::list<std::string> expectedOrderedIdsGroup = {"DEFAULT"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup.begin(), expectedOrderedIdsGroup.end(), orderedGroupIds.begin(), orderedGroupIds.end());

    //check no group is selected if any is null
    POWSYBL_ASSERT_THROW(leg.addSelectedOperationalLimitsGroups({"activated_3_1", "", "not_activated"}), PowsyblException, "One or more of the provided IDs for the group selection were null, none of the provided groups were selected");
    orderedGroupIds = leg.getAllSelectedOperationalLimitsGroupIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup.begin(), expectedOrderedIdsGroup.end(), orderedGroupIds.begin(), orderedGroupIds.end());

    //check no group selected if any does not exist
    POWSYBL_ASSERT_THROW(leg.addSelectedOperationalLimitsGroups({"activated_3_1", "not a group", "also wrong", "not_activated"}), PowsyblException, "The following IDs did not correspond to an existing group, they cannot be selected : [not a group, also wrong]");
    orderedGroupIds = leg.getAllSelectedOperationalLimitsGroupIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup.begin(), expectedOrderedIdsGroup.end(), orderedGroupIds.begin(), orderedGroupIds.end());

}

BOOST_AUTO_TEST_CASE(checkGetOrCreateSetsSelected) {
    Network n = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedApparentPowerLimits();
    TwoWindingsTransformer& transformer = n.getTwoWindingsTransformer("NGEN_NHV1");

    // no default group
    std::set<std::string> expectedIdsGroup = {"activated_2_1", "activated_2_2"};
    std::set<std::string> idsGroup;
    for (const auto& group: transformer.getOperationalLimitsGroups2()) {
        idsGroup.emplace(group.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedIdsGroup.begin(), expectedIdsGroup.end(), idsGroup.begin(), idsGroup.end());
    BOOST_CHECK_EQUAL(2, boost::size(transformer.getAllSelectedOperationalLimitsGroupIds(TwoSides::TWO)));
    stdcxx::Reference<OperationalLimitsGroup> alreadySelectedGroup = transformer.getSelectedOperationalLimitsGroup2();
    POWSYBL_ASSERT_REF_TRUE(alreadySelectedGroup);
    BOOST_CHECK_EQUAL("activated_2_2", alreadySelectedGroup.get().getId());

    //retrieve the already selected limit group
    auto& rslt1 = transformer.getOrCreateSelectedOperationalLimitsGroup2();
    BOOST_CHECK(stdcxx::areSame(rslt1, alreadySelectedGroup.get()));
    BOOST_CHECK_EQUAL(2, boost::size(transformer.getAllSelectedOperationalLimitsGroupIds(TwoSides::TWO)));

    transformer.cancelSelectedOperationalLimitsGroup2();
    POWSYBL_ASSERT_REF_FALSE(transformer.getSelectedOperationalLimitsGroup2());
    //No selected group, and DEFAULT group does not exist -> create it and selects it
    auto& rsltDefault = transformer.getOrCreateSelectedOperationalLimitsGroup2();
    BOOST_CHECK_EQUAL("DEFAULT", rsltDefault.getId());

    std::list<std::string> orderedGroupIds = transformer.getAllSelectedOperationalLimitsGroupIds(TwoSides::TWO);
    std::list<std::string> expectedOrderedIdsGroup = {"DEFAULT"};
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedOrderedIdsGroup.begin(), expectedOrderedIdsGroup.end(), orderedGroupIds.begin(), orderedGroupIds.end());
}

struct ExpectedOverload {
    std::string previousLimitName;
    std::string operationalLimitsGroupId;
    double limit;
    unsigned long acceptableDuration;

    bool operator==(const ExpectedOverload& comp) const {
        return comp.previousLimitName == previousLimitName && comp.operationalLimitsGroupId==operationalLimitsGroupId && comp.limit == limit && comp.acceptableDuration == acceptableDuration;
    }
    // bool operator!=(const ExpectedOverload& comp) const {
    //     return comp.previousLimitName != previousLimitName || comp.operationalLimitsGroupId!=operationalLimitsGroupId || comp.limit != limit || comp.acceptableDuration != acceptableDuration;
    // }
};

void checkLimitViolationUtilTemporaryLimits(Identifiable& identifiable, const ThreeSides& side, std::vector<double> limitReductions, const LimitType& type, double value, const std::vector<ExpectedOverload>& expectedOverloads) {

    for (auto limitReduction : limitReductions) {
        std::vector<std::unique_ptr<Overload>> overloads;
        if (stdcxx::isInstanceOf<Branch>(identifiable)) {
            if(limitReduction == 1.0) {
                overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<Branch&>(identifiable), static_cast<TwoSides>(side), limitReduction, value, type);
            } else {
                //multiply by limitReduction because all the limits will be reduced, so we also want to reduce the actual value to match that.
                //the arguments to this method test both with and without limitReduction. All the cases with a limitReduction are the exact same case as without, but with a coefficient
                //To get the same situation, we need to reduce both the limits and the value. The limits are supposed to be reduced by checkAllTemporaryLimits, and we reduce the value in the test
                overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<Branch&>(identifiable), static_cast<TwoSides>(side), limitReduction, value * limitReduction, type);
            }
        } else if(stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)) {
            
            if(limitReduction == 1.0) {
                overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<ThreeWindingsTransformer&>(identifiable), side, limitReduction, value, type);
            } else {
                //multiply by limitReduction because all the limits will be reduced, so we also want to reduce the actual value to match that.
                //the arguments to this method test both with and without limitReduction. All the cases with a limitReduction are the exact same case as without, but with a coefficient
                //To get the same situation, we need to reduce both the limits and the value. The limits are supposed to be reduced by checkAllTemporaryLimits, and we reduce the value in the test
                overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<ThreeWindingsTransformer&>(identifiable), side, limitReduction, value * limitReduction, type);
            }
        } else {
            throw PowsyblException(stdcxx::format("The class %1% cannot be used to check temporary limits", stdcxx::demangle(identifiable)));
        }

        std::vector<ExpectedOverload> results;
        for (const auto& overloadPtr : overloads) {
            results.emplace_back(ExpectedOverload{
                overloadPtr->getPreviousLimitName(),
                overloadPtr->getOperationalLimitsGroupId(),
                overloadPtr->getPreviousLimit(),
                overloadPtr->getTemporaryLimit().getAcceptableDuration(),
            });
        }

        BOOST_CHECK_EQUAL(results.size(), expectedOverloads.size());
        for(const auto& rslt : results) {
            BOOST_CHECK(std::find(expectedOverloads.begin(), expectedOverloads.end(),rslt)!=expectedOverloads.end());
        }
    }

}

BOOST_AUTO_TEST_CASE(testLimitViolationUtilsCheckTemporaryLimits) {

    Network networkLine = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();
    Line& line = networkLine.getLine("NHV1_NHV2_1");

    Network network3wt = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedActivePowerLimits();
    ThreeWindingsTransformer& transformer = network3wt.getThreeWindingsTransformer("NGEN_V2_NHV1");

    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.95}, LimitType::CURRENT, 299, {}); //below any Permanent Limit
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.8}, LimitType::CURRENT, 310,
        {ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_1_2", 300,60 * 40}}); // above permanent of activated_1_2
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.82}, LimitType::CURRENT, 510, 
        {ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_1_2", 300, 60 * 40}}); // above permanent of Default, but no temporary above. `checkAllTemporaryLimits` doesn't detect anything in this case. It is the responsibility of `checkPermanentLimit`
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.7}, LimitType::CURRENT, 701, 
        {ExpectedOverload{"40'", "activated_1_2", 700, 30}}); // above first temporary of 1_2
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.92}, LimitType::CURRENT, 1122, 
        {ExpectedOverload{"40'", "activated_1_2", 700, 30}, 
         ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_1_1", 1100, 60 * 10}}); // above permanent of 1_1
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.87}, LimitType::CURRENT, 1450, 
        {ExpectedOverload{"40'", "activated_1_2", 700, 30}, 
         ExpectedOverload{"10'", "activated_1_1", 1200, 60}}); // above first temporary of 1_1
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.3}, LimitType::CURRENT, 1500, 
        {ExpectedOverload{"40'", "activated_1_2", 700, 30}, 
         ExpectedOverload{"1'", "activated_1_1", 1500, 0}}); // above last temporary of 1_1
    checkLimitViolationUtilTemporaryLimits(line, ThreeSides::ONE, {1., 0.84}, LimitType::CURRENT, 1601, 
        {ExpectedOverload{"0.5'", "activated_1_2", 1600, 0}, 
         ExpectedOverload{"1'", "activated_1_1", 1500, 0}}); // above last temporary of 1_2


    checkLimitViolationUtilTemporaryLimits(transformer, ThreeSides::THREE, {1., 0.99}, LimitType::ACTIVE_POWER, 200, {}); // under all limits
    checkLimitViolationUtilTemporaryLimits(transformer, ThreeSides::THREE, {1., 0.96}, LimitType::ACTIVE_POWER, 275, {}); // above permanent of Default, but no temporary above. `checkAllTemporaryLimits` doesn't detect anything in this case. It is the responsibility of `checkPermanentLimit`
    checkLimitViolationUtilTemporaryLimits(transformer, ThreeSides::THREE, {1., 0.88}, LimitType::ACTIVE_POWER, 375, 
        {ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_3_1", 350, 45 * 60}}); // above permanent of activated_3_1
    checkLimitViolationUtilTemporaryLimits(transformer, ThreeSides::THREE, {1., 0.77}, LimitType::ACTIVE_POWER, 405, 
        {ExpectedOverload{"45'", "activated_3_1", 400, 0}}); // above last temporary of activated_3_1
}

void checkLimitViolationUtilTemporaryLimitsByGroup(Identifiable& identifiable, const ThreeSides& side, double limitReduction, const std::list<std::string>& groupsToApplyLimitReduction, const LimitType& type, double value, const std::vector<ExpectedOverload>& expectedOverloads) {

    std::vector<std::unique_ptr<Overload>> overloads;
    if (stdcxx::isInstanceOf<Branch>(identifiable)) {
        overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<Branch&>(identifiable), static_cast<TwoSides>(side), limitReduction, groupsToApplyLimitReduction, value, type);
    } else if(stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)) {
        overloads = LimitViolationUtils::checkAllTemporaryLimits(dynamic_cast<ThreeWindingsTransformer&>(identifiable), side, limitReduction, groupsToApplyLimitReduction, value, type);
    } else {
        throw PowsyblException(stdcxx::format("The class %1% cannot be used to check temporary limits", stdcxx::demangle(identifiable)));
    }

    std::vector<ExpectedOverload> results;
    for (const auto& overloadPtr : overloads) {
        results.emplace_back(ExpectedOverload{
            overloadPtr->getPreviousLimitName(),
            overloadPtr->getOperationalLimitsGroupId(),
            overloadPtr->getPreviousLimit(),
            overloadPtr->getTemporaryLimit().getAcceptableDuration(),
        });
    }

    BOOST_CHECK_EQUAL(results.size(), expectedOverloads.size());
    for(const auto& rslt : results) {
        BOOST_CHECK(std::find(expectedOverloads.begin(), expectedOverloads.end(),rslt)!=expectedOverloads.end());
    }
}

BOOST_AUTO_TEST_CASE(testLimitViolationUtilsCheckTemporaryLimitsByGroup) {
    Network networkLine = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();
    Line& l = networkLine.getLine("NHV1_NHV2_1");
    //activate the usually not activated group to have 3 groups with temporary (default doesn't have any)
    l.addSelectedOperationalLimitsGroups(TwoSides::ONE, {"not_activated"});

    Network network3wt = powsybl::network::EurostagFactory::createWithMultipleSelectedFixedActivePowerLimits();
    ThreeWindingsTransformer& transformer = network3wt.getThreeWindingsTransformer("NGEN_V2_NHV1");
    //activate the not_activated to have more groups to test on
    transformer.getLeg(ThreeSides::THREE).addSelectedOperationalLimitsGroups({"not_activated"});

    checkLimitViolationUtilTemporaryLimitsByGroup(l, ThreeSides::ONE, 0.01, {}, LimitType::CURRENT, 250, //Apply on all selected groups
        {ExpectedOverload{"1'","activated_1_1",1500, 0},
         ExpectedOverload{"0.5'", "activated_1_2", 1600, 0},
         ExpectedOverload{"30'", "not_activated", 600, 0}});
    checkLimitViolationUtilTemporaryLimitsByGroup(l, ThreeSides::ONE, 0.01, {"activated_1_2"}, LimitType::CURRENT, 250, //Apply only on "activated_1_2"
        {ExpectedOverload{"0.5'", "activated_1_2", 1600, 0}});
    checkLimitViolationUtilTemporaryLimitsByGroup(l, ThreeSides::ONE, 0.01, {"activated_1_1", "activated_1_2"}, LimitType::CURRENT, 250, // apply to only 2 groups out of 4
         {ExpectedOverload{"1'", "activated_1_1", 1500, 0},
          ExpectedOverload{"0.5'", "activated_1_2", 1600, 0}});

    checkLimitViolationUtilTemporaryLimitsByGroup(transformer, ThreeSides::THREE, 0.8, {}, LimitType::ACTIVE_POWER, 290, // apply to all selected groups
        {ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_3_1", 350, 45 * 60}, 
         ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "not_activated", 300, 25 * 60}}), 
    checkLimitViolationUtilTemporaryLimitsByGroup(transformer, ThreeSides::THREE, 0.8, {"activated_3_1"}, LimitType::ACTIVE_POWER, 290, // only reduce "activated_3_1"
        {ExpectedOverload{LimitViolationUtils::PERMANENT_LIMIT_NAME, "activated_3_1", 350, 45 * 60}});
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
