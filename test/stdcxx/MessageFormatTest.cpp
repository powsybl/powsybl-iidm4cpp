/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(MessageFormatTestSuite)

BOOST_AUTO_TEST_CASE(formatTest) {
    const std::string& msg1 = format("message");
    BOOST_CHECK_EQUAL("message", msg1);

    const std::string& msg2 = format("message: %1%", "value");
    BOOST_CHECK_EQUAL("message: value", msg2);

    const std::string& msg3 = format("message: %1% %2%", 1, 2);
    BOOST_CHECK_EQUAL("message: 1 2", msg3);

    const std::string& msg4 = format("message: %2% %1%", 1, 2);
    BOOST_CHECK_EQUAL("message: 2 1", msg4);

    BOOST_CHECK_THROW(format("error: %1%"), boost::io::too_few_args);

    BOOST_CHECK_THROW(format("error", 1), boost::io::too_many_args);
}

BOOST_AUTO_TEST_CASE(toStringTest) {
    std::initializer_list<unsigned long> list = {0, 1, 2};
    BOOST_CHECK_EQUAL("[0, 1, 2]", toString(list));

    std::set<unsigned long> set(list);
    BOOST_CHECK_EQUAL("{0, 1, 2}", toString(set));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
