/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createThreeWindingsTransformerTestNetwork() {
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

     vl1.newLoad()
         .setId("LOAD1")
         .setName("LOAD1_NAME")
         .setBus(vl1Bus1.getId())
         .setConnectableBus(vl1Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(50.0)
         .setQ0(40.0)
         .add();

     VoltageLevel& vl2 = substation.newVoltageLevel()
         .setId("VL2")
         .setName("VL2_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .setLowVoltageLimit(200.0)
         .setHighVoltageLimit(260.0)
         .add();

     Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
         .setId("VL2_BUS1")
         .add();

     vl2.newLoad()
         .setId("LOAD2")
         .setName("LOAD2_NAME")
         .setBus(vl2Bus1.getId())
         .setConnectableBus(vl2Bus1.getId())
         .setLoadType(LoadType::UNDEFINED)
         .setP0(60.0)
         .setQ0(70.0)
         .add();

     VoltageLevel& vl3 = substation.newVoltageLevel()
         .setId("VL3")
         .setName("VL3_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .setLowVoltageLimit(340.0)
         .setHighVoltageLimit(420.0)
         .add();

     Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
         .setId("VL3_BUS1")
         .add();

     Substation& substation2 = network.newSubstation()
         .setId("S2")
         .setName("S2_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
         .add();

     VoltageLevel& vl4 = substation2.newVoltageLevel()
         .setId("VL4")
         .setName("VL4_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(225.0)
         .setLowVoltageLimit(200.0)
         .setHighVoltageLimit(260.0)
         .add();

     vl4.getBusBreakerView().newBus()
         .setId("VL4_BUS1")
         .add();

     substation.newThreeWindingsTransformer()
         .setId("3WT_VL1_VL2_VL3")
         .setName("3WT_VL1_VL2_VL3_NAME")
         .newLeg1()
         .setR(1.3)
         .setX(1.4)
         .setG(1.6)
         .setB(1.7)
         .setRatedU(1.1)
         .setRatedS(2.2)
         .setVoltageLevel(vl1.getId())
         .setBus(vl1Bus1.getId())
         .setConnectableBus(vl1Bus1.getId())
         .add()
         .newLeg2()
         .setR(2.3)
         .setX(2.4)
         .setG(0.0)
         .setB(0.0)
         .setRatedU(2.1)
         .setVoltageLevel(vl2.getId())
         .setBus(vl2Bus1.getId())
         .setConnectableBus(vl2Bus1.getId())
         .add()
         .newLeg3()
         .setR(3.3)
         .setX(3.4)
         .setG(0.0)
         .setB(0.0)
         .setRatedU(3.1)
         .setVoltageLevel(vl3.getId())
         .setBus(vl3Bus1.getId())
         .setConnectableBus(vl3Bus1.getId())
         .add()
         .add();

     return network;
}

void addRatioTapChangerLeg2(ThreeWindingsTransformer& transformer, Terminal& terminal) {
    transformer.getLeg2().newRatioTapChanger()
        .setTapPosition(2L)
        .setLowTapPosition(1L)
        .beginStep()
        .setB(10.0)
        .setG(11.0)
        .setR(12.0)
        .setRho(13.0)
        .setX(14.0)
        .endStep()
        .beginStep()
        .setB(15.0)
        .setG(16.0)
        .setR(17.0)
        .setRho(18.0)
        .setX(19.0)
        .endStep()
        .beginStep()
        .setB(20.0)
        .setG(21.0)
        .setR(22.0)
        .setRho(23.0)
        .setX(24.0)
        .endStep()
        .setLoadTapChangingCapabilities(true)
        .setRegulating(true)
        .setRegulationTerminal(stdcxx::ref<Terminal>(terminal))
        .setTargetV(25.0)
        .setTargetDeadband(1.0)
        .add();
}

void addRatioTapChangerLeg3(ThreeWindingsTransformer& transformer, Terminal& terminal) {
    transformer.getLeg3().newRatioTapChanger()
        .setTapPosition(-2L)
        .setLowTapPosition(-3L)
        .beginStep()
        .setB(-10.0)
        .setG(-11.0)
        .setR(-12.0)
        .setRho(-13.0)
        .setX(-14.0)
        .endStep()
        .beginStep()
        .setB(-15.0)
        .setG(-16.0)
        .setR(-17.0)
        .setRho(-18.0)
        .setX(-19.0)
        .endStep()
        .beginStep()
        .setB(-20.0)
        .setG(-21.0)
        .setR(-22.0)
        .setRho(-23.0)
        .setX(-24.0)
        .endStep()
        .setLoadTapChangingCapabilities(true)
        .setRegulating(false)
        .setRegulationTerminal(stdcxx::ref<Terminal>(terminal))
        .setTargetV(26.0)
        .setTargetDeadband(2.0)
        .add();
}

BOOST_AUTO_TEST_SUITE(ThreeWindingsTransformerTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createThreeWindingsTransformerTestNetwork();
    const Network& cNetwork = network;
    Substation& substation = network.getSubstation("S1");
    const Substation& cSubstation = substation;

    BOOST_CHECK_EQUAL(1UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getThreeWindingsTransformers()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cNetwork.getThreeWindingsTransformers()));
    BOOST_CHECK_EQUAL(1UL, boost::size(substation.getThreeWindingsTransformers()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cSubstation.getThreeWindingsTransformers()));

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    const ThreeWindingsTransformer& cTransformer = cNetwork.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    BOOST_TEST(stdcxx::areSame(cTransformer, transformer));

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3", transformer.getId());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3_NAME", transformer.getOptionalName());
    BOOST_CHECK_EQUAL(ConnectableType::THREE_WINDINGS_TRANSFORMER, transformer.getType());
    std::ostringstream oss;
    oss << transformer.getType();
    BOOST_CHECK_EQUAL("THREE_WINDINGS_TRANSFORMER", oss.str());

    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation()));
    BOOST_TEST(stdcxx::areSame(substation, cTransformer.getSubstation()));

    Terminal& terminal1 = transformer.getTerminal(ThreeWindingsTransformer::Side::ONE);
    Terminal& terminal2 = transformer.getTerminal(ThreeWindingsTransformer::Side::TWO);
    Terminal& terminal3 = transformer.getTerminal(ThreeWindingsTransformer::Side::THREE);
    POWSYBL_ASSERT_THROW(transformer.getTerminal(static_cast<ThreeWindingsTransformer::Side>(4)), AssertionError, "Unexpected Side value: 4");


    const Terminal& cTerminal1 = cTransformer.getTerminal(ThreeWindingsTransformer::Side::ONE);
    const Terminal& cTerminal2 = cTransformer.getTerminal(ThreeWindingsTransformer::Side::TWO);
    const Terminal& cTerminal3 = cTransformer.getTerminal(ThreeWindingsTransformer::Side::THREE);
    POWSYBL_ASSERT_THROW(cTransformer.getTerminal(static_cast<ThreeWindingsTransformer::Side>(5)), AssertionError, "Unexpected Side value: 5");

    BOOST_TEST(stdcxx::areSame(terminal1, cTerminal1));
    BOOST_TEST(stdcxx::areSame(terminal2, cTerminal2));
    BOOST_TEST(stdcxx::areSame(terminal3, cTerminal3));

    const Terminal& load1Terminal = network.getLoad("LOAD1").getTerminal();
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::ONE, transformer.getSide(terminal1));
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::TWO, transformer.getSide(terminal2));
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::THREE, transformer.getSide(terminal3));
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::ONE, cTransformer.getSide(cTerminal1));
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::TWO, cTransformer.getSide(cTerminal2));
    BOOST_CHECK_EQUAL(ThreeWindingsTransformer::Side::THREE, cTransformer.getSide(cTerminal3));
    POWSYBL_ASSERT_THROW(transformer.getSide(load1Terminal), AssertionError, "The terminal is not connected to this three windings transformer");

    // Leg 1 tests
    ThreeWindingsTransformer::Leg& leg1 = transformer.getLeg1();
    const ThreeWindingsTransformer::Leg& cLeg1 = cTransformer.getLeg1();
    BOOST_TEST(stdcxx::areSame(cLeg1, leg1));
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", cLeg1.toString());
    BOOST_CHECK_CLOSE(1.3, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.4, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.6, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.7, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.1, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, leg1.getRatedS(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_TEST(!cLeg1.getCurrentLimits());

    BOOST_TEST(stdcxx::areSame(terminal1, leg1.getTerminal()));
    BOOST_TEST(stdcxx::areSame(cTerminal1, cLeg1.getTerminal()));

    // Leg 2 tests
    ThreeWindingsTransformer::Leg& leg2 = transformer.getLeg2();
    const ThreeWindingsTransformer::Leg& cLeg2 = cTransformer.getLeg2();
    BOOST_TEST(stdcxx::areSame(cLeg2, leg2));
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", cLeg2.toString());
    BOOST_CHECK_CLOSE(2.3, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.4, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.1, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(leg2.getRatedS()));
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_TEST(!cLeg2.getCurrentLimits());
    BOOST_TEST(!leg2.hasRatioTapChanger());
    BOOST_TEST(!cLeg2.hasRatioTapChanger());

    BOOST_TEST(stdcxx::areSame(terminal2, leg2.getTerminal()));
    BOOST_TEST(stdcxx::areSame(cTerminal2, cLeg2.getTerminal()));
    BOOST_TEST(stdcxx::areSame(network, leg2.getNetwork()));
    BOOST_TEST(stdcxx::areSame(network, cLeg2.getNetwork()));

    // Leg 3 tests
    ThreeWindingsTransformer::Leg& leg3 = transformer.getLeg3();
    const ThreeWindingsTransformer::Leg& cLeg3 = cTransformer.getLeg3();
    BOOST_TEST(stdcxx::areSame(cLeg3, leg3));
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", cLeg3.toString());
    BOOST_CHECK_CLOSE(3.3, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.4, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.1, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());
    BOOST_TEST(!cLeg3.getCurrentLimits());
    BOOST_TEST(!leg3.hasRatioTapChanger());
    BOOST_TEST(!cLeg3.hasRatioTapChanger());

    BOOST_TEST(stdcxx::areSame(terminal3, leg3.getTerminal()));
    BOOST_TEST(stdcxx::areSame(cTerminal3, cLeg3.getTerminal()));
    BOOST_TEST(stdcxx::areSame(network, leg3.getNetwork()));
    BOOST_TEST(stdcxx::areSame(network, cLeg3.getNetwork()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createThreeWindingsTransformerTestNetwork();

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    const ThreeWindingsTransformer& cTransformer = transformer;
    BOOST_TEST(stdcxx::areSame(cTransformer, transformer));

    Terminal& load1Terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& load2Terminal = network.getLoad("LOAD2").getTerminal();

    // Leg 1 tests
    ThreeWindingsTransformer::Leg& leg1 = transformer.getLeg1();
    const ThreeWindingsTransformer::Leg& cLeg1 = transformer.getLeg1();
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setR(100.0)));
    BOOST_CHECK_CLOSE(100.0, leg1.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg1.setR(stdcxx::nan()), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': r is invalid");

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setX(200.0)));
    BOOST_CHECK_CLOSE(200.0, leg1.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg1.setX(stdcxx::nan()), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': x is invalid");

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setG(300.0)));
    BOOST_CHECK_CLOSE(300.0, leg1.getG(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg1.setG(stdcxx::nan()), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': g is invalid");

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setB(400.0)));
    BOOST_CHECK_CLOSE(400.0, leg1.getB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg1.setB(stdcxx::nan()), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': b is invalid");

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setRatedU(500.0)));
    BOOST_CHECK_CLOSE(500.0, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg1.setRatedU(stdcxx::nan()), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': rated U is invalid");

    BOOST_TEST(stdcxx::areSame(leg1, leg1.setRatedS(600.0)));
    BOOST_CHECK_CLOSE(600.0, leg1.getRatedS(), std::numeric_limits<double>::epsilon());
    leg1.setRatedS(stdcxx::nan());
    BOOST_TEST(std::isnan(leg1.getRatedS()));
    POWSYBL_ASSERT_THROW(leg1.setRatedS(-1.0), ValidationException, "3 windings transformer leg1 '3WT_VL1_VL2_VL3': Invalid rated S value: -1");

    BOOST_TEST(!leg1.getCurrentLimits());
    leg1.newCurrentLimits()
        .setPermanentLimit(4.0)
        .beginTemporaryLimit()
        .setName("TL1")
        .setValue(7.0)
        .setAcceptableDuration(1UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2")
        .setValue(6.0)
        .setAcceptableDuration(2UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3")
        .setValue(5.0)
        .setAcceptableDuration(3UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();
    BOOST_TEST(leg1.getCurrentLimits());

    BOOST_TEST(!leg1.getOptionalRatioTapChanger());
    BOOST_TEST(!cLeg1.getOptionalRatioTapChanger());
    BOOST_TEST(!leg1.hasRatioTapChanger());

    // Leg 2 tests
    ThreeWindingsTransformer::Leg& leg2 = transformer.getLeg2();
    const ThreeWindingsTransformer::Leg& cLeg2 = transformer.getLeg2();
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());

    BOOST_TEST(stdcxx::areSame(leg2, leg2.setR(600.0)));
    BOOST_CHECK_CLOSE(600.0, leg2.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg2.setR(stdcxx::nan()), ValidationException, "3 windings transformer leg2 '3WT_VL1_VL2_VL3': r is invalid");

    BOOST_TEST(stdcxx::areSame(leg2, leg2.setX(700.0)));
    BOOST_CHECK_CLOSE(700.0, leg2.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg2.setX(stdcxx::nan()), ValidationException, "3 windings transformer leg2 '3WT_VL1_VL2_VL3': x is invalid");

    BOOST_TEST(stdcxx::areSame(leg2, leg2.setRatedU(800.0)));
    BOOST_CHECK_CLOSE(800.0, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg2.setRatedU(stdcxx::nan()), ValidationException, "3 windings transformer leg2 '3WT_VL1_VL2_VL3': rated U is invalid");

    BOOST_TEST(!leg2.getCurrentLimits());
    leg2.newCurrentLimits()
        .setPermanentLimit(40.0)
        .beginTemporaryLimit()
        .setName("TL1")
        .setValue(70.0)
        .setAcceptableDuration(10UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2")
        .setValue(60.0)
        .setAcceptableDuration(20UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3")
        .setValue(50.0)
        .setAcceptableDuration(30UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();
    BOOST_TEST(leg2.getCurrentLimits());


    BOOST_TEST(!leg2.hasRatioTapChanger());
    addRatioTapChangerLeg2(transformer, load1Terminal);
    BOOST_TEST(leg2.hasRatioTapChanger());
    BOOST_TEST(leg2.getOptionalRatioTapChanger());
    BOOST_CHECK(stdcxx::areSame(leg2.getOptionalRatioTapChanger().get(), leg2.getRatioTapChanger()));
    BOOST_TEST(leg2.hasRatioTapChanger());
    BOOST_TEST(cLeg2.hasRatioTapChanger());
    BOOST_TEST(cLeg2.getOptionalRatioTapChanger());
    BOOST_CHECK(stdcxx::areSame(cLeg2.getOptionalRatioTapChanger().get(), leg2.getRatioTapChanger()));
    leg2.getRatioTapChanger().remove();
    BOOST_TEST(!leg2.hasRatioTapChanger());
    BOOST_TEST(!leg2.getOptionalRatioTapChanger());
    BOOST_TEST(!leg2.hasRatioTapChanger());
    BOOST_TEST(!cLeg2.hasRatioTapChanger());
    BOOST_TEST(!cLeg2.getOptionalRatioTapChanger());

    BOOST_TEST(!leg2.getOptionalPhaseTapChanger());
    BOOST_TEST(!cLeg2.getOptionalPhaseTapChanger());
    BOOST_TEST(!leg2.hasPhaseTapChanger());

    // Leg 3 tests
    ThreeWindingsTransformer::Leg& leg3 = transformer.getLeg3();
    const ThreeWindingsTransformer::Leg& cLeg3 = transformer.getLeg3();
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());

    BOOST_TEST(stdcxx::areSame(leg3, leg3.setR(900.0)));
    BOOST_CHECK_CLOSE(900.0, leg3.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg3.setR(stdcxx::nan()), ValidationException, "3 windings transformer leg3 '3WT_VL1_VL2_VL3': r is invalid");

    BOOST_TEST(stdcxx::areSame(leg3, leg3.setX(1000.0)));
    BOOST_CHECK_CLOSE(1000.0, leg3.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg3.setX(stdcxx::nan()), ValidationException, "3 windings transformer leg3 '3WT_VL1_VL2_VL3': x is invalid");

    BOOST_TEST(stdcxx::areSame(leg3, leg3.setRatedU(1100.0)));
    BOOST_CHECK_CLOSE(1100.0, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(leg3.setRatedU(stdcxx::nan()), ValidationException, "3 windings transformer leg3 '3WT_VL1_VL2_VL3': rated U is invalid");

    BOOST_TEST(!leg3.getCurrentLimits());
    leg3.newCurrentLimits()
        .setPermanentLimit(400.0)
        .beginTemporaryLimit()
        .setName("TL1")
        .setValue(700.0)
        .setAcceptableDuration(100UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2")
        .setValue(600.0)
        .setAcceptableDuration(200UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3")
        .setValue(500.0)
        .setAcceptableDuration(300UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();
    BOOST_TEST(leg3.getCurrentLimits());

    BOOST_TEST(!leg3.hasRatioTapChanger());
    addRatioTapChangerLeg3(transformer, load2Terminal);
    BOOST_TEST(leg3.hasRatioTapChanger());
    BOOST_TEST(leg3.getOptionalRatioTapChanger());
    BOOST_TEST(leg3.hasRatioTapChanger());
    BOOST_TEST(cLeg3.hasRatioTapChanger());
    BOOST_TEST(cLeg3.getOptionalRatioTapChanger());
    leg3.getRatioTapChanger().remove();
    BOOST_TEST(!leg3.hasRatioTapChanger());
    BOOST_TEST(!leg3.getOptionalRatioTapChanger());
    BOOST_TEST(!leg3.hasRatioTapChanger());
    BOOST_TEST(!cLeg3.hasRatioTapChanger());
    BOOST_TEST(!cLeg3.getOptionalRatioTapChanger());

    BOOST_TEST(!leg3.getOptionalPhaseTapChanger());
    BOOST_TEST(!leg3.hasPhaseTapChanger());
    BOOST_TEST(!cLeg3.getOptionalPhaseTapChanger());

    transformer.setFictitious(true);
    BOOST_CHECK(transformer.isFictitious());
    transformer.setFictitious(false);
    BOOST_CHECK(!transformer.isFictitious());

    transformer.remove();
    POWSYBL_ASSERT_THROW(network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3"), PowsyblException, "Unable to find to the identifiable '3WT_VL1_VL2_VL3'");
}

BOOST_AUTO_TEST_CASE(adders) {
    Network network = createThreeWindingsTransformerTestNetwork();
    Substation& substation2 = network.getSubstation("S2");
    unsigned long threeWindingsTransformerCount = network.getThreeWindingsTransformerCount();

    BOOST_CHECK_EQUAL(1UL, threeWindingsTransformerCount);

    ThreeWindingsTransformerAdder adder = substation2.newThreeWindingsTransformer();
    adder.setId("TWT");

    // Leg 1 tests
    ThreeWindingsTransformerAdder::LegAdder l1Adder = adder.newLeg1();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer 'TWT': leg 1 is not defined");

    POWSYBL_ASSERT_THROW(l1Adder.add(), ValidationException, "3 windings transformer leg1: r is not set");
    l1Adder.setR(1.0);

    POWSYBL_ASSERT_THROW(l1Adder.add(), ValidationException, "3 windings transformer leg1: x is not set");
    l1Adder.setX(1.1);

    POWSYBL_ASSERT_THROW(l1Adder.add(), ValidationException, "3 windings transformer leg1: g is not set");
    l1Adder.setG(1.3);

    POWSYBL_ASSERT_THROW(l1Adder.add(), ValidationException, "3 windings transformer leg1: b is not set");
    l1Adder.setB(1.4);

    POWSYBL_ASSERT_THROW(l1Adder.add(), ValidationException, "3 windings transformer leg1: rated U is not set");
    BOOST_CHECK_NO_THROW(l1Adder.setRatedU(1.2).add());

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg1: voltage level is not set");
    l1Adder.setVoltageLevel("INVALID").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg1: voltage level 'INVALID' not found");
    l1Adder.setVoltageLevel("VL1").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg1: voltage level shall belong to the substation 'S2'");
    l1Adder.setVoltageLevel("VL4").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg1: connectable bus is not set");
    l1Adder.setConnectableBus("VL4_BUS1").add();

    adder.newLeg1()
        .setR(1.0)
        .setX(1.1)
        .setRatedU(1.2)
        .setG(1.3)
        .setB(1.4)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .setNode(1UL)
        .add();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg1: connection node and connection bus are exclusives");
    adder.newLeg1()
        .setR(1.0)
        .setX(1.1)
        .setRatedU(1.2)
        .setG(1.3)
        .setB(1.4)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .add();

    // Leg 2 tests
    ThreeWindingsTransformerAdder::LegAdder l2Adder = adder.newLeg2();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer 'TWT': leg 2 is not defined");

    POWSYBL_ASSERT_THROW(l2Adder.add(), ValidationException, "3 windings transformer leg2: r is not set");
    l2Adder.setR(2.0);

    POWSYBL_ASSERT_THROW(l2Adder.add(), ValidationException, "3 windings transformer leg2: x is not set");
    l2Adder.setX(2.1);

    POWSYBL_ASSERT_THROW(l2Adder.add(), ValidationException, "3 windings transformer leg2: rated U is not set");
    BOOST_CHECK_NO_THROW(l2Adder.setRatedU(2.2).add());

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg2: voltage level is not set");
    l2Adder.setVoltageLevel("INVALID").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg2: voltage level 'INVALID' not found");
    l2Adder.setVoltageLevel("VL1").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg2: voltage level shall belong to the substation 'S2'");
    l2Adder.setVoltageLevel("VL4").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg2: connectable bus is not set");
    l2Adder.setConnectableBus("VL4_BUS1");

    POWSYBL_ASSERT_THROW(l2Adder.setRatedS(-1.0).add(), ValidationException, "3 windings transformer leg2: Invalid rated S value: -1");
    l2Adder.setRatedS(1.0).add();

    adder.newLeg2()
        .setR(2.0)
        .setX(2.1)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.2)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .setNode(1UL)
        .add();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg2: connection node and connection bus are exclusives");
    adder.newLeg2()
        .setR(2.0)
        .setX(2.1)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.2)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .add();

    // Leg 3 tests
    ThreeWindingsTransformerAdder::LegAdder l3Adder = adder.newLeg3();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer 'TWT': leg 3 is not defined");

    POWSYBL_ASSERT_THROW(l3Adder.add(), ValidationException, "3 windings transformer leg3: r is not set");
    l3Adder.setR(3.0);

    POWSYBL_ASSERT_THROW(l3Adder.add(), ValidationException, "3 windings transformer leg3: x is not set");
    l3Adder.setX(3.1);

    POWSYBL_ASSERT_THROW(l3Adder.add(), ValidationException, "3 windings transformer leg3: rated U is not set");
    BOOST_CHECK_NO_THROW(l3Adder.setRatedU(3.2).add());

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg3: voltage level is not set");
    l3Adder.setVoltageLevel("INVALID").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg3: voltage level 'INVALID' not found");
    l3Adder.setVoltageLevel("VL1").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg3: voltage level shall belong to the substation 'S2'");
    l3Adder.setVoltageLevel("VL4").add();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg3: connectable bus is not set");
    l3Adder.setConnectableBus("VL4_BUS1");

    adder.newLeg3()
        .setR(3.0)
        .setX(3.1)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.2)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .setNode(1UL)
        .add();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "3 windings transformer leg3: connection node and connection bus are exclusives");
    adder.newLeg3()
        .setR(3.0)
        .setX(3.1)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.2)
        .setVoltageLevel("VL4")
        .setConnectableBus("VL4_BUS1")
        .add();

    adder.setId("");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "3 windings transformer id is not set");
    adder.setId("3WT_VL1_VL2_VL3");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'ThreeWindingsTransformer' with the id '3WT_VL1_VL2_VL3'");
    adder.setName("3WT_VL4_VL4_VL4_NAME");
    adder.setId("UNIQUE_3WT_ID");
    BOOST_CHECK_NO_THROW(adder.add());

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'ThreeWindingsTransformer' with the id 'UNIQUE_3WT_ID'");

    BOOST_CHECK_EQUAL(threeWindingsTransformerCount + 1, network.getThreeWindingsTransformerCount());
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createThreeWindingsTransformerTestNetwork();
    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    addRatioTapChangerLeg2(transformer, terminal);
    addRatioTapChangerLeg3(transformer, terminal2);

    ThreeWindingsTransformer::Leg& leg1 = transformer.getLeg1();
    ThreeWindingsTransformer::Leg& leg2 = transformer.getLeg2();
    ThreeWindingsTransformer::Leg& leg3 = transformer.getLeg3();
    RatioTapChanger& ratioTapChangerLeg2 = leg2.getRatioTapChanger();
    RatioTapChanger& ratioTapChangerLeg3 = leg3.getRatioTapChanger();

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.3, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.4, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.6, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.7, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.1, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.3, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.4, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.1, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.3, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.4, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.1, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(26.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg3.getRegulationTerminal().get()));

    leg1.setR(1.31).setX(1.41).setG(1.61).setB(1.71).setRatedU(1.11);
    leg2.setR(2.31).setX(2.41).setRatedU(2.11);
    leg3.setR(3.31).setX(3.41).setRatedU(3.11);
    ratioTapChangerLeg2.setTapPosition(1L).setRegulating(false).setRegulationTerminal(stdcxx::ref<Terminal>(terminal2)).setTargetV(100.0);
    ratioTapChangerLeg3.setTapPosition(-3L).setRegulating(false).setRegulationTerminal(stdcxx::ref<Terminal>(terminal)).setTargetV(200.0);

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.31, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.41, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.61, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.71, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.11, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.31, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.41, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.11, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.31, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.41, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.11, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(100.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(200.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg3.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant("s2");

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.31, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.41, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.61, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.71, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.11, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.31, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.41, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.11, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.31, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.41, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.11, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(26.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg3.getRegulationTerminal().get()));

    leg1.setRatedU(1.12).setR(1.32).setX(1.42).setG(1.62).setB(1.72);
    leg2.setRatedU(2.12).setR(2.32).setX(2.42);
    leg3.setRatedU(3.12).setR(3.32).setX(3.42);
    ratioTapChangerLeg2.setTapPosition(3L).setRegulationTerminal(stdcxx::ref<Terminal>(terminal)).setTargetV(150.0).setRegulating(false);
    ratioTapChangerLeg3.setTapPosition(-1L).setRegulationTerminal(stdcxx::ref<Terminal>(terminal2)).setTargetV(250.0).setRegulating(false);

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.32, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.42, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.62, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.72, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.12, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.32, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.42, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.12, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.32, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.42, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.12, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(150.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(250.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg3.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.32, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.42, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.62, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.72, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.12, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.32, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.42, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.12, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.32, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.42, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.12, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(26.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg3.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg1", leg1.toString());
    BOOST_CHECK_CLOSE(1.32, leg1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.42, leg1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.62, leg1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.72, leg1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.12, leg1.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg1.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg2", leg2.toString());
    BOOST_CHECK_CLOSE(2.32, leg2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.42, leg2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.12, leg2.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg2.getCurrentLimits());
    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3 leg3", leg3.toString());
    BOOST_CHECK_CLOSE(3.32, leg3.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.42, leg3.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.12, leg3.getRatedU(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!leg3.getCurrentLimits());

    BOOST_CHECK_EQUAL(1L, ratioTapChangerLeg2.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getHighTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChangerLeg2.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg2.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg2.isRegulating());
    BOOST_TEST(ratioTapChangerLeg2.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(150.0, ratioTapChangerLeg2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChangerLeg2.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, ratioTapChangerLeg3.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getHighTapPosition());
    BOOST_CHECK_EQUAL(-1L, ratioTapChangerLeg3.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChangerLeg3.getStepCount());
    BOOST_TEST(!ratioTapChangerLeg3.isRegulating());
    BOOST_TEST(ratioTapChangerLeg3.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(250.0, ratioTapChangerLeg3.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChangerLeg3.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(voltageLevel) {
    Network network = createThreeWindingsTransformerTestNetwork();
    Load& load = network.getLoad("LOAD1");
    Line& line = network.newLine()
        .setId("VL1_VL3")
        .setVoltageLevel1("VL1")
        .setBus1("VL1_BUS1")
        .setConnectableBus1("VL1_BUS1")
        .setVoltageLevel2("VL3")
        .setBus2("VL3_BUS1")
        .setConnectableBus2("VL3_BUS1")
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();
    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");

    VoltageLevel& voltageLevel1 = network.getVoltageLevel("VL1");
    Load& load2 = voltageLevel1.getConnectable<Load>("LOAD1");
    BOOST_TEST(stdcxx::areSame(load, load2));
    Line& line2 = voltageLevel1.getConnectable<Line>("VL1_VL3");
    BOOST_TEST(stdcxx::areSame(line, line2));
    ThreeWindingsTransformer& tranformer2 = voltageLevel1.getConnectable<ThreeWindingsTransformer>("3WT_VL1_VL2_VL3");
    BOOST_TEST(stdcxx::areSame(transformer, tranformer2));

    VoltageLevel& voltageLevel4 = network.getVoltageLevel("VL4");
    POWSYBL_ASSERT_THROW(voltageLevel4.getConnectable<Load>("LOAD1"), PowsyblException, "The injection 'LOAD1' is not connected to the voltage level 'VL4'");
    POWSYBL_ASSERT_THROW(voltageLevel4.getConnectable<Line>("VL1_VL3"), PowsyblException, "The branch 'VL1_VL3' is not connected to the voltage level 'VL4'");
    POWSYBL_ASSERT_THROW(voltageLevel4.getConnectable<ThreeWindingsTransformer>("3WT_VL1_VL2_VL3"), PowsyblException, "The 3 windings transformer '3WT_VL1_VL2_VL3' is not connected to the voltage level 'VL4'");

    BOOST_TEST(!voltageLevel1.getConnectable<Load>("INVALID"));
    BOOST_TEST(!voltageLevel4.getConnectable<Load>("INVALID"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
