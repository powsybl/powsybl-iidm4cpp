/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/EnergySource.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(EnergySourceTestSuite)

BOOST_AUTO_TEST_CASE(is_intermittent)
{
    BOOST_TEST(!isIntermittent(EnergySource::HYDRO));
    BOOST_TEST(!isIntermittent(EnergySource::NUCLEAR));
    BOOST_TEST(isIntermittent(EnergySource::WIND));
    BOOST_TEST(!isIntermittent(EnergySource::THERMAL));
    BOOST_TEST(isIntermittent(EnergySource::SOLAR));
    BOOST_TEST(!isIntermittent(EnergySource::OTHER));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
