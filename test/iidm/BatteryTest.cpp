/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createBatteryTestNetwork() {
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

    vl1.newBattery()
        .setId("BAT1")
        .setName("BAT1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setP0(100.0)
        .setQ0(200.0)
        .setMinP(-200.0)
        .setMaxP(300.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(BatteryTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createBatteryTestNetwork();
    unsigned long batteryCount = network.getBatteryCount();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    BatteryAdder adder = vl1.newBattery()
        .setId("BAT1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': p0 is invalid");
    adder.setP0(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': q0 is invalid");
    adder.setQ0(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': Minimum active power is not set");
    adder.setMinP(60.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': Maximum active power is not set");
    adder.setMaxP(55.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': Invalid active limits [60, 55]");
    adder.setMaxP(70.0);

    adder.setMinP(80.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Battery 'BAT1': Invalid active limits [80, 70]");
    adder.setMinP(60.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'BAT1' already exists (powsybl::iidm::Battery)");
    adder.setId("UNIQUE_BAT_ID");

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(batteryCount + 1, network.getBatteryCount());
}

BOOST_AUTO_TEST_CASE(integrity) {
    const Network& network = createBatteryTestNetwork();
    BOOST_CHECK_EQUAL(1ul, network.getBatteryCount());
    Battery& battery = network.getBattery("BAT1");

    BOOST_CHECK_EQUAL("BAT1", battery.getId());
    BOOST_CHECK_EQUAL("BAT1_NAME", battery.getName());
    BOOST_CHECK_EQUAL(ConnectableType::BATTERY, battery.getType());
    BOOST_CHECK_CLOSE(100.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-200.0, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, battery.getMaxP(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(battery, battery.setP0(110.0)));
    BOOST_CHECK_CLOSE(110.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(battery.setP0(stdcxx::nan()), ValidationException, "Battery 'BAT1': p0 is invalid");
    BOOST_TEST(stdcxx::areSame(battery, battery.setP0(400.0)));
    BOOST_CHECK_CLOSE(400.0, battery.getP0(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(battery, battery.setQ0(210.0)));
    BOOST_CHECK_CLOSE(210.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(battery.setQ0(stdcxx::nan()), ValidationException, "Battery 'BAT1': q0 is invalid");

    BOOST_TEST(stdcxx::areSame(battery, battery.setMinP(90.0)));
    BOOST_CHECK_CLOSE(90.0, battery.getMinP(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(battery.setMinP(stdcxx::nan()), ValidationException, "Battery 'BAT1': Minimum active power is not set");
    POWSYBL_ASSERT_THROW(battery.setMinP(400.0), ValidationException, "Battery 'BAT1': Invalid active limits [400, 300]");

    BOOST_TEST(stdcxx::areSame(battery, battery.setMaxP(500.0)));
    BOOST_CHECK_CLOSE(500.0, battery.getMaxP(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(battery.setMaxP(stdcxx::nan()), ValidationException, "Battery 'BAT1': Maximum active power is not set");
    POWSYBL_ASSERT_THROW(battery.setMaxP(-400.0), ValidationException, "Battery 'BAT1': Invalid active limits [90, -400]");

    battery.remove();
    POWSYBL_ASSERT_THROW(network.getBattery("BAT1"), PowsyblException, "Unable to find to the identifiable 'BAT1'");
    BOOST_CHECK_EQUAL(0ul, network.getBatteryCount());
}

BOOST_AUTO_TEST_CASE(reactiveLimits) {
    const Network& network = createBatteryTestNetwork();
    Battery& battery = network.getBattery("BAT1");

    BOOST_CHECK_NO_THROW(battery.getReactiveLimits<MinMaxReactiveLimits>());
    battery.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(1)
        .setMinQ(15)
        .setMaxQ(25)
        .endPoint()
        .beginPoint()
        .setP(2)
        .setMinQ(10)
        .setMaxQ(20)
        .endPoint()
        .add();
    BOOST_CHECK_NO_THROW(battery.getReactiveLimits<ReactiveCapabilityCurve>());
    POWSYBL_ASSERT_THROW(battery.getReactiveLimits<MinMaxReactiveLimits>(), ValidationException,
        "Battery 'BAT1': Incorrect reactive limits type MinMaxReactiveLimits, expected ReactiveCapabilityCurve");

    battery.newMinMaxReactiveLimits()
        .setMinQ(1)
        .setMaxQ(2)
        .add();
    BOOST_CHECK_NO_THROW(battery.getReactiveLimits<MinMaxReactiveLimits>());
    POWSYBL_ASSERT_THROW(battery.getReactiveLimits<ReactiveCapabilityCurve>(), ValidationException,
        "Battery 'BAT1': Incorrect reactive limits type ReactiveCapabilityCurve, expected MinMaxReactiveLimits");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createBatteryTestNetwork();
    Battery& battery = network.getBattery("BAT1");

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {}), PowsyblException, "Empty target variant id list");
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_CLOSE(100.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-200, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300, battery.getMaxP(), std::numeric_limits<double>::epsilon());
    battery.setP0(110).setQ0(210).setMinP(-210).setMaxP(310);

    BOOST_CHECK_CLOSE(110.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(210.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-210, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(310, battery.getMaxP(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(100.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-210, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(310, battery.getMaxP(), std::numeric_limits<double>::epsilon());
    battery.setP0(120).setQ0(220).setMinP(-220).setMaxP(320);

    BOOST_CHECK_CLOSE(120.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(220.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-220, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(320, battery.getMaxP(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(100.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-220, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(320, battery.getMaxP(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_CLOSE(120.0, battery.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(220.0, battery.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-220, battery.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(320, battery.getMaxP(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1ul, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
