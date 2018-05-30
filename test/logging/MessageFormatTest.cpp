/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

TEST(MessageFormat, format) {
    const std::string& msg1 = format("message");
    ASSERT_EQ("message", msg1);

    const std::string& msg2 = format("message: %1%", "value");
    ASSERT_EQ("message: value", msg2);

    const std::string& msg3 = format("message: %1% %2%", 1, 2);
    ASSERT_EQ("message: 1 2", msg3);

    const std::string& msg4 = format("message: %2% %1%", 1, 2);
    ASSERT_EQ("message: 2 1", msg4);

    ASSERT_THROW(format("error: %1%"), boost::io::too_few_args);

    ASSERT_THROW(format("error", 1), boost::io::too_many_args);
}

TEST(MessageFormat, toString) {
    std::initializer_list<unsigned long> list = {0, 1, 2};
    ASSERT_EQ("[0, 1, 2]", toString(list));

    std::set<unsigned long> set(list);
    ASSERT_EQ("{0, 1, 2}", toString(set));
}

}  // namespace logging

}  // namespace powsybl
