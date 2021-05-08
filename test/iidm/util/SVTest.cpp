/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/network/EurostagFactory.hpp>

namespace powsybl {

namespace iidm {

Network createDanglingLineTestNetwork() {
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
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newDanglingLine()
        .setId("DL1")
        .setName("DL1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setB(0.000193)
        .setG(0.0)
        .setP0(-300)
        .setQ0(-100)
        .setR(1.5)
        .setX(16.5)
        .setUcteXnodeCode("ucteXnodeCodeTest")
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(SVTestSuite)

// computation are not the same for every platforms (due to complex<double> support)
// => increase tolerance by using a threshold bigger than std::numeric_limits<double>::epsilon()
constexpr double ACCEPTABLE_THRESHOLD = 1e-6;

BOOST_AUTO_TEST_CASE(OtherSideTwtTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NGEN_NHV1");
    Terminal& terminal1 = twt.getTerminal1();
    Bus& bus1 = terminal1.getBusBreakerView().getBus().get();

    terminal1.setP(605.558349609375);
    terminal1.setQ(225.28251647949219);
    bus1.setV(24.500000610351563);
    bus1.setAngle(2.3259763717651367);
    SV sv(terminal1.getP(), terminal1.getQ(), bus1.getV(), bus1.getAngle());

    const SV& otherSide = sv.otherSide(twt);
    BOOST_CHECK_CLOSE(-604.89090825537278, otherSide.getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-197.48047051265698, otherSide.getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(402.142839934194, otherSide.getU(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(5.3007050229466078e-08, otherSide.getA(), ACCEPTABLE_THRESHOLD);
}

BOOST_AUTO_TEST_CASE(OtherSideLineTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");
    Terminal& terminal1 = line.getTerminal1();
    Bus& bus1 = terminal1.getBusBreakerView().getBus().get();

    terminal1.setP(302.44406127929688);
    terminal1.setQ(98.740272521972656);
    bus1.setV(402.14284515380859);
    bus1.setAngle(0);
    SV sv(terminal1.getP(), terminal1.getQ(), bus1.getV(), bus1.getAngle());

    const SV& otherSide = sv.otherSide(line);
    BOOST_CHECK_CLOSE(-300.26535137698056, otherSide.getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-137.19794660913607, otherSide.getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(387.38198654517674, otherSide.getU(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-3.4951525055825536, otherSide.getA(), ACCEPTABLE_THRESHOLD);
}

BOOST_AUTO_TEST_CASE(OtherSideDanglingLineTest) {
    Network network = createDanglingLineTestNetwork();
    DanglingLine& dl = network.getDanglingLine("DL1");
    Terminal& terminal1 = dl.getTerminal();
    Bus& bus1 = terminal1.getBusBreakerView().getBus().get();

    terminal1.setP(302.44406127929688);
    terminal1.setQ(98.740272521972656);
    bus1.setV(402.14284515380859);
    bus1.setAngle(0);
    SV sv(terminal1.getP(), terminal1.getQ(), bus1.getV(), bus1.getAngle());

    const SV& otherSide = sv.otherSide(dl);
    BOOST_CHECK_CLOSE(-301.47434650169686, otherSide.getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-118.85058330380885, otherSide.getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(396.50418762074077, otherSide.getU(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-1.7318099978500625, otherSide.getA(), ACCEPTABLE_THRESHOLD);
}

BOOST_AUTO_TEST_CASE(OtherSideY1Y2) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");
    Terminal& terminal1 = line.getTerminal1();
    Bus& bus1 = terminal1.getBusBreakerView().getBus().get();

    terminal1.setP(302.44406127929688);
    terminal1.setQ(98.740272521972656);
    bus1.setV(402.14284515380859);
    bus1.setAngle(0);
    SV sv(terminal1.getP(), terminal1.getQ(), bus1.getV(), bus1.getAngle());

    const SV& otherSide = sv.otherSideY1Y2(line);
    BOOST_CHECK_CLOSE(-300.43390740755751, otherSide.getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-137.18849721702034, otherSide.getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(389.95267268210063, otherSide.getU(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(-3.5063579035161601, otherSide.getA(), ACCEPTABLE_THRESHOLD);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
