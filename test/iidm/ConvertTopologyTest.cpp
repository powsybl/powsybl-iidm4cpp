/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/SlackTerminal.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ConvertTopologyTestSuite)

BOOST_AUTO_TEST_CASE(testBusBreakerToNodeBreaker)
{
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vl = network.getVoltageLevel("VLGEN");

    BOOST_CHECK(vl.getTopologyKind()==TopologyKind::BUS_BREAKER);
    POWSYBL_ASSERT_THROW(vl.convertToTopology(TopologyKind::NODE_BREAKER), PowsyblException, "Topology model conversion from BUS_BREAKER to NODE_BREAKER not yet supported");
}

BOOST_AUTO_TEST_CASE(testNodeBreakerToBusBreaker)
{
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("S1VL2");

    BOOST_CHECK(vl.getTopologyKind()==TopologyKind::NODE_BREAKER);

    for (Switch& sw : vl.getSwitches()) {
        sw.setRetained((sw.getId()=="S1VL2_COUPLER"));
    }

    Generator& gh1 = network.getGenerator("GH1");
    auto busesNbModel = vl.getBusBreakerView().getBuses();

    BOOST_CHECK_EQUAL(2, boost::size(busesNbModel));
    std::set<std::string> expectedBusesId = {"S1VL2_0", "S1VL2_1"};
    std::set<std::string> busesId;
    for (const auto& bus : busesNbModel) {
        busesId.insert(bus.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedBusesId.cbegin(), expectedBusesId.cend(), busesId.cbegin(), busesId.cend());
    //"Calculated bus" that are not in the network index though:
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_0"));
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_1"));
    

    Bus& bus1 = vl.getBusBreakerView().getBus("S1VL2_0");
    Bus& bus2 = vl.getBusBreakerView().getBus("S1VL2_1");
    std::vector<std::string> expectedConnectedTerminal1 = {"S1VL2_BBS1", "TWT", "GH1", "GH2", "GH3", "SHUNT"};
    std::vector<std::string> expectedConnectedTerminal2 = {"S1VL2_BBS2", "VSC1", "LD2", "LD3", "LD4", "LCC1"};
    std::vector<std::string> connectedTerminal1;
    std::vector<std::string> connectedTerminal2;
    
    for (const auto& connectedTerminal : bus1.getConnectedTerminals()) {
        connectedTerminal1.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    for (const auto& connectedTerminal : bus2.getConnectedTerminals()) {
        connectedTerminal2.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal1.cbegin(), expectedConnectedTerminal1.cend(), connectedTerminal1.cbegin(), connectedTerminal1.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal2.cbegin(), expectedConnectedTerminal2.cend(), connectedTerminal2.cbegin(), connectedTerminal2.cend());

    BOOST_CHECK_EQUAL(0, gh1.getTerminal().getReferrers().size());
    //Force regulating terminal reference to check that both regulating Terminal reference and Connectable::getTerminal() will return updated reference after replacement
    gh1.setRegulatingTerminal(stdcxx::ref(gh1.getTerminal()));
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL("S1VL2_0", gh1.getRegulatingTerminal().getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("S1VL2_0", gh1.getTerminal().getBusBreakerView().getBus().get().getId());

    std::set<std::string> switchesId;
    for (auto& sw : vl.getSwitches()) {
        switchesId.insert(sw.getId());
    }

    vl.convertToTopology(TopologyKind::BUS_BREAKER);
    //Check new topologyModel :
    BOOST_CHECK(vl.getTopologyKind()==TopologyKind::BUS_BREAKER);
    
    //Calculated Buses are converted :
    auto busesBbModel = vl.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(2, boost::size(busesBbModel));
    std::set<std::string> newBusesId;
    for (const auto& bus : busesBbModel) {
        newBusesId.insert(bus.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedBusesId.cbegin(), expectedBusesId.cend(), newBusesId.cbegin(), newBusesId.cend());
    //And are now "configured bus" that are found in the network:
    POWSYBL_ASSERT_REF_TRUE(network.find("S1VL2_0"));
    POWSYBL_ASSERT_REF_TRUE(network.find("S1VL2_1"));

    // compare to initial node/breaker model, only difference is that there is no more busbar sections
    expectedConnectedTerminal1 = {"TWT", "GH1", "GH2", "GH3", "SHUNT"};
    expectedConnectedTerminal2 = {"VSC1", "LD2", "LD3", "LD4", "LCC1"};
    Bus& bbbus1 = vl.getBusBreakerView().getBus("S1VL2_0").get();
    Bus& bbbus2 = vl.getBusBreakerView().getBus("S1VL2_1").get();
    connectedTerminal1.clear();
    connectedTerminal2.clear();
    
    for (const auto& connectedTerminal : bbbus1.getConnectedTerminals()) {
        connectedTerminal1.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    for (const auto& connectedTerminal : bbbus2.getConnectedTerminals()) {
        connectedTerminal2.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal1.cbegin(), expectedConnectedTerminal1.cend(), connectedTerminal1.cbegin(), connectedTerminal1.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal2.cbegin(), expectedConnectedTerminal2.cend(), connectedTerminal2.cbegin(), connectedTerminal2.cend());

    //Only retained switch have been kept:
    BOOST_CHECK_EQUAL(1, vl.getBusBreakerView().getSwitchCount());
    POWSYBL_ASSERT_REF_TRUE(vl.getBusBreakerView().getSwitch("S1VL2_COUPLER"));
    //All others switches have been removed:
    switchesId.erase("S1VL2_COUPLER");
    for (const std::string& id : switchesId) {
        POWSYBL_ASSERT_REF_FALSE(network.find(id));
    }
    // Busbar sections also have been removed
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_BBS1"));
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_BBS2"));

    //Check regulating terminal reference has been correctly updated:
    BOOST_CHECK_EQUAL("S1VL2_0", gh1.getTerminal().getBusBreakerView().getBus().get().getId());
    //Referenced regulating terminal also:
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL("S1VL2_0", gh1.getRegulatingTerminal().getBusBreakerView().getBus().get().getId());
}

BOOST_AUTO_TEST_CASE(testNodeBreakerToBusBreakerOneElementDisconnected) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("S1VL2");
    for (Switch& sw : vl.getSwitches()) {
        sw.setRetained((sw.getId()=="S1VL2_COUPLER"));
    }

    Generator& gh2 = network.getGenerator("GH2");
    gh2.disconnect();

    auto busesNbModel = vl.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(3, boost::size(busesNbModel));
    std::set<std::string> expectedBusesId = {"S1VL2_0", "S1VL2_1", "S1VL2_9"};
    std::set<std::string> busesId;
    for (const auto& bus : busesNbModel) {
        busesId.insert(bus.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedBusesId.cbegin(), expectedBusesId.cend(), busesId.cbegin(), busesId.cend());
    //"Calculated bus" that are not in the network index though:
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_0"));
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_1"));
    POWSYBL_ASSERT_REF_FALSE(network.find("S1VL2_9"));

    Bus& bus1 = vl.getBusBreakerView().getBus("S1VL2_0");
    Bus& bus2 = vl.getBusBreakerView().getBus("S1VL2_1");
    Bus& bus3 = vl.getBusBreakerView().getBus("S1VL2_9");
    std::vector<std::string> expectedConnectedTerminal1 = {"S1VL2_BBS1", "TWT", "GH1", "GH3", "SHUNT"};
    std::vector<std::string> expectedConnectedTerminal2 = {"S1VL2_BBS2", "VSC1", "LD2", "LD3", "LD4", "LCC1"};
    std::vector<std::string> expectedConnectedTerminal3 = {"GH2"};
    std::vector<std::string> connectedTerminals;
    
    for (const auto& connectedTerminal : bus1.getConnectedTerminals()) {
        connectedTerminals.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal1.cbegin(), expectedConnectedTerminal1.cend(), connectedTerminals.cbegin(), connectedTerminals.cend());

    connectedTerminals.clear();
    for (const auto& connectedTerminal : bus2.getConnectedTerminals()) {
        connectedTerminals.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal2.cbegin(), expectedConnectedTerminal2.cend(), connectedTerminals.cbegin(), connectedTerminals.cend());

    connectedTerminals.clear();
    for (const auto& connectedTerminal : bus3.getConnectedTerminals()) {
        connectedTerminals.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal3.cbegin(), expectedConnectedTerminal3.cend(), connectedTerminals.cbegin(), connectedTerminals.cend());

    std::set<std::string> switchesId;
    for (auto& sw : vl.getSwitches()) {
        switchesId.insert(sw.getId());
    }


    vl.convertToTopology(TopologyKind::BUS_BREAKER);

    //Check new topologyModel :
    BOOST_CHECK(vl.getTopologyKind()==TopologyKind::BUS_BREAKER);

    auto busesBbModel = vl.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(3, boost::size(busesBbModel));
    busesId.clear();
    for (const auto& bus : busesBbModel) {
        busesId.insert(bus.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedBusesId.cbegin(), expectedBusesId.cend(), busesId.cbegin(), busesId.cend());
    //And are now "configured bus" that are found in the network:
    POWSYBL_ASSERT_REF_TRUE(network.find("S1VL2_0"));
    POWSYBL_ASSERT_REF_TRUE(network.find("S1VL2_1"));
    POWSYBL_ASSERT_REF_TRUE(network.find("S1VL2_9"));

    // compare to initial node/breaker model, only difference is that there is no more busbar sections
    expectedConnectedTerminal1 = {"TWT", "GH1", "GH3", "SHUNT"};
    expectedConnectedTerminal2 = {"VSC1", "LD2", "LD3", "LD4", "LCC1"};
    Bus& bbbus1 = vl.getBusBreakerView().getBus("S1VL2_0").get();
    Bus& bbbus2 = vl.getBusBreakerView().getBus("S1VL2_1").get();
    Bus& bbbus3 = vl.getBusBreakerView().getBus("S1VL2_9").get();

    connectedTerminals.clear();
    for (const auto& connectedTerminal : bbbus1.getConnectedTerminals()) {
        connectedTerminals.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal1.cbegin(), expectedConnectedTerminal1.cend(), connectedTerminals.cbegin(), connectedTerminals.cend());

    connectedTerminals.clear();
    for (const auto& connectedTerminal : bbbus2.getConnectedTerminals()) {
        connectedTerminals.emplace_back(connectedTerminal.getConnectable().get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectedTerminal2.cbegin(), expectedConnectedTerminal2.cend(), connectedTerminals.cbegin(), connectedTerminals.cend());

    BOOST_CHECK_EQUAL(0, boost::size(bbbus3.getConnectedTerminals()));
    BOOST_CHECK_EQUAL("S1VL2_9", gh2.getTerminal().getBusBreakerView().getConnectableBus().get().getId());
    BOOST_CHECK(!gh2.getTerminal().isConnected());

    // only retained switches have been kept:
    BOOST_CHECK_EQUAL(1, vl.getBusBreakerView().getSwitchCount());
    POWSYBL_ASSERT_REF_TRUE(vl.getBusBreakerView().getSwitch("S1VL2_COUPLER"));
    //All others switches have been removed:
    switchesId.erase("S1VL2_COUPLER");
    for (const std::string& id : switchesId) {
        POWSYBL_ASSERT_REF_FALSE(network.find(id));
    }
}

BOOST_AUTO_TEST_CASE(testNodeBreakerToBusBreakerWithArea) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("S1VL2");
    for (Switch& sw : vl.getSwitches()) {
        sw.setRetained((sw.getId()=="S1VL2_COUPLER"));
    }
    Generator& gh2 = network.getGenerator("GH2");
    network.newArea()
                .setId("area1")
                .setAreaType("fake")
                .addAreaBoundary(gh2.getTerminal(), true)
                .add();
    BOOST_CHECK_EQUAL(1, network.getArea("area1").getAreaBoundaries().size());
    auto boundary = network.getArea("area1").getAreaBoundaries().front();
    BOOST_CHECK(boundary);
    BOOST_CHECK(stdcxx::areSame(gh2.getTerminal(), boundary->getTerminal().get()));

    vl.convertToTopology(TopologyKind::BUS_BREAKER);
    BOOST_CHECK_EQUAL(1, network.getArea("area1").getAreaBoundaries().size());
    BOOST_CHECK(stdcxx::areSame(gh2.getTerminal(), boundary->getTerminal().get()));
    BOOST_CHECK_EQUAL(1, gh2.getTerminal().getReferrers().size());
}

BOOST_AUTO_TEST_CASE(testNodeBreakerToBusBreakerWithSlackTerminal) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("S1VL2");
    for (Switch& sw : vl.getSwitches()) {
        sw.setRetained((sw.getId()=="S1VL2_COUPLER"));
    }
    Generator& gh2 = network.getGenerator("GH2");
    extensions::SlackTerminal::reset(gh2.getTerminal().getVoltageLevel(), stdcxx::ref(gh2.getTerminal()));
    auto& slackTerminal = gh2.getTerminal().getVoltageLevel().getExtension<extensions::SlackTerminal>();
    BOOST_CHECK(stdcxx::areSame(gh2.getTerminal(), slackTerminal.getTerminal().get()));
    BOOST_CHECK_EQUAL(1, gh2.getTerminal().getReferrers().size());

    vl.convertToTopology(TopologyKind::BUS_BREAKER);
    BOOST_CHECK(stdcxx::areSame(gh2.getTerminal(), slackTerminal.getTerminal().get()));
    BOOST_CHECK_EQUAL(1, gh2.getTerminal().getReferrers().size());
}

BOOST_AUTO_TEST_CASE(testNodeBreakerToBusBreakerConnectionIssue) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("S1VL2");
    for (Switch& sw : vl.getSwitches()) {
        sw.setRetained((sw.getId()=="S1VL2_COUPLER"));
    }
    
    auto& vlIssue = network.newVoltageLevel()
        .setId("vl_issue")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400)
        .add();

    vlIssue.getNodeBreakerView().newBusbarSection()
        .setId("bbs")
        .setNode(0)
        .add();

    auto& line = network.newLine()
        .setId("line")
        .setVoltageLevel1("S1VL2")
        .setVoltageLevel2("vl_issue")
        .setNode1(999)
        .setNode2(1)
        .setR(0.1)
        .setX(1.0)
        .setG1(0.0)
        .setG2(0.0)
        .setB1(0.0)
        .setB2(0.0)
        .add();

    network.getVoltageLevel("S1VL2").getNodeBreakerView().newDisconnector()
        .setId("disconnector")
        .setNode1(0)
        .setNode2(999)
        .setOpen(false)
        .add();
    vlIssue.getNodeBreakerView().newDisconnector()
        .setId("disconnector2")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .add();
    vlIssue.newLoad()
        .setId("load")
        .setNode(2)
        .setP0(1.0)
        .setQ0(2.0)
        .add();
    vlIssue.getNodeBreakerView().newBreaker()
        .setId("breaker")
        .setNode1(0)
        .setNode2(2)
        .setOpen(true)
        .add();

    BOOST_CHECK(line.getTerminal1().isConnected());
    BOOST_CHECK(line.getTerminal2().isConnected());

    vlIssue.convertToTopology(TopologyKind::BUS_BREAKER);
    BOOST_CHECK(line.getTerminal1().isConnected());
    BOOST_CHECK(line.getTerminal2().isConnected());

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
