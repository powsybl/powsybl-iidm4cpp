/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/LoadExponentialModel.hpp>
#include <powsybl/iidm/LoadZipModel.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>
#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerStep.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/ReactiveCapabilityCurve.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorLinearModel.hpp>
#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageAngleLimit.hpp>
#include <powsybl/iidm/VoltageAngleLimitAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(PropertiesXmlTestSuite)

static const char* const TEST_PROPERTY_ADDER = "testAdder";
static const char* const TEST_PROPERTY = "test";

Network createNetworkWithProperties() {
    //Create a network with all kind of PrpertiesHolder object:
    Network network("network", "test");
    network.setProperty(TEST_PROPERTY, "valueNetwork");
    Substation& substation1 = network.newSubstation()
        .setId("substation1")
        .setCountry(Country::FR)
        .setTso("TSO1")
        .setGeographicalTags({"region1"})
        .add();
    substation1.setProperty(TEST_PROPERTY,"valueSubstation1");
    VoltageLevel& voltageLevel1 = substation1.newVoltageLevel()
        .setId("voltageLevel1")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    voltageLevel1.setProperty(TEST_PROPERTY,"valueVoltageLevel1");
    VoltageLevel& voltageLevel2 = substation1.newVoltageLevel()
        .setId("voltageLevel2")
        .setNominalV(402)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    voltageLevel2.setProperty(TEST_PROPERTY,"valueVoltageLevel2");
    VoltageLevel& voltageLevel3 = substation1.newVoltageLevel()
        .setId("voltageLevel3")
        .setNominalV(403)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    voltageLevel3.setProperty(TEST_PROPERTY,"valueVoltageLevel3");

    VoltageLevel::NodeBreakerView& topology1 = voltageLevel1.getNodeBreakerView();
    BusbarSection& voltageLevel1BusbarSection1 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection1")
        .setNode(0)
        .add();
    BusbarSection& voltageLevel1BusbarSection2 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection2")
        .setNode(1)
        .add();
    topology1.newBreaker()
        .setId("voltageLevel1Breaker1")
        .setRetained(true)
        .setOpen(false)
        .setNode1(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    Load& load1 = voltageLevel1.newLoad()
        .setId("load1")
        .setNode(2)
        .setP0(10)
        .setQ0(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Disconnector1")
        .setOpen(false)
        .setNode1(load1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Breaker1")
        .setOpen(false)
        .setNode1(3)
        .setNode2(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .add();

    Generator& generator1 = voltageLevel1.newGenerator()
        .setId("generator1")
        .setEnergySource(EnergySource::NUCLEAR)
        .setMinP(200.0)
        .setMaxP(900.0)
        .setVoltageRegulatorOn(true)
        .setTargetP(900.0)
        .setTargetV(380.0)
        .setNode(5)
        .add();
    generator1.setProperty(TEST_PROPERTY,"valueGenerator1");

    Generator& generator2 = voltageLevel1.newGenerator()
        .setId("generator2")
        .setEnergySource(EnergySource::HYDRO)
        .setMinP(200.0)
        .setMaxP(900.0)
        .setVoltageRegulatorOn(true)
        .setTargetP(900.0)
        .setTargetV(380.0)
        .setNode(6)
        .add();
    generator2.setProperty(TEST_PROPERTY,"valueGenerator2");

    //ReactiveLimits :
    ReactiveCapabilityCurveAdder rcCurveAdder = generator1.newReactiveCapabilityCurve();
    ReactiveCapabilityCurveAdder::PointAdder pointAdder1 = rcCurveAdder.beginPoint().setP(200.0).setMinQ(300.0).setMaxQ(500.0);
    pointAdder1.setProperty(TEST_PROPERTY_ADDER,"valuePoint1Adder");
    pointAdder1.endPoint();
    ReactiveCapabilityCurveAdder::PointAdder pointAdder2 = rcCurveAdder.beginPoint().setP(900.0).setMinQ(300.0).setMaxQ(500.0);
    pointAdder2.setProperty(TEST_PROPERTY_ADDER,"valuePoint2Adder");
    pointAdder2.endPoint();
    rcCurveAdder.add();
    
    MinMaxReactiveLimitsAdder minMaxReactiveLimitsAdder = generator2.newMinMaxReactiveLimits().setMinQ(-9999.99).setMaxQ(9999.99);
    minMaxReactiveLimitsAdder.setProperty(TEST_PROPERTY_ADDER,"valueMinMaxReactiveLimitsAdder");
    minMaxReactiveLimitsAdder.add();

    ReactiveCapabilityCurve& reactiveCapabilityCurve = generator1.getReactiveLimits<ReactiveCapabilityCurve>();
    MinMaxReactiveLimits& minMaxReactiveLimits = generator2.getReactiveLimits<MinMaxReactiveLimits>();

    reactiveCapabilityCurve.setProperty(TEST_PROPERTY,"valueCurveReactiveLimits");
    unsigned long index = 1;
    for(auto& point : reactiveCapabilityCurve.getPoints()) {
        point.setProperty(TEST_PROPERTY, stdcxx::format("valuePoint%1%",index++));
    }
    minMaxReactiveLimits.setProperty(TEST_PROPERTY, "valueMinMaxReactiveLimits");

    //Load and LoadModel
    LoadAdder zipLoadAdder = voltageLevel1.newLoad()
                .setId("zipLoad")
                .setNode(3)
                .setP0(10)
                .setQ0(3);
    LoadZipModelAdder zipModelAdder = zipLoadAdder.newZipModel().setC0p(0.5).setC0q(0.25).setC1p(0.25).setC1q(0.25).setC2p(0.25).setC2q(0.5);
    zipModelAdder.setProperty(TEST_PROPERTY_ADDER,"valueZipLoadModelAdder");
    zipModelAdder.add();
    Load& zipLoad = zipLoadAdder.add();
    zipLoad.setProperty(TEST_PROPERTY, "valueZipLoad");
    zipLoad.getModel().setProperty(TEST_PROPERTY, "valueZipLoadModel");

    LoadAdder expLoadAdder = voltageLevel1.newLoad()
                .setId("expLoad")
                .setNode(4)
                .setP0(10)
                .setQ0(3);
    LoadExponentialModelAdder expModelAdder = expLoadAdder.newExponentialModel();
    expModelAdder.setProperty(TEST_PROPERTY_ADDER,"valueExpLoadModelAdder");
    expModelAdder.add();
    Load& expLoad = expLoadAdder.add();
    expLoad.setProperty(TEST_PROPERTY, "valueExpLoad");
    expLoad.getModel().setProperty(TEST_PROPERTY, "valueExpLoadModel");

    //ShuntCompensator and ShuntCompensatorModels
    ShuntCompensator& shuntCompensator = voltageLevel1.newShuntCompensator()
                .setId("shunt")
                .setNode(7)
                .setSectionCount(1)
                .setVoltageRegulatorOn(true)
                .setRegulatingTerminal(stdcxx::ref(zipLoad.getTerminal()))
                .setTargetV(200)
                .setTargetDeadband(5.0)
                .newLinearModel()
                .setMaximumSectionCount(1)
                .setBPerSection(3)
                .add()
                .add();
    shuntCompensator.setProperty(TEST_PROPERTY, "valueShunt");
    auto& linearModel = shuntCompensator.getModel<ShuntCompensatorLinearModel>();
    linearModel.setProperty(TEST_PROPERTY, "valueLinearModel");

    ShuntCompensator& nonLinearShuntCompensator = voltageLevel1.newShuntCompensator()
                .setId("shuntNonLinear")
                .setNode(8)
                .setSectionCount(1)
                .setVoltageRegulatorOn(true)
                .setRegulatingTerminal(stdcxx::ref(zipLoad.getTerminal()))
                .setTargetV(200)
                .setTargetDeadband(5.0)
                .newNonLinearModel()
                .beginSection()
                .setB(1.0)
                .setG(2.0).endSection()
                .add()
                .add();
    nonLinearShuntCompensator.setProperty(TEST_PROPERTY, "valueShuntNonLinear");
    auto& nonLinearModel = nonLinearShuntCompensator.getModel<ShuntCompensatorNonLinearModel>();
    nonLinearModel.setProperty(TEST_PROPERTY, "valueNonLinearModel");
    index = 1;
    for (ShuntCompensatorNonLinearModel::Section& section : nonLinearModel.getAllSections()) {
        section.setProperty(TEST_PROPERTY, stdcxx::format("valueSection%1%",index++));
    }

    //Area and AreaBoundary
    Area& defaultControlArea = network.newArea().setId("defaultControlArea").setAreaType("areaTypeTest").add();
    defaultControlArea.setProperty(TEST_PROPERTY, "valueArea");
    defaultControlArea.newAreaBoundary().setTerminal(load1.getTerminal()).setAc(true).add();
    std::shared_ptr<AreaBoundary> areaBoundaryPtr = defaultControlArea.getAreaBoundary(load1.getTerminal());
    areaBoundaryPtr->setProperty(TEST_PROPERTY, "valueAreaBoundary");

    //Line
    Line& line = network.newLine().setId("line")
        .setVoltageLevel1(voltageLevel1.getId())
        .setNode1(11)
        .setVoltageLevel2(voltageLevel2.getId())
        .setNode2(11)
        .setR(1.0)
        .setX(1.0)
        .setG1(0)
        .setB1(0)
        .setG2(0)
        .setB2(0).add();
    line.setProperty(TEST_PROPERTY, "valueLine");

    //ThreeWindingsTransformer
    ThreeWindingsTransformer& transformer = substation1.newThreeWindingsTransformer().setId("transformer_3wt")
         .setName("transformer_3wt")
         .newLeg1()
            .setR(1.3)
            .setX(1.4)
            .setG(1.6)
            .setB(1.7)
            .setRatedU(1.1)
            .setRatedS(2.2)
            .setVoltageLevel(voltageLevel1.getId())
            .setNode(10)
            .add()
         .newLeg2()
            .setR(2.3)
            .setX(2.4)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(2.1)
            .setVoltageLevel(voltageLevel2.getId())
            .setNode(10)
            .add()
         .newLeg3()
            .setR(3.3)
            .setX(3.4)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(3.1)
            .setVoltageLevel(voltageLevel3.getId())
            .setNode(10)
            .add()
         .add();
    transformer.setProperty(TEST_PROPERTY, "value3WT");

    //OverloadManagementSystem and Trippings
    OverloadManagementSystem& oms = substation1.newOverloadManagementSystem()
                .setId("OMS")
                .setEnabled(false)
                .setMonitoredElementId("line")
                .setMonitoredElementSide(ThreeSides::TWO)
                .newSwitchTripping()
                    ->setSwitchToOperateId("voltageLevel1Breaker1")
                    .setKey("switchTripping")
                    .setName("switchTripping")
                    .setCurrentLimit(100.)
                    .setOpenAction(true)
                .add()
                .newBranchTripping() 
                    ->setBranchToOperateId("line")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("branchTripping")
                    .setName("branchTripping")
                    .setCurrentLimit(100.)
                    .setOpenAction(true)
                .add()
                .newThreeWindingsTransformerTripping()
                    ->setThreeWindingsTransformerToOperateId("transformer_3wt")
                    .setSideToOperate(ThreeSides::THREE)
                    .setKey("3wtTripping")
                    .setName("3wtTripping")
                    .setCurrentLimit(100.)
                    .setOpenAction(true)
                .add()
                .add();
    oms.setProperty(TEST_PROPERTY, "valueOMS");
    std::vector<std::reference_wrapper<OverloadManagementSystem::Tripping>> trippings = oms.getTrippings();
    index = 0;
    for (auto& tripping : trippings){
        tripping.get().setProperty(TEST_PROPERTY,stdcxx::format("valueTripping%1%",index++));
    }

    
    //TapChangers and TapChangers Steps
    transformer.getLeg2().newPhaseTapChanger()
                .setTapPosition(1)
                .setRegulating(false)
                .setRegulationMode(PhaseTapChanger::RegulationMode::CURRENT_LIMITER)
                .setLowTapPosition(0)
                .beginStep().setAlpha(1).setRho(2).setR(3).setG(4).setB(5).setX(6)
                .endStep()
                .beginStep().setAlpha(20).setRho(30).setR(40).setG(50).setB(60).setX(70)
                .endStep()
                .beginStep().setAlpha(300).setRho(400).setR(500).setG(600).setB(700).setX(800)
                .endStep()
                .add();
    PhaseTapChanger& phaseTapChanger = transformer.getLeg2().getPhaseTapChanger();
    phaseTapChanger.setProperty(TEST_PROPERTY, "valuePhaseTapChanger");
    phaseTapChanger.getCurrentStep().setProperty(TEST_PROPERTY, "valuePhaseTapChangerStep");


    transformer.getLeg3().newRatioTapChanger()
                .setLowTapPosition(0)
                .setTapPosition(1)
                .setLoadTapChangingCapabilities(false)
                .beginStep()
                .setR(39.78473)
                .setX(39.784725)
                .setG(0.0)
                .setB(0.0)
                .setRho(1.0)
                .endStep()
                .beginStep()
                .setR(39.78474)
                .setX(39.784726)
                .setG(0.0)
                .setB(0.0)
                .setRho(1.0)
                .endStep()
                .beginStep()
                .setR(39.78475)
                .setX(39.784727)
                .setG(0.0)
                .setB(0.0)
                .setRho(1.0)
                .endStep()
                .add();
    RatioTapChanger& ratioTapChanger = transformer.getLeg3().getRatioTapChanger();
    ratioTapChanger.setProperty(TEST_PROPERTY, "valueRatioTapChanger");
    ratioTapChanger.getCurrentStep().setProperty(TEST_PROPERTY, "valueRatioTapChangerStep");

    //VoltageAngleLimit
    VoltageAngleLimit& voltageAngleLimit = network.newVoltageAngleLimit().setId("voltageAngleLimit").setLowLimit(0).setHighLimit(100)
            .from(stdcxx::ref<Terminal>(transformer.getLeg1().getTerminal()))
            .to(stdcxx::ref<Terminal>(transformer.getLeg1().getTerminal()))
            .add();
    voltageAngleLimit.setProperty(TEST_PROPERTY, "valueVoltageAngleLimit");

    //LoadingLmits
    ActivePowerLimitsAdder activePowerLimitsAdder = transformer.getLeg1().newActivePowerLimits();
    activePowerLimitsAdder.setPermanentLimit(350)
                .beginTemporaryLimit()
                .setValue(370)
                .setAcceptableDuration(20 * 60)
                .setName("20'")
                .addProperty(TEST_PROPERTY, "valueTemporaryLimit")
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setValue(380)
                .setAcceptableDuration(10 * 60)
                .setName("10'")
                .endTemporaryLimit();
    activePowerLimitsAdder.setProperty(TEST_PROPERTY, "valueActivePowerLimits");
    activePowerLimitsAdder.add();

    ApparentPowerLimitsAdder apparentPowerLimitsAdder = transformer.getLeg1().newApparentPowerLimits();
    apparentPowerLimitsAdder.setPermanentLimit(350)
                .beginTemporaryLimit()
                .setValue(370)
                .setAcceptableDuration(20 * 60)
                .setName("20'")
                .addProperty(TEST_PROPERTY, "valueTemporaryLimit")
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setValue(380)
                .setAcceptableDuration(10 * 60)
                .setName("10'")
                .endTemporaryLimit();
    apparentPowerLimitsAdder.setProperty(TEST_PROPERTY, "valueApparentPowerLimits");
    apparentPowerLimitsAdder.add();

    CurrentLimitsAdder currentLimitsAdder = transformer.getLeg1().newCurrentLimits();
    currentLimitsAdder.setPermanentLimit(350)
                .beginTemporaryLimit()
                .setValue(370)
                .setAcceptableDuration(20 * 60)
                .setName("20'")
                .addProperty(TEST_PROPERTY, "valueTemporaryLimit")
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setValue(380)
                .setAcceptableDuration(10 * 60)
                .setName("10'")
                .endTemporaryLimit();
    currentLimitsAdder.setProperty(TEST_PROPERTY, "valueCurrentLimits");
    currentLimitsAdder.add();

    return network;
}

void checkProperties(const Network& network) {
    BOOST_CHECK_EQUAL(network.getProperty(TEST_PROPERTY), "valueNetwork");
    BOOST_CHECK_EQUAL(boost::size(network.getPropertyNames()), 1);

    const auto& substation1 = network.getSubstation("substation1");
    BOOST_CHECK_EQUAL(substation1.getProperty(TEST_PROPERTY), "valueSubstation1");
    BOOST_CHECK_EQUAL(boost::size(substation1.getPropertyNames()), 1);


    const auto& vl1 = network.getVoltageLevel("voltageLevel1");
    BOOST_CHECK_EQUAL(vl1.getProperty(TEST_PROPERTY), "valueVoltageLevel1");
    BOOST_CHECK_EQUAL(boost::size(vl1.getPropertyNames()), 1);
    const auto& vl2 = network.getVoltageLevel("voltageLevel2");
    BOOST_CHECK_EQUAL(vl2.getProperty(TEST_PROPERTY), "valueVoltageLevel2");
    BOOST_CHECK_EQUAL(boost::size(vl2.getPropertyNames()), 1);
    const auto& vl3 = network.getVoltageLevel("voltageLevel3");
    BOOST_CHECK_EQUAL(vl3.getProperty(TEST_PROPERTY), "valueVoltageLevel3");
    BOOST_CHECK_EQUAL(boost::size(vl3.getPropertyNames()), 1);

    const auto& gen1 = network.getGenerator("generator1");
    BOOST_CHECK_EQUAL(gen1.getProperty(TEST_PROPERTY), "valueGenerator1");
    BOOST_CHECK_EQUAL(boost::size(gen1.getPropertyNames()), 1);
    const auto& gen2 = network.getGenerator("generator2");
    BOOST_CHECK_EQUAL(gen2.getProperty(TEST_PROPERTY), "valueGenerator2");
    BOOST_CHECK_EQUAL(boost::size(gen2.getPropertyNames()), 1);

    const auto& curveReactiveLimits = gen1.getReactiveLimits<ReactiveCapabilityCurve>();
    BOOST_CHECK_EQUAL(curveReactiveLimits.getProperty(TEST_PROPERTY), "valueCurveReactiveLimits");
    BOOST_CHECK_EQUAL(boost::size(curveReactiveLimits.getPropertyNames()), 1);
    unsigned long index = 1;
    for(const auto& point : curveReactiveLimits.getPoints()) {
        BOOST_CHECK_EQUAL(point.getProperty(TEST_PROPERTY), stdcxx::format("valuePoint%1%",index));
        BOOST_CHECK_EQUAL(point.getProperty(TEST_PROPERTY_ADDER), stdcxx::format("valuePoint%1%Adder",index));
        BOOST_CHECK_EQUAL(boost::size(point.getPropertyNames()), 2);
        index++;
    }
    const auto& minMaxReactiveLimits = gen2.getReactiveLimits<MinMaxReactiveLimits>();
    BOOST_CHECK_EQUAL(minMaxReactiveLimits.getProperty(TEST_PROPERTY), "valueMinMaxReactiveLimits");
    BOOST_CHECK_EQUAL(minMaxReactiveLimits.getProperty(TEST_PROPERTY_ADDER), "valueMinMaxReactiveLimitsAdder");
    BOOST_CHECK_EQUAL(boost::size(minMaxReactiveLimits.getPropertyNames()), 2);

    const auto& zipLoad = network.getLoad("zipLoad");
    BOOST_CHECK_EQUAL(zipLoad.getProperty(TEST_PROPERTY), "valueZipLoad");
    BOOST_CHECK_EQUAL(boost::size(zipLoad.getPropertyNames()), 1);
    const auto& zipLoadModel = zipLoad.getModel();
    BOOST_CHECK_EQUAL(zipLoadModel.getProperty(TEST_PROPERTY), "valueZipLoadModel");
    BOOST_CHECK_EQUAL(zipLoadModel.getProperty(TEST_PROPERTY_ADDER), "valueZipLoadModelAdder");
    BOOST_CHECK_EQUAL(boost::size(zipLoadModel.getPropertyNames()), 2);
    const auto& expLoad = network.getLoad("expLoad");
    BOOST_CHECK_EQUAL(expLoad.getProperty(TEST_PROPERTY), "valueExpLoad");
    BOOST_CHECK_EQUAL(boost::size(expLoad.getPropertyNames()), 1);
    const auto& expLoadModel = expLoad.getModel();
    BOOST_CHECK_EQUAL(expLoadModel.getProperty(TEST_PROPERTY), "valueExpLoadModel");
    BOOST_CHECK_EQUAL(expLoadModel.getProperty(TEST_PROPERTY_ADDER), "valueExpLoadModelAdder");
    BOOST_CHECK_EQUAL(boost::size(expLoadModel.getPropertyNames()), 2);

    const auto& shuntLinear = network.getShuntCompensator("shunt");
    BOOST_CHECK_EQUAL(shuntLinear.getProperty(TEST_PROPERTY), "valueShunt");
    BOOST_CHECK_EQUAL(boost::size(shuntLinear.getPropertyNames()), 1);
    const auto& linearModel = shuntLinear.getModel<ShuntCompensatorLinearModel>();
    BOOST_CHECK_EQUAL(linearModel.getProperty(TEST_PROPERTY), "valueLinearModel");
    BOOST_CHECK_EQUAL(boost::size(linearModel.getPropertyNames()), 1);
    const auto& shuntNonLinear = network.getShuntCompensator("shuntNonLinear");
    BOOST_CHECK_EQUAL(shuntNonLinear.getProperty(TEST_PROPERTY), "valueShuntNonLinear");
    BOOST_CHECK_EQUAL(boost::size(shuntNonLinear.getPropertyNames()), 1);
    const auto& nonLinearModel = shuntNonLinear.getModel<ShuntCompensatorNonLinearModel>();
    BOOST_CHECK_EQUAL(nonLinearModel.getProperty(TEST_PROPERTY), "valueNonLinearModel");
    BOOST_CHECK_EQUAL(boost::size(nonLinearModel.getPropertyNames()), 1);
    index = 1;
    for (const auto& section : nonLinearModel.getAllSections()) {
        BOOST_CHECK_EQUAL(section.getProperty(TEST_PROPERTY), stdcxx::format("valueSection%1%",index++));
        BOOST_CHECK_EQUAL(boost::size(section.getPropertyNames()), 1);
    }

    const auto& area = network.getArea("defaultControlArea");
    BOOST_CHECK_EQUAL(area.getProperty(TEST_PROPERTY), "valueArea");
    BOOST_CHECK_EQUAL(boost::size(area.getPropertyNames()), 1);
    const auto& areaBoundary = area.getAreaBoundary(network.getLoad("load1").getTerminal()).get();
    BOOST_CHECK_EQUAL(areaBoundary.getProperty(TEST_PROPERTY), "valueAreaBoundary");
    BOOST_CHECK_EQUAL(boost::size(areaBoundary.getPropertyNames()), 1);

    const auto& line = network.getLine("line");
    BOOST_CHECK_EQUAL(line.getProperty(TEST_PROPERTY), "valueLine");
    BOOST_CHECK_EQUAL(boost::size(line.getPropertyNames()), 1);
    const auto& transformer = network.getThreeWindingsTransformer("transformer_3wt");
    BOOST_CHECK_EQUAL(transformer.getProperty(TEST_PROPERTY), "value3WT");
    BOOST_CHECK_EQUAL(boost::size(transformer.getPropertyNames()), 1);
    const auto& oms = network.getOverloadManagementSystem("OMS");
    BOOST_CHECK_EQUAL(oms.getProperty(TEST_PROPERTY), "valueOMS");
    BOOST_CHECK_EQUAL(boost::size(oms.getPropertyNames()), 1);
    index = 0;
    for (const auto& tripping : oms.getTrippings()){
        BOOST_CHECK_EQUAL(tripping.getProperty(TEST_PROPERTY), stdcxx::format("valueTripping%1%",index++));
        BOOST_CHECK_EQUAL(boost::size(tripping.getPropertyNames()), 1);
    }

    const auto& phaseTapChanger = transformer.getLeg2().getPhaseTapChanger();
    BOOST_CHECK_EQUAL(phaseTapChanger.getProperty(TEST_PROPERTY), "valuePhaseTapChanger");
    BOOST_CHECK_EQUAL(boost::size(phaseTapChanger.getPropertyNames()), 1);
    BOOST_CHECK_EQUAL(phaseTapChanger.getCurrentStep().getProperty(TEST_PROPERTY), "valuePhaseTapChangerStep");
    BOOST_CHECK_EQUAL(boost::size(phaseTapChanger.getCurrentStep().getPropertyNames()), 1);
    const auto& ratioTapChanger = transformer.getLeg3().getRatioTapChanger();
    BOOST_CHECK_EQUAL(ratioTapChanger.getProperty(TEST_PROPERTY), "valueRatioTapChanger");
    BOOST_CHECK_EQUAL(boost::size(ratioTapChanger.getPropertyNames()), 1);
    BOOST_CHECK_EQUAL(ratioTapChanger.getCurrentStep().getProperty(TEST_PROPERTY), "valueRatioTapChangerStep");
    BOOST_CHECK_EQUAL(boost::size(ratioTapChanger.getCurrentStep().getPropertyNames()), 1);

    const auto& voltageAngleLimit = network.getVoltageAngleLimit("voltageAngleLimit");
    BOOST_CHECK_EQUAL(voltageAngleLimit.getProperty(TEST_PROPERTY), "valueVoltageAngleLimit");
    BOOST_CHECK_EQUAL(boost::size(voltageAngleLimit.getPropertyNames()), 1);
    const auto& activePowerLimits = transformer.getLeg1().getSelectedOperationalLimitsGroup().get().getActivePowerLimits().get();
    BOOST_CHECK_EQUAL(activePowerLimits.getProperty(TEST_PROPERTY), "valueActivePowerLimits");
    BOOST_CHECK_EQUAL(boost::size(activePowerLimits.getPropertyNames()), 1);
    BOOST_CHECK_EQUAL(activePowerLimits.getTemporaryLimit(20*60).getProperty(TEST_PROPERTY), "valueTemporaryLimit");
    BOOST_CHECK_EQUAL(boost::size(activePowerLimits.getTemporaryLimit(20*60).getPropertyNames()), 1);
    BOOST_CHECK(!activePowerLimits.getTemporaryLimit(10*60).hasProperty());
    const auto& apparentPowerLimits = transformer.getLeg1().getSelectedOperationalLimitsGroup().get().getApparentPowerLimits().get();
    BOOST_CHECK_EQUAL(apparentPowerLimits.getProperty(TEST_PROPERTY), "valueApparentPowerLimits");
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLimits.getPropertyNames()), 1);
    BOOST_CHECK_EQUAL(apparentPowerLimits.getTemporaryLimit(20*60).getProperty(TEST_PROPERTY), "valueTemporaryLimit");
    BOOST_CHECK_EQUAL(boost::size(apparentPowerLimits.getTemporaryLimit(20*60).getPropertyNames()), 1);
    BOOST_CHECK(!apparentPowerLimits.getTemporaryLimit(10*60).hasProperty());
    const auto& currentLimits = transformer.getLeg1().getSelectedOperationalLimitsGroup().get().getCurrentLimits().get();
    BOOST_CHECK_EQUAL(currentLimits.getProperty(TEST_PROPERTY), "valueCurrentLimits");
    BOOST_CHECK_EQUAL(boost::size(currentLimits.getPropertyNames()), 1);
    BOOST_CHECK_EQUAL(currentLimits.getTemporaryLimit(20*60).getProperty(TEST_PROPERTY), "valueTemporaryLimit");
    BOOST_CHECK_EQUAL(boost::size(currentLimits.getTemporaryLimit(20*60).getPropertyNames()), 1);
    BOOST_CHECK(!currentLimits.getTemporaryLimit(10*60).hasProperty());


}

BOOST_FIXTURE_TEST_CASE(EurostagPropertiesRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1-properties.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(PropertiesHolderXmlTest, test::ResourceFixture) {
    Network networkWithProperties = createNetworkWithProperties();


    test::converter::RoundTrip::testForAllVersionsSince(IidmXmlVersion::V1_16(), [&networkWithProperties](const iidm::converter::xml::IidmXmlVersion& version) {
        //Xml Export / Import
        ExportOptions exOptions = ExportOptions().setVersion(version.toString("."));
        std::stringstream buffer;
        Network::writeXml("networkwithproperties.xiidm", buffer, networkWithProperties, exOptions);
        Network importedNetwork = Network::readXml("networkwithproperties.xiidm", buffer);

        //All Properties still present on each object
        checkProperties(importedNetwork);
    });

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

