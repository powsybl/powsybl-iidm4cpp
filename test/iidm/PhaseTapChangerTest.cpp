/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
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

namespace phasetapchangertest {

Terminal& getTerminalFromNetwork2() {
    Network network("test2", "test2");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(1);

    Load& l1 = vl.newLoad()
        .setId("LOAD1")
        .setNode(0)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    return l1.getTerminal();
}

}  // namespace phasetapchangertest

TEST(PhaseTapChanger, constructor) {
    const Network& network = createPhaseTapChangerTestNetwork();
    const Terminal& terminal = network.getLoad("LOAD1").getTerminal();

    PhaseTapChanger& phaseTapChanger = network.getTwoWindingsTransformer("2WT_VL1_VL2").getPhaseTapChanger().get();
    const PhaseTapChanger& cPhaseTapChanger = phaseTapChanger;
    ASSERT_EQ(1L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(2L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(3, phaseTapChanger.getStepCount());
    ASSERT_TRUE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(25.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    ASSERT_TRUE(stdcxx::areSame(terminal, cPhaseTapChanger.getRegulationTerminal().get()));


    PhaseTapChangerStep& step = phaseTapChanger.getCurrentStep();
    ASSERT_TRUE(stdcxx::areSame(step, cPhaseTapChanger.getCurrentStep()));
    ASSERT_TRUE(stdcxx::areSame(step, phaseTapChanger.getStep(2)));
    ASSERT_TRUE(stdcxx::areSame(step, cPhaseTapChanger.getStep(2)));
    ASSERT_DOUBLE_EQ(14.5, step.getAlpha());
    ASSERT_DOUBLE_EQ(15.0, step.getB());
    ASSERT_DOUBLE_EQ(16.0, step.getG());
    ASSERT_DOUBLE_EQ(17.0, step.getR());
    ASSERT_DOUBLE_EQ(18.0, step.getRho());
    ASSERT_DOUBLE_EQ(19.0, step.getX());

    POWSYBL_ASSERT_THROW(phaseTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(cPhaseTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");
    POWSYBL_ASSERT_THROW(cPhaseTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");

    step = phaseTapChanger.getStep(1);
    ASSERT_DOUBLE_EQ(9.0, step.getAlpha());
    ASSERT_DOUBLE_EQ(10.0, step.getB());
    ASSERT_DOUBLE_EQ(11.0, step.getG());
    ASSERT_DOUBLE_EQ(12.0, step.getR());
    ASSERT_DOUBLE_EQ(13.0, step.getRho());
    ASSERT_DOUBLE_EQ(14.0, step.getX());

    step = phaseTapChanger.getStep(3);
    ASSERT_DOUBLE_EQ(20.5, step.getAlpha());
    ASSERT_DOUBLE_EQ(20.0, step.getB());
    ASSERT_DOUBLE_EQ(21.0, step.getG());
    ASSERT_DOUBLE_EQ(22.0, step.getR());
    ASSERT_DOUBLE_EQ(23.0, step.getRho());
    ASSERT_DOUBLE_EQ(24.0, step.getX());
}

TEST(PhaseTapChanger, integrity) {
    const Network& network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger().get();

    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -1 [1, 3]");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setTapPosition(6), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 6 [1, 3]");
    PhaseTapChangerStep& step = phaseTapChanger.setTapPosition(3).getCurrentStep();
    ASSERT_EQ(3L, phaseTapChanger.getTapPosition());
    ASSERT_DOUBLE_EQ(20.5, step.getAlpha());
    ASSERT_DOUBLE_EQ(20.0, step.getB());
    ASSERT_DOUBLE_EQ(21.0, step.getG());
    ASSERT_DOUBLE_EQ(22.0, step.getR());
    ASSERT_DOUBLE_EQ(23.0, step.getRho());
    ASSERT_DOUBLE_EQ(24.0, step.getX());

    ASSERT_TRUE(stdcxx::areSame(step, step.setAlpha(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getAlpha()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setAlpha(26.0)));
    ASSERT_DOUBLE_EQ(26.0, step.getAlpha());

    ASSERT_TRUE(stdcxx::areSame(step, step.setB(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getB()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setB(26.0)));
    ASSERT_DOUBLE_EQ(26.0, step.getB());

    ASSERT_TRUE(stdcxx::areSame(step, step.setG(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getG()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setG(27.0)));
    ASSERT_DOUBLE_EQ(27.0, step.getG());

    ASSERT_TRUE(stdcxx::areSame(step, step.setR(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getR()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setR(28.0)));
    ASSERT_DOUBLE_EQ(28.0, step.getR());

    ASSERT_TRUE(stdcxx::areSame(step, step.setRho(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getRho()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setRho(29.0)));
    ASSERT_DOUBLE_EQ(29.0, step.getRho());

    ASSERT_TRUE(stdcxx::areSame(step, step.setX(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(step.getX()));
    ASSERT_TRUE(stdcxx::areSame(step, step.setX(30.0)));
    ASSERT_DOUBLE_EQ(30.0, step.getX());

    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7)), AssertionError, "Unexpected regulation mode value: 7");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");

    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationValue(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and regulated terminal is not set");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(phasetapchangertest::getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal2))));
    ASSERT_TRUE(stdcxx::areSame(terminal2, phaseTapChanger.getRegulationTerminal().get()));

    phaseTapChanger.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    //TODO(thiebarr) POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");
    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(phaseTapChanger.getRegulationValue()));
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationValue(-15.0)));
    ASSERT_DOUBLE_EQ(-15.0, phaseTapChanger.getRegulationValue());
    ASSERT_NO_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER).setRegulating(true));

    phaseTapChanger.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation terminal is null");
    POWSYBL_ASSERT_THROW(phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(phasetapchangertest::getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, phaseTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal))));
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));
    ASSERT_NO_THROW(phaseTapChanger.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER).setRegulating(true));

    ASSERT_NO_THROW(phaseTapChanger.remove());
    ASSERT_FALSE(transformer.getPhaseTapChanger());
}

TEST(PhaseTapChanger, adder) {
    const Network& network = createPhaseTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    ASSERT_TRUE(transformer.getPhaseTapChanger());
    transformer.getPhaseTapChanger().get().remove();
    ASSERT_FALSE(transformer.getPhaseTapChanger());
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

    ASSERT_NO_THROW(stepAdder.endStep());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -5 [0, 0]");
    adder.setLowTapPosition(-5L).setTapPosition(3L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 3 [-5, -5]");
    adder.setTapPosition(-5L).setRegulating(true);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation cannot be on if mode is FIXED");
    adder.setRegulationMode(static_cast<PhaseTapChanger::RegulationMode>(7));
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected regulation mode value: 7");

    adder.setRegulating(false).setRegulationMode(PhaseTapChanger::RegulationMode::FIXED_TAP);
    ASSERT_NO_THROW(adder.add());
    adder.setRegulationValue(stdcxx::nan());
    ASSERT_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));
    ASSERT_NO_THROW(adder.add());
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(phasetapchangertest::getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    ASSERT_NO_THROW(adder.add());
    transformer.getPhaseTapChanger().get().remove();
    ASSERT_FALSE(transformer.getPhaseTapChanger());

    adder.setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    adder.setRegulationValue(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and threshold/setpoint value is not set");
    adder.setRegulationValue(55.0);
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation is on and regulated terminal is not set");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(phasetapchangertest::getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': phase regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));
    ASSERT_NO_THROW(adder.add());
    ASSERT_TRUE(transformer.getPhaseTapChanger());
}

TEST(PhaseTapChanger, holder) {
    const Network& network = createPhaseTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger().get();
    PhaseTapChangerHolder& holder = dynamic_cast<PhaseTapChangerHolder&>(transformer);
    const PhaseTapChangerHolder& cHolder = holder;

    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, holder.getPhaseTapChanger().get()));
    ASSERT_TRUE(stdcxx::areSame(phaseTapChanger, cHolder.getPhaseTapChanger().get()));

    ASSERT_TRUE(holder.getPhaseTapChanger());
    phaseTapChanger.remove();
    ASSERT_FALSE(holder.getPhaseTapChanger());

    ASSERT_NO_THROW(holder.newPhaseTapChanger());
}

}  // namespace iidm

}  // namespace powsybl
