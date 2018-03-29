/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(VoltageLevel, constructor) {
    const Network& network = createNetwork();
    unsigned long voltageLevelCount = network.getVoltageLevelCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    ASSERT_EQ("VL1", vl1.getId());
    ASSERT_EQ("VL1_NAME", vl1.getName());
    ASSERT_EQ(TopologyKind::BUS_BREAKER, vl1.getTopologyKind());
    ASSERT_DOUBLE_EQ(340, vl1.getLowVoltageLimit());
    ASSERT_DOUBLE_EQ(420, vl1.getHighVoltageLimit());
    ASSERT_DOUBLE_EQ(380, vl1.getNominalVoltage());

    Substation& s1 = network.getSubstation("S1");
    VoltageLevelAdder adder = s1.newVoltageLevel()
        .setId("VL1");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': TopologyKind is not set");
    adder.setTopologyKind(TopologyKind::BUS_BREAKER);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Nominal voltage is not set");
    adder.setNominalVoltage(50);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Low voltage limit is not set");
    adder.setLowVoltageLimit(100);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': High voltage limit is not set");
    adder.setHighVoltageLimit(0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [100, 0]");
    adder
        .setLowVoltageLimit(0)
        .setHighVoltageLimit(0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'VL1' already exists (powsybl::iidm::BusBreakerVoltageLevel)");
    adder.setId("UNIQUE_VOLTAGE_LEVEL_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(voltageLevelCount + 1, network.getVoltageLevelCount());
}

TEST(VoltageLevel, integrity) {
    const Network& network = createNetwork();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    ASSERT_EQ(380, vl1.getNominalVoltage());
    ASSERT_EQ(340, vl1.getLowVoltageLimit());
    ASSERT_EQ(420, vl1.getHighVoltageLimit());

    POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(-10), ValidationException, "Voltage level 'VL1': Nominal voltage is <= 0");
    POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(0), ValidationException, "Voltage level 'VL1': Nominal voltage is <= 0");
    if (std::numeric_limits<double>::has_quiet_NaN) {
        POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(std::numeric_limits<double>::quiet_NaN()), ValidationException, "Voltage level 'VL1': Nominal voltage is undefined");
    }
    if (std::numeric_limits<double>::has_signaling_NaN) {
        POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(std::numeric_limits<double>::signaling_NaN()), ValidationException, "Voltage level 'VL1': Nominal voltage is undefined");
    }
    ASSERT_TRUE(stdcxx::areSame(vl1, vl1.setNominalVoltage(100)));
    ASSERT_EQ(100, vl1.getNominalVoltage());

    POWSYBL_ASSERT_THROW(vl1.setLowVoltageLimit(-10), ValidationException, "Voltage level 'VL1': Low voltage limit is < 0");
    POWSYBL_ASSERT_THROW(vl1.setLowVoltageLimit(440), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [440, 420]");
    ASSERT_TRUE(stdcxx::areSame(vl1, vl1.setLowVoltageLimit(360)));
    ASSERT_EQ(360, vl1.getLowVoltageLimit());

    POWSYBL_ASSERT_THROW(vl1.setHighVoltageLimit(-10), ValidationException, "Voltage level 'VL1': High voltage limit is < 0");
    POWSYBL_ASSERT_THROW(vl1.setHighVoltageLimit(320), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [360, 320]");
    ASSERT_TRUE(stdcxx::areSame(vl1, vl1.setHighVoltageLimit(440)));
    ASSERT_EQ(440, vl1.getHighVoltageLimit());
}

}

}