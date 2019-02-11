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
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

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
        .setId("LOAD1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': p0 is invalid");
    adder.setP0(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': q0 is invalid");
    adder.setQ0(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'LOAD1' already exists (powsybl::iidm::Load)");
    adder.setId("UNIQUE_LOAD_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(loadCount + 1, network.getLoadCount());
}

TEST(Load, integrity) {
    const Network& network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    ASSERT_TRUE(stdcxx::areSame(load1, load1.setP0(100)));
    ASSERT_DOUBLE_EQ(100, load1.getP0());
    POWSYBL_ASSERT_THROW(load1.setP0(stdcxx::nan()), ValidationException, "Load 'LOAD1': p0 is invalid");

    ASSERT_TRUE(stdcxx::areSame(load1, load1.setQ0(100)));
    ASSERT_DOUBLE_EQ(100, load1.getQ0());
    POWSYBL_ASSERT_THROW(load1.setQ0(stdcxx::nan()), ValidationException, "Load 'LOAD1': q0 is invalid");

    load1.remove();
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD1"), PowsyblException, "Unable to find to the identifiable 'LOAD1'");
}

TEST(Load, multivariant) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());
    load1.setP0(100);
    load1.setQ0(80);
    ASSERT_DOUBLE_EQ(100, load1.getP0());
    ASSERT_DOUBLE_EQ(80, load1.getQ0());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());
    load1.setP0(150);
    load1.setQ0(120);
    ASSERT_DOUBLE_EQ(150, load1.getP0());
    ASSERT_DOUBLE_EQ(120, load1.getQ0());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_DOUBLE_EQ(50, load1.getP0());
    ASSERT_DOUBLE_EQ(40, load1.getQ0());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_DOUBLE_EQ(150, load1.getP0());
    ASSERT_DOUBLE_EQ(120, load1.getQ0());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

}  // namespace iidm

}  // namespace powsybl
