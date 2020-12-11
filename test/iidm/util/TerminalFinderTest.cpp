/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/TerminalFinder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/FictitiousSwitchFactory.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TerminalFinderTestSuite)

BOOST_AUTO_TEST_CASE(testBbsTerminal) {
    Network network = powsybl::network::FictitiousSwitchFactory::create();
    VoltageLevel& vlN = network.getVoltageLevel("N");
    stdcxx::Reference<Terminal> optTerminalN0 = vlN.getNodeBreakerView().getOptionalTerminal(0);
    BOOST_CHECK(static_cast<bool>(optTerminalN0));

    const auto& terminalStream = optTerminalN0.get().getBusBreakerView().getBus().get().getConnectedTerminals();
    stdcxx::Reference<Terminal> bestOptTerminal = TerminalFinder::find(terminalStream);
    BOOST_CHECK(static_cast<bool>(bestOptTerminal));

    Terminal& bestTerminal = bestOptTerminal.get();
    BOOST_CHECK_EQUAL(ConnectableType::BUSBAR_SECTION, bestTerminal.getConnectable().get().getType());
    BOOST_CHECK_EQUAL("N_0", bestTerminal.getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("N_0", bestTerminal.getBusView().getBus().get().getId());
}

BOOST_AUTO_TEST_CASE(testNoTerminal) {
    Network network = powsybl::network::FictitiousSwitchFactory::create();
    VoltageLevel& vlN = network.getVoltageLevel("N");

    Bus& bus = vlN.getBusBreakerView().getBus("N_13");

    const auto& terminalIter = bus.getConnectedTerminals();
    BOOST_CHECK(!static_cast<bool>(TerminalFinder::find(terminalIter)));
}

BOOST_AUTO_TEST_CASE(testLineTerminal1) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vlhv2 = network.getVoltageLevel("VLHV1");

    Bus& bus = vlhv2.getBusBreakerView().getBus("NHV1");

    const auto& terminalStream = bus.getConnectedTerminals();
    stdcxx::Reference<Terminal> bestOptTerminal = TerminalFinder::find(terminalStream);
    BOOST_CHECK(static_cast<bool>(bestOptTerminal));

    Terminal& bestTerminal = bestOptTerminal.get();
    BOOST_CHECK_EQUAL(ConnectableType::LINE, bestTerminal.getConnectable().get().getType());
    BOOST_CHECK_EQUAL("NHV1", bestTerminal.getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("VLHV1_0", bestTerminal.getBusView().getBus().get().getId());
}

BOOST_AUTO_TEST_CASE(testLineTerminal2) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vlhv2 = network.getVoltageLevel("VLHV2");

    Bus& bus = vlhv2.getBusBreakerView().getBus("NHV2");

    const auto& terminalStream = bus.getConnectedTerminals();
    stdcxx::Reference<Terminal> bestOptTerminal = TerminalFinder::find(terminalStream);
    BOOST_CHECK(static_cast<bool>(bestOptTerminal));

    Terminal& bestTerminal = bestOptTerminal.get();
    BOOST_CHECK_EQUAL(ConnectableType::LINE, bestTerminal.getConnectable().get().getType());
    BOOST_CHECK_EQUAL("NHV2", bestTerminal.getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("VLHV2_0", bestTerminal.getBusView().getBus().get().getId());
}

BOOST_AUTO_TEST_CASE(testGeneratorTerminal) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vlgen = network.getVoltageLevel("VLGEN");

    const Bus& cBus = vlgen.getBusBreakerView().getBus("NGEN");
    const auto& cTerminalStream = cBus.getConnectedTerminals();
    stdcxx::CReference<Terminal> cBestOptTerminal = TerminalFinder::find(cTerminalStream);
    BOOST_CHECK(static_cast<bool>(cBestOptTerminal));

    Bus& bus = vlgen.getBusBreakerView().getBus("NGEN");
    const auto& terminalStream = bus.getConnectedTerminals();
    stdcxx::Reference<Terminal> bestOptTerminal = TerminalFinder::find(terminalStream);
    BOOST_CHECK(static_cast<bool>(bestOptTerminal));

    Terminal& bestTerminal = bestOptTerminal.get();
    BOOST_CHECK_EQUAL(ConnectableType::GENERATOR, bestTerminal.getConnectable().get().getType());
    BOOST_CHECK_EQUAL("NGEN", bestTerminal.getBusBreakerView().getBus().get().getId());
    BOOST_CHECK_EQUAL("VLGEN_0", bestTerminal.getBusView().getBus().get().getId());
}

BOOST_AUTO_TEST_CASE(testTerminalFromComparator) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vlgen = network.getVoltageLevel("VLGEN");
    const Bus& cBus = vlgen.getBusBreakerView().getBus("NGEN");
    Bus& bus = vlgen.getBusBreakerView().getBus("NGEN");

    const auto& cTerminalStream = cBus.getConnectedTerminals();
    const auto& terminalStream = bus.getConnectedTerminals();

    // sort by id alphabetically
    const TerminalFinder::Comparator& terminalFinderLess = [](const Terminal& t1, const Terminal& t2) {
        return t1.getConnectable().get().getId() < t2.getConnectable().get().getId();
    };
    const Terminal& cBestLessTerminal = TerminalFinder::find(terminalFinderLess, cTerminalStream).get();
    BOOST_CHECK_EQUAL("NGEN_NHV1", cBestLessTerminal.getConnectable().get().getNameOrId());

    Terminal& bestLessTerminal = TerminalFinder::find(terminalFinderLess, terminalStream).get();
    BOOST_CHECK_EQUAL("NGEN_NHV1", bestLessTerminal.getConnectable().get().getNameOrId());

    // sort by id reverse alphabetically
    const TerminalFinder::Comparator& terminalFinderMore([](const Terminal& t1, const Terminal& t2) {
        return t1.getConnectable().get().getId() > t2.getConnectable().get().getId();
    });
    Terminal& bestMoreTerminal = TerminalFinder::find(terminalFinderMore, terminalStream).get();
    BOOST_CHECK_EQUAL("GEN", bestMoreTerminal.getConnectable().get().getNameOrId());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
