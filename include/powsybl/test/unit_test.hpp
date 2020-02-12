/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_BOOST_UNIT_TEST_HPP
#define POWSYBL_TEST_BOOST_UNIT_TEST_HPP

#include <boost/version.hpp>

#if BOOST_VERSION < 105900
#   define BOOST_TEST BOOST_CHECK
#endif

#if BOOST_VERSION < 106500
#   define BOOST_TEST_GLOBAL_FIXTURE BOOST_GLOBAL_FIXTURE
#endif

#define POWSYBL_TEST_GLOBAL_FIXTURE( T, F ) \
static boost::unit_test::ut_detail::global_fixture_impl<T> BOOST_JOIN( gf_, F )


#endif  // POWSYBL_TEST_BOOST_UNIT_TEST_HPP
