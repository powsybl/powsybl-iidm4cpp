/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <functional>
#include <set>
#include <string>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "NetworkFactory.hpp"

namespace std {

std::ostream& operator<<(std::ostream& stream, const std::pair<std::string, int>& ip) {
    stream << ip.first << " " << ip.second;
    return stream;
}

}  // namespace std

namespace powsybl {

namespace iidm {

using IdPos = std::pair<std::string, int>;

using IdPosSet = std::set<IdPos, std::less<IdPos>>;

class CustomTopologyTraverser : public Terminal::TopologyTraverser {
public:
    using SwitchTest = std::function<math::TraverseResult(Switch&)>;

    using TerminalTest = std::function<math::TraverseResult(Terminal&)>;

public:  // voltage_level::TopologyTraverser
    math::TraverseResult traverse(Terminal& terminal, bool /*connected*/) override {
        auto pair = m_visited.insert(terminal);
        if (!pair.second) {
            BOOST_FAIL("Traversing an already visited terminal");
        }
        return m_terminalTest(terminal);
    }

    math::TraverseResult traverse(Switch& aSwitch) override {
        return m_switchTest(aSwitch);
    }

public:
    CustomTopologyTraverser(TerminalSet& visited, const SwitchTest& switchTest, const TerminalTest& terminalTest) :
        m_visited(visited),
        m_switchTest(switchTest),
        m_terminalTest(terminalTest) {
    }

private:
    TerminalSet& m_visited;

    SwitchTest m_switchTest;

    TerminalTest m_terminalTest;
};

static int indexOfTerminal(const Terminal& terminal) {
    const auto& allTerminals = terminal.getConnectable().get().getTerminals();
    auto it = std::find_if(allTerminals.begin(), allTerminals.end(), [&terminal](const std::reference_wrapper<Terminal>& term) {
        return stdcxx::areSame(terminal, term.get());
    });
    return it != allTerminals.end() ? it - allTerminals.begin() : -1;
}

IdPosSet getVisitedList(Terminal& start, const CustomTopologyTraverser::SwitchTest& switchTest, const CustomTopologyTraverser::TerminalTest& terminalTest) {
    TerminalSet visited;

    CustomTopologyTraverser traverser(visited, switchTest, terminalTest);
    start.traverse(traverser);

    IdPosSet visitedInfos;
    std::transform(visited.begin(), visited.end(), std::inserter(visitedInfos,visitedInfos.begin()), [](const std::reference_wrapper<Terminal>& terminal) {
        return std::make_pair(terminal.get().getConnectable().get().getId(), indexOfTerminal(terminal.get()));
    });

    return visitedInfos;
}

IdPosSet getVisitedList(Terminal& start, const CustomTopologyTraverser::SwitchTest& switchTest) {
    const CustomTopologyTraverser::TerminalTest& terminalTest = [](Terminal& /*Terminal*/) {
        return math::TraverseResult::CONTINUE;
    };
    return getVisitedList(start, switchTest, terminalTest);
}

IdPosSet getVisitedList(Terminal& start) {
    const CustomTopologyTraverser::SwitchTest& switchTest = [](Switch& /*sw*/) {
        return math::TraverseResult::CONTINUE;
    };
    return getVisitedList(start, switchTest);
}

BOOST_AUTO_TEST_SUITE(TopologyTraverserTestSuite)

BOOST_AUTO_TEST_CASE(test1) {
    Network network = createNodeBreakerNetwork();
    Terminal& start = network.getGenerator("G").getTerminal();
    const auto& res = getVisitedList(start);

    IdPosSet expected = {{"G", 0}, {"BBS1", 0}, {"L1", 0}, {"L1", 1}, {"BBS2", 0}, {"LD", 0}};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE(test2) {
    Network network = createNodeBreakerNetwork();
    Terminal& start = network.getVoltageLevel("VL1").getNodeBreakerView().getBusbarSection("BBS1").get().getTerminal();
    const auto& res = getVisitedList(start, [](Switch& sw) {
        return (!sw.isOpen() && sw.getKind() != SwitchKind::BREAKER) ? math::TraverseResult::CONTINUE : math::TraverseResult::TERMINATE_PATH;
    });

    IdPosSet expected = { {"BBS1", 0}, {"G", 0} };

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE(test3) {
    Network network = createMixedNodeBreakerBusBreakerNetwork();
    Terminal& start = network.getGenerator("G").getTerminal();
    const auto& visited1 = getVisitedList(start);
    IdPosSet expected1 = { {"BBS1", 0}, {"G", 0}, {"L1", 0}, 
                           {"L1", 1}, {"BBS2", 0}, {"LD", 0}, {"L2", 0},
                           {"L2", 1}, {"LD2", 0} };
    BOOST_CHECK_EQUAL_COLLECTIONS(expected1.begin(), expected1.end(), visited1.begin(), visited1.end());

    const auto& visited2 = getVisitedList(start, [](Switch& /*sw*/) { return math::TraverseResult::CONTINUE; }, [](Terminal& terminal) {
        return terminal.getConnectable().get().getId() == "L2" ? math::TraverseResult::TERMINATE_PATH : math::TraverseResult::CONTINUE;
    });
    IdPosSet expected2 = { {"G", 0}, {"BBS1", 0}, {"L1", 0}, {"L1", 1},
                           {"BBS2", 0}, {"LD", 0}, {"L2", 0} };
    BOOST_CHECK_EQUAL_COLLECTIONS(expected2.begin(), expected2.end(), visited2.begin(), visited2.end());

    const auto& visited3 = getVisitedList(network.getLoad("LD2").getTerminal(), [](Switch& /*sw*/) { return math::TraverseResult::CONTINUE; }, [](Terminal& terminal) {
        return terminal.getConnectable().get().getId() == "L2" ? math::TraverseResult::TERMINATE_PATH : math::TraverseResult::CONTINUE;
    });
    IdPosSet expected3 = { {"LD2", 0}, {"L2", 1} };
    BOOST_CHECK_EQUAL_COLLECTIONS(expected3.begin(), expected3.end(), visited3.begin(), visited3.end());
}

BOOST_AUTO_TEST_CASE(test4) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Terminal& start = network.getGenerator("GEN").getTerminal();
    const auto& res = getVisitedList(start);

    IdPosSet expected = { {"GEN", 0}, {"NGEN_NHV1", 0}, {"NGEN_NHV1", 1}, 
                          {"NHV1_NHV2_1", 0}, {"NHV1_NHV2_2", 0}, {"NHV1_NHV2_1", 1}, 
                          {"NHV1_NHV2_2", 1}, {"NHV2_NLOAD", 0}, {"NHV2_NLOAD", 1}, {"LOAD", 0} };

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE(test5) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    // Duplicate 2wt to go from VLGEN to VLHV1 even if traverser stops at one of them
    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("NGEN_NHV1");
    TwoWindingsTransformer& duplicatedTransformer = network.getSubstation("P1")
        .newTwoWindingsTransformer()
        .setId("duplicate")
        .setVoltageLevel1("VLGEN").setBus1("NGEN")
        .setVoltageLevel2("VLHV1").setBus2("NHV1")
        .setRatedU1(transformer.getRatedU1())
        .setRatedU2(transformer.getRatedU2())
        .setR(transformer.getR())
        .setX(transformer.getX())
        .setG(transformer.getG())
        .setB(transformer.getB())
        .add();

    Terminal& start = network.getGenerator("GEN").getTerminal();
    const auto& res = getVisitedList(start, [](Switch& /*sw*/) { return math::TraverseResult::CONTINUE; }, [&duplicatedTransformer](Terminal& terminal) {
        return (stdcxx::areSame(terminal.getConnectable().get(), duplicatedTransformer) && terminal.getVoltageLevel().getId() == "VLGEN") ? math::TraverseResult::TERMINATE_PATH : math::TraverseResult::CONTINUE;
    });
    IdPosSet expected = { {"GEN", 0}, {"NGEN_NHV1", 0}, {"duplicate", 0}, {"NGEN_NHV1", 1},
                          {"NHV1_NHV2_1", 0}, {"NHV1_NHV2_2", 0}, {"duplicate", 1}, {"NHV1_NHV2_1", 1},
                          {"NHV1_NHV2_2", 1}, {"NHV2_NLOAD", 0}, {"NHV2_NLOAD", 1}, {"LOAD", 0} };

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
