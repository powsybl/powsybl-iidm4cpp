/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(BranchTestSuite)

BOOST_AUTO_TEST_CASE(utils) {
    BOOST_CHECK_EQUAL(TwoSides::ONE, Enum::fromString<TwoSides>(Enum::toString(TwoSides::ONE)));
    BOOST_CHECK_EQUAL(TwoSides::TWO, Enum::fromString<TwoSides>(Enum::toString(TwoSides::TWO)));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl

