/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(BusBreakerVoltageLevelTestSuite)

BOOST_AUTO_TEST_CASE(BusTest) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    Bus& bus = voltageLevel.getBusBreakerView().newBus()
        .setId("BUS")
        .setName("BUS_NAME")
        .add();
    BOOST_CHECK_EQUAL("BUS", bus.getId());
    BOOST_CHECK_EQUAL("BUS_NAME", bus.getName());
    BOOST_TEST(stdcxx::areSame(voltageLevel, bus.getVoltageLevel()));
    BOOST_CHECK_EQUAL(0ul, bus.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(bus.getV()));
    BOOST_TEST(std::isnan(bus.getAngle()));

    bus.setV(10.0)
        .setAngle(0.01);
    BOOST_CHECK_EQUAL(10.0, bus.getV());
    BOOST_CHECK_EQUAL(0.01, bus.getAngle());

    voltageLevel.newLoad()
        .setId("LOAD2")
        .setBus("BUS")
        .setConnectableBus("BUS")
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    BOOST_CHECK_EQUAL(1ul, bus.getConnectedTerminalCount());
}

BOOST_AUTO_TEST_CASE(SwitchTest) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS1")
        .add();
    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS2")
        .add();

    auto swAdder = voltageLevel.getBusBreakerView().newSwitch()
        .setId("SW")
        .setName("SW_NAME")
        .setFictitious(false)
        .setOpen(false);
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'SW': First connection bus is not set");
    swAdder.setBus1("BUS1");
    POWSYBL_ASSERT_THROW(swAdder.add(), ValidationException, "Switch 'SW': Second connection bus is not set");
    swAdder.setBus2("BUS2");
    Switch& aSwitch = swAdder.add();
    BOOST_CHECK_EQUAL("SW", aSwitch.getId());
    BOOST_CHECK_EQUAL("SW_NAME", aSwitch.getName());
    BOOST_TEST(stdcxx::areSame(voltageLevel, aSwitch.getVoltageLevel()));
    BOOST_TEST(!aSwitch.isFictitious());
    BOOST_TEST(!aSwitch.isOpen());
    BOOST_TEST(aSwitch.isRetained());

    aSwitch.setFictitious(true)
        .setOpen(true);
    BOOST_TEST(aSwitch.isFictitious());
    BOOST_TEST(aSwitch.isOpen());

    const Network& cNetwork = network;
    BOOST_TEST(stdcxx::areSame(aSwitch, cNetwork.getSwitch("SW")));

    POWSYBL_ASSERT_THROW(aSwitch.setRetained(true), ValidationException, "Switch 'SW': retain status is not modifiable in a non node/breaker voltage level");
}

BOOST_AUTO_TEST_CASE(NodeBreakerViewTest) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    POWSYBL_ASSERT_THROW(voltageLevel.getNodeBreakerView(), AssertionError, "Not implemented");

    // test const version
    const Network& cNetwork = network;
    const VoltageLevel& vl = cNetwork.getVoltageLevel("VL1");
    POWSYBL_ASSERT_THROW(vl.getNodeBreakerView(), AssertionError, "Not implemented");
}

BOOST_AUTO_TEST_CASE(BusBreakerViewTest) {
    Network network = createNetwork();

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL1");
    const VoltageLevel& cVoltageLevel = voltageLevel;

    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, voltageLevel.getTopologyKind());

    auto& view = voltageLevel.getBusBreakerView();
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

    const auto& cView = view;

    // get bus
    const auto& refBus1 = view.getBus("BUS1");
    POWSYBL_ASSERT_REF_TRUE(refBus1);
    BOOST_TEST(stdcxx::areSame(bus1, refBus1.get()));

    const auto& refUnknownBus = view.getBus("UNKNOWN");
    BOOST_TEST(!refUnknownBus);

    // get switch
    BOOST_CHECK_EQUAL(2UL, view.getSwitchCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(view.getSwitches()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cView.getSwitches()));
    BOOST_CHECK_EQUAL(2UL, cVoltageLevel.getSwitchCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(voltageLevel.getSwitches()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cVoltageLevel.getSwitches()));
    const auto& refSwitch1 = view.getSwitch("SW1");
    POWSYBL_ASSERT_REF_TRUE(refSwitch1);
    BOOST_TEST(stdcxx::areSame(switch1, refSwitch1.get()));
    const auto& cRefSwitch1 = cView.getSwitch("SW1");
    POWSYBL_ASSERT_REF_TRUE(cRefSwitch1);
    BOOST_TEST(stdcxx::areSame(switch1, cRefSwitch1.get()));

    const auto& refUnknownSwitch = view.getSwitch("UNKNOWN");
    BOOST_TEST(!refUnknownSwitch);
    const auto& cRefUnknownSwitch = cView.getSwitch("UNKNOWN");
    BOOST_TEST(!cRefUnknownSwitch);

    // get bus from switch
    const auto& refBus2 = view.getBus1("SW2");
    POWSYBL_ASSERT_REF_TRUE(refBus2);
    BOOST_TEST(stdcxx::areSame(bus2, refBus2.get()));
    const auto& cRefBus2 = cView.getBus1("SW2");
    POWSYBL_ASSERT_REF_TRUE(cRefBus2);
    BOOST_TEST(stdcxx::areSame(bus2, cRefBus2.get()));

    const auto& refBus3 = view.getBus2("SW2");
    POWSYBL_ASSERT_REF_TRUE(refBus3);
    BOOST_TEST(stdcxx::areSame(bus3, refBus3.get()));
    const auto& cRefBus3 = cView.getBus2("SW2");
    POWSYBL_ASSERT_REF_TRUE(cRefBus3);
    BOOST_TEST(stdcxx::areSame(bus3, cRefBus3.get()));

    const auto& buses = view.getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(buses));

    // // get bus from unknown switch
    POWSYBL_ASSERT_THROW(view.getBus1("UNKNOWN"), PowsyblException,
                         "Switch 'UNKNOWN' not found in the voltage level 'VL1'");
    POWSYBL_ASSERT_THROW(cView.getBus1("UNKNOWN"), PowsyblException,
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
    BOOST_CHECK_EQUAL(0ul, view.getSwitchCount());
    POWSYBL_ASSERT_THROW(view.removeAllBuses(), ValidationException,
                         "Voltage level 'VL1': Cannot remove bus 'VL1_BUS1' due to connected equipments");
    network.getLoad("LOAD1").remove();
    view.removeAllBuses();

    // test const versions
    const VoltageLevel& vl = network.getVoltageLevel("VL1");
    const auto& view2 = vl.getBusBreakerView();
    const auto& refUnknownBus2 = view2.getBus("UNKNOWN");
    BOOST_TEST(!refUnknownBus2);
}

BOOST_AUTO_TEST_CASE(CalculatedBusTopologyTest) {
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

    BOOST_CHECK_EQUAL(1UL, boost::size(vl.getBusView().getBuses()));
    stdcxx::Reference<Bus> mergedBus1 = vl.getBusView().getMergedBus("BUS1");
    stdcxx::Reference<Bus> mergedBus2 = vl.getBusView().getMergedBus("BUS2");
    BOOST_TEST(stdcxx::areSame(mergedBus1.get(), mergedBus2.get()));
    BOOST_CHECK_EQUAL("VL_0", mergedBus1.get().getId());
    BOOST_CHECK_EQUAL("VL_0", mergedBus1.get().getName());

    sw.setOpen(true);
    VoltageLevel& vlTest = vl;
    auto& busView = vlTest.getBusView();
    const auto& cBusView = vlTest.getBusView();
    BOOST_CHECK_EQUAL(2UL, boost::size(busView.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cBusView.getBuses()));
    mergedBus1 = busView.getMergedBus("BUS1");
    mergedBus2 = busView.getMergedBus("BUS2");
    BOOST_TEST(!stdcxx::areSame(mergedBus1.get(), mergedBus2.get()));
    const auto& cMergedBus1 = cBusView.getMergedBus("BUS1");
    const auto& cMergedBus2 = cBusView.getMergedBus("BUS2");
    BOOST_TEST(!stdcxx::areSame(cMergedBus1.get(), cMergedBus2.get()));
    BOOST_TEST(stdcxx::areSame(cMergedBus1.get(), mergedBus1.get()));
    BOOST_TEST(stdcxx::areSame(cMergedBus2.get(), mergedBus2.get()));

    Bus& testBus = mergedBus1.get();
    BOOST_TEST(std::isnan(testBus.getAngle()));
    BOOST_TEST(std::isnan(testBus.getV()));
    BOOST_CHECK_CLOSE(7.7, testBus.setAngle(7.7).setV(8.8).getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(8.8, testBus.getV(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(testBus.setV(-9.0), ValidationException, "Bus 'BUS1': voltage cannot be < 0");
    BOOST_CHECK_EQUAL(2ul, testBus.getConnectedTerminalCount());
    const auto& terminals = testBus.getConnectedTerminals();
    BOOST_CHECK_EQUAL(boost::size(terminals), testBus.getConnectedTerminalCount());
    BOOST_TEST(stdcxx::areSame(vl, testBus.getVoltageLevel()));

    sw.setOpen(true);
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_1"));
    sw.setOpen(false);
    POWSYBL_ASSERT_REF_TRUE(busView.getBus("VL_0"));
    POWSYBL_ASSERT_REF_FALSE(busView.getBus("VL_1"));
}

BOOST_AUTO_TEST_CASE(TerminalTest) {
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
        .setName("BUS1_NAME")
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

    VoltageLevel& vl2 = s.newVoltageLevel()
        .setId("VL2")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("BUS2")
        .setName("BUS2_NAME")
        .add();

    network.newLine()
        .setId("VL_VL2")
        .setVoltageLevel1(vl.getId())
        .setBus1("BUS1")
        .setConnectableBus1("BUS1")
        .setVoltageLevel2(vl2.getId())
        .setBus2("BUS2")
        .setConnectableBus2("BUS2")
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    Terminal& terminal = l1.getTerminal();
    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(vl.disconnect(terminal));
    BOOST_TEST(!terminal.isConnected());
    BOOST_TEST(!vl.disconnect(terminal));
    BOOST_TEST(!terminal.isConnected());
    BOOST_TEST(vl.connect(terminal));
    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(!vl.connect(terminal));
    BOOST_TEST(terminal.isConnected());
    BOOST_TEST(terminal.disconnect());
    BOOST_TEST(!terminal.isConnected());
    BOOST_TEST(terminal.connect());
    BOOST_TEST(terminal.isConnected());

    BOOST_TEST(std::isnan(terminal.getV()));
    BOOST_TEST(std::isnan(terminal.getAngle()));
    BOOST_TEST(std::isnan(terminal.getI()));
    terminal.setP(1.0);
    terminal.setQ(2.0);
    BOOST_CHECK_CLOSE(1.0, terminal.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, terminal.getQ(), std::numeric_limits<double>::epsilon());

    const Terminal& cTerminal = l1.getTerminal();
    BOOST_TEST(stdcxx::areSame(cTerminal.getVoltageLevel(), vl));
    auto& busBreakerView = terminal.getBusBreakerView();
    const auto& cBusBreakerView = cTerminal.getBusBreakerView();
    BOOST_TEST(stdcxx::areSame(busBreakerView, cBusBreakerView));
    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus(""), PowsyblException, "busId is empty");
    POWSYBL_ASSERT_THROW(busBreakerView.setConnectableBus("UNKNOWN"), PowsyblException, "Bus 'UNKNOWN' not found in the voltage level 'VL'");
    busBreakerView.setConnectableBus("BUS1");
    const auto& configuredBus = busBreakerView.getBus().get();
    BOOST_CHECK_EQUAL("BUS1", configuredBus.getId());
    BOOST_CHECK_EQUAL("BUS1_NAME", configuredBus.getName());
    const auto& cConfiguredBus = cBusBreakerView.getBus().get();
    BOOST_TEST(stdcxx::areSame(configuredBus, cConfiguredBus));


    BOOST_TEST(stdcxx::areSame(terminal.getBusView(), cTerminal.getBusView()));
    POWSYBL_ASSERT_REF_TRUE(terminal.getBusView().getBus());
    POWSYBL_ASSERT_REF_TRUE(cTerminal.getBusView().getBus());
    const auto& mergedBus = terminal.getBusView().getBus().get();
    BOOST_CHECK_EQUAL("VL_0", mergedBus.getId());
    BOOST_CHECK_EQUAL("VL_0", mergedBus.getName());

    POWSYBL_ASSERT_THROW(terminal.getNodeBreakerView(), AssertionError, "Not implemented");
    POWSYBL_ASSERT_THROW(cTerminal.getNodeBreakerView(), AssertionError, "Not implemented");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
