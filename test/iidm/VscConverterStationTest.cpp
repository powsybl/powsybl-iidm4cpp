/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

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

Network createVscConverterStationTestNetwork() {
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

TEST(VscConverterStation, constructor) {
    const Network& network = createVscConverterStationTestNetwork();

    unsigned long vscCount = network.getVscConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    ASSERT_EQ(vscCount + network.getLccConverterStationCount(), hvdcCount);

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    VscConverterStationAdder adder = vl1.newVscConverterStation()
        .setId("VSC1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': loss factor is invalid");
    adder.setLossFactor(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': loss factor must be >= 0");
    adder.setLossFactor(10.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': voltage regulator status is not set");
    adder.setVoltageRegulatorOn(true);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    adder.setVoltageSetpoint(-20.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (-20) while voltage regulator is on");
    adder.setVoltageSetpoint(0.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (0) while voltage regulator is on");
    adder.setVoltageSetpoint(20.0);
    adder.setVoltageRegulatorOn(false);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    adder.setReactivePowerSetpoint(30.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'VSC1' already exists (powsybl::iidm::VscConverterStation)");
    adder.setId("UNIQUE_VSC_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(vscCount + 1, network.getVscConverterStationCount());
    ASSERT_EQ(hvdcCount + 1, network.getHvdcConverterStationCount());
}

TEST(VscConverterStation, integrity) {
    const Network& network = createVscConverterStationTestNetwork();

    VscConverterStation& vsc = network.getVscConverterStation("VSC1");
    HvdcConverterStation& hvdc = network.getHvdcConverterStation("VSC1");
    ASSERT_TRUE(stdcxx::areSame(vsc, hvdc));
    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ(vsc.getId(), hvdc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(vsc.getName(), hvdc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_EQ(vsc.getHvdcType(), hvdc.getHvdcType());
    ASSERT_DOUBLE_EQ(3.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(vsc.getLossFactor(), hvdc.getLossFactor());
    ASSERT_DOUBLE_EQ(4.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(5.0, vsc.getReactivePowerSetpoint());
    ASSERT_TRUE(vsc.isVoltageRegulatorOn());

    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setLossFactor(100.0)));
    ASSERT_DOUBLE_EQ(100.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(vsc.getLossFactor(), hvdc.getLossFactor());
    POWSYBL_ASSERT_THROW(vsc.setLossFactor(stdcxx::nan()), ValidationException, "vscConverterStation 'VSC1': loss factor is invalid");
    POWSYBL_ASSERT_THROW(vsc.setLossFactor(-100.0), ValidationException, "vscConverterStation 'VSC1': loss factor must be >= 0");

    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(false)));
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(vsc.getVoltageSetpoint()));
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(-300.0)));
    ASSERT_DOUBLE_EQ(-300.0, vsc.getVoltageSetpoint());
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (-300) while voltage regulator is on");
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(0.0)));
    ASSERT_DOUBLE_EQ(0.0, vsc.getVoltageSetpoint());
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (0) while voltage regulator is on");
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(300.0)));
    ASSERT_DOUBLE_EQ(300.0, vsc.getVoltageSetpoint());

    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(true)));
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setReactivePowerSetpoint(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(vsc.getReactivePowerSetpoint()));
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(false), ValidationException, "vscConverterStation 'VSC1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setReactivePowerSetpoint(400.0)));
    ASSERT_DOUBLE_EQ(400.0, vsc.getReactivePowerSetpoint());
    ASSERT_TRUE(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(false)));

    vsc.remove();
    POWSYBL_ASSERT_THROW(network.getVscConverterStation("VSC1"), PowsyblException, "Unable to find to the identifiable 'VSC1'");
}

TEST(VscConverterStation, multivariant) {
    Network network = createVscConverterStationTestNetwork();

    VscConverterStation& vsc = network.getVscConverterStation("VSC1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(3.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(4.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(5.0, vsc.getReactivePowerSetpoint());
    ASSERT_TRUE(vsc.isVoltageRegulatorOn());
    vsc.setLossFactor(100.0).setVoltageSetpoint(200.0).setReactivePowerSetpoint(300.0).setVoltageRegulatorOn(false);

    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(100.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(200.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(300.0, vsc.getReactivePowerSetpoint());
    ASSERT_FALSE(vsc.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(100.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(4.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(5.0, vsc.getReactivePowerSetpoint());
    ASSERT_TRUE(vsc.isVoltageRegulatorOn());
    vsc.setLossFactor(150.0).setVoltageSetpoint(250.0).setReactivePowerSetpoint(350.0).setVoltageRegulatorOn(false);

    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(150.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(250.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(350.0, vsc.getReactivePowerSetpoint());
    ASSERT_FALSE(vsc.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(150.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(4.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(5.0, vsc.getReactivePowerSetpoint());
    ASSERT_TRUE(vsc.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_EQ("VSC1", vsc.getId());
    ASSERT_EQ("VSC1_NAME", vsc.getName());
    ASSERT_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    ASSERT_DOUBLE_EQ(150.0, vsc.getLossFactor());
    ASSERT_DOUBLE_EQ(250.0, vsc.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(350.0, vsc.getReactivePowerSetpoint());
    ASSERT_FALSE(vsc.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

TEST(VscConverterStation, loop) {
    const Network& network = createVscConverterStationTestNetwork();

    unsigned long vscCount = network.getVscConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    ASSERT_EQ(vscCount + network.getLccConverterStationCount(), hvdcCount);

    unsigned long vscLoopCount = 0ul;
    unsigned long hvdcLoopCount = 0ul;
    for(auto it = network.cbegin<VscConverterStation>(); it != network.cend<VscConverterStation>(); ++it) {
        vscLoopCount++;
    }
    for(auto it = network.cbegin<HvdcConverterStation>(); it != network.cend<HvdcConverterStation>(); ++it) {
        hvdcLoopCount++;
    }
    ASSERT_EQ(vscLoopCount, vscCount);
    ASSERT_EQ(hvdcLoopCount, hvdcCount);
}

}  // namespace iidm

}  // namespace powsybl
