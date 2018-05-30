/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

TEST(BusBreakerVoltageLevel, Bus) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    ASSERT_EQ(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    Bus& bus = voltageLevel.getBusBreakerView().newBus()
        .setId("BUS")
        .add();
    ASSERT_EQ("BUS", bus.getId());
    ASSERT_TRUE(stdcxx::areSame(voltageLevel, bus.getVoltageLevel()));
    ASSERT_EQ(0ul, bus.getConnectedTerminalCount());
    ASSERT_TRUE(std::isnan(bus.getV()));
    ASSERT_TRUE(std::isnan(bus.getAngle()));

    bus.setV(10.0)
        .setAngle(0.01);
    ASSERT_EQ(10.0, bus.getV());
    ASSERT_EQ(0.01, bus.getAngle());
}

TEST(BusBreakerVoltageLevel, Switch) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    ASSERT_EQ(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS1")
        .add();
    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS2")
        .add();

    Switch& aSwitch = voltageLevel.getBusBreakerView().newSwitch()
        .setId("SW")
        .setName("SW_NAME")
        .setFictitious(false)
        .setOpen(false)
        .setBus1("BUS1")
        .setBus2("BUS2")
        .add();
    ASSERT_EQ("SW", aSwitch.getId());
    ASSERT_EQ("SW_NAME", aSwitch.getName());
    ASSERT_TRUE(stdcxx::areSame(voltageLevel, aSwitch.getVoltageLevel()));
    ASSERT_FALSE(aSwitch.isFictitious());
    ASSERT_FALSE(aSwitch.isOpen());
    ASSERT_TRUE(aSwitch.isRetained());

    aSwitch.setFictitious(true)
        .setOpen(true)
        .setRetained(true);
    ASSERT_TRUE(aSwitch.isFictitious());
    ASSERT_TRUE(aSwitch.isOpen());
    ASSERT_TRUE(aSwitch.isRetained());
}

TEST(BusBreakerVoltageLevel, NodeBreakerView) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    ASSERT_EQ(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView(), AssertionError, "Not implemented");
}

TEST(BusBreakerVoltageLevel, BusBreakerView) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    ASSERT_EQ(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    BusBreakerView& view = voltageLevel.getBusBreakerView();
    Bus& bus1 = view.newBus()
        .setId("BUS1")
        .setName("BUS1_NAME")
        .add();
    Bus& bus2 = view.newBus()
        .setId("BUS2")
        .setName("BUS2_NAME")
        .add();
    Bus& bus3 = view.newBus()
        .setId("BUS3")
        .setName("BUS3_NAME")
        .add();

    Switch& switch1 = view.newSwitch()
        .setId("SW1")
        .setName("SW1_NAME")
        .setBus1("BUS1")
        .setBus2("BUS2")
        .add();
    view.newSwitch()
        .setId("SW2")
        .setName("SW2_NAME")
        .setBus1("BUS2")
        .setBus2("BUS3")
        .add();

    // get bus
    const auto& refBus1 = view.getBus("BUS1");
    ASSERT_TRUE(refBus1);
    ASSERT_TRUE(stdcxx::areSame(bus1, refBus1.get()));

    const auto& refUnknownBus = view.getBus("UNKNOWN");
    ASSERT_FALSE(refUnknownBus);

    // get switch
    const auto& refSwitch1 = view.getSwitch("SW1");
    ASSERT_TRUE(refSwitch1);
    ASSERT_TRUE(stdcxx::areSame(switch1, refSwitch1.get()));

    const auto& refUnknownSwitch = view.getSwitch("UNKNOWN");
    ASSERT_FALSE(refUnknownSwitch);

    // get bus from switch
    const auto& refBus2 = view.getBus1("SW2");
    ASSERT_TRUE(refBus2);
    ASSERT_TRUE(stdcxx::areSame(bus2, refBus2.get()));

    const auto& refBus3 = view.getBus2("SW2");
    ASSERT_TRUE(refBus3);
    ASSERT_TRUE(stdcxx::areSame(bus3, refBus3.get()));

    // // get bus from unknown switch
    POWSYBL_ASSERT_THROW(view.getBus1("UNKNOWN"), PowsyblException,
                         "Switch 'UNKNOWN' not found in the voltage level 'VL1'");

    // Remove bus
    POWSYBL_ASSERT_THROW(view.removeBus("BUS2"), PowsyblException,
                         "Cannot remove bus 'BUS2' due to the connected switch 'SW1'");
    POWSYBL_ASSERT_THROW(view.removeBus("UNKNOWN"), PowsyblException,
                         "Bus 'UNKNOWN' not found in the voltage level 'VL1'");

    // Remove switch
    view.removeSwitch("SW2");
    POWSYBL_ASSERT_THROW(view.removeSwitch("SW2"), PowsyblException,
                         "Switch 'SW2' not found in voltage level 'VL1'");
    view.removeBus("BUS3");

    // Remove all buses
    POWSYBL_ASSERT_THROW(view.removeAllBuses(), ValidationException,
                         "Voltage level 'VL1': Cannot remove all buses because there is still some switches");
    view.removeAllSwitches();
    POWSYBL_ASSERT_THROW(view.removeAllBuses(), ValidationException,
                         "Voltage level 'VL1': Cannot remove bus 'VL1_BUS1' due to connected equipments");
    network.getLoad("LOAD1").remove();
    view.removeAllBuses();
}

}  // namespace iidm

}  // namespace powsybl
