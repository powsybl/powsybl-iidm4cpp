/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createLccConverterStationTestNetwork() {
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

    vl1.newLccConverterStation()
        .setId("LCC1")
        .setName("LCC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(1.0)
        .setPowerFactor(2.0)
        .add();

    vl1.newVscConverterStation()
        .setId("VSC1")
        .setName("VSC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(3.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(4.0)
        .setReactivePowerSetpoint(5.0)
        .add();

    return network;
}

TEST(LccConverterStation, constructor) {
    const Network& network = createLccConverterStationTestNetwork();

    unsigned long lccCount = network.getLccConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    ASSERT_EQ(lccCount + network.getVscConverterStationCount(), hvdcCount);

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    LccConverterStationAdder adder = vl1.newLccConverterStation()
        .setId("LCC1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': loss factor is invalid");
    adder.setLossFactor(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': loss factor must be >= 0");
    adder.setLossFactor(10.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': power factor is invalid");
    adder.setPowerFactor(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'LCC1' already exists (powsybl::iidm::LccConverterStation)");
    adder.setId("UNIQUE_LCC_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(lccCount + 1, network.getLccConverterStationCount());
    ASSERT_EQ(hvdcCount + 1, network.getHvdcConverterStationCount());
}

TEST(LccConverterStation, integrity) {
    const Network& network = createLccConverterStationTestNetwork();

    LccConverterStation& lcc = network.getLccConverterStation("LCC1");
    HvdcConverterStation& hvdc = network.getHvdcConverterStation("LCC1");
    ASSERT_TRUE(stdcxx::areSame(lcc, hvdc));
    ASSERT_EQ("LCC1", lcc.getId());
    ASSERT_EQ(lcc.getId(), hvdc.getId());
    ASSERT_EQ("LCC1_NAME", lcc.getName());
    ASSERT_EQ(lcc.getName(), hvdc.getName());
    ASSERT_EQ(ConnectableType::HVDC_CONVERTER_STATION, lcc.getType());
    ASSERT_EQ(hvdc.getType(), lcc.getType());
    ASSERT_EQ(HvdcConverterStation::HvdcType::LCC, lcc.getHvdcType());
    ASSERT_EQ(lcc.getHvdcType(), hvdc.getHvdcType());
    ASSERT_DOUBLE_EQ(1.0, lcc.getLossFactor());
    ASSERT_DOUBLE_EQ(lcc.getLossFactor(), hvdc.getLossFactor());
    ASSERT_DOUBLE_EQ(2.0, lcc.getPowerFactor());

    ASSERT_TRUE(stdcxx::areSame(lcc, lcc.setLossFactor(100.0)));
    ASSERT_DOUBLE_EQ(100.0, lcc.getLossFactor());
    ASSERT_DOUBLE_EQ(lcc.getLossFactor(), hvdc.getLossFactor());
    POWSYBL_ASSERT_THROW(lcc.setLossFactor(stdcxx::nan()), ValidationException, "lccConverterStation 'LCC1': loss factor is invalid");
    POWSYBL_ASSERT_THROW(lcc.setLossFactor(-100.0), ValidationException, "lccConverterStation 'LCC1': loss factor must be >= 0");

    ASSERT_TRUE(stdcxx::areSame(lcc, lcc.setPowerFactor(300.0)));
    ASSERT_TRUE(stdcxx::areSame(hvdc, lcc.setPowerFactor(400.0)));
    ASSERT_DOUBLE_EQ(400.0, lcc.getPowerFactor());
    POWSYBL_ASSERT_THROW(lcc.setPowerFactor(stdcxx::nan()), ValidationException, "lccConverterStation 'LCC1': power factor is invalid");

    lcc.remove();
    POWSYBL_ASSERT_THROW(network.getLccConverterStation("LCC1"), PowsyblException, "Unable to find to the identifiable 'LCC1'");
}

TEST(LccConverterStation, loop) {
    const Network& network = createLccConverterStationTestNetwork();

    unsigned long lccCount = network.getLccConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    ASSERT_EQ(lccCount + network.getVscConverterStationCount(), hvdcCount);

    unsigned long lccLoopCount = 0ul;
    unsigned long hvdcLoopCount = 0ul;
    for(auto it = network.cbegin<LccConverterStation>(); it != network.cend<LccConverterStation>(); ++it) {
        lccLoopCount++;
    }
    for(auto it = network.cbegin<HvdcConverterStation>(); it != network.cend<HvdcConverterStation>(); ++it) {
        hvdcLoopCount++;
    }
    ASSERT_EQ(lccLoopCount, lccCount);
    ASSERT_EQ(hvdcLoopCount, hvdcCount);
}

}  // namespace iidm

}  // namespace powsybl
