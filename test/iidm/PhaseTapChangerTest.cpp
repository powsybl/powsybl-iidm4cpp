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
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"
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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(225.0)
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
        .setAlpha(14.5)
        .setB(15.0)
        .setG(16.0)
        .setR(17.0)
        .setRho(18.0)
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
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(PhaseTapChangerTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createPhaseTapChangerTestNetwork();
    const Terminal& terminal = network.getLoad("LOAD1").getTerminal();

    PhaseTapChanger& phaseTapChanger = network.getTwoWindingsTransformer("2WT_VL1_VL2").getPhaseTapChanger().get();
    const PhaseTapChanger& cPhaseTapChanger = phaseTapChanger;
    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, phaseTapChanger.getStepCount());
    BOOST_TEST(phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(25.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    BOOST_TEST(stdcxx::areSame(terminal, cPhaseTapChanger.getRegulationTerminal().get()));


    PhaseTapChangerStep& step = phaseTapChanger.getCurrentStep();
    BOOST_TEST(stdcxx::areSame(step, cPhaseTapChanger.getCurrentStep()));
    BOOST_TEST(stdcxx::areSame(step, phaseTapChanger.getStep(2)));
    BOOST_TEST(stdcxx::areSame(step, cPhaseTapChanger.getStep(2)));
    BOOST_CHECK_CLOSE(14.5, step.getAlpha(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(15.0, step.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(16.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(17.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(18.0, step.getRho(), std::numeric_limits<double>::epsilon());
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
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger().get();

    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -1 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(6), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 6 [1, 3]");
    PhaseTapChangerStep& step = phaseTapChanger.setTapPosition(3).getCurrentStep();
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getTapPosition());
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

    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7)), AssertionError, "Unexpected RegulationMode value: 7");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");

    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationValue(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and regulated terminal is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal2))));
    BOOST_TEST(stdcxx::areSame(terminal2, phaseTapChanger.getRegulationTerminal().get()));

    phaseTapChanger.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    //TODO(thiebarr) POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(stdcxx::nan())));
    BOOST_TEST(std::isnan(phaseTapChanger.getRegulationValue()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(-15.0)));
    BOOST_CHECK_CLOSE(-15.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER).setRegulating(true));

    phaseTapChanger.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal))));
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    BOOST_CHECK_NO_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER).setRegulating(true));

    BOOST_CHECK_EQUAL(1L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, phaseTapChanger.getTapPosition());
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setLowTapPosition(-3L)));
    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(-1L, phaseTapChanger.getTapPosition());

    BOOST_TEST(std::isnan(phaseTapChanger.getTargetDeadband()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTargetDeadband(-1.0), ValidationException, "2 windings transformer '2WT_VL1_VL2': Unexpected value for target deadband of tap changer: -1");
    BOOST_CHECK_NO_THROW(phaseTapChanger.setTargetDeadband(1.0));
    BOOST_CHECK_CLOSE(1.0, phaseTapChanger.getTargetDeadband(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(phaseTapChanger.setTargetDeadband(stdcxx::nan()));
    BOOST_TEST(std::isnan(phaseTapChanger.getTargetDeadband()));

    BOOST_CHECK_NO_THROW(phaseTapChanger.remove());
    BOOST_TEST(!transformer.getPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createPhaseTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_TEST(transformer.getPhaseTapChanger());
    transformer.getPhaseTapChanger().get().remove();
    BOOST_TEST(!transformer.getPhaseTapChanger());
    PhaseTapChangerAdder adder = transformer.newPhaseTapChanger();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': tap position is not set");
    adder.setTapPosition(-5L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a phase tap changer should have at least one step");

    auto stepAdder = adder.beginStep();
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step alpha is not set");
    stepAdder.setAlpha(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step alpha is not set");
    stepAdder.setAlpha(1.5);

    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step rho is not set");
    stepAdder.setRho(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step rho is not set");
    stepAdder.setRho(1.0);

    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step r is not set");
    stepAdder.setR(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step r is not set");
    stepAdder.setR(2.0);

    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step x is not set");
    stepAdder.setX(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step x is not set");
    stepAdder.setX(3.0);

    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step g is not set");
    stepAdder.setG(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step g is not set");
    stepAdder.setG(4.0);

    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step b is not set");
    stepAdder.setB(stdcxx::nan());
    POWSYBL_ASSERT_THROW(stepAdder.endStep(), ValidationException, "2 windings transformer '2WT_VL1_VL2': step b is not set");
    stepAdder.setB(5.0);

    BOOST_CHECK_NO_THROW(stepAdder.endStep());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -5 [0, 0]");
    adder.setLowTapPosition(-5L).setTapPosition(3L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 3 [-5, -5]");
    adder.setTapPosition(-5L).setRegulating(true);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");
    adder.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7));
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected RegulationMode value: 7");

    adder.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationValue(stdcxx::nan());
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));
    BOOST_CHECK_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
     POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    BOOST_CHECK_NO_THROW(adder.add());
    transformer.getPhaseTapChanger().get().remove();
    BOOST_TEST(!transformer.getPhaseTapChanger());

    adder.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    adder.setRegulationValue(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    adder.setRegulationValue(55.0);
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and regulated terminal is not set");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));

    adder.setTargetDeadband(-2.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': Unexpected value for target deadband of tap changer: -2");
    adder.setTargetDeadband(stdcxx::nan());

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_TEST(transformer.getPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(holder) {
    Network network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    const TwoWindingsTransformer& cTransformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger().get();

    BOOST_TEST(stdcxx::areSame(phaseTapChanger, transformer.getPhaseTapChanger().get()));
    BOOST_TEST(stdcxx::areSame(phaseTapChanger, cTransformer.getPhaseTapChanger().get()));

    BOOST_TEST(transformer.getPhaseTapChanger());
    phaseTapChanger.remove();
    BOOST_TEST(!cTransformer.getPhaseTapChanger());

    BOOST_CHECK_NO_THROW(transformer.newPhaseTapChanger());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
