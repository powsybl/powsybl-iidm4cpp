/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <limits>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/ContainerLogger.hpp>
#include <powsybl/logging/LogMessage.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(225.0)
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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(225.0)
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

BOOST_AUTO_TEST_SUITE(CurrentLimitsTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createCurrentLimitsTestNetwork();
    const Network& cNetwork = network;

    Line& line = network.getLine("VL1_VL3");
    const Line& cLine = cNetwork.getLine("VL1_VL3");;
    BOOST_TEST(stdcxx::areSame(line, cLine));

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(cLine.getCurrentLimits1());
    BOOST_TEST(line.getCurrentLimits(Branch::Side::ONE));
    BOOST_TEST(cLine.getCurrentLimits(Branch::Side::ONE));

    BOOST_TEST(!line.getCurrentLimits2());
    BOOST_TEST(!cLine.getCurrentLimits2());
    BOOST_TEST(!line.getCurrentLimits(Branch::Side::TWO));
    BOOST_TEST(!cLine.getCurrentLimits(Branch::Side::TWO));

    POWSYBL_ASSERT_THROW(line.getCurrentLimits(static_cast<Branch::Side>(5)), AssertionError, "Unexpected side value: 5");
    POWSYBL_ASSERT_THROW(cLine.getCurrentLimits(static_cast<Branch::Side>(6)), AssertionError, "Unexpected side value: 6");

    CurrentLimits limits = line.getCurrentLimits1().get();
    const CurrentLimits& cLimits = limits;
    BOOST_TEST(stdcxx::areSame(limits, cLimits));
    BOOST_CHECK_CLOSE(4.0, limits.getPermanentLimit(), std::numeric_limits<double>::epsilon());

    std::vector<std::reference_wrapper<CurrentLimits::TemporaryLimit>> tempLimits = limits.getTemporaryLimits();
    BOOST_CHECK_EQUAL(3, tempLimits.size());
    CurrentLimits::TemporaryLimit tl = limits.getTemporaryLimit(3UL);
    CurrentLimits::TemporaryLimit tl2 = cLimits.getTemporaryLimit(3UL);
    unsigned int index = 0;

    BOOST_CHECK_EQUAL(3UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(3UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL(3UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL3", tl.getName());
    BOOST_CHECK_EQUAL("TL3", tl2.getName());
    BOOST_CHECK_EQUAL("TL3", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(5.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_TEST(!tl2.isFictitious());
    BOOST_TEST(!tempLimits.at(index).get().isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    tl = limits.getTemporaryLimit(2UL);
    tl2 = cLimits.getTemporaryLimit(2UL);
    index++;

    BOOST_CHECK_EQUAL(2UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(2UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL(2UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL2", tl.getName());
    BOOST_CHECK_EQUAL("TL2", tl2.getName());
    BOOST_CHECK_EQUAL("TL2", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(6.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(tl.isFictitious());
    BOOST_TEST(tl2.isFictitious());
    BOOST_TEST(tempLimits.at(index).get().isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    tl = limits.getTemporaryLimit(1UL);
    tl2 = cLimits.getTemporaryLimit(1UL);
    index++;

    BOOST_CHECK_EQUAL(1UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(1UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL(1UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL1", tl.getName());
    BOOST_CHECK_EQUAL("TL1", tl2.getName());
    BOOST_CHECK_EQUAL("TL1", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(7.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(7.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(7.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_TEST(!tl2.isFictitious());
    BOOST_TEST(!tempLimits.at(index).get().isFictitious());
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
    POWSYBL_ASSERT_THROW(limits.setPermanentLimit(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");
    POWSYBL_ASSERT_THROW(limits.setPermanentLimit(-1.0), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(cLine.getCurrentLimits1());
    BOOST_TEST(line.getCurrentLimits(Branch::Side::ONE));
    BOOST_TEST(cLine.getCurrentLimits(Branch::Side::ONE));
}

BOOST_AUTO_TEST_CASE(adder) {
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm", stdcxx::make_unique<logging::ContainerLogger>());

    Network network = createCurrentLimitsTestNetwork();

    Line& line = network.getLine("VL1_VL3");
    BOOST_TEST(!line.getCurrentLimits2());
    auto adder = line.newCurrentLimits2();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");
    adder.setPermanentLimit(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");
    adder.setPermanentLimit(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC line 'VL1_VL3': permanent limit must be > 0");
    adder.setPermanentLimit(100.0);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_TEST(line.getCurrentLimits2());

    auto adder2 = adder;
    auto tempAdder = adder2.beginTemporaryLimit();
    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': temporary limit value is not set");
    tempAdder.setValue(stdcxx::nan());
    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': temporary limit value is not set");
    tempAdder.setValue(-10.0);
    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': temporary limit value must be > 0");
    tempAdder.setValue(10.0);

    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': acceptable duration is not set");
    tempAdder.setAcceptableDuration(1UL);

    POWSYBL_ASSERT_THROW(tempAdder.endTemporaryLimit(), ValidationException, "AC line 'VL1_VL3': name is not set");
    tempAdder.setName("TL_1");
    BOOST_CHECK_NO_THROW(tempAdder.endTemporaryLimit());

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
    Bus& b2 = network.getVoltageLevel("VL3").getBusBreakerView().getBus("VL3_BUS1");

    BOOST_TEST(line.getCurrentLimits1());
    BOOST_TEST(std::isnan(t1.getI()));
    BOOST_TEST(std::isnan(b1.getV()));
    BOOST_TEST(!line.checkPermanentLimit1(2.0));

    b1.setV(1000.0 * std::sqrt(3.0));
    t1.setQ(0.0).setP(10.0);
    BOOST_CHECK_CLOSE(t1.getP(), t1.getI(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(line.checkPermanentLimit1(2.0));
    BOOST_TEST(!line.checkPermanentLimit1(5.0));

    BOOST_TEST(line.checkPermanentLimit(Branch::Side::ONE, 2.0));
    BOOST_TEST(!line.checkPermanentLimit(Branch::Side::ONE, 5.0));

    BOOST_TEST(line.checkPermanentLimit1());
    BOOST_TEST(line.checkPermanentLimit(Branch::Side::ONE));
    t1.setP(1.0);
    BOOST_TEST(!line.checkPermanentLimit1());
    BOOST_TEST(!line.checkPermanentLimit(Branch::Side::ONE));

    BOOST_TEST(!line.getCurrentLimits2());
    BOOST_TEST(!line.checkPermanentLimit2(2.0));

    line.newCurrentLimits2()
        .setPermanentLimit(8.0)
        .beginTemporaryLimit()
        .setName("TL1_2")
        .setValue(11.0)
        .setAcceptableDuration(4UL)
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
    BOOST_TEST(!line.checkPermanentLimit2(2.0));

    b2.setV(1000.0 * std::sqrt(3.0));
    t2.setQ(0.0).setP(20.0);
    BOOST_CHECK_CLOSE(t2.getP(), t2.getI(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(line.checkPermanentLimit2(2.0));
    BOOST_TEST(!line.checkPermanentLimit2(5.0));

    BOOST_TEST(line.checkPermanentLimit(Branch::Side::TWO, 2.0));
    BOOST_TEST(!line.checkPermanentLimit(Branch::Side::TWO, 5.0));

    BOOST_TEST(line.checkPermanentLimit2());
    BOOST_TEST(line.checkPermanentLimit(Branch::Side::TWO));
    t2.setP(1.0);
    BOOST_TEST(!line.checkPermanentLimit2());
    BOOST_TEST(!line.checkPermanentLimit(Branch::Side::TWO));

    POWSYBL_ASSERT_THROW(line.checkPermanentLimit(static_cast<Branch::Side>(5), 3.0), AssertionError, "Unexpected side value: 5");
    POWSYBL_ASSERT_THROW(line.checkPermanentLimit(static_cast<Branch::Side>(6)), AssertionError, "Unexpected side value: 6");

    BOOST_TEST(!line.isOverloaded(2.0));
    BOOST_TEST(!line.isOverloaded());
    t2.setP(20.0);
    BOOST_TEST(line.isOverloaded(2.0));
    t1.setP(10.0);
    BOOST_TEST(line.isOverloaded(2.0));
    t2.setP(1.0);
    BOOST_TEST(line.isOverloaded(2.0));

    BOOST_TEST(line.isOverloaded());
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
    std::unique_ptr<Branch::Overload> ptrOverload = line.checkTemporaryLimits1(2.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    b1.setV(1000.0 * std::sqrt(3.0));
    t1.setQ(0.0).setP(9.0);
    BOOST_CHECK_CLOSE(t1.getP(), t1.getI(), std::numeric_limits<double>::epsilon());

    ptrOverload = line.checkTemporaryLimits1(2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Branch::Overload& overload = *ptrOverload;
    BOOST_CHECK_EQUAL("", overload.getPreviousLimitName());
    BOOST_CHECK_CLOSE(4.0, overload.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    const CurrentLimits::TemporaryLimit& tl = overload.getTemporaryLimit();
    auto limits = line.getCurrentLimits1().get();
    const auto& tempLimits = limits.getTemporaryLimits();
    unsigned long index = 0;

    BOOST_CHECK_EQUAL(3UL, tl.getAcceptableDuration());
    BOOST_CHECK_EQUAL(3UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL3", tl.getName());
    BOOST_CHECK_EQUAL("TL3", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(5.0, tl.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl.isFictitious());
    BOOST_TEST(!tempLimits.at(index).get().isFictitious());
    BOOST_CHECK_CLOSE(tl.getValue(), limits.getTemporaryLimitValue(tl.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(11.0);
    ptrOverload = line.checkTemporaryLimits1(2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Branch::Overload& overload2 = *ptrOverload;
    const CurrentLimits::TemporaryLimit& tl2 = overload2.getTemporaryLimit();
    BOOST_CHECK_EQUAL("TL3", overload2.getPreviousLimitName());
    BOOST_CHECK_CLOSE(5.0, overload2.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    index = 1;
    BOOST_CHECK_EQUAL(2UL, tl2.getAcceptableDuration());
    BOOST_CHECK_EQUAL(2UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL2", tl2.getName());
    BOOST_CHECK_EQUAL("TL2", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(6.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(tl2.isFictitious());
    BOOST_TEST(tempLimits.at(index).get().isFictitious());
    BOOST_CHECK_CLOSE(tl2.getValue(), limits.getTemporaryLimitValue(tl2.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(13.0);
    ptrOverload = line.checkTemporaryLimits1(2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    Branch::Overload& overload3 = *ptrOverload;
    const CurrentLimits::TemporaryLimit& tl3 = overload3.getTemporaryLimit();
    BOOST_CHECK_EQUAL("TL2", overload3.getPreviousLimitName());
    BOOST_CHECK_CLOSE(6.0, overload3.getPreviousLimit(), std::numeric_limits<double>::epsilon());
    index = 2;
    BOOST_CHECK_EQUAL(1UL, tl3.getAcceptableDuration());
    BOOST_CHECK_EQUAL(1UL, tempLimits.at(index).get().getAcceptableDuration());
    BOOST_CHECK_EQUAL("TL1", tl3.getName());
    BOOST_CHECK_EQUAL("TL1", tempLimits.at(index).get().getName());
    BOOST_CHECK_CLOSE(7.0, tl3.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(7.0, tempLimits.at(index).get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tl3.isFictitious());
    BOOST_TEST(!tempLimits.at(index).get().isFictitious());
    BOOST_CHECK_CLOSE(tl3.getValue(), limits.getTemporaryLimitValue(tl3.getAcceptableDuration()), std::numeric_limits<double>::epsilon());

    t1.setP(15.0);
    ptrOverload = line.checkTemporaryLimits1(2.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    ptrOverload = line.checkTemporaryLimits(Branch::Side::ONE, 2.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    t1.setP(9.0);
    ptrOverload = line.checkTemporaryLimits(Branch::Side::ONE, 2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));

    t1.setP(5.0);
    ptrOverload = line.checkTemporaryLimits1();
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(Branch::Side::ONE);
    BOOST_TEST(static_cast<bool>(ptrOverload));

    t1.setP(1.0);
    ptrOverload = line.checkTemporaryLimits1();
    BOOST_TEST(!static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(Branch::Side::ONE);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    BOOST_TEST(!line.getCurrentLimits2());
    ptrOverload = line.checkTemporaryLimits2(2.0);
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
    ptrOverload = line.checkTemporaryLimits2(2.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    b2.setV(1000.0 * std::sqrt(3.0));
    t2.setQ(0.0).setP(20.0);
    BOOST_CHECK_CLOSE(t2.getP(), t2.getI(), std::numeric_limits<double>::epsilon());

    ptrOverload = line.checkTemporaryLimits2(2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits2(5.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    ptrOverload = line.checkTemporaryLimits(Branch::Side::TWO, 2.0);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(Branch::Side::TWO, 5.0);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    t2.setP(9.0);
    ptrOverload = line.checkTemporaryLimits2();
    BOOST_TEST(static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(Branch::Side::TWO);
    BOOST_TEST(static_cast<bool>(ptrOverload));
    t2.setP(1.0);
    ptrOverload = line.checkTemporaryLimits2();
    BOOST_TEST(!static_cast<bool>(ptrOverload));
    ptrOverload = line.checkTemporaryLimits(Branch::Side::TWO);
    BOOST_TEST(!static_cast<bool>(ptrOverload));

    POWSYBL_ASSERT_THROW(line.checkTemporaryLimits(static_cast<Branch::Side>(5), 3.0), AssertionError, "Unexpected side value: 5");
    POWSYBL_ASSERT_THROW(line.checkTemporaryLimits(static_cast<Branch::Side>(6)), AssertionError, "Unexpected side value: 6");

    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), line.getOverloadDuration());
    t1.setP(4.5);
    BOOST_CHECK_EQUAL(3UL, line.getOverloadDuration());
    t1.setP(1.0);
    t2.setP(10.5);
    BOOST_CHECK_EQUAL(2UL, line.getOverloadDuration());
    t1.setP(6.0);
    BOOST_CHECK_EQUAL(1UL, line.getOverloadDuration());
    t1.setP(4.5);
    BOOST_CHECK_EQUAL(2UL, line.getOverloadDuration());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
