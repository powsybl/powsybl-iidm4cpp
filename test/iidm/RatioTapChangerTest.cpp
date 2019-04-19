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
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerStep.hpp>
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

Network createRatioTapChangerTestNetwork() {
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
        .newRatioTapChanger()
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
        .setRegulationTerminal(stdcxx::ref<Terminal>(l1.getTerminal()))
        .setTargetV(25.0)
        .add();

    return network;
}

TEST(RatioTapChanger, constructor) {
    const Network& network = createRatioTapChangerTestNetwork();
    const Terminal& terminal = network.getLoad("LOAD1").getTerminal();

    RatioTapChanger& ratioTapChanger = network.getTwoWindingsTransformer("2WT_VL1_VL2").getRatioTapChanger().get();
    const RatioTapChanger& cRatioTapChanger = ratioTapChanger;
    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(2L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_TRUE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(25.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal, ratioTapChanger.getRegulationTerminal().get()));
    ASSERT_TRUE(stdcxx::areSame(terminal, cRatioTapChanger.getRegulationTerminal().get()));


    RatioTapChangerStep& step = ratioTapChanger.getCurrentStep();
    ASSERT_TRUE(stdcxx::areSame(step, cRatioTapChanger.getCurrentStep()));
    ASSERT_TRUE(stdcxx::areSame(step, ratioTapChanger.getStep(2)));
    ASSERT_TRUE(stdcxx::areSame(step, cRatioTapChanger.getStep(2)));
    ASSERT_DOUBLE_EQ(15.0, step.getB());
    ASSERT_DOUBLE_EQ(16.0, step.getG());
    ASSERT_DOUBLE_EQ(17.0, step.getR());
    ASSERT_DOUBLE_EQ(18.0, step.getRho());
    ASSERT_DOUBLE_EQ(19.0, step.getX());

    POWSYBL_ASSERT_THROW(ratioTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(cRatioTapChanger.getStep(0), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 0 [1, 3]");
    POWSYBL_ASSERT_THROW(ratioTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");
    POWSYBL_ASSERT_THROW(cRatioTapChanger.getStep(5), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 5 [1, 3]");

    step = ratioTapChanger.getStep(1);
    ASSERT_DOUBLE_EQ(10.0, step.getB());
    ASSERT_DOUBLE_EQ(11.0, step.getG());
    ASSERT_DOUBLE_EQ(12.0, step.getR());
    ASSERT_DOUBLE_EQ(13.0, step.getRho());
    ASSERT_DOUBLE_EQ(14.0, step.getX());

    step = ratioTapChanger.getStep(3);
    ASSERT_DOUBLE_EQ(20.0, step.getB());
    ASSERT_DOUBLE_EQ(21.0, step.getG());
    ASSERT_DOUBLE_EQ(22.0, step.getR());
    ASSERT_DOUBLE_EQ(23.0, step.getRho());
    ASSERT_DOUBLE_EQ(24.0, step.getX());
}

TEST(RatioTapChanger, integrity) {
    const Network& network = createRatioTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    RatioTapChanger& ratioTapChanger = transformer.getRatioTapChanger().get();

    POWSYBL_ASSERT_THROW(ratioTapChanger.setTapPosition(-1), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position -1 [1, 3]");
    POWSYBL_ASSERT_THROW(ratioTapChanger.setTapPosition(6), ValidationException, "2 windings transformer '2WT_VL1_VL2': incorrect tap position 6 [1, 3]");
    RatioTapChangerStep& step = ratioTapChanger.setTapPosition(3).getCurrentStep();
    ASSERT_EQ(3L, ratioTapChanger.getTapPosition());
    ASSERT_DOUBLE_EQ(20.0, step.getB());
    ASSERT_DOUBLE_EQ(21.0, step.getG());
    ASSERT_DOUBLE_EQ(22.0, step.getR());
    ASSERT_DOUBLE_EQ(23.0, step.getRho());
    ASSERT_DOUBLE_EQ(24.0, step.getX());

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

    POWSYBL_ASSERT_THROW(ratioTapChanger.setTargetV(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': a target voltage has to be set for a regulating ratio tap changer");
    POWSYBL_ASSERT_THROW(ratioTapChanger.setTargetV(-15.0), ValidationException, "2 windings transformer '2WT_VL1_VL2': bad target voltage -15");
    ratioTapChanger.setRegulating(false);
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, ratioTapChanger.setTargetV(stdcxx::nan())));
    ASSERT_TRUE(std::isnan(ratioTapChanger.getTargetV()));
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': a target voltage has to be set for a regulating ratio tap changer");
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, ratioTapChanger.setTargetV(-15.0)));
    ASSERT_DOUBLE_EQ(-15.0, ratioTapChanger.getTargetV());
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulating(true), ValidationException, "2 windings transformer '2WT_VL1_VL2': bad target voltage -15");
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, ratioTapChanger.setTargetV(31.0)));
    ASSERT_DOUBLE_EQ(31.0, ratioTapChanger.getTargetV());
    ASSERT_NO_THROW(ratioTapChanger.setRegulating(true));

    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': a regulation terminal has to be set for a regulating ratio tap changer");
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation terminal is not part of the network");
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, ratioTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal2))));
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ratioTapChanger.setRegulating(false);
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulationTerminal(stdcxx::Reference<Terminal>()), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation terminal is null");
    POWSYBL_ASSERT_THROW(ratioTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation terminal is not part of the network");
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, ratioTapChanger.setRegulationTerminal(stdcxx::ref<Terminal>(terminal))));
    ASSERT_TRUE(stdcxx::areSame(terminal, ratioTapChanger.getRegulationTerminal().get()));
    ASSERT_NO_THROW(ratioTapChanger.setRegulating(true));

    ASSERT_NO_THROW(ratioTapChanger.remove());
    ASSERT_FALSE(transformer.getRatioTapChanger());
}

TEST(RatioTapChanger, adder) {
    const Network& network = createRatioTapChangerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    ASSERT_TRUE(transformer.getRatioTapChanger());
    transformer.getRatioTapChanger().get().remove();
    ASSERT_FALSE(transformer.getRatioTapChanger());
    RatioTapChangerAdder adder = transformer.newRatioTapChanger();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': tap position is not set");
    adder.setTapPosition(-5L);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': ratio tap changer should have at least one step");

    auto stepAdder = adder.beginStep();
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
    adder.setTapPosition(-5L).setRegulating(true).setLoadTapChangingCapabilities(true);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a target voltage has to be set for a regulating ratio tap changer");
    adder.setTargetV(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a target voltage has to be set for a regulating ratio tap changer");
    adder.setTargetV(-1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': bad target voltage -1");
    adder.setTargetV(6.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a regulation terminal has to be set for a regulating ratio tap changer");
    adder.setRegulationTerminal(stdcxx::Reference<Terminal>());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': a regulation terminal has to be set for a regulating ratio tap changer");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '2WT_VL1_VL2': regulation terminal is not part of the network");
    adder.setRegulationTerminal(stdcxx::ref<Terminal>(network.getLoad("LOAD1").getTerminal()));
    ASSERT_NO_THROW(adder.add());
    ASSERT_TRUE(transformer.getRatioTapChanger());
}

TEST(RatioTapChanger, holder) {
    const Network& network = createRatioTapChangerTestNetwork();
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    const TwoWindingsTransformer& cTransformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    RatioTapChanger& ratioTapChanger = transformer.getRatioTapChanger().get();

    ASSERT_TRUE(stdcxx::areSame(network, transformer.getNetwork()));
    ASSERT_TRUE(stdcxx::areSame(network, cTransformer.getNetwork()));

    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, transformer.getRatioTapChanger().get()));
    ASSERT_TRUE(stdcxx::areSame(ratioTapChanger, cTransformer.getRatioTapChanger().get()));

    ASSERT_TRUE(transformer.getRatioTapChanger());
    ratioTapChanger.remove();
    ASSERT_FALSE(transformer.getRatioTapChanger());

    ASSERT_NO_THROW(transformer.newRatioTapChanger());
}

}  // namespace iidm

}  // namespace powsybl
