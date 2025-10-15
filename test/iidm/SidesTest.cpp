/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/iidm/TwoSides.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(SidesTestSuite)

BOOST_AUTO_TEST_CASE(threeSidesTest) {
    ThreeSides one = ThreeSides(1);
    ThreeSides two = ThreeSides(2);
    ThreeSides three = ThreeSides(3);
    ThreeSides four = ThreeSides(4);
    ThreeSides zero = ThreeSides(0);
    ThreeSides minusone = ThreeSides(-1);

    BOOST_CHECK_EQUAL(0, static_cast<int>(zero));
    BOOST_CHECK_EQUAL(1, static_cast<int>(one));
    BOOST_CHECK_EQUAL(2, static_cast<int>(two));
    BOOST_CHECK_EQUAL(3, static_cast<int>(three));
    BOOST_CHECK_EQUAL(ThreeSides::UNDEFINED, zero);
    BOOST_CHECK_EQUAL(ThreeSides::ONE, one);
    BOOST_CHECK_EQUAL(ThreeSides::TWO, two);
    BOOST_CHECK_EQUAL(ThreeSides::THREE, three);
    BOOST_CHECK_EQUAL("UNDEFINED", Enum::toString(zero));
    BOOST_CHECK_EQUAL("ONE", Enum::toString(one));
    BOOST_CHECK_EQUAL("TWO", Enum::toString(two));
    BOOST_CHECK_EQUAL("THREE", Enum::toString(three));

    POWSYBL_ASSERT_THROW(std::cout << four, AssertionError, "Unexpected ThreeSides value: 4");
    POWSYBL_ASSERT_THROW(std::cout << minusone, AssertionError, "Unexpected ThreeSides value: 255");
}

BOOST_AUTO_TEST_CASE(twoSidesTest) {
    TwoSides one = TwoSides(1);
    TwoSides two = TwoSides(2);
    TwoSides three = TwoSides(3);
    TwoSides zero = TwoSides(0);
    TwoSides minusone = TwoSides(-1);
    BOOST_CHECK_EQUAL(0, static_cast<int>(zero));
    BOOST_CHECK_EQUAL(1, static_cast<int>(one));
    BOOST_CHECK_EQUAL(2, static_cast<int>(two));
    BOOST_CHECK_EQUAL(3, static_cast<int>(three));
    BOOST_CHECK_EQUAL(TwoSides::UNDEFINED, zero);
    BOOST_CHECK_EQUAL(TwoSides::ONE, one);
    BOOST_CHECK_EQUAL(TwoSides::TWO, two);
    BOOST_CHECK_EQUAL("UNDEFINED", Enum::toString(zero));
    BOOST_CHECK_EQUAL("ONE", Enum::toString(one));
    BOOST_CHECK_EQUAL("TWO", Enum::toString(two));

    POWSYBL_ASSERT_THROW(std::cout << three, AssertionError, "Unexpected TwoSides value: 3");
    POWSYBL_ASSERT_THROW(std::cout << minusone, AssertionError, "Unexpected TwoSides value: 255");
}

BOOST_AUTO_TEST_CASE(conversionSidesTest) {

    BOOST_CHECK_EQUAL(ThreeSides::UNDEFINED, static_cast<ThreeSides>(TwoSides::UNDEFINED));
    BOOST_CHECK_EQUAL(ThreeSides::ONE, static_cast<ThreeSides>(TwoSides::ONE));
    BOOST_CHECK_EQUAL(ThreeSides::TWO, static_cast<ThreeSides>(TwoSides::TWO));

    BOOST_CHECK_EQUAL(TwoSides::UNDEFINED, static_cast<TwoSides>(ThreeSides::UNDEFINED));
    BOOST_CHECK_EQUAL(TwoSides::ONE, static_cast<TwoSides>(ThreeSides::ONE));
    BOOST_CHECK_EQUAL(TwoSides::TWO, static_cast<TwoSides>(ThreeSides::TWO));

    ThreeSides threeSidesOne = static_cast<ThreeSides>(TwoSides::ONE);
    BOOST_CHECK_EQUAL(1, static_cast<int>(threeSidesOne));
    BOOST_CHECK_EQUAL(TwoSides::ONE, static_cast<TwoSides>(threeSidesOne));
    BOOST_CHECK_EQUAL(ThreeSides::ONE, threeSidesOne);
    
    TwoSides twoSidesThree = static_cast<TwoSides>(ThreeSides::THREE);
    BOOST_CHECK_EQUAL(3, static_cast<int>(twoSidesThree));
    BOOST_CHECK_EQUAL(ThreeSides::THREE, static_cast<ThreeSides>(twoSidesThree));
    POWSYBL_ASSERT_THROW(std::cout << twoSidesThree, AssertionError, "Unexpected TwoSides value: 3");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
