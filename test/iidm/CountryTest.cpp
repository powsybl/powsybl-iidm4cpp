/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Country.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(CountryTestSuite)

BOOST_AUTO_TEST_CASE(getCountryCodeTest) {
    BOOST_CHECK_EQUAL(u8"FR", getCountryCode(Country::FR));
    BOOST_CHECK_EQUAL(u8"BE", getCountryCode(Country::BE));
}

BOOST_AUTO_TEST_CASE(getCountryFromCodeTest) {
    POWSYBL_ASSERT_ENUM_EQ(Country::FR, getCountryFromCode(u8"FR"));
    POWSYBL_ASSERT_ENUM_EQ(Country::BE, getCountryFromCode(u8"BE"));
    POWSYBL_ASSERT_THROW(getCountryFromCode(u8"INVALID"), PowsyblException, "Unable to retrieve country from ISO code 'INVALID'");
}

BOOST_AUTO_TEST_CASE(getCountryNameTest) {
    BOOST_CHECK_EQUAL(u8"FRANCE", getCountryName(Country::FR));
    BOOST_CHECK_EQUAL(u8"BELGIUM", getCountryName(Country::BE));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
