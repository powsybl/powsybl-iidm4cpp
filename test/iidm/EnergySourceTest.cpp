/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/EnergySource.hpp>

namespace powsybl {

namespace iidm {

TEST(EnergySource, isIntermittent) {
    ASSERT_FALSE(isIntermittent(EnergySource::HYDRO));
    ASSERT_FALSE(isIntermittent(EnergySource::NUCLEAR));
    ASSERT_TRUE(isIntermittent(EnergySource::WIND));
    ASSERT_FALSE(isIntermittent(EnergySource::THERMAL));
    ASSERT_TRUE(isIntermittent(EnergySource::SOLAR));
    ASSERT_FALSE(isIntermittent(EnergySource::OTHER));
}

}  // namespace iidm

}  // namespace powsybl
