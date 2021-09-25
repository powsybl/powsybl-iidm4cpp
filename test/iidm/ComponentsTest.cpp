/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/util/ConnectedComponents.hpp>
#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createMultiConnectedComponentsNetwork() {
    Network network("test_mcc", "test");

    Substation& s1 = network.newSubstation()
            .setId("A")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
            .setId("B")
            .setNominalV(225.0)
            .setLowVoltageLimit(0.0)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl1.getNodeBreakerView().newBusbarSection()
            .setId("C")
            .setNode(0)
            .add();
    vl1.getNodeBreakerView().newSwitch()
            .setId("D")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(false)
            .setOpen(false)
            .setNode1(0)
            .setNode2(1)
            .add();
    vl1.getNodeBreakerView().newSwitch()
            .setId("E")
            .setKind(SwitchKind::BREAKER)
            .setRetained(false)
            .setOpen(false)
            .setNode1(1)
            .setNode2(2)
            .add();

    Substation& s2 = network.newSubstation()
            .setId("F")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
            .setId("G")
            .setNominalV(225.0)
            .setLowVoltageLimit(0.0)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("H")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("I")
            .setNode(1)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("J")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(true)
            .setOpen(false)
            .setNode1(0)
            .setNode2(2)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("K")
            .setKind(SwitchKind::DISCONNECTOR)
            .setRetained(true)
            .setOpen(false)
            .setNode1(1)
            .setNode2(3)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("L")
            .setKind(SwitchKind::BREAKER)
            .setRetained(true)
            .setOpen(false)
            .setNode1(2)
            .setNode2(3)
            .add();
    vl2.getNodeBreakerView().newSwitch()
            .setId("M")
            .setKind(SwitchKind::BREAKER)
            .setRetained(false)
            .setOpen(false)
            .setNode1(0)
            .setNode2(4)
            .add();

    network.newLine()
            .setId("N")
            .setR(0.001)
            .setX(0.1)
            .setG1(0.0)
            .setB1(0.0)
            .setG2(0.0)
            .setB2(0.0)
            .setVoltageLevel1("B")
            .setNode1(2)
            .setVoltageLevel2("G")
            .setNode2(4)
            .add();
    return network;
}

Network createNetworkWithHvdcLine() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation()
            .setId("S1")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
            .setId("VL1")
            .setNominalV(400)
            .setTopologyKind(TopologyKind::BUS_BREAKER)
            .add();
    vl1.getBusBreakerView().newBus()
            .setId("B1")
            .add();
    Substation& s2 = network.newSubstation()
            .setId("S2")
            .setCountry(Country::FR)
            .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
            .setId("VL2")
            .setNominalV(400)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();
    vl2.getNodeBreakerView().newBusbarSection()
            .setId("BBS1")
            .setName("BusbarSection")
            .setNode(0)
            .add();
    vl2.getNodeBreakerView().newDisconnector()
            .setId("DISC_BBS1_BK1")
            .setName("Disconnector")
            .setNode1(0)
            .setNode2(1)
            .setOpen(false)
            .setRetained(false)
            .add();
    vl2.getNodeBreakerView().newBreaker()
            .setId("BK1")
            .setName("Breaker")
            .setNode1(1)
            .setNode2(2)
            .setOpen(false)
            .setRetained(true)
            .add();
    VscConverterStation& cs1 = vl1.newVscConverterStation()
            .setId("C1")
            .setName("Converter1")
            .setConnectableBus("B1")
            .setBus("B1")
            .setLossFactor(1.1)
            .setVoltageSetpoint(405.0)
            .setVoltageRegulatorOn(true)
            .add();
    cs1.getTerminal()
            .setP(100.0)
            .setQ(50.0);
    cs1.newReactiveCapabilityCurve()
            .beginPoint()
            .setP(5.0)
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .endPoint()
            .beginPoint()
            .setP(10.0)
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .endPoint()
            .add();
    VscConverterStation& cs2 = vl2.newVscConverterStation()
            .setId("C2")
            .setName("Converter2")
            .setNode(2)
            .setLossFactor(1.1)
            .setReactivePowerSetpoint(123)
            .setVoltageRegulatorOn(false)
            .add();
    cs2.newMinMaxReactiveLimits()
            .setMinQ(0.0)
            .setMaxQ(10.0)
            .add();
    network.newHvdcLine()
            .setId("L")
            .setName("HVDC")
            .setConverterStationId1("C1")
            .setConverterStationId2("C2")
            .setR(1)
            .setNominalV(400)
            .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
            .setMaxP(300.0)
            .setActivePowerSetpoint(280)
            .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(ComponentsTestSuite)

BOOST_AUTO_TEST_CASE(NodeBreakerTest) {
    Network network = createComponentsTestNetworkNB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    Bus& bus = network.getBusBreakerView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_0")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusBreakerView().getBus("VL10_0");
    const Bus& cBus10 = cNetwork.getBusBreakerView().getBus("VL10_0");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(BusBreakerTest) {
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusBreakerView().getBuses()));

    Bus& bus = network.getBusBreakerView().getBus("VL1_BUS1");
    const Bus& cBus = cNetwork.getBusBreakerView().getBus("VL1_BUS1");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());

    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL1_BUS1")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusBreakerView().getBus("VL10_BUS1");
    const Bus& cBus10 = cNetwork.getBusBreakerView().getBus("VL10_BUS1");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusBreakerView().getBus("VL10_BUS1")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusBreakerView().getBus("VL10_BUS1")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(BusViewTest) {
    Network network = createComponentsTestNetworkBB();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(2UL, boost::size(cNetwork.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(10UL, boost::size(network.getBusView().getBuses()));

    BOOST_CHECK_EQUAL(3UL, boost::size(cNetwork.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(3UL, boost::size(network.getBusView().getSynchronousComponents()));

    Bus& bus = network.getBusView().getBus("VL1_0");
    const Bus& cBus = cNetwork.getBusView().getBus("VL1_0");
    BOOST_TEST(bus.isInMainConnectedComponent());
    BOOST_TEST(cBus.isInMainConnectedComponent());
    BOOST_TEST(bus.isInMainSynchronousComponent());
    BOOST_TEST(cBus.isInMainSynchronousComponent());

    auto& comp = bus.getConnectedComponent().get();
    const auto& cComp = cBus.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(8UL, boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(8UL, boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(comp.getSize(), boost::size(comp.getBuses()));
    BOOST_CHECK_EQUAL(cComp.getSize(), boost::size(cComp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, comp.getNum());
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(bus));
    BOOST_CHECK_EQUAL(comp.getNum(), *ConnectedComponents::getCcNum(network.getBusView().getBus("VL1_0")));
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cComp.getNum());
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cBus));
    BOOST_CHECK_EQUAL(cComp.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL1_0")));
    BOOST_CHECK(!ConnectedComponents::getCcNum(network.getBusView().getBus("INVALID")));

    auto& scomp = bus.getSynchronousComponent().get();
    const auto& cScomp = cBus.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(6UL, boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(scomp.getSize(), boost::size(scomp.getBuses()));
    BOOST_CHECK_EQUAL(cScomp.getSize(), boost::size(cScomp.getBuses()));

    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, scomp.getNum());
    BOOST_CHECK_EQUAL(ComponentConstants::MAIN_NUM, cScomp.getNum());

    Bus& bus10 = network.getBusView().getBus("VL10_0");
    const Bus& cBus10 = cNetwork.getBusView().getBus("VL10_0");
    BOOST_TEST(!bus10.isInMainConnectedComponent());
    BOOST_TEST(!cBus10.isInMainConnectedComponent());
    BOOST_TEST(!bus10.isInMainSynchronousComponent());
    BOOST_TEST(!cBus10.isInMainSynchronousComponent());

    auto& comp10 = bus10.getConnectedComponent().get();
    const auto& cComp10 = cBus10.getConnectedComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(comp10.getSize(), boost::size(comp10.getBuses()));
    BOOST_CHECK_EQUAL(cComp10.getSize(), boost::size(cComp10.getBuses()));

    BOOST_CHECK_EQUAL(1L, comp10.getNum());
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(bus10));
    BOOST_CHECK_EQUAL(comp10.getNum(), *ConnectedComponents::getCcNum(network.getBusView().getBus("VL10_0")));
    BOOST_CHECK_EQUAL(1L, cComp10.getNum());
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cBus10));
    BOOST_CHECK_EQUAL(cComp10.getNum(), *ConnectedComponents::getCcNum(cNetwork.getBusView().getBus("VL10_0")));

    auto& scomp10 = bus10.getSynchronousComponent().get();
    const auto& cScomp10 = cBus10.getSynchronousComponent().get();
    BOOST_CHECK_EQUAL(2UL, boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(2UL, boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(scomp10.getSize(), boost::size(scomp10.getBuses()));
    BOOST_CHECK_EQUAL(cScomp10.getSize(), boost::size(cScomp10.getBuses()));

    BOOST_CHECK_EQUAL(2L, scomp10.getNum());
    BOOST_CHECK_EQUAL(2L, cScomp10.getNum());
}

BOOST_AUTO_TEST_CASE(MainConnectedComponentWithSwitchTest) {
    Network network = createMultiConnectedComponentsNetwork();

    const VoltageLevel& vl1 = network.getVoltageLevel("B");

    for (Bus& b : network.getBusView().getBuses()) {
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            b.setV(230.0).setAngle(0.5);
        } else {
            b.setV(220.0).setAngle(0.7);
        }
    }

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getBuses()));
    for (Bus& b : network.getBusView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
    }

    BOOST_CHECK_EQUAL(5, boost::size(network.getBusBreakerView().getBuses()));
    for (Bus& b : network.getBusBreakerView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            BOOST_CHECK_CLOSE(230.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.5, b.getAngle(), std::numeric_limits<double>::epsilon());
        } else {
            BOOST_CHECK_CLOSE(220.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.7, b.getAngle(), std::numeric_limits<double>::epsilon());
        }
    }
}

BOOST_AUTO_TEST_CASE(ComponentsWithHvdcLine) {
    Network network = createNetworkWithHvdcLine();
    VscConverterStation& cs1 = network.getVscConverterStation("C1");
    VscConverterStation& cs2 = network.getVscConverterStation("C2");

    const auto& bus1 = cs1.getTerminal().getBusView().getBus().get();
    const auto& cComp1 = bus1.getConnectedComponent().get();
    const auto& sComp1 = bus1.getSynchronousComponent().get();

    const auto& bus2 = cs2.getTerminal().getBusView().getBus().get();
    const auto& cComp2 = bus2.getConnectedComponent().get();
    const auto& sComp2 = bus2.getSynchronousComponent().get();

    // Same connected component
    BOOST_CHECK(bus1.isInMainConnectedComponent());
    BOOST_CHECK(bus2.isInMainConnectedComponent());
    BOOST_CHECK(stdcxx::areSame(cComp1, cComp2));

    // Not the same synchronous component
    BOOST_CHECK(bus1.isInMainSynchronousComponent());
    BOOST_CHECK(!bus2.isInMainSynchronousComponent());
    BOOST_CHECK(!stdcxx::areSame(sComp1, sComp2));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
