/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(Substation, constructor) {
    const Network& network = createNetwork();

    ASSERT_EQ(1, network.getSubstationCount());

    Substation& substation = network.getSubstation("S1");
    ASSERT_EQ("S1", substation.getId());
    ASSERT_EQ("S1_NAME", substation.getName());
    ASSERT_EQ(Country::FR, substation.getCountry());
    ASSERT_EQ("TSO", substation.getTso());
}

TEST(Substation, country) {
    Network network("id", "test");

    SubstationAdder substationAdder = network.newSubstation();
    substationAdder
        .setId("S");
    POWSYBL_ASSERT_THROW(substationAdder.add(), ValidationException, "Substation 'S': Country is not set");
    ASSERT_EQ(0, network.getSubstationCount());

    substationAdder.setCountry(Country::FR);
    ASSERT_NO_THROW(substationAdder.add());
    ASSERT_EQ(1, network.getSubstationCount());
}

}

}
