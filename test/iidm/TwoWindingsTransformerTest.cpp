/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerStep.hpp>
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

namespace powsybl {

namespace iidm {

Network createTwoWindingsTransformerTestNetwork() {
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
        .add();

    return network;
}

void addRatioTapChanger(TwoWindingsTransformer& transformer, Terminal& terminal) {
    transformer.newRatioTapChanger()
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
        .add();
}

void addPhaseTapChanger(TwoWindingsTransformer& transformer, Terminal& terminal) {
    transformer.newPhaseTapChanger()
        .setTapPosition(-2L)
        .setLowTapPosition(-3L)
        .beginStep()
        .setAlpha(99.0)
        .setB(100.0)
        .setG(101.0)
        .setR(102.0)
        .setRho(103.0)
        .setX(104.0)
        .endStep()
        .beginStep()
        .setAlpha(104.5)
        .setB(105.0)
        .setG(106.0)
        .setR(107.0)
        .setRho(108.0)
        .setX(109.0)
        .endStep()
        .beginStep()
        .setAlpha(200.5)
        .setB(200.0)
        .setG(201.0)
        .setR(202.0)
        .setRho(203.0)
        .setX(204.0)
        .endStep()
        .beginStep()
        .setAlpha(205.5)
        .setB(205.0)
        .setG(206.0)
        .setR(207.0)
        .setRho(208.0)
        .setX(209.0)
        .endStep()
        .setRegulationMode(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL)
        .setRegulating(false)
        .setRegulationTerminal(stdcxx::ref<Terminal>(terminal))
        .setRegulationValue(250.0)
        .add();
}

TEST(TwoWindingsTransformer, constructor) {
    const Network& network = createTwoWindingsTransformerTestNetwork();
    const Substation& substation = network.getSubstation("S1");

    ASSERT_EQ(1ul, network.getTwoWindingsTransformerCount());

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_EQ("", transformer.getName());
    ASSERT_EQ(ConnectableType::TWO_WINDINGS_TRANSFORMER, transformer.getType());
    ASSERT_DOUBLE_EQ(3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(network, transformer.getNetwork()));
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));
    ASSERT_FALSE(transformer.getRatioTapChanger());
    ASSERT_FALSE(transformer.getPhaseTapChanger());

    const TwoWindingsTransformer& cTransformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    ASSERT_TRUE(stdcxx::areSame(transformer, cTransformer));
    ASSERT_TRUE(stdcxx::areSame(network, cTransformer.getNetwork()));
    ASSERT_TRUE(stdcxx::areSame(substation, cTransformer.getSubstation().get()));
    ASSERT_FALSE(cTransformer.getRatioTapChanger());
    ASSERT_FALSE(cTransformer.getPhaseTapChanger());
}

TEST(TwoWindingsTransformer, integrity) {
    const Network& network = createTwoWindingsTransformerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setR(100.0)));
    ASSERT_DOUBLE_EQ(100.0, transformer.getR());
    POWSYBL_ASSERT_THROW(transformer.setR(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': r is invalid");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setX(200.0)));
    ASSERT_DOUBLE_EQ(200.0, transformer.getX());
    POWSYBL_ASSERT_THROW(transformer.setX(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': x is invalid");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setG(300.0)));
    ASSERT_DOUBLE_EQ(300.0, transformer.getG());
    POWSYBL_ASSERT_THROW(transformer.setG(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': g is invalid");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setB(400.0)));
    ASSERT_DOUBLE_EQ(400.0, transformer.getB());
    POWSYBL_ASSERT_THROW(transformer.setB(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': b is invalid");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setRatedU1(500.0)));
    ASSERT_DOUBLE_EQ(500.0, transformer.getRatedU1());
    POWSYBL_ASSERT_THROW(transformer.setRatedU1(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': rated U1 is invalid");

    ASSERT_TRUE(stdcxx::areSame(transformer, transformer.setRatedU2(600.0)));
    ASSERT_DOUBLE_EQ(600.0, transformer.getRatedU2());
    POWSYBL_ASSERT_THROW(transformer.setRatedU2(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': rated U2 is invalid");

    transformer.remove();
    POWSYBL_ASSERT_THROW(network.getTwoWindingsTransformer("2WT_VL1_VL2"), PowsyblException, "Unable to find to the identifiable '2WT_VL1_VL2'");
}

TEST(TwoWindingsTransformer, adder) {
    Network network = createTwoWindingsTransformerTestNetwork();
    Substation& substation2 = network.getSubstation("S2");
    unsigned long twoWindingsTransformerCount = network.getTwoWindingsTransformerCount();

    ASSERT_EQ(1ul, twoWindingsTransformerCount);

    TwoWindingsTransformerAdder adder = substation2.newTwoWindingsTransformer();

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': First voltage level is not set");
    adder.setVoltageLevel1("INVALID");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': First voltage level 'INVALID' not found");
    adder.setVoltageLevel1("VL1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': Second voltage level is not set");
    adder.setVoltageLevel2("INVALID");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': Second voltage level 'INVALID' not found");
    adder.setVoltageLevel2("VL2");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': the 2 windings of the transformer shall belong to the substation 'S2' ('S1', 'S1')");
    adder.setVoltageLevel1("VL3");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': the 2 windings of the transformer shall belong to the substation 'S2' ('S2', 'S1')");
    adder.setVoltageLevel2("VL4");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': connectable bus is not set");
    adder.setBus1("VL3_BUS1");
    adder.setConnectableBus1("INVALID");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': connection bus is different from connectable bus");
    adder.setConnectableBus1("");

    TwoWindingsTransformerAdder adder2 = adder;
    adder2.setNode1(0);
    POWSYBL_ASSERT_THROW(adder2.add(), ValidationException, "2 windings transformer '': connection node and connection bus are exclusives");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': connectable bus is not set");
    adder.setBus1("");
    adder.setConnectableBus1("VL3_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': connectable bus is not set");
    adder.setBus2("VL4_BUS1");
    adder.setConnectableBus2("INVALID");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': connection bus is different from connectable bus");
    adder.setConnectableBus2("");

    TwoWindingsTransformerAdder adder3 = adder;
    adder3.setNode2(0);
    POWSYBL_ASSERT_THROW(adder3.add(), ValidationException, "2 windings transformer '': connection node and connection bus are exclusives");

    adder.setBus2("");
    adder.setConnectableBus2("VL4_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': r is invalid");
    adder.setR(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': r is invalid");
    adder.setR(100.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': x is invalid");
    adder.setX(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': x is invalid");
    adder.setX(200.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': g is invalid");
    adder.setG(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': g is invalid");
    adder.setG(300.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': b is invalid");
    adder.setB(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': b is invalid");
    adder.setB(400.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': rated U1 is invalid");
    adder.setRatedU1(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': rated U1 is invalid");
    adder.setRatedU1(500.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': rated U2 is invalid");
    adder.setRatedU2(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': rated U2 is invalid");
    adder.setRatedU2(600.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Invalid id");
    adder.setId("2WT_VL1_VL2");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object '2WT_VL1_VL2' already exists (powsybl::iidm::TwoWindingsTransformer)");
    adder.setName("2WT_VL3_VL4");
    adder.setId("UNIQUE_2WT_ID");
    ASSERT_NO_THROW(adder.add());

    ASSERT_EQ(twoWindingsTransformerCount + 1, network.getTwoWindingsTransformerCount());
}

TEST(TwoWindingsTransformer, multivariant) {
    Network network = createTwoWindingsTransformerTestNetwork();
    const Substation& substation = network.getSubstation("S1");

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    Terminal& terminal2 = network.getLoad("LOAD2").getTerminal();
    addRatioTapChanger(transformer, terminal);
    addPhaseTapChanger(transformer, terminal2);
    RatioTapChanger& ratioTapChanger = transformer.getRatioTapChanger().get();
    PhaseTapChanger& phaseTapChanger = transformer.getPhaseTapChanger().get();

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(2L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_TRUE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(25.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(-2L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_FALSE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(250.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal2, phaseTapChanger.getRegulationTerminal().get()));

    transformer.setR(-3.0).setRatedU1(-2.0).setRatedU2(-0.4).setX(-33.0).setG(-1.0).setB(-0.2);
    ratioTapChanger.setTapPosition(1L).setRegulating(false).setRegulationTerminal(stdcxx::ref<Terminal>(terminal2)).setTargetV(300.0);
    phaseTapChanger.setTapPosition(0L).setRegulating(true).setRegulationTerminal(stdcxx::ref<Terminal>(terminal)).setRegulationValue(400.0).setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER);

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(-3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(-33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(-1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(-0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(-2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(-0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(1L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_FALSE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(300.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_TRUE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(400.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant("s2");

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(-3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(-33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(-1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(-0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(-2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(-0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(2L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_TRUE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(25.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(-2L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_FALSE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(250.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    transformer.setR(3.0).setRatedU1(2.0).setRatedU2(0.4).setX(33.0).setG(1.0).setB(0.2);
    ratioTapChanger.setTapPosition(3L).setRegulating(false).setTargetV(350.0);
    phaseTapChanger.setTapPosition(-3L).setRegulating(true).setRegulationValue(450.0);

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_FALSE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(350.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(-3L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_TRUE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(450.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(2L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_TRUE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(25.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(-2L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_FALSE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(250.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");

    ASSERT_EQ("2WT_VL1_VL2", transformer.getId());
    ASSERT_DOUBLE_EQ(3.0, transformer.getR());
    ASSERT_DOUBLE_EQ(33.0, transformer.getX());
    ASSERT_DOUBLE_EQ(1.0, transformer.getG());
    ASSERT_DOUBLE_EQ(0.2, transformer.getB());
    ASSERT_DOUBLE_EQ(2.0, transformer.getRatedU1());
    ASSERT_DOUBLE_EQ(0.4, transformer.getRatedU2());
    ASSERT_TRUE(stdcxx::areSame(substation, transformer.getSubstation().get()));

    ASSERT_EQ(1L, ratioTapChanger.getLowTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getHighTapPosition());
    ASSERT_EQ(3L, ratioTapChanger.getTapPosition());
    ASSERT_EQ(3, ratioTapChanger.getStepCount());
    ASSERT_FALSE(ratioTapChanger.isRegulating());
    ASSERT_TRUE(ratioTapChanger.hasLoadTapChangingCapabilities());
    ASSERT_DOUBLE_EQ(350.0, ratioTapChanger.getTargetV());
    ASSERT_TRUE(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    ASSERT_EQ(-3L, phaseTapChanger.getLowTapPosition());
    ASSERT_EQ(0L, phaseTapChanger.getHighTapPosition());
    ASSERT_EQ(-3L, phaseTapChanger.getTapPosition());
    ASSERT_EQ(4, phaseTapChanger.getStepCount());
    ASSERT_TRUE(phaseTapChanger.isRegulating());
    ASSERT_EQ(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    ASSERT_DOUBLE_EQ(450.0, phaseTapChanger.getRegulationValue());
    ASSERT_TRUE(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

}  // namespace iidm

}  // namespace powsybl
