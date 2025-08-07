/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(CoordinateTestSuite)

BOOST_AUTO_TEST_CASE(CoordinateEquals) {
    Coordinate coord1;
    Coordinate coord2(48.1, 1.5);
    Coordinate coord3(48.06, 1.3);

    BOOST_CHECK(std::isnan(coord1.getLatitude()));
    BOOST_CHECK(std::isnan(coord1.getLongitude()));
    BOOST_CHECK(coord1!=coord1);
    BOOST_CHECK(coord2==coord2);
    BOOST_CHECK(coord3==coord3);
    BOOST_CHECK(coord1!=coord2);
    BOOST_CHECK(coord1!=coord3);
}

BOOST_AUTO_TEST_CASE(CoordinateCopy) {

    Coordinate coord1(48.1, 1.5);
    Coordinate coord2(coord1);
    Coordinate coord3;
    coord3 = coord1;

    BOOST_CHECK(coord1==coord2);
    BOOST_CHECK(coord2==coord3);
}

BOOST_AUTO_TEST_CASE(CoordinateToString) {

    Coordinate coord;
    Coordinate coord1(48.1, 1.5);

    BOOST_CHECK_EQUAL("Coordinate(lat=nan, lon=nan)", coord.toString());
    BOOST_CHECK_EQUAL("Coordinate(lat=48.1, lon=1.5)", coord1.toString());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

