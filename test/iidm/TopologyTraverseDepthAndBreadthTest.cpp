/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <string>

namespace powsybl {

namespace iidm {

Network createTopologyDepthAndBreadthNetwork() {
    Network network("test", "test");
    Substation& s1 = network.newSubstation().setId("S1").setCountry(Country::FR).add();
    VoltageLevel& vl1 = s1.newVoltageLevel().setId("VL1").setNominalV(400.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl1.getNodeBreakerView().newBusbarSection().setId("VL1_B1").setNode(0).add();
    vl1.getNodeBreakerView().newBusbarSection().setId("VL1_B2").setNode(1).add();
    vl1.newGenerator().setId("G1").setNode(2).setMinP(0).setMaxP(1).setTargetP(1).setTargetQ(0).setVoltageRegulatorOn(false).add();
    vl1.newGenerator().setId("G2").setNode(5).setMinP(0).setMaxP(1).setTargetP(1).setTargetQ(0).setVoltageRegulatorOn(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER3").setOpen(false).setNode1(3).setNode2(9).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER1").setOpen(false).setNode1(3).setNode2(4).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER2").setOpen(false).setNode1(9).setNode2(10).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER7").setOpen(false).setNode1(6).setNode2(12).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER5").setOpen(false).setNode1(6).setNode2(7).add();
    vl1.getNodeBreakerView().newBreaker().setId("VL1_BREAKER6").setOpen(false).setNode1(12).setNode2(13).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR1").setOpen(false).setNode1(2).setNode2(3).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR2").setOpen(false).setNode1(8).setNode2(9).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR3").setOpen(false).setNode1(1).setNode2(4).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR4").setOpen(false).setNode1(0).setNode2(10).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR5").setOpen(false).setNode1(5).setNode2(6).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR6").setOpen(false).setNode1(1).setNode2(7).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR7").setOpen(false).setNode1(11).setNode2(12).add();
    vl1.getNodeBreakerView().newDisconnector().setId("VL1_DISCONNECTOR8").setOpen(false).setNode1(0).setNode2(13).add();

    Substation& s2 = network.newSubstation().setId("S2").setCountry(Country::FR).add();
    VoltageLevel& vl2 = s2.newVoltageLevel().setId("VL2").setNominalV(400.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl2.getNodeBreakerView().newBusbarSection().setId("VL2_B1").setNode(0).add();
    vl2.getNodeBreakerView().newBreaker().setId("VL2_BREAKER1").setOpen(false).setNode1(1).setNode2(2).add();
    vl2.getNodeBreakerView().newDisconnector().setId("VL2_DISCONNECTOR1").setOpen(false).setNode1(0).setNode2(2).add();

    Substation& s3 = network.newSubstation().setId("S3").setCountry(Country::FR).add();
    VoltageLevel& vl3 = s3.newVoltageLevel().setId("VL3").setNominalV(400.0).setTopologyKind(TopologyKind::NODE_BREAKER).add();
    vl3.getNodeBreakerView().newBusbarSection().setId("VL3_B1").setNode(0).add();
    vl3.getNodeBreakerView().newBreaker().setId("VL3_BREAKER1").setOpen(false).setNode1(1).setNode2(2).add();
    vl3.getNodeBreakerView().newDisconnector().setId("VL3_DISCONNECTOR1").setOpen(false).setNode1(0).setNode2(2).add();

    network.newLine().setId("L1").setVoltageLevel1("VL1").setVoltageLevel2("VL2").setNode1(8).setNode2(1).setR(1).setX(1).setG1(0).setB1(0).setG2(0).setB2(0).add();
    network.newLine().setId("L2").setVoltageLevel1("VL1").setVoltageLevel2("VL3").setNode1(11).setNode2(1).setR(1).setX(1).setG1(0).setB1(0).setG2(0).setB2(0).add();

    return network;
}

class CustomBusbarSectionFinderTraverser : public Terminal::TopologyTraverser {

public:  // voltage_level::TopologyTraverser
    math::TraverseResult traverse(Terminal& terminal, bool /*connected*/) override {
        if (terminal.getConnectable() && terminal.getConnectable().get().getType() == IdentifiableType::BUSBAR_SECTION){
            m_firstTraversedBbsId = terminal.getConnectable().get().getId();
            return math::TraverseResult::TERMINATE_TRAVERSER;
        }
        return math::TraverseResult::CONTINUE;
    }

    math::TraverseResult traverse(Switch& aSwitch) override {
        if (m_onlyConnectedBbs && aSwitch.isOpen()) {
                return math::TraverseResult::TERMINATE_PATH;
            }
            return math::TraverseResult::CONTINUE;
    }

public:
    CustomBusbarSectionFinderTraverser(bool onlyConnectedBbs) :
    m_onlyConnectedBbs(onlyConnectedBbs) {
    }

    std::string getFirstTraversedBbsId() {
        return m_firstTraversedBbsId;
    }

private:
    bool m_onlyConnectedBbs;
    std::string m_firstTraversedBbsId;
};

static std::string getBusbarSectionId(Terminal& terminal, math::TraversalType traversalType) {
    CustomBusbarSectionFinderTraverser connectedBusbarSectionFinder(terminal.isConnected());
    terminal.traverse(connectedBusbarSectionFinder, traversalType);
    return connectedBusbarSectionFinder.getFirstTraversedBbsId();
}

BOOST_AUTO_TEST_SUITE(TopologyTraverserDepthAndBreadthTestSuite)

BOOST_AUTO_TEST_CASE(testDepthFirst) {
    Network network = createTopologyDepthAndBreadthNetwork();

    BOOST_CHECK_EQUAL("VL1_B1", getBusbarSectionId(network.getGenerator("G1").getTerminal(), math::TraversalType::DEPTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B1", getBusbarSectionId(network.getGenerator("G2").getTerminal(), math::TraversalType::DEPTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B2", getBusbarSectionId(network.getLine("L1").getTerminalFromVoltageLevel("VL1"), math::TraversalType::DEPTH_FIRST));
    BOOST_CHECK_EQUAL("VL2_B1", getBusbarSectionId(network.getLine("L1").getTerminalFromVoltageLevel("VL2"), math::TraversalType::DEPTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B2", getBusbarSectionId(network.getLine("L2").getTerminalFromVoltageLevel("VL1"), math::TraversalType::DEPTH_FIRST));
    BOOST_CHECK_EQUAL("VL3_B1", getBusbarSectionId(network.getLine("L2").getTerminalFromVoltageLevel("VL3"), math::TraversalType::DEPTH_FIRST));
}

BOOST_AUTO_TEST_CASE(testBreadthFirst) {
    Network network = createTopologyDepthAndBreadthNetwork();

    BOOST_CHECK_EQUAL("VL1_B2", getBusbarSectionId(network.getGenerator("G1").getTerminal(), math::TraversalType::BREADTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B2", getBusbarSectionId(network.getGenerator("G2").getTerminal(), math::TraversalType::BREADTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B1", getBusbarSectionId(network.getLine("L1").getTerminalFromVoltageLevel("VL1"), math::TraversalType::BREADTH_FIRST));
    BOOST_CHECK_EQUAL("VL2_B1", getBusbarSectionId(network.getLine("L1").getTerminalFromVoltageLevel("VL2"), math::TraversalType::BREADTH_FIRST));
    BOOST_CHECK_EQUAL("VL1_B1", getBusbarSectionId(network.getLine("L2").getTerminalFromVoltageLevel("VL1"), math::TraversalType::BREADTH_FIRST));
    BOOST_CHECK_EQUAL("VL3_B1", getBusbarSectionId(network.getLine("L2").getTerminalFromVoltageLevel("VL3"), math::TraversalType::BREADTH_FIRST));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
