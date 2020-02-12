/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <chrono>

#include <boost/test/unit_test.hpp>

#include <powsybl/logging/ContainerLogger.hpp>
#include <powsybl/stdcxx/put_time.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace logging {

BOOST_AUTO_TEST_SUITE(ContainerLoggerTestSuite)

BOOST_AUTO_TEST_CASE(level) {
    ContainerLogger logger;
    BOOST_TEST(logger.isTraceEnabled());
    BOOST_TEST(logger.isDebugEnabled());
    BOOST_TEST(logger.isInfoEnabled());
    BOOST_TEST(logger.isWarnEnabled());
    BOOST_TEST(logger.isErrorEnabled());
}

BOOST_AUTO_TEST_CASE(logMessage) {
    ContainerLogger logger;
    const std::time_t& before = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logger.trace("trace message");
    logger.debug("debug message");
    logger.info("info message");
    logger.warn("warn message");
    logger.error("error message");
    const std::time_t& after = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    BOOST_CHECK_EQUAL(5, logger.size());
    unsigned int size = 0;
    std::time_t prev = before;
    for (auto it = logger.cbegin(); it != logger.cend(); ++it) {
        BOOST_TEST(it->getInstant() >= before);
        BOOST_TEST(it->getInstant() >= prev);
        BOOST_TEST(it->getInstant() <= after);

        size++;
        prev = it->getInstant();
    }
    BOOST_CHECK_EQUAL(size, logger.size());

    std::ostringstream oss;
    struct std::tm result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    std::time_t instant;
    const LogMessage& logMessage1 = logger.getLogMessage(0);
    BOOST_CHECK_EQUAL("trace message", logMessage1.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(Level::TRACE, logMessage1.getLevel());
    instant = logMessage1.getInstant();
    localtime_r(&instant, &result);
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(logMessage1.getLevel()) << " - " << logMessage1.getMessage();
    BOOST_CHECK_EQUAL(oss.str(), logMessage1.toString());

    const LogMessage& logMessage2 = logger.getLogMessage(1);
    BOOST_CHECK_EQUAL("debug message", logMessage2.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(Level::DEBUG, logMessage2.getLevel());
    instant = logMessage2.getInstant();
    localtime_r(&instant, &result);
    oss.str("");
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(logMessage2.getLevel()) << " - " << logMessage2.getMessage();
    BOOST_CHECK_EQUAL(oss.str(), logMessage2.toString());

    const LogMessage& logMessage3 = logger.getLogMessage(2);
    BOOST_CHECK_EQUAL("info message", logMessage3.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(Level::INFO, logMessage3.getLevel());
    instant = logMessage3.getInstant();
    localtime_r(&instant, &result);
    oss.str("");
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(logMessage3.getLevel()) << " - " << logMessage3.getMessage();
    BOOST_CHECK_EQUAL(oss.str(), logMessage3.toString());

    const LogMessage& logMessage4 = logger.getLogMessage(3);
    BOOST_CHECK_EQUAL("warn message", logMessage4.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(Level::WARN, logMessage4.getLevel());
    instant = logMessage4.getInstant();
    localtime_r(&instant, &result);
    oss.str("");
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(logMessage4.getLevel()) << " - " << logMessage4.getMessage();
    BOOST_CHECK_EQUAL(oss.str(), logMessage4.toString());

    const LogMessage& logMessage5 = logger.getLogMessage(4);
    BOOST_CHECK_EQUAL("error message", logMessage5.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(Level::ERROR, logMessage5.getLevel());
    instant = logMessage5.getInstant();
    localtime_r(&instant, &result);
    oss.str("");
    oss << stdcxx::put_time(&result, "%Y-%m-%d %X") << " - " << getLevelName(logMessage5.getLevel()) << " - " << logMessage5.getMessage();
    BOOST_CHECK_EQUAL(oss.str(), logMessage5.toString());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace logging

}  // namespace powsybl
