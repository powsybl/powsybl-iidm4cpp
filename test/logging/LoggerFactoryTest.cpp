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

namespace powsybl {

namespace logging {

TEST(LoggerFactory, test) {
    Logger& logger = LoggerFactory::getLogger("unknown");
    ASSERT_EQ(typeid(NoopLogger), typeid(logger));

    LoggerFactory::getInstance().addLogger("console", stdcxx::make_unique<ConsoleLogger>());
    Logger& consoleLogger = LoggerFactory::getLogger("console");
    ASSERT_EQ(typeid(ConsoleLogger), typeid(consoleLogger));

    LoggerFactory::getInstance().addLogger(typeid(LoggerFactory).name(), stdcxx::make_unique<NoopLogger>());
    Logger& noopLogger1 = LoggerFactory::getLogger(typeid(LoggerFactory));
    ASSERT_EQ(typeid(NoopLogger), typeid(noopLogger1));

    Logger& noopLogger2 = LoggerFactory::getLogger<LoggerFactory>();
    ASSERT_EQ(typeid(NoopLogger), typeid(noopLogger2));
}

}

}
