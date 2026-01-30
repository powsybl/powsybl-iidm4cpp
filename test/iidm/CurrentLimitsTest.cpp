/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <limits>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Overload.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLegAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>
#include <powsybl/logging/ContainerLogger.hpp>
#include <powsybl/logging/LogMessage.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createCurrentLimitsTestNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl3 = substation2.newVoltageLevel()
        .setId("VL3")
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    VoltageLevel& vl4 = substation.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    network.newLine()
        .setId("VL1_VL3")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl3.getId())
        .setBus2(vl3Bus1.getId())
        .setConnectableBus2(vl3Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add()
        .newCurrentLimits1()
        .setPermanentLimit(4.0)
        .beginTemporaryLimit()
        .setName("TL1")
        .setValue(7.0)
        .setAcceptableDuration(1UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2")
        .setValue(6.0)
        .setAcceptableDuration(2UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3")
        .setValue(5.0)
        .setAcceptableDuration(3UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();

    return network;
}

Network createThreeWindingsTransformerCurrentLimitsTestNetwork() {
    Network network("test_3wt", "test");
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
    ThreeWindingsTransformer& transformer = s1.newThreeWindingsTransformer()
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

    transformer.getLeg1().newCurrentLimits()
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
    transformer.getLeg2().newCurrentLimits()
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
    transformer.getLeg3().newCurrentLimits()
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

    return network;
}

Network createOneLineCurrentLimitsTestNetwork() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation()
                .setId("S1")
                .setCountry(Country::FR)
                .add();
    VoltageLevel &vl1 = s1.newVoltageLevel()
                            .setId("VL1")
                            .setNominalV(400.0)
                            .setTopologyKind(TopologyKind::BUS_BREAKER)
                            .add();
    vl1.getBusBreakerView().newBus().setId("B1").add();
    Substation& s2 = network.newSubstation()
                        .setId("S2")
                        .setCountry(Country::FR)
                        .add();
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
    l.newCurrentLimits1()
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
    return network;
}

BOOST_AUTO_TEST_SUITE(CurrentLimitsTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createCurrentLimitsTestNetwork();
    const Network& cNetwork = network;

    Line& line = network.getLine("VL1_VL3");
    const Line& cLine = cNetwork.getLine("VL1_VL3");;
    BOOST_TEST(stdcxx::areSame(line, cLine));

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(cLine.getCurrentLimits1());
    BOOST_TEST(line.getCurrentLimits(TwoSides::ONE));
    BOOST_TEST(cLine.getCurrentLimits(TwoSides::ONE));

    BOOST_TEST(!line.getCurrentLimits2());
    BOOST_TEST(!cLine.getCurrentLimits2());
    BOOST_TEST(!line.getCurrentLimits(TwoSides::TWO));
    BOOST_TEST(!cLine.getCurrentLimits(TwoSides::TWO));

    POWSYBL_ASSERT_THROW(line.getCurrentLimits(static_cast<TwoSides>(0)), AssertionError, "Unexpected TwoSides value: UNDEFINED");
    POWSYBL_ASSERT_THROW(line.getCurrentLimits(static_cast<TwoSides>(5)), AssertionError, "Unexpected TwoSides value: 5");
    POWSYBL_ASSERT_THROW(cLine.getCurrentLimits(static_cast<TwoSides>(6)), AssertionError, "Unexpected TwoSides value: 6");

    CurrentLimits limits = line.getCurrentLimits1().get();
    const CurrentLimits& cLimits = limits;
    BOOST_TEST(stdcxx::areSame(limits, cLimits));
    BOOST_CHECK_CLOSE(4.0, limits.getPermanentLimit(), std::numeric_limits<double>::epsilon());

    stdcxx::range<CurrentLimits::TemporaryLimit> tempLimits = limits.getTemporaryLimits();
    BOOST_CHECK_EQUAL(3, boost::size(tempLimits));
    CurrentLimits::TemporaryLimit tl = limits.getTemporaryLimit(3UL);
    CurrentLimits::TemporaryLimit tl2 = cLimits.getTemporaryLimit(3UL);

    BOOST_CHECK_EQUAL(3UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(3UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL3", tl.getName());
    BOOST_CHECK_EQUAL("TL3", tl2.getName());
    BOOST_CHECK_CLOSE(5.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_TEST(!tl2.isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    tl = limits.getTemporaryLimit(2UL);
    tl2 = cLimits.getTemporaryLimit(2UL);

    BOOST_CHECK_EQUAL(2UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(2UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL2", tl.getName());
    BOOST_CHECK_EQUAL("TL2", tl2.getName());
    BOOST_CHECK_CLOSE(6.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(tl.isFictitious());
    BOOST_TEST(tl2.isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    tl = limits.getTemporaryLimit(1UL);
    tl2 = cLimits.getTemporaryLimit(1UL);

    BOOST_CHECK_EQUAL(1UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(1UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL1", tl.getName());
    BOOST_CHECK_EQUAL("TL1", tl2.getName());
    BOOST_CHECK_CLOSE(7.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(7.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_TEST(!tl2.isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createCurrentLimitsTestNetwork();

    Line& line = network.getLine("VL1_VL3");
    const Line& cLine = line;
    BOOST_TEST(stdcxx::areSame(line, cLine));
    CurrentLimits limits = line.getCurrentLimits1().get();

    BOOST_TEST(std::isnan(limits.getTemporaryLimitValue(10UL)));

    BOOST_TEST(stdcxx::areSame(limits, limits.setPermanentLimit(100.0)));
    BOOST_CHECK_CLOSE(100.0, limits.getPermanentLimit(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(limits.setPermanentLimit(-1.0), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(cLine.getCurrentLimits1());
    BOOST_TEST(line.getCurrentLimits(TwoSides::ONE));
    BOOST_TEST(cLine.getCurrentLimits(TwoSides::ONE));
}

BOOST_AUTO_TEST_CASE(adder) {
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm", stdcxx::make_unique<logging::ContainerLogger>());

    Network network = createCurrentLimitsTestNetwork();

    Line& line = network.getLine("VL1_VL3");
    BOOST_TEST(!line.getCurrentLimits2());
    auto adder = line.newCurrentLimits2();

    BOOST_CHECK(!adder.hasTemporaryLimits());

    adder.setPermanentLimit(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");
    adder.setPermanentLimit(100.0);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_TEST(line.getCurrentLimits2());

    BOOST_CHECK(!adder.hasTemporaryLimits());

    auto adder2 = adder;
    auto tempAdder = adder2.beginTemporaryLimit();
    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': temporary limit value is not set");
    tempAdder.setValue(-10.0);
    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': temporary limit value must be > 0");
    tempAdder.setValue(10.0);

    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': acceptable duration is not set");
    tempAdder.setAcceptableDuration(1UL);

    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': name is not set");
    tempAdder.setName("TL_1");
    BOOST_CHECK_NO_THROW(tempAdder.endTemporaryLimit());

    BOOST_CHECK(!adder.hasTemporaryLimits());
    BOOST_CHECK(adder2.hasTemporaryLimits());

    logging::ContainerLogger& logger = dynamic_cast<logging::ContainerLogger&>(logging::LoggerFactory::getLogger("powsybl::iidm"));
    BOOST_CHECK_EQUAL(0, logger.size());
    BOOST_CHECK_NO_THROW(adder2.add());
    BOOST_CHECK_EQUAL(1, logger.size());

    int indexLog = 0;
    BOOST_CHECK_EQUAL("AC line 'VL1_VL3': temporary limit should be greater than permanent limit", logger.getLogMessage(indexLog).getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logger.getLogMessage(indexLog).getLevel());
    BOOST_TEST(line.getCurrentLimits2());

    auto adder3 = adder;
    adder3.beginTemporaryLimit().setAcceptableDuration(1UL).setFictitious(true).setName("TL_1").setValue(105.0).endTemporaryLimit();
    adder3.beginTemporaryLimit().setAcceptableDuration(2UL).setFictitious(true).setName("TL_2").setValue(110.0).endTemporaryLimit();
    BOOST_CHECK_NO_THROW(adder3.add());
    BOOST_CHECK_EQUAL(2, logger.size());

    indexLog++;
    BOOST_CHECK_EQUAL("AC line 'VL1_VL3': temporary limits should be in ascending value order", logger.getLogMessage(indexLog).getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logger.getLogMessage(indexLog).getLevel());
    BOOST_TEST(line.getCurrentLimits2());

    auto adder4 = adder;
    adder4.beginTemporaryLimit().setAcceptableDuration(1UL).setFictitious(true).setName("TL_1").setValue(120.0).endTemporaryLimit();
    adder4.beginTemporaryLimit().setAcceptableDuration(2UL).setFictitious(false).setName("TL_2").setValue(130.0).endTemporaryLimit();
    adder4.beginTemporaryLimit().setAcceptableDuration(3UL).setFictitious(true).setName("TL_3").setValue(110.0).endTemporaryLimit();
    BOOST_CHECK_NO_THROW(adder4.add());
    //TODO(thiebarr) BOOST_CHECK_EQUAL(3, logger.size());
    //TODO(thiebarr) indexLog++;
    //TODO(thiebarr) BOOST_CHECK_EQUAL("AC line 'VL1_VL3': temporary limits should be in ascending value order", logger.getLogMessage(indexLog).getMessage());
    //TODO(thiebarr) POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logger.getLogMessage(indexLog).getLevel());
    BOOST_CHECK_EQUAL(2, logger.size());
    BOOST_TEST(line.getCurrentLimits2());

    auto adder5 = adder;
    adder5.beginTemporaryLimit().setAcceptableDuration(1UL).setFictitious(true).setName("TL_DUPLICATE").setValue(130.0).endTemporaryLimit();
    adder5.beginTemporaryLimit().setAcceptableDuration(2UL).setFictitious(false).setName("TL_2").setValue(120.0).endTemporaryLimit();
    adder5.beginTemporaryLimit().setAcceptableDuration(3UL).setFictitious(true).setName("TL_DUPLICATE").setValue(110.0).endTemporaryLimit();
    POWSYBL_ASSERT_THROW(adder5.add(), ValidationException, "AC line 'VL1_VL3': 2 temporary limits have the same name TL_DUPLICATE");

    adder.beginTemporaryLimit().setAcceptableDuration(1UL).setFictitious(true).setName("TL_1").setValue(130.0).endTemporaryLimit();
    adder.beginTemporaryLimit().setAcceptableDuration(2UL).setFictitious(false).setName("TL_2").setValue(120.0).endTemporaryLimit();
    adder.beginTemporaryLimit().setAcceptableDuration(3UL).setFictitious(true).setName("TL_3").setValue(110.0).endTemporaryLimit();
    BOOST_CHECK_NO_THROW(adder.add());

    // to ensure adder reusability
    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL("TL_1", line.getCurrentLimits2().get().getTemporaryLimit(1UL).getName());
    BOOST_CHECK_EQUAL("TL_2", line.getCurrentLimits2().get().getTemporaryLimit(2UL).getName());
    BOOST_CHECK_EQUAL("TL_3", line.getCurrentLimits2().get().getTemporaryLimit(3UL).getName());

    BOOST_CHECK_EQUAL(2, logger.size());
    BOOST_TEST(line.getCurrentLimits2());

    logging::LoggerFactory::getInstance().removeLogger("powsybl::iidm");
}

BOOST_AUTO_TEST_CASE(checkPermanentLimitsTest) {
    Network network = createCurrentLimitsTestNetwork();

    Line& line = network.getLine("VL1_VL3");
    Terminal& t1 = line.getTerminal1();
    Terminal& t2 = line.getTerminal2();
    Bus& b1 = network.getVoltageLevel("VL1").getBusBreakerView().getBus("VL1_BUS1");

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(std::isnan(t1.getI()));
    BOOST_TEST(std::isnan(b1.getV()));
    BOOST_TEST(!line.checkPermanentLimit1(2.0, LimitType::CURRENT));

    b1.setV(1000.0 * std::sqrt(3.0));
    t1.setQ(0.0).setP(10.0);
    BOOST_CHECK_CLOSE(t1.getP() / (std::sqrt(3.0) * std::sqrt(3.0)), t1.getI(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(line.checkPermanentLimit1(0.5, LimitType::CURRENT));
    BOOST_TEST(!line.checkPermanentLimit1(LimitType::CURRENT));

    BOOST_TEST(line.checkPermanentLimit(TwoSides::ONE, 0.5, LimitType::CURRENT));
    BOOST_TEST(!line.checkPermanentLimit(TwoSides::ONE, LimitType::CURRENT));

    t1.setP(1.0);
    BOOST_TEST(!line.checkPermanentLimit1(LimitType::CURRENT));
    BOOST_TEST(!line.checkPermanentLimit(TwoSides::ONE, LimitType::CURRENT));

    BOOST_TEST(!line.getCurrentLimits2());
    BOOST_TEST(!line.checkPermanentLimit2(2.0, LimitType::CURRENT));

    line.newCurrentLimits2()
        .setPermanentLimit(1000.0)
        .beginTemporaryLimit()
        .setName("TL1_2")
        .setValue(1200.0)
        .setAcceptableDuration(20*60)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2_2")
        .setValue(1400.0)
        .setAcceptableDuration(5*60)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3_2")
        .setValue(1600.0)
        .setAcceptableDuration(60)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();

    BOOST_TEST(line.getCurrentLimits2());

    line.getTerminal2().getBusBreakerView().getBus().get().setV(390);
    line.getTerminal2().setP(100.0).setQ(50.0); // i = 165.5121
    BOOST_TEST(!std::isnan(line.getTerminal2().getI()));
    BOOST_TEST(!line.isOverloaded());
    BOOST_TEST(!line.checkPermanentLimit2(LimitType::CURRENT));
    BOOST_TEST(!line.checkPermanentLimit(TwoSides::TWO, LimitType::CURRENT));
    std::unique_ptr<Overload> ptrOverload = line.checkTemporaryLimits2(LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    line.getTerminal2().setP(800.0).setQ(400.0); // i = 1324.09687
    BOOST_TEST(line.isOverloaded());
    BOOST_CHECK_EQUAL(5 * 60L, line.getOverloadDuration());
    BOOST_TEST(line.checkPermanentLimit2(LimitType::CURRENT));
    ptrOverload = line.checkTemporaryLimits(TwoSides::TWO, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    BOOST_CHECK_EQUAL(5 * 60L, ptrOverload->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL1_2", ptrOverload->getPreviousLimitName());
    BOOST_CHECK_CLOSE(1200.0, ptrOverload->getPreviousLimit(), std::numeric_limits<double>::epsilon());

    line.getTerminal2().setP(900.0).setQ(500.0); // i = 1524.149957
    BOOST_CHECK_EQUAL(60, line.getOverloadDuration());
    ptrOverload = line.checkTemporaryLimits2(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    BOOST_CHECK_EQUAL(60, ptrOverload->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL2_2", ptrOverload->getPreviousLimitName());
    BOOST_CHECK_CLOSE(1400.0, ptrOverload->getPreviousLimit(), std::numeric_limits<double>::epsilon()); 

    POWSYBL_ASSERT_THROW(line.checkPermanentLimit(static_cast<TwoSides>(5), 3.0, LimitType::CURRENT), AssertionError, "Unexpected TwoSides value: 5");
    POWSYBL_ASSERT_THROW(line.checkPermanentLimit(static_cast<TwoSides>(6), LimitType::CURRENT), AssertionError, "Unexpected TwoSides value: 6");

    BOOST_TEST(!line.isOverloaded(2.0));
    BOOST_TEST(line.isOverloaded());
    t2.setP(1.0);
    BOOST_TEST(!line.isOverloaded(2.0));
    BOOST_TEST(!line.isOverloaded());
}

BOOST_AUTO_TEST_CASE(checkLimits3wtLeg1) {
    Network network = createThreeWindingsTransformerCurrentLimitsTestNetwork();
    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT");
    ThreeWindingsTransformer::Leg& leg1 = transformer.getLeg(ThreeSides::ONE);

    BOOST_TEST(!transformer.isOverloaded());
    leg1.getTerminal().getBusBreakerView().getBus().get().setV(390.0);
    leg1.getTerminal().setP(100.0).setQ(50.0); // i = 165.5121
    BOOST_TEST(!std::isnan(leg1.getTerminal().getI()));
    BOOST_TEST(!transformer.isOverloaded());
    BOOST_TEST(!transformer.checkPermanentLimit1(LimitType::CURRENT));
    std::unique_ptr<Overload> ptrOverload = transformer.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    leg1.getTerminal().setP(800.0).setQ(400.0); // i = 1324.09687
    BOOST_TEST(transformer.isOverloaded());
    BOOST_CHECK_EQUAL(5 * 60L, transformer.getOverloadDuration());
    BOOST_TEST(transformer.checkPermanentLimit1(LimitType::CURRENT));
    ptrOverload = transformer.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    BOOST_CHECK_EQUAL(5 * 60L, ptrOverload->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_EQUAL("20'", ptrOverload->getPreviousLimitName());
    BOOST_CHECK_CLOSE(1200.0, ptrOverload->getPreviousLimit(), std::numeric_limits<double>::epsilon());

    leg1.getTerminal().setP(900.0).setQ(500.0); // i = 1524.149957
    BOOST_CHECK_EQUAL(60, transformer.getOverloadDuration());
    ptrOverload = transformer.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    BOOST_CHECK_EQUAL(60, ptrOverload->getTemporaryLimit().getAcceptableDuration());
    BOOST_CHECK_EQUAL("5'", ptrOverload->getPreviousLimitName());
    BOOST_CHECK_CLOSE(1400.0, ptrOverload->getPreviousLimit(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(checkTemporaryLimitsTest) {
    Network network = createCurrentLimitsTestNetwork();

    Line& line = network.getLine("VL1_VL3");
    Terminal& t1 = line.getTerminal1();
    Terminal& t2 = line.getTerminal2();
    Bus& b1 = network.getVoltageLevel("VL1").getBusBreakerView().getBus("VL1_BUS1");
    Bus& b2 = network.getVoltageLevel("VL3").getBusBreakerView().getBus("VL3_BUS1");

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(std::isnan(t1.getI()));
    BOOST_TEST(std::isnan(b1.getV()));
    std::unique_ptr<Overload> ptrOverload = line.checkTemporaryLimits1(2.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    b1.setV(1000.0 * std::sqrt(3.0));
    t1.setQ(0.0).setP(9.0);
    BOOST_CHECK_CLOSE(t1.getP() / (std::sqrt(3.0) * std::sqrt(3.0)), t1.getI(), std::numeric_limits<double>::epsilon());

    ptrOverload = line.checkTemporaryLimits1(0.7, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Overload& overload = *ptrOverload;
    BOOST_CHECK_EQUAL(LimitViolationUtils::PERMANENT_LIMIT_NAME, overload.getPreviousLimitName());
    BOOST_CHECK_CLOSE(4.0, overload.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    const CurrentLimits::TemporaryLimit& tl = overload.getTemporaryLimit();
    auto limits = line.getCurrentLimits1().get();

    BOOST_CHECK_EQUAL(3UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL3", tl.getName());
    BOOST_CHECK_CLOSE(5.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(15.5);
    ptrOverload = line.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Overload& overload2 = *ptrOverload;
    const CurrentLimits::TemporaryLimit& tl2 = overload2.getTemporaryLimit();
    BOOST_CHECK_EQUAL("TL3", overload2.getPreviousLimitName());
    BOOST_CHECK_CLOSE(5.0, overload2.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL2", tl2.getName());
    BOOST_CHECK_CLOSE(6.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(tl2.isFictitious());
    BOOST_CHECK_CLOSE(tl2.getValue(), limits.getTemporaryLimitValue(tl2.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(18.5);
    ptrOverload = line.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Overload& overload3 = *ptrOverload;
    const CurrentLimits::TemporaryLimit& tl3 = overload3.getTemporaryLimit();
    BOOST_CHECK_EQUAL("TL2", overload3.getPreviousLimitName());
    BOOST_CHECK_CLOSE(6.0, overload3.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(1UL, tl3.getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL1", tl3.getName());
    BOOST_CHECK_CLOSE(7.0, tl3.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl3.isFictitious());
    BOOST_CHECK_CLOSE(tl3.getValue(), limits.getTemporaryLimitValue(tl3.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(50.0);
    ptrOverload = line.checkTemporaryLimits1(2.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    ptrOverload = line.checkTemporaryLimits(TwoSides::ONE, 2.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    t1.setP(30.0);
    ptrOverload = line.checkTemporaryLimits(TwoSides::ONE, 2.0, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));

    t1.setP(14.0);
    ptrOverload = line.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(TwoSides::ONE, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));

    t1.setP(1.0);
    ptrOverload = line.checkTemporaryLimits1(LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(TwoSides::ONE, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    BOOST_TEST(!line.getCurrentLimits2());
    ptrOverload = line.checkTemporaryLimits2(2.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    line.newCurrentLimits2()
        .setPermanentLimit(8.0)
        .beginTemporaryLimit()
        .setName("TL1_2")
        .setValue(11.0)
        .setAcceptableDuration(2UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2_2")
        .setValue(10.0)
        .setAcceptableDuration(5UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3_2")
        .setValue(9.0)
        .setAcceptableDuration(6UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();

    BOOST_TEST(line.getCurrentLimits2());
    BOOST_TEST(std::isnan(t2.getI()));
    BOOST_TEST(std::isnan(b2.getV()));
    ptrOverload = line.checkTemporaryLimits2(2.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    b2.setV(1000.0 * std::sqrt(3.0));
    t2.setQ(0.0).setP(20.0);
    BOOST_CHECK_CLOSE(t2.getP() / (std::sqrt(3.0) * std::sqrt(3.0)), t2.getI(), std::numeric_limits<double>::epsilon());

    ptrOverload = line.checkTemporaryLimits2(0.7, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits2(5.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    ptrOverload = line.checkTemporaryLimits(TwoSides::TWO, 0.7, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(TwoSides::TWO, 5.0, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    t2.setP(25.0);
    ptrOverload = line.checkTemporaryLimits2(LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(TwoSides::TWO, LimitType::CURRENT);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    t2.setP(1.0);
    ptrOverload = line.checkTemporaryLimits2(LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(TwoSides::TWO, LimitType::CURRENT);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    POWSYBL_ASSERT_THROW(line.checkTemporaryLimits(static_cast<TwoSides>(5), 3.0, LimitType::CURRENT), AssertionError, "Unexpected TwoSides value: 5");
    POWSYBL_ASSERT_THROW(line.checkTemporaryLimits(static_cast<TwoSides>(6), LimitType::CURRENT), AssertionError, "Unexpected TwoSides value: 6");

    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), line.getOverloadDuration());
    t1.setP(13.0);
    BOOST_CHECK_EQUAL(3UL, line.getOverloadDuration());
    t1.setP(1.0);
    t2.setP(31.0);
    BOOST_CHECK_EQUAL(2UL, line.getOverloadDuration());
    t1.setP(18.5);
    BOOST_CHECK_EQUAL(1UL, line.getOverloadDuration());
    t1.setP(15.5);
    BOOST_CHECK_EQUAL(2UL, line.getOverloadDuration());
}

BOOST_AUTO_TEST_CASE(adderGetLimitsValues) {
    Network network = createOneLineCurrentLimitsTestNetwork();

    CurrentLimitsAdder adder = network.getLine("L").newCurrentLimits1();

    BOOST_CHECK(std::isnan(adder.getLowestTemporaryLimitValue()));
    BOOST_CHECK(std::isnan(adder.getPermanentLimit()));
    BOOST_CHECK(!adder.hasTemporaryLimits());

    adder.setPermanentLimit(1000.)
        .beginTemporaryLimit()
            .setName("TL1")
            .setAcceptableDuration(20 * 60)
            .setValue(1200.0)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("TL2")
            .setAcceptableDuration(10 * 60)
            .setValue(1400.0)
        .endTemporaryLimit();

    BOOST_CHECK(adder.hasTemporaryLimits());

    BOOST_CHECK_CLOSE(1200., adder.getTemporaryLimitValue("TL1"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1400., adder.getTemporaryLimitValue("TL2"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(adder.getTemporaryLimitValue("Unknown")));

    BOOST_CHECK_EQUAL(20 * 60, adder.getTemporaryLimitAcceptableDuration("TL1"));
    BOOST_CHECK_EQUAL(10 * 60, adder.getTemporaryLimitAcceptableDuration("TL2"));
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), adder.getTemporaryLimitAcceptableDuration("Unknown"));

    BOOST_CHECK_CLOSE(1200.0, adder.getLowestTemporaryLimitValue(), 0.0);
}

BOOST_AUTO_TEST_CASE(adderRemoveTemporaryLimit) {
    Network network = createOneLineCurrentLimitsTestNetwork();
    CurrentLimitsAdder adder = network.getLine("L").newCurrentLimits1();
    adder.setPermanentLimit(1000.)
                .beginTemporaryLimit()
                    .setName("TL1")
                    .setAcceptableDuration(20 * 60)
                    .setValue(1200.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                    .setName("TL2")
                    .setAcceptableDuration(10 * 60)
                    .setValue(1400.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                    .setName("TL3")
                    .setAcceptableDuration(5 * 60)
                    .setValue(1600.0)
                .endTemporaryLimit();

    BOOST_CHECK_EQUAL(3, boost::size(adder.getTemporaryLimitNames()));
    BOOST_CHECK_EQUAL("TL1", adder.getTemporaryLimitNames()[0]);
    BOOST_CHECK_EQUAL("TL2", adder.getTemporaryLimitNames()[1]);
    BOOST_CHECK_EQUAL("TL3", adder.getTemporaryLimitNames()[2]);

    BOOST_CHECK_CLOSE(1200.0, adder.getTemporaryLimitValue("TL1"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1400.0, adder.getTemporaryLimitValue("TL2"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1600.0, adder.getTemporaryLimitValue("TL3"), std::numeric_limits<double>::epsilon());

    adder.removeTemporaryLimit("TL2");

    BOOST_CHECK_EQUAL(2, boost::size(adder.getTemporaryLimitNames()));
    BOOST_CHECK_EQUAL("TL1", adder.getTemporaryLimitNames()[0]);
    BOOST_CHECK_EQUAL("TL3", adder.getTemporaryLimitNames()[1]);

    BOOST_CHECK_CLOSE(1200.0, adder.getTemporaryLimitValue("TL1"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(adder.getTemporaryLimitValue("TL2")));
    BOOST_CHECK_CLOSE(1600.0, adder.getTemporaryLimitValue("TL3"), std::numeric_limits<double>::epsilon());

    adder.removeTemporaryLimit("TL1");
    adder.removeTemporaryLimit("TL3");

    BOOST_CHECK_NO_THROW(adder.removeTemporaryLimit("TL3"));

    BOOST_CHECK(!adder.hasTemporaryLimits());
    BOOST_CHECK_EQUAL(0, boost::size(adder.getTemporaryLimitNames()));
}

BOOST_AUTO_TEST_CASE(adderFixPermanentLimit) {
    Network network = createOneLineCurrentLimitsTestNetwork();
    CurrentLimitsAdder adder = network.getLine("L").newCurrentLimits1();
    adder.beginTemporaryLimit()
                    .setName("TL1")
                    .setAcceptableDuration(20 * 60)
                    .setValue(1200.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                    .setName("TL2")
                    .setAcceptableDuration(10 * 60)
                    .setValue(1400.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                    .setName("TL3")
                    .setAcceptableDuration(5 * 60)
                    .setValue(1600.0)
                .endTemporaryLimit();

    BOOST_CHECK(std::isnan(adder.getPermanentLimit()));
    adder.fixLimits(90.0);
    BOOST_CHECK_CLOSE(1080., adder.getPermanentLimit(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(adderFixPermanentLimitAlreadySet) {
    Network network = createOneLineCurrentLimitsTestNetwork();
    CurrentLimitsAdder adder = network.getLine("L").newCurrentLimits1();
    
    adder.setPermanentLimit(1000.)
                .beginTemporaryLimit()
                    .setName("TL1")
                    .setAcceptableDuration(20 * 60)
                    .setValue(1200.0)
                .endTemporaryLimit();
    adder.fixLimits(90.);
    BOOST_CHECK_CLOSE(1000., adder.getPermanentLimit(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(adderFixPermanentLimitWithInfiniteDurationValue) {
    Network network = createOneLineCurrentLimitsTestNetwork();
    CurrentLimitsAdder adder = network.getLine("L").newCurrentLimits1();
    
    adder.beginTemporaryLimit()
                    .setName("INFINITE")
                    .setAcceptableDuration(std::numeric_limits<unsigned long>::max())
                    .setValue(800.0)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                    .setName("TL1")
                    .setAcceptableDuration(20 * 60)
                    .setValue(1200.0)
                .endTemporaryLimit();

    BOOST_CHECK(std::isnan(adder.getPermanentLimit()));
    adder.fixLimits(90.);
    BOOST_CHECK_CLOSE(800., adder.getPermanentLimit(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_EQUAL(1, boost::size(adder.getTemporaryLimitNames()));
    BOOST_CHECK_CLOSE(1200.0, adder.getTemporaryLimitValue("TL1"), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(adder.getTemporaryLimitValue("INFINITE")));
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
