/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Country.hpp>

namespace powsybl {

namespace iidm {

TEST(Country, getCountryName) {
    ASSERT_EQ(u8"FRANCE", getCountryName(Country::FR));
    ASSERT_EQ(u8"BELGIUM", getCountryName(Country::BE));
}

}

}
