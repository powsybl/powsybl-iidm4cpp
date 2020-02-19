/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

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

BOOST_AUTO_TEST_SUITE(HvdcLineTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createHvdcLineTestNetwork();

    unsigned long hvdcCount = network.getHvdcLineCount();
    BOOST_CHECK_EQUAL(1UL, hvdcCount);
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getHvdcLines()));

    HvdcLineAdder adder = network.newHvdcLine()
        .setId("HVDC1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': r is invalid");
    adder.setR(20.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "hvdcLine 'HVDC1': converter mode is invalid");
    adder.setConvertersMode(static_cast<HvdcLine::ConvertersMode>(5));

    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected ConvertersMode value: 5");
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

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(hvdcCount + 1, network.getHvdcLineCount());
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createHvdcLineTestNetwork();
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getHvdcLines()));

    const HvdcLine& hvdc = network.getHvdcLine("HVDC1");
    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(11.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(12.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(13.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(14.0, hvdc.getR(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(network, hvdc.getNetwork()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createHvdcLineTestNetwork();

    HvdcLine& hvdc = network.getHvdcLine("HVDC1");
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_TEST(stdcxx::areSame(network, hvdc.getNetwork()));

    BOOST_TEST(stdcxx::areSame(hvdc, hvdc.setActivePowerSetpoint(100.0)));
    BOOST_CHECK_CLOSE(100.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(hvdc.setActivePowerSetpoint(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Active power setpoint is not set");

    BOOST_TEST(stdcxx::areSame(hvdc, hvdc.setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)));
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    POWSYBL_ASSERT_THROW(hvdc.setConvertersMode(static_cast<HvdcLine::ConvertersMode>(7)), AssertionError, "Unexpected ConvertersMode value: 7");

    BOOST_TEST(stdcxx::areSame(hvdc, hvdc.setMaxP(200.0)));
    BOOST_CHECK_CLOSE(200.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(hvdc.setMaxP(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Maximum active power is not set");

    BOOST_TEST(stdcxx::areSame(hvdc, hvdc.setNominalVoltage(300.0)));
    BOOST_CHECK_CLOSE(300.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(hvdc.setNominalVoltage(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': Nominal voltage is undefined");

    BOOST_TEST(stdcxx::areSame(hvdc, hvdc.setR(400.0)));
    BOOST_CHECK_CLOSE(400.0, hvdc.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(hvdc.setR(stdcxx::nan()), ValidationException, "hvdcLine 'HVDC1': r is invalid");

    hvdc.remove();
    POWSYBL_ASSERT_THROW(network.getHvdcLine("HVDC1"), PowsyblException, "Unable to find to the identifiable 'HVDC1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createHvdcLineTestNetwork();

    HvdcLine& hvdc = network.getHvdcLine("HVDC1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(11.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(12.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(13.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(14.0, hvdc.getR(), std::numeric_limits<double>::epsilon());
    hvdc.setActivePowerSetpoint(100.0).setR(200.0).setMaxP(300.0).setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER).setNominalVoltage(400.0);

    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(100.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(300.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, hvdc.getR(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(11.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(300.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, hvdc.getR(), std::numeric_limits<double>::epsilon());
    hvdc.setActivePowerSetpoint(150.0).setR(250.0).setMaxP(350.0).setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER).setNominalVoltage(450.0);

    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(150.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(350.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, hvdc.getR(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(11.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(350.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, hvdc.getR(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("HVDC1", hvdc.getId());
    BOOST_CHECK_EQUAL("HVDC1_NAME", hvdc.getName());
    BOOST_CHECK_CLOSE(150.0, hvdc.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, hvdc.getConvertersMode());
    BOOST_CHECK_EQUAL("LCC1", hvdc.getConverterStation1().get().getId());
    BOOST_CHECK_EQUAL("LCC2", hvdc.getConverterStation2().get().getId());
    BOOST_CHECK_CLOSE(350.0, hvdc.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, hvdc.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, hvdc.getR(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1ul, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
