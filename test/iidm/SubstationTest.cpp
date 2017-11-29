/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

TEST(Substation, constructor) {
    Network network("id", "test");
    Substation& substation = network.newSubstation()
        .setId("s.id")
        .setName("s.name")
        .setCountry(Country::FR)
        .setTso("s.tso")
        .add();
    ASSERT_EQ(1, network.getSubstationCount());

    ASSERT_EQ("s.id", substation.getId());
    ASSERT_EQ("s.name", substation.getName());
    ASSERT_EQ(Country::FR, substation.getCountry());
    ASSERT_EQ("s.tso", substation.getTso());

    Substation& substation2 = network.getSubstation("s.id");
    ASSERT_EQ(&substation, &substation2);
}

TEST(Substation, country) {
    Network network("id", "test");

    SubstationAdder substationAdder = network.newSubstation();
    substationAdder
        .setId("s.id")
        .setName("s.name")
        .setTso("s.tso");
    ASSERT_THROW(substationAdder.add(), ValidationException);
    ASSERT_EQ(0, network.getSubstationCount());

    substationAdder.setCountry(Country::FR);
    ASSERT_NO_THROW(substationAdder.add());
}

}

}
