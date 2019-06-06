/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <iostream>

#include <powsybl/stdcxx/demangle.hpp>

namespace stdcxx {

struct Dummy {};

BOOST_AUTO_TEST_SUITE(DemangleSuite)

BOOST_AUTO_TEST_CASE(demangleTest) {
    std::string expected("stdcxx::Dummy");

    BOOST_CHECK_EQUAL(expected, demangle<Dummy>());

    Dummy dummy;
    BOOST_CHECK_EQUAL(expected, demangle(dummy));
}

BOOST_AUTO_TEST_CASE(simpleClassNameTest) {
    std::string expected("Dummy");

    BOOST_CHECK_EQUAL(expected, simpleClassName<Dummy>());

    Dummy dummy;
    BOOST_CHECK_EQUAL(expected, simpleClassName(dummy));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
