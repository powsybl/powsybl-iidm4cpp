/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/EnergySource.hpp>
#include <powsybl/iidm/Enum.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(EnergySourceTestSuite)

BOOST_AUTO_TEST_CASE(energySourceName)
{
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::HYDRO, Enum::fromString<EnergySource>(Enum::toString(EnergySource::HYDRO)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::NUCLEAR, Enum::fromString<EnergySource>(Enum::toString(EnergySource::NUCLEAR)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::WIND, Enum::fromString<EnergySource>(Enum::toString(EnergySource::WIND)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::THERMAL, Enum::fromString<EnergySource>(Enum::toString(EnergySource::THERMAL)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::SOLAR, Enum::fromString<EnergySource>(Enum::toString(EnergySource::SOLAR)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::OTHER, Enum::fromString<EnergySource>(Enum::toString(EnergySource::OTHER)));
}

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
