/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ConnectedComponent.hpp>
#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/SynchronousComponent.hpp>
#include <powsybl/iidm/SynchronousComponentsManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

namespace powsybl {

namespace iidm {

Network createVsc() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T16:34:55.930+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(400)
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
        .setNominalVoltage(400)
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
        .setNominalVoltage(400)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
        .setMaxP(300.0)
        .setActivePowerSetpoint(280)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(AbstractVscTestSuite)

BOOST_AUTO_TEST_CASE(testBase) {
    Network network = createVsc();
    HvdcLine& hvdcLine = network.getHvdcLine("L");
    VscConverterStation& cs1 = network.getVscConverterStation("C1");
    VscConverterStation& cs2 = network.getVscConverterStation("C2");

    BOOST_CHECK(HvdcConverterStation::HvdcType::VSC == cs1.getHvdcType());
    BOOST_CHECK_EQUAL(1, network.getVoltageLevel("VL1").getVscConverterStationCount());
    BOOST_CHECK_EQUAL(1, network.getVoltageLevel("VL2").getVscConverterStationCount());
    BOOST_CHECK_CLOSE(1.1, cs1.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.1, cs2.getLossFactor(), std::numeric_limits<double>::epsilon());
    cs1.setLossFactor(2.2);
    BOOST_CHECK_CLOSE(2.2, cs1.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(cs1.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(405.0, cs1.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    cs1.setVoltageSetpoint(406.0);
    BOOST_CHECK_CLOSE(406.0, cs1.getVoltageSetpoint(), 0.0);
    BOOST_CHECK(std::isnan(cs1.getReactivePowerSetpoint()));
    BOOST_CHECK_CLOSE(1.1, cs2.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!cs2.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(123.0, cs2.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    cs2.setReactivePowerSetpoint(124.0);
    BOOST_CHECK_CLOSE(124.0, cs2.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(cs2.getVoltageSetpoint()));
    cs2.setVoltageSetpoint(405);
    cs2.setVoltageRegulatorOn(true);
    BOOST_CHECK(cs2.isVoltageRegulatorOn());
    BOOST_CHECK_EQUAL(1, network.getHvdcLineCount());
    HvdcLine& l = network.getHvdcLine("L");
    BOOST_CHECK_CLOSE(1.0, l.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER, l.getConvertersMode());
    BOOST_CHECK_CLOSE(300.0, l.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(cs1, l.getConverterStation1().get()));
    BOOST_CHECK(stdcxx::areSame(cs2, l.getConverterStation2().get()));
    BOOST_CHECK(l.getConverterStation1().get().getTerminal().getBusView().getBus().get().isInMainConnectedComponent());
    BOOST_CHECK(l.getConverterStation2().get().getTerminal().getBusView().getBus().get().isInMainConnectedComponent());
    BOOST_CHECK(!stdcxx::areSame(l.getConverterStation1().get().getTerminal().getBusView().getBus().get().getSynchronousComponent().get().getNum(),
                                    l.getConverterStation2().get().getTerminal().getBusView().getBus().get().getSynchronousComponent().get().getNum()));

    BOOST_CHECK(stdcxx::areSame(hvdcLine, cs1.getHvdcLine().get()));
    BOOST_CHECK(stdcxx::areSame(hvdcLine, cs2.getHvdcLine().get()));
    BOOST_CHECK(stdcxx::areSame(cs1, hvdcLine.getConverterStation1().get()));
    BOOST_CHECK(stdcxx::areSame(cs1, hvdcLine.getConverterStation(HvdcLine::Side::ONE).get()));
    BOOST_CHECK(stdcxx::areSame(cs2, hvdcLine.getConverterStation2().get()));
    BOOST_CHECK(stdcxx::areSame(cs2, hvdcLine.getConverterStation(HvdcLine::Side::TWO).get()));

    BOOST_CHECK_EQUAL(2, boost::size(hvdcLine.getConverterStation1().get().getTerminal().getBusView().getBus().get().getConnectedComponent().get().getBuses()));
    BOOST_CHECK_EQUAL(2, hvdcLine.getConverterStation1().get().getTerminal().getBusView().getBus().get().getConnectedComponent().get().getSize());
    BOOST_CHECK_EQUAL(1, boost::size(hvdcLine.getConverterStation1().get().getTerminal().getBusView().getBus().get().getSynchronousComponent().get().getBuses()));
    BOOST_CHECK_EQUAL(1, hvdcLine.getConverterStation1().get().getTerminal().getBusView().getBus().get().getSynchronousComponent().get().getSize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
