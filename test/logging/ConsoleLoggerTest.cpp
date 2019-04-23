/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/logging/ConsoleLogger.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace logging {

BOOST_AUTO_TEST_SUITE(ConsoleLoggerTestSuite)

BOOST_AUTO_TEST_CASE(level) {
    ConsoleLogger logger;
    BOOST_TEST(logger.isTraceEnabled());
    BOOST_TEST(logger.isDebugEnabled());
    BOOST_TEST(logger.isInfoEnabled());
    BOOST_TEST(logger.isWarnEnabled());
    BOOST_TEST(logger.isErrorEnabled());
}

BOOST_AUTO_TEST_CASE(logMessage) {
    ConsoleLogger logger;
    logger.trace("trace message");
    logger.debug("debug message");
    logger.info("info message");
    logger.warn("warn message");
    logger.error("error message");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace logging

}  // namespace powsybl
