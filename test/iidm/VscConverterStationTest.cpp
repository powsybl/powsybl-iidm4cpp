/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(VscConverterStationTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createHvdcConverterStationTestNetwork();

    unsigned long vscCount = network.getVscConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    BOOST_CHECK_EQUAL(vscCount + network.getLccConverterStationCount(), hvdcCount);

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    VscConverterStationAdder adder = vl1.newVscConverterStation()
        .setId("VSC1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': loss factor is invalid");
    adder.setLossFactor(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "vscConverterStation 'VSC1': loss factor must be >= 0 and <= 1");
    adder.setLossFactor(0.5);

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

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'VscConverterStation' with the id 'VSC1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(vscCount + 1, network.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcCount + 1, network.getHvdcConverterStationCount());
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createHvdcConverterStationTestNetwork();

    const VscConverterStation& vsc = network.getVscConverterStation("VSC1");
    const HvdcConverterStation& hvdc = network.getHvdcConverterStation("VSC1");
    BOOST_TEST(stdcxx::areSame(vsc, hvdc));
    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL(vsc.getId(), hvdc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    BOOST_CHECK_EQUAL(vsc.getName(), hvdc.getName());
    BOOST_CHECK_EQUAL(ConnectableType::HVDC_CONVERTER_STATION, vsc.getType());
    std::ostringstream oss;
    oss << vsc.getType();
    BOOST_CHECK_EQUAL("HVDC_CONVERTER_STATION", oss.str());
    BOOST_CHECK_EQUAL(hvdc.getType(), vsc.getType());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    POWSYBL_ASSERT_ENUM_EQ(vsc.getHvdcType(), hvdc.getHvdcType());
    BOOST_CHECK_CLOSE(0.5, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(vsc.getLossFactor(), hvdc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(vsc.isVoltageRegulatorOn());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createHvdcConverterStationTestNetwork();

    VscConverterStation& vsc = network.getVscConverterStation("VSC1");
    HvdcConverterStation& hvdc = network.getHvdcConverterStation("VSC1");
    BOOST_TEST(stdcxx::areSame(vsc, hvdc));

    BOOST_TEST(stdcxx::areSame(vsc, vsc.setLossFactor(0.1)));
    BOOST_CHECK_CLOSE(0.1, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(vsc.getLossFactor(), hvdc.getLossFactor(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(vsc.setLossFactor(stdcxx::nan()), ValidationException, "vscConverterStation 'VSC1': loss factor is invalid");
    POWSYBL_ASSERT_THROW(vsc.setLossFactor(-100.0), ValidationException, "vscConverterStation 'VSC1': loss factor must be >= 0 and <= 1");

    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(false)));
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(stdcxx::nan())));
    BOOST_TEST(std::isnan(vsc.getVoltageSetpoint()));
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(-300.0)));
    BOOST_CHECK_CLOSE(-300.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (-300) while voltage regulator is on");
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(0.0)));
    BOOST_CHECK_CLOSE(0.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(true), ValidationException, "vscConverterStation 'VSC1': Invalid voltage setpoint value (0) while voltage regulator is on");
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageSetpoint(300.0)));
    BOOST_CHECK_CLOSE(300.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(true)));
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setReactivePowerSetpoint(stdcxx::nan())));
    BOOST_TEST(std::isnan(vsc.getReactivePowerSetpoint()));
    POWSYBL_ASSERT_THROW(vsc.setVoltageRegulatorOn(false), ValidationException, "vscConverterStation 'VSC1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setReactivePowerSetpoint(400.0)));
    BOOST_CHECK_CLOSE(400.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(vsc, vsc.setVoltageRegulatorOn(false)));

    vsc.remove();
    POWSYBL_ASSERT_THROW(network.getVscConverterStation("VSC1"), PowsyblException, "Unable to find to the identifiable 'VSC1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createHvdcConverterStationTestNetwork();

    VscConverterStation& vsc = network.getVscConverterStation("VSC1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(4.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(vsc.isVoltageRegulatorOn());
    vsc.setLossFactor(0.1).setVoltageSetpoint(200.0).setReactivePowerSetpoint(300.0).setVoltageRegulatorOn(false);

    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(0.1, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!vsc.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(0.1, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(vsc.isVoltageRegulatorOn());
    vsc.setLossFactor(0.3).setVoltageSetpoint(250.0).setReactivePowerSetpoint(350.0).setVoltageRegulatorOn(false);

    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(0.3, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!vsc.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(0.3, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(vsc.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("VSC1", vsc.getId());
    BOOST_CHECK_EQUAL("VSC1_NAME", vsc.getName());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::VSC, vsc.getHvdcType());
    BOOST_CHECK_CLOSE(0.3, vsc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, vsc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, vsc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!vsc.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(loop) {
    Network network = createHvdcConverterStationTestNetwork();

    unsigned long vscCount = network.getVscConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    BOOST_CHECK_EQUAL(vscCount + network.getLccConverterStationCount(), hvdcCount);

    unsigned long vscLoopCount = boost::size(network.getVscConverterStations());
    unsigned long hvdcLoopCount = boost::size(network.getHvdcConverterStations());
    BOOST_CHECK_EQUAL(vscLoopCount, vscCount);
    BOOST_CHECK_EQUAL(hvdcLoopCount, hvdcCount);

    const Network& cNetwork = network;
    BOOST_CHECK_EQUAL(boost::size(network.getVscConverterStations()), boost::size(cNetwork.getVscConverterStations()));
    BOOST_CHECK_EQUAL(boost::size(network.getHvdcConverterStations()), boost::size(cNetwork.getHvdcConverterStations()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
