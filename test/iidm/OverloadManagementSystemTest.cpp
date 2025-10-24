/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>
#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

void createSwitch(VoltageLevel &vl, std::string id, SwitchKind kind, bool open, int node1, int node2)
{
    vl.getNodeBreakerView().newSwitch()
        .setId(id)
        .setName(id)
        .setKind(kind)
        .setRetained(kind == SwitchKind::BREAKER)
        .setOpen(open)
        .setFictitious(false)
        .setNode1(node1)
        .setNode2(node2)
        .add();
}

Network createOmsTestNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2022-12-18T16:00:00.000+01:00"));
    network.setForecastDistance(0);

    // first substation
    Substation& s1 = network.newSubstation()
                .setId("S1")
                .add();

    VoltageLevel& s1vl1 = s1.newVoltageLevel()
                .setId("S1VL1")
                .setNominalV(400)
                .setTopologyKind(TopologyKind::NODE_BREAKER)
                .add();

    s1vl1.getNodeBreakerView().newBusbarSection()
                .setId("S1VL1_BBS")
                .setName("S1VL1_BBS")
                .setNode(0)
                .add();

    VoltageLevel& s1vl2 = s1.newVoltageLevel()
                .setId("S1VL2")
                .setNominalV(225)
                .setTopologyKind(TopologyKind::NODE_BREAKER)
                .add();
    s1vl2.getNodeBreakerView().newBusbarSection()
                .setId("S1VL2_BBS1")
                .setName("S1VL2_BBS1")
                .setNode(0)
                .add();

    // second substation
    Substation& s2 = network.newSubstation()
                .setId("S2")
                .add();

    VoltageLevel& s2vl1 = s2.newVoltageLevel()
                .setId("S2VL1")
                .setNominalV(400)
                .setTopologyKind(TopologyKind::NODE_BREAKER)
                .add();
    s2vl1.getNodeBreakerView().newBusbarSection()
                .setId("S2VL1_BBS1")
                .setName("S2VL1_BBS1")
                .setNode(0)
                .add();
    VoltageLevel& s2vl2 = s2.newVoltageLevel()
                .setId("S2VL2")
                .setNominalV(225)
                .setTopologyKind(TopologyKind::NODE_BREAKER)
                .add();
    s2vl2.getNodeBreakerView().newBusbarSection()
                .setId("S2VL2_BBS1")
                .setName("S2VL2_BBS1")
                .setNode(0)
                .add();

    // generator
    createSwitch(s1vl1, "S1VL1_BBS1_GEN_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 1);
    createSwitch(s1vl1, "S1VL1_BBS1_GEN_BREAKER", SwitchKind::BREAKER, false, 1, 2);
    s1vl1.newGenerator()
                .setId("GEN")
                .setEnergySource(EnergySource::OTHER)
                .setMinP(0)
                .setMaxP(150)
                .setVoltageRegulatorOn(true)
                .setTargetV(400)
                .setTargetP(100.0)
                .setNode(2)
                .add();

    // Loads
    createSwitch(s1vl2, "S1VL2_BBS1_LD1_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 3);
    createSwitch(s1vl2, "S1VL2_BBS1_LD1_BREAKER", SwitchKind::BREAKER, false, 3, 4);
    createSwitch(s2vl2, "S2VL2_BBS1_LD2_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 3);
    createSwitch(s2vl2, "S2VL2_BBS1_LD2_BREAKER", SwitchKind::BREAKER, false, 3, 4);
    s1vl2.newLoad()
                .setId("LD1")
                .setLoadType(LoadType::UNDEFINED)
                .setP0(50)
                .setQ0(4)
                .setNode(4)
                .add();
    s2vl2.newLoad()
                .setId("LD2")
                .setLoadType(LoadType::UNDEFINED)
                .setP0(50)
                .setQ0(4)
                .setNode(4)
                .add();

    // lines
    createSwitch(s1vl1, "S1VL2_BBS_LINES1S2V1_1_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 16);
    createSwitch(s1vl1, "S1VL2_LINES1S2V1_1_BREAKER", SwitchKind::BREAKER, false, 16, 17);
    createSwitch(s2vl1, "S2VL2_BBS_LINES1S2V1_1_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 16);
    createSwitch(s2vl1, "S2VL2_LINES1S2V1_1_BREAKER", SwitchKind::BREAKER, false, 16, 17);
    network.newLine()
                .setId("LINE_S1S2V1_1")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(17)
                .setVoltageLevel1("S1VL1")
                .setNode2(17)
                .setVoltageLevel2("S2VL1")
                .add();
    createSwitch(s1vl1, "S1VL2_BBS_LINES1S2V1_2_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 14);
    createSwitch(s1vl1, "S1VL2_LINES1S2V1_2_BREAKER", SwitchKind::BREAKER, false, 14, 15);
    createSwitch(s2vl1, "S2VL2_BBS_LINES1S2V1_2_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 14);
    createSwitch(s2vl1, "S2VL2_LINES1S2V1_2_BREAKER", SwitchKind::BREAKER, false, 14, 15);
    network.newLine()
                .setId("LINE_S1S2V1_2")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(15)
                .setVoltageLevel1("S1VL1")
                .setNode2(15)
                .setVoltageLevel2("S2VL1")
                .add();
    createSwitch(s1vl2, "S1VL2_BBS_LINES1S2_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 12);
    createSwitch(s1vl2, "S1VL2_LINES1S2_BREAKER", SwitchKind::BREAKER, false, 12, 13);
    createSwitch(s2vl2, "S2VL2_BBS_LINES1S2_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 12);
    createSwitch(s2vl2, "S2VL2_LINES1S2_BREAKER", SwitchKind::BREAKER, false, 12, 13);
    network.newLine()
                .setId("LINE_S1S2V2")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(13)
                .setVoltageLevel1("S1VL2")
                .setNode2(13)
                .setVoltageLevel2("S2VL2")
                .add();

    // transformers
    createSwitch(s1vl1, "S1VL1_BBS_TWT_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 10);
    createSwitch(s1vl1, "S1VL1_TWT_BREAKER", SwitchKind::BREAKER, false, 10, 11);
    createSwitch(s1vl2, "S1VL2_BBS_TWT_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 10);
    createSwitch(s1vl2, "S1VL2_TWT_BREAKER", SwitchKind::BREAKER, false, 10, 11);
    s1.newTwoWindingsTransformer()
                .setId("TWT")
                .setR(2.0)
                .setX(25)
                .setG(0.0)
                .setB(3.2E-5)
                .setRatedU1(400.0)
                .setRatedU2(225.0)
                .setNode1(11)
                .setVoltageLevel1("S1VL1")
                .setNode2(11)
                .setVoltageLevel2("S1VL2")
                .add();
    createSwitch(s2vl1, "S2VL1_BBS_TWT_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 10);
    createSwitch(s2vl1, "S2VL1_TWT_BREAKER", SwitchKind::BREAKER, false, 10, 11);
    createSwitch(s2vl2, "S2VL2_BBS_TWT_DISCONNECTOR", SwitchKind::DISCONNECTOR, false, 0, 10);
    createSwitch(s2vl2, "S2VL2_TWT_BREAKER", SwitchKind::BREAKER, false, 10, 11);
    s2.newTwoWindingsTransformer()
                .setId("TWT2")
                .setR(2.0)
                .setX(50)
                .setG(0.0)
                .setB(3.2E-5)
                .setRatedU1(400.0)
                .setRatedU2(225.0)
                .setNode1(11)
                .setVoltageLevel1("S2VL1")
                .setNode2(11)
                .setVoltageLevel2("S2VL2")
                .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(OverloadManagementSystemTestSuite)

BOOST_AUTO_TEST_CASE(baseConstructorTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");

    // Using intermediate Tripping adder :
    OverloadManagementSystemAdder omsAdder1 = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setName("1st OMS")
                .setEnabled(true)
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE);
    omsAdder1.newSwitchTripping()
                    ->setSwitchToOperateId("S1VL2_LINES1S2V1_1_BREAKER")
                    .setKey("SwTrip")
                    .setName("Tripping on switch")
                    .setCurrentLimit(80.)
                    .setOpenAction(true)
                    .add();
    auto branchTrippingAdder = omsAdder1.newBranchTripping();
    branchTrippingAdder->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("LineTrip")
                    .setCurrentLimit(50)
                    .setOpenAction(false)
                    .add();
    OverloadManagementSystem& oms1 = omsAdder1.add();

    // only using the OverloadManagementSystemAdder
    OverloadManagementSystem& oms2 = substation.newOverloadManagementSystem()
                .setId("OMS2")
                .setEnabled(false)
                .setMonitoredElementId("LINE_S1S2V1_2")
                .setMonitoredElementSide(ThreeSides::TWO)
                .newBranchTripping() 
                    ->setBranchToOperateId("TWT")
                    .setSideToOperate(TwoSides::TWO)
                    .setKey("2wtTrip")
                    .setName("Tripping on 2 windings transformer")
                    .setCurrentLimit(100.)
                    .setOpenAction(true)
                    .add()
                .add();

    BOOST_CHECK_EQUAL(2, network.getOverloadManagementSystemCount());
    BOOST_CHECK_EQUAL(2, substation.getOverloadManagementSystemCount());

    BOOST_CHECK(stdcxx::areSame(oms1, network.getOverloadManagementSystem("OMS1")));
    BOOST_CHECK(stdcxx::areSame(oms2, network.getOverloadManagementSystem("OMS2")));

    auto retrieved = substation.getOverloadManagementSystems();
    BOOST_CHECK_EQUAL(2, boost::size(retrieved));

    BOOST_CHECK_EQUAL("OMS1", oms1.getId());
    BOOST_CHECK_EQUAL("OMS2", oms2.getId());
    BOOST_CHECK_EQUAL("1st OMS", oms1.getNameOrId());
    BOOST_CHECK_EQUAL("OMS2", oms2.getNameOrId());
    BOOST_CHECK(oms1.isEnabled());
    BOOST_CHECK(!oms2.isEnabled());
    BOOST_CHECK_EQUAL("LINE_S1S2V1_1", oms1.getMonitoredElementId());
    BOOST_CHECK_EQUAL("LINE_S1S2V1_2", oms2.getMonitoredElementId());
    BOOST_CHECK_EQUAL(ThreeSides::ONE, oms1.getMonitoredSide());
    BOOST_CHECK_EQUAL(ThreeSides::TWO, oms2.getMonitoredSide());

    std::vector<std::reference_wrapper<OverloadManagementSystem::Tripping>> trippings = oms1.getTrippings();
    BOOST_CHECK_EQUAL(2, trippings.size());

    BOOST_CHECK(OverloadManagementSystem::Tripping::Type::SWITCH_TRIPPING == trippings.at(0).get().getType());
    BOOST_CHECK(stdcxx::isInstanceOf<overload_management_system::SwitchTripping>(trippings.at(0).get()));
    overload_management_system::SwitchTripping& swTripping = dynamic_cast<overload_management_system::SwitchTripping&>(trippings.at(0).get());
    BOOST_CHECK_EQUAL("SwTrip", swTripping.getKey());
    BOOST_CHECK_EQUAL("Tripping on switch", swTripping.getNameOrKey());
    BOOST_CHECK_CLOSE(80., swTripping.getCurrentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(swTripping.isOpenAction());
    BOOST_CHECK_EQUAL("S1VL2_LINES1S2V1_1_BREAKER", swTripping.getSwitchToOperateId());

    BOOST_CHECK(OverloadManagementSystem::Tripping::Type::BRANCH_TRIPPING == trippings.at(1).get().getType());
    BOOST_CHECK(stdcxx::isInstanceOf<overload_management_system::BranchTripping>(trippings.at(1).get()));
    overload_management_system::BranchTripping& brTripping = dynamic_cast<overload_management_system::BranchTripping&>(trippings.at(1).get());
    BOOST_CHECK_EQUAL("LineTrip", brTripping.getKey());
    BOOST_CHECK_EQUAL("LineTrip", brTripping.getNameOrKey());
    BOOST_CHECK_CLOSE(50., brTripping.getCurrentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!brTripping.isOpenAction());
    BOOST_CHECK_EQUAL("LINE_S1S2V1_2", brTripping.getBranchToOperateId());
    BOOST_CHECK_EQUAL(TwoSides::ONE, brTripping.getSideToOperate());

    trippings = oms2.getTrippings();
    BOOST_CHECK_EQUAL(1, trippings.size());
    BOOST_CHECK(OverloadManagementSystem::Tripping::Type::BRANCH_TRIPPING == trippings.at(0).get().getType());
    BOOST_CHECK_EQUAL("2wtTrip", trippings.at(0).get().getKey());
    BOOST_CHECK_EQUAL("Tripping on 2 windings transformer", trippings.at(0).get().getNameOrKey());
    BOOST_CHECK_CLOSE(100., trippings.at(0).get().getCurrentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(trippings.at(0).get().isOpenAction());

    BOOST_CHECK(stdcxx::isInstanceOf<overload_management_system::BranchTripping>(trippings.at(0).get()));
    overload_management_system::BranchTripping& brTripping2 = dynamic_cast<overload_management_system::BranchTripping&>(trippings.at(0).get());
    BOOST_CHECK_EQUAL("TWT", brTripping2.getBranchToOperateId());
    BOOST_CHECK_EQUAL(TwoSides::TWO, brTripping2.getSideToOperate());
}


BOOST_AUTO_TEST_CASE(threeWindingsTransformerTrippingTest) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();
    Substation& substation = network.getSubstation("SUBSTATION");
    OverloadManagementSystem& oms = substation.newOverloadManagementSystem()
                                       .setId("OMS")
                                       .setName("An OMS")
                                       .setEnabled(true)
                                       .setMonitoredElementId("3WT")
                                       .setMonitoredElementSide(ThreeSides::TWO)
                                       .newThreeWindingsTransformerTripping()
                                            ->setThreeWindingsTransformerToOperateId("3WT")
                                            .setSideToOperate(ThreeSides::THREE)
                                            .setKey("3wtTrip")
                                            .setName("Tripping on 3 windings transformer")
                                            .setCurrentLimit(60.)
                                            .setOpenAction(true)
                                            .add()
                                       .add();

    std::vector<std::reference_wrapper<OverloadManagementSystem::Tripping>> trippings = oms.getTrippings();
    stdcxx::const_range<overload_management_system::Tripping> cTrippings = oms.getTrippings();
    BOOST_CHECK_EQUAL(1, trippings.size());
    BOOST_CHECK_EQUAL(1, boost::size(cTrippings));
    BOOST_CHECK(OverloadManagementSystem::Tripping::Type::THREE_WINDINGS_TRANSFORMER_TRIPPING == trippings.at(0).get().getType());
    overload_management_system::ThreeWindingsTransformerTripping& twtTripping =
        dynamic_cast<overload_management_system::ThreeWindingsTransformerTripping&>(trippings.at(0).get());
    BOOST_CHECK_EQUAL("3wtTrip", twtTripping.getKey());
    BOOST_CHECK_EQUAL("Tripping on 3 windings transformer", twtTripping.getNameOrKey());
    BOOST_CHECK_CLOSE(60., twtTripping.getCurrentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(twtTripping.isOpenAction());
    BOOST_CHECK_EQUAL("3WT", twtTripping.getThreeWindingsTransformerToOperateId());
    BOOST_CHECK_EQUAL(ThreeSides::THREE, twtTripping.getSideToOperate());
}

BOOST_AUTO_TEST_CASE(setGetInMultiVariantsTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    OverloadManagementSystem& oms1 = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setEnabled(true)
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newBranchTripping()
                    ->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("LineTrip")
                    .setCurrentLimit(50)
                    .setOpenAction(false)
                    .add()
                .add();

    VariantManager& variantManager = network.getVariantManager();
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{"s1", "s2", "s3", "s4"});

    variantManager.setWorkingVariant("s4");
    // check value cloned by extend
    BOOST_CHECK(oms1.isEnabled());
    // change value in s4
    oms1.setEnabled(false);

    // remove s2
    variantManager.removeVariant("s2");

    variantManager.cloneVariant("s4", "s2b");
    variantManager.setWorkingVariant("s2b");
    // check value cloned by allocate
    BOOST_CHECK(!oms1.isEnabled());

    // recheck initial variant value
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(oms1.isEnabled());

    // remove working variant s4
    variantManager.setWorkingVariant("s4");
    variantManager.removeVariant("s4");
    POWSYBL_ASSERT_THROW(oms1.isEnabled(), PowsyblException, "Variant index not set");
}

BOOST_AUTO_TEST_CASE(invalidCurrentLimitsTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE);
    std::shared_ptr<overload_management_system::BranchTrippingAdder> trippingAdderPtr = omsAdder.newBranchTripping();
    BOOST_CHECK(static_cast<bool>(trippingAdderPtr));
    trippingAdderPtr->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("LineTrip")
                    .setOpenAction(false)
                    .setCurrentLimit(-50);
    trippingAdderPtr->add();
    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, "Overload management system 'OMS1' - tripping 'LineTrip':Current limit must be positive.");

    trippingAdderPtr->setCurrentLimit(stdcxx::nan());
    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, "Overload management system 'OMS1' - tripping 'LineTrip':Current limit is mandatory.");
}

BOOST_AUTO_TEST_CASE(duplicateTrippingKeyOnSameOmsTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    std::string duplicateKey = "duplicate";
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newSwitchTripping()
                    ->setSwitchToOperateId("S1VL2_LINES1S2V1_1_BREAKER")
                    .setKey(duplicateKey)
                    .setCurrentLimit(80.)
                    .setOpenAction(true)
                    .add()
                .newBranchTripping()
                    ->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey(duplicateKey)
                    .setOpenAction(false)
                    .setCurrentLimit(50)
                    .add();
    // dupliacte tripping keys are NOT allowed inside the same OverloadManagementSystem.
    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, 
    "Overload management system in substation 'S1':  - tripping 'duplicate':key 'duplicate' is already used for another tripping in the overload management system 'OMS1'.");
}

BOOST_AUTO_TEST_CASE(duplicateTrippingKeyOnDifferentOmsTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    std::string duplicateKey = "duplicate";
    substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newSwitchTripping()
                    ->setSwitchToOperateId("S1VL2_LINES1S2V1_1_BREAKER")
                    .setKey(duplicateKey)
                    .setCurrentLimit(80.)
                    .setOpenAction(true)
                    .add();
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS2")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::TWO)
                .newBranchTripping()
                    ->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey(duplicateKey)
                    .setOpenAction(false)
                    .setCurrentLimit(50)
                    .add();
    // dupliacte tripping keys are allowed on distinct OverloadManagementSystems
    BOOST_CHECK_NO_THROW(omsAdder.add());
}

BOOST_AUTO_TEST_CASE(unknownMonitoredElementTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newBranchTripping()
                    ->setBranchToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("LineTrip")
                    .setOpenAction(false)
                    .setCurrentLimit(50)
                    .add();

    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, "Overload management system 'OMS1': monitoredElementId is not set");

    omsAdder.setMonitoredElementId("UNKNOWN");
    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, "Overload management system 'OMS1': Identifiable (UNKNOWN) not found in this network (test)");
}

BOOST_AUTO_TEST_CASE(unknownTrippingElementTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newBranchTripping()
                    ->setBranchToOperateId("UNKNOWN")
                    .setSideToOperate(TwoSides::ONE)
                    .setKey("LineTrip")
                    .setOpenAction(false)
                    .setCurrentLimit(50)
                    .add();

    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException, 
    "Overload management system in substation 'S1':  - tripping 'LineTrip':Identifiable (UNKNOWN) not found in this network (test)");
}

BOOST_AUTO_TEST_CASE(invalidTypeTrippingElementTest) {
    Network network = createOmsTestNetwork();
    Substation& substation = network.getSubstation("S1");
    OverloadManagementSystemAdder omsAdder = substation.newOverloadManagementSystem()
                .setId("OMS1")
                .setMonitoredElementId("LINE_S1S2V1_1")
                .setMonitoredElementSide(ThreeSides::ONE)
                .newThreeWindingsTransformerTripping()
                    ->setThreeWindingsTransformerToOperateId("LINE_S1S2V1_2")
                    .setSideToOperate(ThreeSides::TWO)
                    .setKey("LineTrip")
                    .setOpenAction(false)
                    .setCurrentLimit(50)
                    .add();

    POWSYBL_ASSERT_THROW(omsAdder.add(), ValidationException,
    "Overload management system in substation 'S1':  - tripping 'LineTrip':Element (LINE_S1S2V1_2) is of type : powsybl::iidm::Line (powsybl::iidm::ThreeWindingsTransformer expected)");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl

