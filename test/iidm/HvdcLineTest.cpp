/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createHvdcLineTestNetwork() {
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

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl2 = substation2.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    vl2.newLccConverterStation()
        .setId("LCC2")
        .setName("LCC2_NAME")
        .setBus(vl2Bus1.getId())
        .setConnectableBus(vl2Bus1.getId())
        .setLossFactor(6.0)
        .setPowerFactor(7.0)
        .add();

    vl2.newVscConverterStation()
        .setId("VSC2")
        .setName("VSC2_NAME")
        .setBus(vl2Bus1.getId())
        .setConnectableBus(vl2Bus1.getId())
        .setLossFactor(8.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(9.0)
        .setReactivePowerSetpoint(10.0)
        .add();

    network.newHvdcLine()
        .setId("HVDC1")
        .setName("HVDC1_NAME")
        .setActivePowerSetpoint(11.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .setConverterStationId1("LCC1")
        .setConverterStationId2("LCC2")
        .setMaxP(12.0)
        .setNominalVoltage(13.0)
        .setR(14.0)
        .add();

    return network;
}

TEST(HvdcLine, constructor) {
    Network network = createHvdcLineTestNetwork();

    unsigned long hvdcCount = network.getHvdcLineCount();
    ASSERT_EQ(1, hvdcCount);

    HvdcLineAdder adder = network.newHvdcLine()
        .setId("HVDC1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': r is invalid");
    adder.setR(20.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': converter mode is invalid");
    adder.setConvertersMode(static_cast<HvdcLine::ConvertersMode>(5));

    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected converter mode value: 5");
    adder.setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': Nominal voltage is undefined");
    adder.setNominalVoltage(30.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': Active power setpoint is not set");
    adder.setActivePowerSetpoint(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': Maximum active power is not set");
    adder.setMaxP(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Side 1 converter station not set");
    adder.setConverterStationId1("UNKNOWN");

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Side 1 converter station UNKNOWN not found");
    adder.setConverterStationId1("VSC1");

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Side 2 converter station not set");
    adder.setConverterStationId2("UNKNOWN");

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Side 2 converter station UNKNOWN not found");
    adder.setConverterStationId2("VSC2");

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'HVDC1' already exists (powsybl::iidm::HvdcLine)");
    adder.setId("UNIQUE_HVDC_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(hvdcCount + 1, network.getHvdcLineCount());
}

TEST(HvdcLine, integrity) {
    const Network& network = createHvdcLineTestNetwork();

    HvdcLine& hvdc = network.getHvdcLine("HVDC1");
    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(11.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(12.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(13.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(14.0, hvdc.getR());
    ASSERT_TRUE(stdcxx::areSame(network, hvdc.getNetwork()));

    ASSERT_TRUE(stdcxx::areSame(hvdc, hvdc.setActivePowerSetpoint(100.0)));
    ASSERT_DOUBLE_EQ(100.0, hvdc.getActivePowerSetpoint());
    POWSYBL_ASSERT_THROW(hvdc.setActivePowerSetpoint(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Active power setpoint is not set");

    ASSERT_TRUE(stdcxx::areSame(hvdc, hvdc.setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)));
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    POWSYBL_ASSERT_THROW(hvdc.setConvertersMode(static_cast<HvdcLine::ConvertersMode>(7)), AssertionError, "Unexpected converter mode value: 7");

    ASSERT_TRUE(stdcxx::areSame(hvdc, hvdc.setMaxP(200.0)));
    ASSERT_DOUBLE_EQ(200.0, hvdc.getMaxP());
    POWSYBL_ASSERT_THROW(hvdc.setMaxP(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Maximum active power is not set");

    ASSERT_TRUE(stdcxx::areSame(hvdc, hvdc.setNominalVoltage(300.0)));
    ASSERT_DOUBLE_EQ(300.0, hvdc.getNominalVoltage());
    POWSYBL_ASSERT_THROW(hvdc.setNominalVoltage(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Nominal voltage is undefined");

    ASSERT_TRUE(stdcxx::areSame(hvdc, hvdc.setR(400.0)));
    ASSERT_DOUBLE_EQ(400.0, hvdc.getR());
    POWSYBL_ASSERT_THROW(hvdc.setR(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': r is invalid");

    const HvdcLine& cHvdc = network.getHvdcLine("HVDC1");
    ASSERT_TRUE(stdcxx::areSame(cHvdc, hvdc));
    ASSERT_TRUE(stdcxx::areSame(network, cHvdc.getNetwork()));
    ASSERT_EQ("LCC1", cHvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", cHvdc.getConverterStation2().get().getId());

    hvdc.remove();
    POWSYBL_ASSERT_THROW(network.getHvdcLine("HVDC1"), PowsyblException, "Unable to find to the identifiable 'HVDC1'");
}

TEST(HvdcLine, multivariant) {
    Network network = createHvdcLineTestNetwork();

    HvdcLine& hvdc = network.getHvdcLine("HVDC1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(11.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(12.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(13.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(14.0, hvdc.getR());
    hvdc.setActivePowerSetpoint(100.0).setR(200.0).setMaxP(300.0).setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER).setNominalVoltage(400.0);

    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(100.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(300.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(400.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(200.0, hvdc.getR());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(11.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(300.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(400.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(200.0, hvdc.getR());
    hvdc.setActivePowerSetpoint(150.0).setR(250.0).setMaxP(350.0).setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER).setNominalVoltage(450.0);

    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(150.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(350.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(450.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(250.0, hvdc.getR());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(11.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(350.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(450.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(250.0, hvdc.getR());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_EQ("HVDC1", hvdc.getId());
    ASSERT_EQ("HVDC1_NAME", hvdc.getName());
    ASSERT_DOUBLE_EQ(150.0, hvdc.getActivePowerSetpoint());
    ASSERT_EQ(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    ASSERT_EQ("LCC1", hvdc.getConverterStation1().get().getId());
    ASSERT_EQ("LCC2", hvdc.getConverterStation2().get().getId());
    ASSERT_DOUBLE_EQ(350.0, hvdc.getMaxP());
    ASSERT_DOUBLE_EQ(450.0, hvdc.getNominalVoltage());
    ASSERT_DOUBLE_EQ(250.0, hvdc.getR());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

}  // namespace iidm

}  // namespace powsybl
