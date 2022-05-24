/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TerminalTestSuite)

class CustomTopologyTraverser : public VoltageLevel::TopologyTraverser {
public:  // VoltageLevel::TopologyTraverser
    bool traverse(Terminal& terminal, bool /*connected*/) override {
        m_traversedConnectables.insert(terminal.getConnectable().get().getId());
        return m_traverseTerminals;
    }

    bool traverse(Switch& aSwitch) override {
        m_traversedSwitches.insert(aSwitch.getId());
        return m_traverseSwitches;
    }

public:
    CustomTopologyTraverser(bool traverseTerminals, bool traverseSwitches) :
        m_traverseTerminals(traverseTerminals),
        m_traverseSwitches(traverseSwitches) {
    }

    CustomTopologyTraverser(const CustomTopologyTraverser&) = default;

    CustomTopologyTraverser(CustomTopologyTraverser&&) = default;

    ~CustomTopologyTraverser() noexcept override = default;

    CustomTopologyTraverser& operator=(const CustomTopologyTraverser&) = default;

    CustomTopologyTraverser& operator=(CustomTopologyTraverser&&) = default;

    const std::set<std::string>& getTraversedConnectables() const {
        return m_traversedConnectables;
    }

    const std::set<std::string>& getTraversedSwitches() const {
        return m_traversedSwitches;
    }

private:
    std::set<std::string> m_traversedConnectables;

    std::set<std::string> m_traversedSwitches;

    bool m_traverseTerminals;

    bool m_traverseSwitches;
};

void traversalTest(Terminal& terminal, bool traverseTerminals, bool traverseSwitches, const std::set<std::string>& expectedConnectables, const std::set<std::string>& expectedSwitches) {

    CustomTopologyTraverser traverser(traverseTerminals, traverseSwitches);
    terminal.traverse(traverser);

    const auto& traversedConnectables = traverser.getTraversedConnectables();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedConnectables.begin(), expectedConnectables.end(), traversedConnectables.begin(), traversedConnectables.end());

    const auto& traversedSwitches = traverser.getTraversedSwitches();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedSwitches.begin(), expectedSwitches.end(), traversedSwitches.begin(), traversedSwitches.end());
}

BOOST_AUTO_TEST_CASE(traverseBusBreaker) {
    Network network = createComponentsTestNetworkBB();

    CustomTopologyTraverser fullTraverser(true, true);
    network.getLoad("LOAD1").getTerminal().traverse(fullTraverser);
    const auto& fullTraversedConnectables = fullTraverser.getTraversedConnectables();
    const std::set<std::string>& fullExpectedConnectables = { "2WT_VL4_VL5", "3WT_VL1_VL2_VL3", "LCC1", "LOAD1", "LOAD2", "LOAD3", "LOAD4", "LOAD5", "LOAD6", "TL_VL4_VL6", "VL2_VL5" };
    BOOST_CHECK_EQUAL_COLLECTIONS(fullExpectedConnectables.begin(), fullExpectedConnectables.end(), fullTraversedConnectables.begin(), fullTraversedConnectables.end());
    BOOST_CHECK(fullTraverser.getTraversedSwitches().empty());

    CustomTopologyTraverser partialTraverser(false, true);
    network.getLoad("LOAD1").getTerminal().traverse(partialTraverser);
    const auto& partialTraversedConnectables = partialTraverser.getTraversedConnectables();
    const std::set<std::string>& partialExpectedConnectables = { "LOAD1" };
    BOOST_CHECK_EQUAL_COLLECTIONS(partialExpectedConnectables.begin(), partialExpectedConnectables.end(), partialTraversedConnectables.begin(), partialTraversedConnectables.end());
    BOOST_CHECK(partialTraverser.getTraversedSwitches().empty());
}

BOOST_AUTO_TEST_CASE(traverseNodeBreaker) {
    Network network = createComponentsTestNetworkNB();

    CustomTopologyTraverser fullTraverser(true, true);
    network.getLoad("LOAD1").getTerminal().traverse(fullTraverser);
    const auto& fullTraversedConnectables = fullTraverser.getTraversedConnectables();
    const auto& fullTraversedSwitches = fullTraverser.getTraversedSwitches();
    const std::set<std::string>& fullExpectedConnectables = { "2WT_VL4_VL5", "3WT_VL1_VL2_VL3", "LOAD1", "LOAD2", "LOAD3", "LOAD4", "LOAD5", "LOAD6", "TL_VL4_VL6", "VL2_VL5", "VSC1" };
    BOOST_CHECK_EQUAL_COLLECTIONS(fullExpectedConnectables.begin(), fullExpectedConnectables.end(), fullTraversedConnectables.begin(), fullTraversedConnectables.end());
    const std::set<std::string>& fullExpectedSwitches = { "SWB1", "SWB10", "SWB2", "SWB3", "SWB4", "SWB5", "SWB6", "SWB7", "SWB8", "SWB9" };
    BOOST_CHECK_EQUAL_COLLECTIONS(fullExpectedSwitches.begin(), fullExpectedSwitches.end(), fullTraversedSwitches.begin(), fullTraversedSwitches.end());

    CustomTopologyTraverser partialTraverser(false, true);
    network.getLoad("LOAD1").getTerminal().traverse(partialTraverser);
    const auto& partialTraversedConnectables = partialTraverser.getTraversedConnectables();
    const std::set<std::string>& partialExpectedConnectables = { "LOAD1" };
    BOOST_CHECK_EQUAL_COLLECTIONS(partialExpectedConnectables.begin(), partialExpectedConnectables.end(), partialTraversedConnectables.begin(), partialTraversedConnectables.end());
    BOOST_CHECK(partialTraverser.getTraversedSwitches().empty());
}

BOOST_AUTO_TEST_CASE(moveBbk) {
    Network network = createMixedTopolyKindNetwork();
    Load& load = network.getLoad("LOAD3");

    POWSYBL_ASSERT_THROW(load.getTerminal().getBusBreakerView().moveConnectable("wrongBusId", true), PowsyblException, "Bus 'wrongBusId' not found");

    BOOST_CHECK(load.getTerminal().isConnected());
    load.getTerminal().getBusBreakerView().moveConnectable("VL3_BUS2", false);
    BOOST_CHECK(!load.getTerminal().isConnected());

    load.getTerminal().getBusBreakerView().moveConnectable("VL3_BUS2", true);
    BOOST_CHECK(load.getTerminal().isConnected());
    BOOST_CHECK_EQUAL("VL3_BUS2", load.getTerminal().getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("VL3_BUS2", load.getTerminal().getBusBreakerView().getConnectableBus().get().getId());
}

BOOST_AUTO_TEST_CASE(moveNbk) {
    Network network = createMixedTopolyKindNetwork();
    Load& load = network.getLoad("LOAD1");

    BOOST_CHECK_EQUAL(2, load.getTerminal().getNodeBreakerView().getNode());

    POWSYBL_ASSERT_THROW(load.getTerminal().getNodeBreakerView().moveConnectable(100, "wrongVoltageLevelId"), PowsyblException, "Voltage level 'wrongVoltageLevelId' not found");
    POWSYBL_ASSERT_THROW(load.getTerminal().getNodeBreakerView().moveConnectable(100, "VL3"), PowsyblException, "Trying to move connectable LOAD1 to node 100 of voltage level VL3, which is a bus breaker voltage level");

    load.getTerminal().getNodeBreakerView().moveConnectable(100, "VL2");
    BOOST_CHECK_EQUAL(100, load.getTerminal().getNodeBreakerView().getNode());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
