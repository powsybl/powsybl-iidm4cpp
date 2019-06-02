/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/DateTime.hpp>

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(DateTimeTestSuite)

BOOST_AUTO_TEST_CASE(parse) {
    std::string strDate = "1977-04-22T01:00:00+05:00";
    DateTime dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL(strDate, dateTime.toString());

    std::string strDate2 = "1977-04-21T20:00:00Z";
    DateTime dateTime2 = DateTime::parse(strDate2);

    BOOST_CHECK_EQUAL(dateTime, dateTime2);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
