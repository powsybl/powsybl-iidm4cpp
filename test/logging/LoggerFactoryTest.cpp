/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/logging/ConsoleLogger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/logging/NoopLogger.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace logging {

TEST(LoggerFactory, test) {
    Logger& unknown1 = LoggerFactory::getLogger("unknown1");
    Logger& unknown2 = LoggerFactory::getLogger("unknown2");
    Logger& unknown3 = LoggerFactory::getLogger<LoggerFactory>();
    ASSERT_EQ(typeid(NoopLogger), typeid(unknown1));
    ASSERT_EQ(typeid(NoopLogger), typeid(unknown2));
    ASSERT_EQ(typeid(NoopLogger), typeid(unknown3));
    ASSERT_TRUE(stdcxx::areSame(unknown1, unknown2));
    ASSERT_TRUE(stdcxx::areSame(unknown1, unknown3));

    LoggerFactory::getInstance().addLogger("console", stdcxx::make_unique<ConsoleLogger>());
    Logger& consoleLogger = LoggerFactory::getLogger("console");
    ASSERT_EQ(typeid(ConsoleLogger), typeid(consoleLogger));

    LoggerFactory::getInstance().addLogger<LoggerFactory>(stdcxx::make_unique<NoopLogger>());
    Logger& noopLogger = LoggerFactory::getLogger<LoggerFactory>();
    ASSERT_EQ(typeid(NoopLogger), typeid(noopLogger));
    ASSERT_FALSE(stdcxx::areSame(unknown3, noopLogger));

    LoggerFactory::getInstance().addLogger("powsybl", stdcxx::make_unique<NoopLogger>());
    Logger& expected = LoggerFactory::getLogger("powsybl");
    Logger& logger1 = LoggerFactory::getLogger("powsybl::logging");
    Logger& logger2 = LoggerFactory::getLogger("powsybl::logging::ConsoleLogger");
    Logger& logger3 = LoggerFactory::getLogger<ConsoleLogger>();
    ASSERT_TRUE(stdcxx::areSame(expected, logger1));
    ASSERT_TRUE(stdcxx::areSame(expected, logger2));
    ASSERT_TRUE(stdcxx::areSame(expected, logger3));
}

}

}
