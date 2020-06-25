/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
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

#include <powsybl/test/AssertionUtils.hpp>

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
        .setRatedS(3.0)
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

BOOST_AUTO_TEST_SUITE(TwoWindingsTransformerTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createTwoWindingsTransformerTestNetwork();
    Substation& substation = network.getSubstation("S1");
    const Substation& cSubstation = substation;

    BOOST_CHECK_EQUAL(1UL, network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getTwoWindingsTransformers()));
    BOOST_CHECK_EQUAL(1UL, boost::size(substation.getTwoWindingsTransformers()));
    BOOST_CHECK_EQUAL(1UL, boost::size(cSubstation.getTwoWindingsTransformers()));

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getName());
    BOOST_CHECK_EQUAL(ConnectableType::TWO_WINDINGS_TRANSFORMER, transformer.getType());
    std::ostringstream oss;
    oss << transformer.getType();
    BOOST_CHECK_EQUAL("TWO_WINDINGS_TRANSFORMER", oss.str());
    BOOST_CHECK_CLOSE(3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, transformer.getRatedS(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(network, transformer.getNetwork()));
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));
    BOOST_TEST(!transformer.getRatioTapChanger());
    BOOST_TEST(!transformer.getPhaseTapChanger());

    const TwoWindingsTransformer& cTransformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");
    BOOST_TEST(stdcxx::areSame(transformer, cTransformer));
    BOOST_TEST(stdcxx::areSame(network, cTransformer.getNetwork()));
    BOOST_TEST(stdcxx::areSame(substation, cTransformer.getSubstation().get()));
    BOOST_TEST(!cTransformer.getRatioTapChanger());
    BOOST_TEST(!cTransformer.getPhaseTapChanger());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createTwoWindingsTransformerTestNetwork();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("2WT_VL1_VL2");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setR(100.0)));
    BOOST_CHECK_CLOSE(100.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setR(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': r is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setX(200.0)));
    BOOST_CHECK_CLOSE(200.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setX(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': x is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setG(300.0)));
    BOOST_CHECK_CLOSE(300.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setG(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': g is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setB(400.0)));
    BOOST_CHECK_CLOSE(400.0, transformer.getB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setB(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': b is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setRatedU1(500.0)));
    BOOST_CHECK_CLOSE(500.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setRatedU1(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': rated U1 is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setRatedU2(600.0)));
    BOOST_CHECK_CLOSE(600.0, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(transformer.setRatedU2(stdcxx::nan()), ValidationException, "2 windings transformer '2WT_VL1_VL2': rated U2 is invalid");

    BOOST_TEST(stdcxx::areSame(transformer, transformer.setRatedS(700.0)));
    BOOST_CHECK_CLOSE(700.0, transformer.getRatedS(), std::numeric_limits<double>::epsilon());
    transformer.setRatedS(stdcxx::nan());
    BOOST_TEST(std::isnan(transformer.getRatedS()));
    POWSYBL_ASSERT_THROW(transformer.setRatedS(-1.0), ValidationException, "2 windings transformer '2WT_VL1_VL2': Invalid rated S value: -1");

    transformer.remove();
    POWSYBL_ASSERT_THROW(network.getTwoWindingsTransformer("2WT_VL1_VL2"), PowsyblException, "Unable to find to the identifiable '2WT_VL1_VL2'");
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createTwoWindingsTransformerTestNetwork();
    Substation& substation2 = network.getSubstation("S2");
    unsigned long twoWindingsTransformerCount = network.getTwoWindingsTransformerCount();

    BOOST_CHECK_EQUAL(1UL, twoWindingsTransformerCount);

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

    adder.setRatedS(-1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "2 windings transformer '': Invalid rated S value: -1");
    adder.setRatedS(1.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "2 windings transformer id is not set");
    adder.setId("2WT_VL1_VL2");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'TwoWindingsTransformer' with the id '2WT_VL1_VL2'");
    adder.setName("2WT_VL3_VL4");
    adder.setEnsureIdUnicity(true);
    BOOST_CHECK_NO_THROW(adder.add());

    BOOST_CHECK_EQUAL(twoWindingsTransformerCount + 1, network.getTwoWindingsTransformerCount());
}

BOOST_AUTO_TEST_CASE(multivariant) {
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
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(!phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::ACTIVE_POWER_CONTROL, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(250.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, phaseTapChanger.getRegulationTerminal().get()));

    transformer.setR(-3.0).setRatedU1(-2.0).setRatedU2(-0.4).setX(-33.0).setG(-1.0).setB(-0.2);
    ratioTapChanger.setTapPosition(1L).setRegulating(false).setRegulationTerminal(stdcxx::ref<Terminal>(terminal2)).setTargetV(300.0);
    phaseTapChanger.setTapPosition(0L).setRegulating(true).setRegulationTerminal(stdcxx::ref<Terminal>(terminal)).setRegulationValue(400.0).setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER);

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(-3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(!ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(300.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(400.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant("s2");

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(-3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(!phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(250.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    transformer.setR(3.0).setRatedU1(2.0).setRatedU2(0.4).setX(33.0).setG(1.0).setB(0.2);
    ratioTapChanger.setTapPosition(3L).setRegulating(false).setTargetV(350.0);
    phaseTapChanger.setTapPosition(-3L).setRegulating(true).setRegulationValue(450.0);

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(!ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(350.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(450.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(25.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(-2L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(!phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(250.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");

    BOOST_CHECK_EQUAL("2WT_VL1_VL2", transformer.getId());
    BOOST_CHECK_CLOSE(3.0, transformer.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, transformer.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, transformer.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, transformer.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, transformer.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, transformer.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(substation, transformer.getSubstation().get()));

    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(3L, ratioTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(3, ratioTapChanger.getStepCount());
    BOOST_TEST(!ratioTapChanger.isRegulating());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_CHECK_CLOSE(350.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal2, ratioTapChanger.getRegulationTerminal().get()));

    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(0L, phaseTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(-3L, phaseTapChanger.getTapPosition());
    BOOST_CHECK_EQUAL(4, phaseTapChanger.getStepCount());
    BOOST_TEST(phaseTapChanger.isRegulating());
    BOOST_CHECK_EQUAL(PhaseTapChanger::RegulationMode::CURRENT_LIMITER, phaseTapChanger.getRegulationMode());
    BOOST_CHECK_CLOSE(450.0, phaseTapChanger.getRegulationValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(terminal, phaseTapChanger.getRegulationTerminal().get()));

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
