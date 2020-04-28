/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(MathSuite)

BOOST_AUTO_TEST_CASE(isEqualTest) {
    double a = 0.5;
    double b = 1.5;

    BOOST_CHECK_EQUAL(true, stdcxx::isEqual(a, a));
    BOOST_CHECK_EQUAL(false, stdcxx::isEqual(a, b));
    BOOST_CHECK_EQUAL(false, stdcxx::isEqual(b, a));

    BOOST_CHECK_EQUAL(false, stdcxx::isEqual(stdcxx::nan(), a));
    BOOST_CHECK_EQUAL(false, stdcxx::isEqual(a, stdcxx::nan()));

    BOOST_CHECK_EQUAL(true, stdcxx::isEqual(stdcxx::nan(), stdcxx::nan()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
