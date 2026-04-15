/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerStep.hpp>
#include <powsybl/iidm/PhaseTapChangerStepsReplacer.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createPhaseTapChangerTestNetwork() {
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
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    Load& l1 = vl1.newLoad()
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
        .setNominalV(225.0)
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

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl3 = substation2.newVoltageLevel()
        .setId("VL3")
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL2")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl2.getId())
        .setBus2(vl2Bus1.getId())
        .setConnectableBus2(vl2Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add()
        .newPhaseTapChanger()
        .setTapPosition(2L)
        .setLowTapPosition(1L)
        .beginStep()
        .setAlpha(9.0)
        .setB(10.0)
        .setG(11.0)
        .setR(12.0)
        .setRho(13.0)
        .setX(14.0)
        .endStep()
        .beginStep()
        .setAlpha(0.0)
        .setB(15.0)
        .setG(16.0)
        .setR(17.0)
        .setRho(1.0)
        .setX(19.0)
        .endStep()
        .beginStep()
        .setAlpha(20.5)
        .setB(20.0)
        .setG(21.0)
        .setR(22.0)
        .setRho(23.0)
        .setX(24.0)
        .endStep()
        .setRegulationMode(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL)
        .setRegulating(true)
        .setRegulationTerminal(stdcxx::ref<Terminal>(l1.getTerminal()))
        .setRegulationValue(25.0)
        .setTargetDeadband(0.0)
        .add();


    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL2_2")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl2.getId())
        .setBus2(vl2Bus1.getId())
        .setConnectableBus2(vl2Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(PhaseTapChangerTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createPhaseTapChangerTestNetwork();
    const Terminal& terminal = network.getLoad("LOAD1").getTerminal();

    PhaseTapChanger& phaseTapChanger = network.getTwoWindingsTransformer("2WT_VL1_VL2").getPhaseTapChanger();
    const PhaseTapChanger& cPhaseTapChanger = phaseTapChanger;
    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK(!phaseTapChanger.getSolvedTapPosition());
    POWSYBL_ASSERT_THROW(phaseTapChanger.getSolvedStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': solved tap position is not set");
    BOOST_CHECK(phaseTapChanger.getNeutralPosition().has_value());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getNeutralPosition().get());
    BOOST_CHECK_EQUAL(3, phaseTapChanger.getStepCount());
    BOOST_TEST(phaseTapChanger.hasLoadTapChangingCapabilities());
    BOOST_TEST(phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(25.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    BOOST_TEST(stdcxx::areSame(terminal, cPhaseTapChanger.getRegulationTerminal().get()));

    auto neutralStep = phaseTapChanger.getNeutralStep();
    POWSYBL_ASSERT_REF_TRUE(neutralStep);
    PhaseTapChangerStep& step = phaseTapChanger.getCurrentStep();
    BOOST_TEST(stdcxx::areSame(step, cPhaseTapChanger.getCurrentStep()));
    BOOST_TEST(stdcxx::areSame(step, neutralStep.get()));
    BOOST_TEST(stdcxx::areSame(step, phaseTapChanger.getStep(2)));
    BOOST_TEST(stdcxx::areSame(step, cPhaseTapChanger.getStep(2)));
    BOOST_CHECK_CLOSE(0.0, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(15.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(16.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(17.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(19.0, step.getX(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(phaseTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(cPhaseTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");
    POWSYBL_ASSERT_THROW(cPhaseTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");

    step = phaseTapChanger.getStep(1);
    BOOST_CHECK_CLOSE(9.0, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(11.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(12.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(13.0, step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(14.0, step.getX(), std::numeric_limits<double>::epsilon());

    step = phaseTapChanger.getStep(3);
    BOOST_CHECK_CLOSE(20.5, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(21.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(22.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(23.0, step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(24.0, step.getX(), std::numeric_limits<double>::epsilon());

    auto stepMap = phaseTapChanger.getAllSteps();
    BOOST_CHECK_EQUAL(3, stepMap.size());
    BOOST_CHECK(stdcxx::areSame(stepMap.find(1L)->second.get(), phaseTapChanger.getStep(1)));
    BOOST_CHECK(stdcxx::areSame(stepMap.find(2L)->second.get(), phaseTapChanger.getStep(2)));
    BOOST_CHECK(stdcxx::areSame(stepMap.find(3L)->second.get(), phaseTapChanger.getStep(3)));

    auto cStepMap = cPhaseTapChanger.getAllSteps();
    BOOST_CHECK_EQUAL(3, cStepMap.size());
    BOOST_CHECK(stdcxx::areSame(cStepMap.find(1L)->second.get(), cPhaseTapChanger.getStep(1)));
    BOOST_CHECK(stdcxx::areSame(cStepMap.find(2L)->second.get(), cPhaseTapChanger.getStep(2)));
    BOOST_CHECK(stdcxx::areSame(cStepMap.find(3L)->second.get(), cPhaseTapChanger.getStep(3)));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger();

    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -1 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(6), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 6 [1, 3]");
    PhaseTapChangerStep& step = phaseTapChanger.setTapPosition(3).getCurrentStep();
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getTapPosition());

    POWSYBL_ASSERT_THROW(phaseTapChanger.setSolvedTapPosition(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect solved tap position -1 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setSolvedTapPosition(6), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect solved tap position 6 [1, 3]");
    PhaseTapChangerStep& solvedStep = phaseTapChanger.setSolvedTapPosition(3).getSolvedStep();
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getSolvedTapPosition().get());
    BOOST_CHECK(stdcxx::areSame(step, solvedStep));

    BOOST_CHECK_CLOSE(20.5, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(21.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(22.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(23.0, step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(24.0, step.getX(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setAlpha(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getAlpha()));
    BOOST_TEST(stdcxx::areSame(step, step.setAlpha(26.0)));
    BOOST_CHECK_CLOSE(26.0, step.getAlpha(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setB(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getB()));
    BOOST_TEST(stdcxx::areSame(step, step.setB(26.0)));
    BOOST_CHECK_CLOSE(26.0, step.getB(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setG(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getG()));
    BOOST_TEST(stdcxx::areSame(step, step.setG(27.0)));
    BOOST_CHECK_CLOSE(27.0, step.getG(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setR(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getR()));
    BOOST_TEST(stdcxx::areSame(step, step.setR(28.0)));
    BOOST_CHECK_CLOSE(28.0, step.getR(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setRho(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getRho()));
    BOOST_TEST(stdcxx::areSame(step, step.setRho(29.0)));
    BOOST_CHECK_CLOSE(29.0, step.getRho(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(step, step.setX(stdcxx::nan())));
    BOOST_TEST(std::isnan(step.getX()));
    BOOST_TEST(stdcxx::areSame(step, step.setX(30.0)));
    BOOST_CHECK_CLOSE(30.0, step.getX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(phaseTapChanger.getNeutralPosition().has_value());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getNeutralPosition().get());
    auto neutralStep = phaseTapChanger.getNeutralStep();
    POWSYBL_ASSERT_REF_TRUE(neutralStep);
    neutralStep.get().setAlpha(10.0);
    BOOST_CHECK(!phaseTapChanger.getNeutralPosition().has_value());
    POWSYBL_ASSERT_REF_FALSE(phaseTapChanger.getNeutralStep());

    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7)), AssertionError, "Unexpected RegulationMode value: 7");

    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationValue(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and regulated terminal is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal2))));
    BOOST_TEST(stdcxx::areSame(terminal2, phaseTapChanger.getRegulationTerminal().get()));


    POWSYBL_ASSERT_THROW(phaseTapChanger.setLoadTapChangingCapabilities(false), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation cannot be enabled on phase tap changer without load tap changing capabilities");
    phaseTapChanger.setRegulating(false);
    BOOST_CHECK_NO_THROW(phaseTapChanger.setLoadTapChangingCapabilities(false));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation cannot be enabled on phase tap changer without load tap changing capabilities");
    phaseTapChanger.setLoadTapChangingCapabilities(true);
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(stdcxx::nan())));
    BOOST_TEST(std::isnan(phaseTapChanger.getRegulationValue()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(-15.0)));
    BOOST_CHECK_CLOSE(-15.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER).setRegulating(true));

    phaseTapChanger.setRegulating(false);
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal))));
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulating(true));

    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getTapPosition());
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setLowTapPosition(-3L)));
    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, phaseTapChanger.getTapPosition());

    BOOST_CHECK_CLOSE(0.0, phaseTapChanger.getTargetDeadband(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTargetDeadband(-1.0), ValidationException, "2 windings transformer '2WT_VL1_VL2': Unexpected value for target deadband of tap changer: -1");
    BOOST_CHECK_NO_THROW(phaseTapChanger.setTargetDeadband(1.0));
    BOOST_CHECK_CLOSE(1.0, phaseTapChanger.getTargetDeadband(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTargetDeadband(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': Undefined value for target deadband of regulating phase tap changer");

    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulating(true).setTargetDeadband(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': Undefined value for target deadband of regulating phase tap changer");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTargetDeadband(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': Unexpected value for target deadband of tap changer: -1");

    BOOST_CHECK_NO_THROW(phaseTapChanger.remove());
    BOOST_TEST(!transformer.hasPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createPhaseTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_TEST(transformer.hasPhaseTapChanger());
    transformer.getPhaseTapChanger().remove();
    BOOST_TEST(!transformer.hasPhaseTapChanger());
    PhaseTapChangerAdder adder = transformer.newPhaseTapChanger();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': tap position is not set");
    adder.setTapPosition(-5L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a phase tap changer should have at least one step");

    auto stepAdder = adder.beginStep();
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step alpha is not set");
    stepAdder.setAlpha(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step alpha is not set");
    stepAdder.setAlpha(1.5);

    stepAdder.setRho(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step rho is not set");
    stepAdder.setRho(1.0);

    stepAdder.setR(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step r is not set");
    stepAdder.setR(2.0);

    stepAdder.setX(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step x is not set");
    stepAdder.setX(3.0);

    stepAdder.setG(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step g is not set");
    stepAdder.setG(4.0);

    stepAdder.setB(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step b is not set");
    stepAdder.setB(5.0);

    BOOST_CHECK_NO_THROW(stepAdder.endStep());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -5 [0, 0]");
    adder.setLowTapPosition(-5L).setTapPosition(3L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 3 [-5, -5]");
    adder.setTapPosition(-5L).setRegulating(true);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    //PhaseTapChangerAdder default loadTapChangingCapabilities is true
    adder.setLoadTapChangingCapabilities(false);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation cannot be enabled on phase tap changer without load tap changing capabilities");
    adder.setLoadTapChangingCapabilities(true);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    adder.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7));
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected RegulationMode value: 7");

    adder.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER);
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationValue(stdcxx::nan());
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
     POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    BOOST_CHECK_NO_THROW(adder.add());
    transformer.getPhaseTapChanger().remove();
    BOOST_TEST(!transformer.hasPhaseTapChanger());

    adder.setRegulationValue(55.0);
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));

    adder.setTargetDeadband(-2.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': Unexpected value for target deadband of tap changer: -2");
    adder.setTargetDeadband(stdcxx::nan());

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_TEST(transformer.hasPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(adderByCopyActivePowerControl) {
    Network network = createPhaseTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_TEST(transformer.hasPhaseTapChanger());
    transformer.getPhaseTapChanger().setSolvedTapPosition(3);
    const auto& existingPhaseTapChanger = transformer.getPhaseTapChanger();

    TwoWindingsTransformer& transformer2 = network.getTwoWindingsTransformer("2WT_VL1_VL2_2");
    BOOST_TEST(!transformer2.hasPhaseTapChanger());
    transformer2.newPhaseTapChanger(existingPhaseTapChanger)
                    .add();

    const auto& copiedPhaseTapChanger = transformer2.getPhaseTapChanger();
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getTapPosition(), copiedPhaseTapChanger.getTapPosition());
    BOOST_CHECK(copiedPhaseTapChanger.getSolvedTapPosition().has_value());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getSolvedTapPosition().get(), copiedPhaseTapChanger.getSolvedTapPosition().get());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getLowTapPosition(), copiedPhaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationValue(), copiedPhaseTapChanger.getRegulationValue());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationMode(), copiedPhaseTapChanger.getRegulationMode());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.isRegulating(), copiedPhaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getTargetDeadband(), copiedPhaseTapChanger.getTargetDeadband());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationTerminal(), copiedPhaseTapChanger.getRegulationTerminal());

    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getStepCount(), copiedPhaseTapChanger.getStepCount());
    for(const auto& existingStep : existingPhaseTapChanger.getAllSteps()) {
        const auto& copiedStep = copiedPhaseTapChanger.getStep(existingStep.first);
        BOOST_CHECK_EQUAL(existingStep.second.get().getAlpha(), copiedStep.getAlpha());
        BOOST_CHECK_EQUAL(existingStep.second.get().getRho(), copiedStep.getRho());
        BOOST_CHECK_EQUAL(existingStep.second.get().getR(), copiedStep.getR());
        BOOST_CHECK_EQUAL(existingStep.second.get().getG(), copiedStep.getG());
        BOOST_CHECK_EQUAL(existingStep.second.get().getB(), copiedStep.getB());
        BOOST_CHECK_EQUAL(existingStep.second.get().getX(), copiedStep.getX());
    }
}
BOOST_AUTO_TEST_CASE(adderByCopyFixedTap) {
    Network network = createPhaseTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_TEST(transformer.hasPhaseTapChanger());
    transformer.getPhaseTapChanger().remove();
    BOOST_TEST(!transformer.hasPhaseTapChanger());
    transformer.newPhaseTapChanger()
                .setTapPosition(1)
                .setRegulationValue(12)
                .setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER)
                .setLowTapPosition(0)
                .setRegulating(false)
                .setTargetDeadband(3)
                .beginStep().setAlpha(1).setRho(2).setR(3).setG(4).setB(5).setX(6)
                .endStep()
                .beginStep().setAlpha(20).setRho(30).setR(40).setG(50).setB(60).setX(70)
                .endStep()
                .beginStep().setAlpha(300).setRho(400).setR(500).setG(600).setB(700).setX(800)
                .endStep()
                .add();
    const auto& existingPhaseTapChanger = transformer.getPhaseTapChanger();

    TwoWindingsTransformer& transformer2 = network.getTwoWindingsTransformer("2WT_VL1_VL2_2");
    BOOST_TEST(!transformer2.hasPhaseTapChanger());
    transformer2.newPhaseTapChanger(existingPhaseTapChanger)
                    .add();

    const auto& copiedPhaseTapChanger = transformer2.getPhaseTapChanger();
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getTapPosition(), copiedPhaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getLowTapPosition(), copiedPhaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationValue(), copiedPhaseTapChanger.getRegulationValue());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationMode(), copiedPhaseTapChanger.getRegulationMode());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.isRegulating(), copiedPhaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getTargetDeadband(), copiedPhaseTapChanger.getTargetDeadband());
    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getRegulationTerminal(), copiedPhaseTapChanger.getRegulationTerminal());

    BOOST_CHECK_EQUAL(existingPhaseTapChanger.getStepCount(), copiedPhaseTapChanger.getStepCount());
    for(const auto& existingStep : existingPhaseTapChanger.getAllSteps()) {
        const auto& copiedStep = copiedPhaseTapChanger.getStep(existingStep.first);
        BOOST_CHECK_EQUAL(existingStep.second.get().getAlpha(), copiedStep.getAlpha());
        BOOST_CHECK_EQUAL(existingStep.second.get().getRho(), copiedStep.getRho());
        BOOST_CHECK_EQUAL(existingStep.second.get().getR(), copiedStep.getR());
        BOOST_CHECK_EQUAL(existingStep.second.get().getG(), copiedStep.getG());
        BOOST_CHECK_EQUAL(existingStep.second.get().getB(), copiedStep.getB());
        BOOST_CHECK_EQUAL(existingStep.second.get().getX(), copiedStep.getX());
    }
}

BOOST_AUTO_TEST_CASE(holder) {
    Network network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    const TwoWindingsTransformer& cTransformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger();

    BOOST_TEST(stdcxx::areSame(phaseTapChanger, transformer.getPhaseTapChanger()));
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, cTransformer.getPhaseTapChanger()));

    BOOST_TEST(stdcxx::areSame(phaseTapChanger, transformer.getOptionalPhaseTapChanger().get()));
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, cTransformer.getOptionalPhaseTapChanger().get()));

    BOOST_TEST(transformer.hasPhaseTapChanger());
    BOOST_TEST(cTransformer.hasPhaseTapChanger());

    phaseTapChanger.remove();

    BOOST_TEST(!transformer.hasPhaseTapChanger());
    BOOST_TEST(!cTransformer.hasPhaseTapChanger());

    BOOST_TEST(!transformer.getOptionalPhaseTapChanger());
    BOOST_TEST(!cTransformer.getOptionalPhaseTapChanger());

    POWSYBL_ASSERT_THROW(transformer.getPhaseTapChanger(), PowsyblException, "Phase tap changer not set");
    POWSYBL_ASSERT_THROW(cTransformer.getPhaseTapChanger(), PowsyblException, "Phase tap changer not set");

    BOOST_CHECK_NO_THROW(transformer.newPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(stepsReplacer) {
    Network network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger();

    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK(phaseTapChanger.getNeutralPosition().has_value());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getNeutralPosition().get());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, phaseTapChanger.getStepCount());

    PhaseTapChangerStepsReplacer stepReplacer = phaseTapChanger.stepsReplacer();

    POWSYBL_ASSERT_THROW(stepReplacer.replaceSteps(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a tap changer shall have at least one step");
    auto stepAdder = stepReplacer.beginStep();
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step alpha is not set");

    stepAdder.setR(6.0)
            .setX(5.0)
            .setG(4.0)
            .setB(3.0)
            .setAlpha(2.0)
            .setRho(1.0)
            .endStep();

    stepReplacer.beginStep()
                    .setR(60.0)
                    .setX(50.0)
                    .setG(40.0)
                    .setB(30.0)
                    .setAlpha(20.0)
                    .setRho(10.0)
                    .endStep();

    phaseTapChanger.setTapPosition(3);
    POWSYBL_ASSERT_THROW(stepReplacer.replaceSteps(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 3 [1, 2]");
    phaseTapChanger.setTapPosition(1);
    stepReplacer.replaceSteps();

    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK(!phaseTapChanger.getNeutralPosition().has_value());
    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(2, phaseTapChanger.getStepCount());

    auto& step = phaseTapChanger.getStep(2);
    BOOST_CHECK_CLOSE(10.0, step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(50.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, step.getR(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
