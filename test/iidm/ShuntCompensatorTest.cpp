/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createShuntCompensatorTestNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newShuntCompensator()
        .setId("SHUNT1")
        .setName("SHUNT1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setbPerSection(12.0)
        .setCurrentSectionCount(2ul)
        .setMaximumSectionCount(3ul)
        .add();

    return network;
}

TEST(ShuntCompensator, constructor) {
    const Network& network = createShuntCompensatorTestNetwork();

    unsigned long shuntCompensatorCount = network.getShuntCompensatorCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    ShuntCompensatorAdder adder = vl1.newShuntCompensator()
        .setId("SHUNT1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is invalid");
    adder.setbPerSection(0.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is equal to zero");
    adder.setbPerSection(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");
    adder.setMaximumSectionCount(0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");
    adder.setMaximumSectionCount(10);
    adder.setCurrentSectionCount(20);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the current number (20) of section should be lesser than the maximum number of section (10)");
    adder.setCurrentSectionCount(5);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'SHUNT1' already exists (powsybl::iidm::ShuntCompensator)");
    adder.setId("UNIQUE_SHUNT_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(shuntCompensatorCount + 1, network.getShuntCompensatorCount());
}

TEST(ShuntCompensator, integrity) {
    const Network& network = createShuntCompensatorTestNetwork();

    ShuntCompensator& shunt = network.getShuntCompensator("SHUNT1");
    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(12.0, shunt.getbPerSection());
    ASSERT_EQ(2ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(3ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(24.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(36.0, shunt.getMaximumB());

    ASSERT_TRUE(stdcxx::areSame(shunt, shunt.setbPerSection(100.0)));
    ASSERT_DOUBLE_EQ(100.0, shunt.getbPerSection());
    ASSERT_DOUBLE_EQ(200.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(300.0, shunt.getMaximumB());
    POWSYBL_ASSERT_THROW(shunt.setbPerSection(stdcxx::nan()), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is invalid");
    POWSYBL_ASSERT_THROW(shunt.setbPerSection(0.0), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is equal to zero");

    ASSERT_TRUE(stdcxx::areSame(shunt, shunt.setMaximumSectionCount(400ul)));
    ASSERT_EQ(400ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(200.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(40000.0, shunt.getMaximumB());
    POWSYBL_ASSERT_THROW(shunt.setMaximumSectionCount(0ul), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");

    ASSERT_TRUE(stdcxx::areSame(shunt, shunt.setCurrentSectionCount(350ul)));
    ASSERT_EQ(350ul, shunt.getCurrentSectionCount());
    ASSERT_DOUBLE_EQ(35000.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(40000.0, shunt.getMaximumB());
    POWSYBL_ASSERT_THROW(shunt.setCurrentSectionCount(500ul), ValidationException, "Shunt compensator 'SHUNT1': the current number (500) of section should be lesser than the maximum number of section (400)");
    POWSYBL_ASSERT_THROW(shunt.setMaximumSectionCount(250ul), ValidationException, "Shunt compensator 'SHUNT1': the current number (350) of section should be lesser than the maximum number of section (250)");

    POWSYBL_ASSERT_THROW(shunt.getTerminal().setP(1.0), ValidationException, "Shunt compensator 'SHUNT1': cannot set active power on a shunt compensator");

    shunt.remove();
    POWSYBL_ASSERT_THROW(network.getShuntCompensator("SHUNT1"), PowsyblException, "Unable to find to the identifiable 'SHUNT1'");
}

TEST(ShuntCompensator, multivariant) {
    Network network = createShuntCompensatorTestNetwork();

    ShuntCompensator& shunt = network.getShuntCompensator("SHUNT1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_EQ(ConnectableType::SHUNT_COMPENSATOR, shunt.getType());
    ASSERT_DOUBLE_EQ(12.0, shunt.getbPerSection());
    ASSERT_EQ(2ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(3ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(24.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(36.0, shunt.getMaximumB());
    shunt.setbPerSection(100.0).setMaximumSectionCount(300ul).setCurrentSectionCount(200ul);

    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(100.0, shunt.getbPerSection());
    ASSERT_EQ(200ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(300ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(20000.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(30000.0, shunt.getMaximumB());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(100.0, shunt.getbPerSection());
    ASSERT_EQ(2ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(300ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(200.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(30000.0, shunt.getMaximumB());
    shunt.setbPerSection(150.0).setMaximumSectionCount(350ul).setCurrentSectionCount(250ul);

    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(150.0, shunt.getbPerSection());
    ASSERT_EQ(250ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(350ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(37500.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(52500.0, shunt.getMaximumB());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(150.0, shunt.getbPerSection());
    ASSERT_EQ(2ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(350ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(300.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(52500.0, shunt.getMaximumB());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_EQ("SHUNT1", shunt.getId());
    ASSERT_EQ("SHUNT1_NAME", shunt.getName());
    ASSERT_DOUBLE_EQ(150.0, shunt.getbPerSection());
    ASSERT_EQ(250ul, shunt.getCurrentSectionCount());
    ASSERT_EQ(350ul, shunt.getMaximumSectionCount());
    ASSERT_DOUBLE_EQ(37500.0, shunt.getCurrentB());
    ASSERT_DOUBLE_EQ(52500.0, shunt.getMaximumB());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

}  // namespace iidm

}  // namespace powsybl
