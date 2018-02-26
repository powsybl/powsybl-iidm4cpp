/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/logging/NoopLogger.hpp>

namespace powsybl {

namespace logging {

TEST(NoopLogger, level) {
    NoopLogger logger;
    ASSERT_FALSE(logger.isTraceEnabled());
    ASSERT_FALSE(logger.isDebugEnabled());
    ASSERT_FALSE(logger.isInfoEnabled());
    ASSERT_FALSE(logger.isWarnEnabled());
    ASSERT_FALSE(logger.isErrorEnabled());
}

}

}

