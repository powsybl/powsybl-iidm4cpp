/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/stdcxx/DateTime.hpp>

#include "AssertionUtils.hpp"

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(DateTimeTestSuite)

BOOST_AUTO_TEST_CASE(parse) {
    DateTime dateTime;
    std::cout << dateTime << std::endl;

    std::string strDate = "1977-04-22T01:00:00+05:00";
    dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL(strDate, dateTime.toString());

    std::string strDate2 = "1977-04-21T20:00:00Z";
    DateTime dateTime2 = DateTime::parse(strDate2);

    BOOST_CHECK_EQUAL(dateTime, dateTime2);

    strDate = "2015-03-17T14:10:00.123000+01:30";
    dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL(strDate, dateTime.toString());

    BOOST_TEST(dateTime != dateTime2);

    strDate = "2015-03-17T14:10:00.123+01";
    dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL("2015-03-17T14:10:00.123000+01:00", dateTime.toString());

    strDate = "2015-03-17T14:10:00.123000+00:00";
    dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL(strDate, dateTime.toString());

    strDate2 = "2015-03-17T14:10:00.123000Z";
    dateTime2 = DateTime::parse(strDate2);

    BOOST_CHECK_EQUAL(dateTime, dateTime2);

    strDate = "2015-03-17T14:10:00.123000-00:00";
    dateTime = DateTime::parse(strDate);
    BOOST_CHECK_EQUAL("2015-03-17T14:10:00.123000+00:00", dateTime.toString());

    BOOST_CHECK_EQUAL(dateTime, dateTime2);

    strDate = "INVALID";
    POWSYBL_ASSERT_THROW(DateTime::parse(strDate), powsybl::PowsyblException, "Unable to parse the date 'INVALID'");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
