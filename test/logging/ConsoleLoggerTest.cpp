/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/logging/ConsoleLogger.hpp>

namespace powsybl {

namespace logging {

TEST(ConsoleLogger, level) {
    ConsoleLogger logger;
    ASSERT_TRUE(logger.isTraceEnabled());
    ASSERT_TRUE(logger.isDebugEnabled());
    ASSERT_TRUE(logger.isInfoEnabled());
    ASSERT_TRUE(logger.isWarnEnabled());
    ASSERT_TRUE(logger.isErrorEnabled());
}

}

}
