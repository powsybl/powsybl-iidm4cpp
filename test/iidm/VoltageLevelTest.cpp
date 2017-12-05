/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

TEST(VoltageLevel, constructor) {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setId("VL")
        .setName("VLNAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setLowVoltageLimit(200)
        .setHighVoltageLimit(250)
        .setNominalVoltage(225)
        .add();

    ASSERT_EQ(1, network.getVoltageLevelCount());

    ASSERT_EQ("VL", voltageLevel.getId());
    ASSERT_EQ("VLNAME", voltageLevel.getName());
    ASSERT_EQ(TopologyKind::NODE_BREAKER, voltageLevel.getTopologyKind());
    ASSERT_EQ(200, voltageLevel.getLowVoltageLimit());
    ASSERT_EQ(250, voltageLevel.getHighVoltageLimit());
    ASSERT_EQ(225, voltageLevel.getNominalVoltage());

    VoltageLevel& voltageLevel2 = network.getVoltageLevel("VL");
    ASSERT_EQ(&voltageLevel, &voltageLevel2);
}

}

}