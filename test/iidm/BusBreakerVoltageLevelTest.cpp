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
#include <powsybl/iidm/BusView.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

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

    voltageLevel.newLoad()
        .setId("LOAD2")
        .setBus("BUS")
        .setConnectableBus("BUS")
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    ASSERT_EQ(1ul, bus.getConnectedTerminalCount());
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
        .setOpen(true);
    ASSERT_TRUE(aSwitch.isFictitious());
    ASSERT_TRUE(aSwitch.isOpen());

    POWSYBL_ASSERT_THROW(aSwitch.setRetained(true), ValidationException, "Voltage level 'VL1': retain status is not modifiable in a non node/breaker voltage level");
}

TEST(BusBreakerVoltageLevel, NodeBreakerView) {
    const Network& network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    ASSERT_EQ(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView(), AssertionError, "Not implemented");

    // test const version
    const VoltageLevel& vl = network.getVoltageLevel("VL1");
    POWSYBL_ASSERT_THROW(vl.getNodeBreakerView(), AssertionError, "Not implemented");
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
    POWSYBL_ASSERT_THROW(view.removeBus("VL1_BUS1"), PowsyblException,
                         "Voltage level 'VL1': Cannot remove bus 'VL1_BUS1' due to connectable equipments");
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

    // test const versions
    const VoltageLevel& vl = network.getVoltageLevel("VL1");
    const BusBreakerView& view2 = vl.getBusBreakerView();
    const auto& refUnknownBus2 = view2.getBus("UNKNOWN");
    ASSERT_FALSE(refUnknownBus2);
}

TEST(BusBreakerVoltageLevel, CalculatedBusTopology) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getBusBreakerView().newBus()
        .setId("BUS1")
        .add();

    vl.getBusBreakerView().newBus()
        .setId("BUS2")
        .add();

    Switch& sw = vl.getBusBreakerView().newSwitch()
        .setId("SW")
        .setBus1("BUS1")
        .setBus2("BUS2")
        .setOpen(false)
        .add();

    vl.newLoad()
        .setId("LOAD1")
        .setBus("BUS1")
        .setConnectableBus("BUS1")
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    vl.newLoad()
        .setId("LOAD2")
        .setBus("BUS2")
        .setConnectableBus("BUS2")
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    network.newLine()
        .setId("VL1_VL2")
        .setVoltageLevel1(vl.getId())
        .setBus1("BUS1")
        .setConnectableBus1("BUS1")
        .setVoltageLevel2(vl2.getId())
        .setBus2("VL2_BUS1")
        .setConnectableBus2("VL2_BUS1")
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    network.newLine()
        .setId("VL1_VL2_2")
        .setVoltageLevel1(vl.getId())
        .setBus1("BUS2")
        .setConnectableBus1("BUS2")
        .setVoltageLevel2(vl2.getId())
        .setBus2("VL2_BUS1")
        .setConnectableBus2("VL2_BUS1")
        .setR(4.0)
        .setX(44.0)
        .setG1(5.0)
        .setB1(0.3)
        .setG2(6.0)
        .setB2(0.5)
        .add();

    ASSERT_EQ(1ul, vl.getBusView().getBuses().size());
    stdcxx::Reference<Bus> mergedBus1 = vl.getBusView().getMergedBus("BUS1");
    stdcxx::Reference<Bus> mergedBus2 = vl.getBusView().getMergedBus("BUS2");
    ASSERT_TRUE(stdcxx::areSame(mergedBus1.get(), mergedBus2.get()));

    sw.setOpen(true);
    const VoltageLevel& vlTest = vl;
    const BusView& busView = vlTest.getBusView();
    ASSERT_EQ(2ul, busView.getBuses().size());
    mergedBus1 = busView.getMergedBus("BUS1");
    mergedBus2 = busView.getMergedBus("BUS2");
    ASSERT_FALSE(stdcxx::areSame(mergedBus1.get(), mergedBus2.get()));

    Bus& testBus = mergedBus1.get();
    ASSERT_TRUE(std::isnan(testBus.getAngle()));
    ASSERT_TRUE(std::isnan(testBus.getV()));
    ASSERT_DOUBLE_EQ(7.7, testBus.setAngle(7.7).setV(8.8).getAngle());
    ASSERT_DOUBLE_EQ(8.8, testBus.getV());
    POWSYBL_ASSERT_THROW(testBus.setV(-9.0), ValidationException, "Bus 'BUS1': voltage cannot be < 0");
    ASSERT_EQ(2ul, testBus.getConnectedTerminalCount());
    std::vector<std::reference_wrapper<Terminal> > terminals = testBus.getConnectedTerminals();
    ASSERT_EQ(terminals.size(), testBus.getConnectedTerminalCount());
    ASSERT_TRUE(stdcxx::areSame(vl, testBus.getVoltageLevel()));

    // TODO(thiebarr): ASSERT_TRUE(busView.getBus("BUS1"));
}

TEST(BusBreakerVoltageLevel, Terminal) {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getBusBreakerView().newBus()
        .setId("BUS1")
        .add();

    Load& l1 = vl.newLoad()
        .setId("LOAD1")
        .setBus("BUS1")
        .setConnectableBus("BUS1")
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    Terminal& terminal = l1.getTerminal();
    ASSERT_TRUE(terminal.isConnected());
    ASSERT_TRUE(vl.disconnect(terminal));
    ASSERT_FALSE(terminal.isConnected());
    ASSERT_FALSE(vl.disconnect(terminal));
    ASSERT_FALSE(terminal.isConnected());
    ASSERT_TRUE(vl.connect(terminal));
    ASSERT_TRUE(terminal.isConnected());
    ASSERT_FALSE(vl.connect(terminal));
    ASSERT_TRUE(terminal.isConnected());
    ASSERT_TRUE(terminal.disconnect());
    ASSERT_FALSE(terminal.isConnected());
    ASSERT_TRUE(terminal.connect());
    ASSERT_TRUE(terminal.isConnected());

    ASSERT_TRUE(std::isnan(terminal.getV()));
    ASSERT_TRUE(std::isnan(terminal.getAngle()));
    ASSERT_TRUE(std::isnan(terminal.getI()));
    terminal.setP(1.0);
    terminal.setQ(2.0);
    ASSERT_DOUBLE_EQ(1.0, terminal.getP());
    ASSERT_DOUBLE_EQ(2.0, terminal.getQ());

    const Terminal& cTerminal = l1.getTerminal();
    ASSERT_TRUE(stdcxx::areSame(cTerminal.getVoltageLevel(), vl));
    auto& busBreakerView = terminal.getBusBreakerView();
    const auto& cBusBreakerView = cTerminal.getBusBreakerView();
    ASSERT_TRUE(stdcxx::areSame(busBreakerView, cBusBreakerView));
    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus(""), PowsyblException, "busId is empty");
    busBreakerView.setConnectableBus("BUS1");

    ASSERT_TRUE(stdcxx::areSame(terminal.getBusView(), cTerminal.getBusView()));
    ASSERT_FALSE(terminal.getBusView().getBus());

    POWSYBL_ASSERT_THROW(terminal.getNodeBreakerView(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(cTerminal.getNodeBreakerView(), AssertionError, "Not implemented");
}

}  // namespace iidm

}  // namespace powsybl
