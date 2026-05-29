/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/TerminalNumber.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TerminalNumberTestSuite)

BOOST_AUTO_TEST_CASE(conversionTerminalNumberTest) {

    TerminalNumber one = TerminalNumber(1);
    TerminalNumber two = TerminalNumber(2);
    TerminalNumber zero = TerminalNumber(0);

    TerminalNumber three = TerminalNumber(3);
    TerminalNumber minusOne = TerminalNumber(-1);

    BOOST_CHECK_EQUAL(0, static_cast<int>(zero));
    BOOST_CHECK_EQUAL(1, static_cast<int>(one));
    BOOST_CHECK_EQUAL(2, static_cast<int>(two));
    BOOST_CHECK_EQUAL(3, static_cast<int>(three));
    BOOST_CHECK_EQUAL(255, static_cast<int>(minusOne));

    BOOST_CHECK_EQUAL(TerminalNumber::UNDEFINED, zero);
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, one);
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, two);

    BOOST_CHECK_NE(TerminalNumber::UNDEFINED, three);
    BOOST_CHECK_NE(TerminalNumber::ONE, three);
    BOOST_CHECK_NE(TerminalNumber::TWO, three);
    BOOST_CHECK_NE(TerminalNumber::UNDEFINED, minusOne);
    BOOST_CHECK_NE(TerminalNumber::ONE, minusOne);
    BOOST_CHECK_NE(TerminalNumber::TWO, minusOne);

    BOOST_CHECK_EQUAL("UNDEFINED", Enum::toString(zero));
    BOOST_CHECK_EQUAL("ONE", Enum::toString(one));
    BOOST_CHECK_EQUAL("TWO", Enum::toString(two));

    POWSYBL_ASSERT_THROW(std::cout << three, AssertionError, "Unexpected TerminalNumber value: 3");
    POWSYBL_ASSERT_THROW(std::cout << minusOne, AssertionError, "Unexpected TerminalNumber value: 255");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
