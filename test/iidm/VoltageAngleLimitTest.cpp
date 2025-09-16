/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/iidm/VoltageAngleLimit.hpp>
#include <powsybl/iidm/VoltageAngleLimitAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
#include <powsybl/stdcxx/reference.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(VoltageAngleLimitTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    VoltageAngleLimit& val0_0 = network.newVoltageAngleLimit()
                .setId("VOLTAGE_ANGLE_LIMIT_LINE_S2S3")
                .from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
                .to(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal2()))
                .setHighLimit(1.0)
                .add();

    network.newVoltageAngleLimit()
                .setId("VOLTAGE_ANGLE_LIMIT_LD1_LD6")
                .from(stdcxx::ref<Terminal>(network.getLoad("LD1").getTerminal()))
                .to(stdcxx::ref<Terminal>(network.getLoad("LD6").getTerminal()))
                .setLowLimit(1.0)
                .add();

    BOOST_CHECK_EQUAL(2, network.getVoltageAngleLimitsCount());
    BOOST_CHECK_EQUAL(2, boost::size(network.getVoltageAngleLimits()));
    BOOST_CHECK_EQUAL(2, network.getVoltageAngleLimitsIndex().size());

    VoltageAngleLimit& val0_1 = network.getVoltageAngleLimitsIndex().at("VOLTAGE_ANGLE_LIMIT_LINE_S2S3");
    VoltageAngleLimit& val0_2 = network.getVoltageAngleLimit("VOLTAGE_ANGLE_LIMIT_LINE_S2S3");
    BOOST_CHECK(stdcxx::areSame(val0_0, val0_1));
    BOOST_CHECK(stdcxx::areSame(val0_0, val0_2));
    BOOST_CHECK_EQUAL("VOLTAGE_ANGLE_LIMIT_LINE_S2S3", val0_0.getId());

    VoltageAngleLimit& val1 = network.getVoltageAngleLimit("VOLTAGE_ANGLE_LIMIT_LD1_LD6");
    POWSYBL_ASSERT_THROW(network.getVoltageAngleLimit("notExisting"), PowsyblException, "Unable to find to the voltage angle limit 'notExisting'");

    BOOST_CHECK_EQUAL("LD1", val1.getTerminalFrom().getConnectable().get().getId());
    BOOST_CHECK(!Terminal::getConnectableSide(val1.getTerminalFrom()).has_value());
    BOOST_CHECK_EQUAL("LD6", val1.getTerminalTo().getConnectable().get().getId());
    BOOST_CHECK(!Terminal::getConnectableSide(val1.getTerminalTo()).has_value());
    BOOST_CHECK(!std::isnan(val1.getLowLimit()));
    BOOST_CHECK(std::isnan(val1.getHighLimit()));
    BOOST_CHECK_CLOSE(1.0, val1.getLowLimit(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_EQUAL("LINE_S2S3", val0_0.getTerminalFrom().getConnectable().get().getId());
    BOOST_CHECK(Terminal::getConnectableSide(val0_0.getTerminalFrom()).has_value());
    BOOST_CHECK_EQUAL(*Terminal::getConnectableSide(val0_0.getTerminalFrom()), ThreeSides::ONE);
    BOOST_CHECK_EQUAL("LINE_S2S3", val0_0.getTerminalTo().getConnectable().get().getId());
    BOOST_CHECK(Terminal::getConnectableSide(val0_0.getTerminalTo()).has_value());
    BOOST_CHECK_EQUAL(*Terminal::getConnectableSide(val0_0.getTerminalTo()), ThreeSides::TWO);
    BOOST_CHECK(!std::isnan(val0_0.getHighLimit()));
    BOOST_CHECK(std::isnan(val0_0.getLowLimit()));
    BOOST_CHECK_CLOSE(1.0, val0_0.getHighLimit(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(noName) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    VoltageAngleLimitAdder adder = network.newVoltageAngleLimit();
    adder.from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
        .to(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal2()));

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Voltage angle limit id is mandatory.");
}

BOOST_AUTO_TEST_CASE(badLimits) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    VoltageAngleLimitAdder adder = network.newVoltageAngleLimit();
    adder.from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
        .to(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal2()))
        .setId("VOLTAGE_ANGLE_LIMIT_LINE_S2S3")
        .setLowLimit(20.0)
        .setHighLimit(-20.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Voltage angle low limit must be lower than the high limit.");
}

BOOST_AUTO_TEST_CASE(uniqueId) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    network.newVoltageAngleLimit()
        .from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
        .to(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal2()))
        .setId("Limit")
        .setLowLimit(-20.0)
        .setHighLimit(20.0)
        .add();

    VoltageAngleLimitAdder adder = network.newVoltageAngleLimit();
    adder.from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
        .to(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal2()))
        .setId("Limit")
        .setLowLimit(-20.0)
        .setHighLimit(20.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network fourSubstations already contains a voltage angle limit with the id 'Limit'");
}

BOOST_AUTO_TEST_CASE(missingTerminal) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    VoltageAngleLimitAdder adder = network.newVoltageAngleLimit();
    adder.from(stdcxx::ref<Terminal>(network.getLine("LINE_S2S3").getTerminal1()))
        .setId("Limit")
        .setLowLimit(-20.0)
        .setHighLimit(20.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Voltage angle limit must be connected to terminals.");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
