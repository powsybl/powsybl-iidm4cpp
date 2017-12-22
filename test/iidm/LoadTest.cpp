/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(Load, constructor) {
    const Network& network = createNetwork();
    unsigned long loadCount = network.getLoadCount();

    Load& load = network.getLoad("LOAD1");
    ASSERT_EQ("LOAD1", load.getId());
    ASSERT_EQ("LOAD1_NAME", load.getName());
    ASSERT_EQ(LoadType::UNDEFINED, load.getLoadType());
    ASSERT_DOUBLE_EQ(50.0, load.getP0());
    ASSERT_DOUBLE_EQ(40.0, load.getQ0());

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    LoadAdder adder = vl1.newLoad()
        .setId("LOAD1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': LoadType is not set");
    adder.setLoadType(LoadType::UNDEFINED);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': p0 is not set");
    adder.setP0(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': q0 is not set");
    adder.setQ0(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'LOAD1' already exists (Load)");
    adder.setId("UNIQUE_LOAD_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(loadCount + 1, network.getLoadCount());
}

TEST(Load, integrity) {
    const Network& network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    ASSERT_NO_THROW(load1.setP0(100));
    ASSERT_DOUBLE_EQ(100, load1.getP0());
    if (std::numeric_limits<double>::has_quiet_NaN) {
        POWSYBL_ASSERT_THROW(load1.setP0(std::numeric_limits<double>::quiet_NaN()), ValidationException, "Load 'LOAD1': p0 is invalid");
    }
    if (std::numeric_limits<double>::has_signaling_NaN) {
        POWSYBL_ASSERT_THROW(load1.setP0(std::numeric_limits<double>::signaling_NaN()), ValidationException, "Load 'LOAD1': p0 is invalid");
    }

    ASSERT_NO_THROW(load1.setQ0(100));
    ASSERT_DOUBLE_EQ(100, load1.getQ0());
    if (std::numeric_limits<double>::has_quiet_NaN) {
        POWSYBL_ASSERT_THROW(load1.setQ0(std::numeric_limits<double>::quiet_NaN()), ValidationException, "Load 'LOAD1': q0 is invalid");
    }
    if (std::numeric_limits<double>::has_signaling_NaN) {
        POWSYBL_ASSERT_THROW(load1.setQ0(std::numeric_limits<double>::signaling_NaN()), ValidationException, "Load 'LOAD1': q0 is invalid");
    }
}

TEST(Load, multistate) {
    const Network& network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    network.getStateManager().cloneState(StateManager::getInitialStateId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getStateManager().getStateArraySize());

    network.getStateManager().setWorkingState("s1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());
    load1.setP0(100);
    load1.setQ0(80);
    ASSERT_DOUBLE_EQ(100, load1.getP0());
    ASSERT_DOUBLE_EQ(80, load1.getQ0());

    network.getStateManager().setWorkingState("s2");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());
    load1.setP0(150);
    load1.setQ0(120);
    ASSERT_DOUBLE_EQ(150, load1.getP0());
    ASSERT_DOUBLE_EQ(120, load1.getQ0());

    network.getStateManager().setWorkingState(StateManager::getInitialStateId());
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    network.getStateManager().removeState("s1");
    ASSERT_EQ(3ul, network.getStateManager().getStateArraySize());

    network.getStateManager().cloneState("s2", "s3");
    network.getStateManager().setWorkingState("s3");
    ASSERT_DOUBLE_EQ(150, load1.getP0());
    ASSERT_DOUBLE_EQ(120, load1.getQ0());

    network.getStateManager().removeState("s3");
    ASSERT_EQ(3ul, network.getStateManager().getStateArraySize());

    network.getStateManager().removeState("s2");
    ASSERT_EQ(1ul, network.getStateManager().getStateArraySize());
}

}

}
