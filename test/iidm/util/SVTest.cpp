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
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>

#include "../NetworkFactory.hpp" // TODO(sebalaig) To be discussed...

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
        .setB(0.0016)
        .setG(0.01)
        .setP0(-300)
        .setQ0(-100)
        .setR(10.30)
        .setX(40.20)
        .setUcteXnodeCode("ucteXnodeCodeTest")
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(SVTestSuite)

BOOST_AUTO_TEST_CASE(testLine) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");
    line.setR(0.15);
    line.setX(0.25);
    line.setG1(0.01);
    line.setB1(0.0020);
    line.setG2(0.01);
    line.setB2(0.0020);

    double tol = 0.0001;
    double p1 = 485.306701;
    double q1 = 48.537745;
    double v1 = 138.0;
    double a1 = 0.0;

    double p2 = -104.996276;
    double q2 = -123.211145;
    double v2 = 137.5232696533203;
    double a2 = -0.18332427740097046;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(line);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(q2, svA2.getQ(), tol);
    BOOST_CHECK_CLOSE(v2, svA2.getU(), tol);
    BOOST_CHECK_CLOSE(a2, svA2.getA(), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(line);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_SMALL(svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testDanglingLine) {
    Network network = createDanglingLineTestNetwork();
    DanglingLine& dl = network.getDanglingLine("DL1");
    dl.setR(10.30);
    dl.setX(40.20);
    dl.setG(0.01);
    dl.setB(0.0016);

    double tol = 0.0001;
    double p1 = 126.818177;
    double q1 = -77.444122;
    double v1 = 118.13329315185547;
    double a1 = 0.19568365812301636;

    double p2 = 15.098317;
    double q2 = 64.333028;
    double v2 = 138.0;
    double a2 = 0.0;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(dl);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(q2, svA2.getQ(), tol);
    BOOST_CHECK_CLOSE(v2, svA2.getU(), tol);
    BOOST_CHECK_SMALL(svA2.getA(), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(dl);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);

    BOOST_CHECK_CLOSE(p2, svA1.otherSideP(dl), tol);
    BOOST_CHECK_CLOSE(q2, svA1.otherSideQ(dl), tol);
    BOOST_CHECK_CLOSE(v2, svA1.otherSideU(dl), tol);
    BOOST_CHECK_SMALL(svA1.otherSideA(dl), tol);

    BOOST_CHECK_CLOSE(p1, svB2.otherSideP(dl), tol);
    BOOST_CHECK_CLOSE(q1, svB2.otherSideQ(dl), tol);
    BOOST_CHECK_CLOSE(v1, svB2.otherSideU(dl), tol);
    BOOST_CHECK_CLOSE(a1, svB2.otherSideA(dl), tol);

    BOOST_CHECK_CLOSE(p2, svA1.otherSideP(dl, false), tol);
    BOOST_CHECK_CLOSE(q2, svA1.otherSideQ(dl, false), tol);
    BOOST_CHECK_CLOSE(v2, svA1.otherSideU(dl, false), tol);
    BOOST_CHECK_SMALL(svA1.otherSideA(dl, false), tol);

    BOOST_CHECK_CLOSE(p1, svB2.otherSideP(dl, false), tol);
    BOOST_CHECK_CLOSE(q1, svB2.otherSideQ(dl, false), tol);
    BOOST_CHECK_CLOSE(v1, svB2.otherSideU(dl, false), tol);
    BOOST_CHECK_CLOSE(a1, svB2.otherSideA(dl, false), tol);
}

BOOST_AUTO_TEST_CASE(testTwoWindingsTransformer) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.setR(0.43);
    twt.setX(15.90);
    twt.setG(0.0);
    twt.setB(0.0);

    twt.getPhaseTapChanger().getCurrentStep().setRho(0.98);
    twt.getPhaseTapChanger().getCurrentStep().setAlpha(-5.0);
    twt.getPhaseTapChanger().getCurrentStep().setR(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setX(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setG(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setB(0.0);

    twt.setRatedU1(230.0);
    twt.setRatedU2(138.0);

    twt.newRatioTapChanger()
        .beginStep()
            .setRho(1)
            .setR(0.0)
            .setX(0.0)
            .setG(0.0)
            .setB(0.0)
        .endStep()
        .setTapPosition(0)
        .setLoadTapChangingCapabilities(true)
        .setRegulating(false)
        .setTargetV(158.0)
        .setTargetDeadband(0.0)
        .add();

    double tol = 0.0001;
    double p1 = 220.644832;
    double q1 = 8.699260;
    double v1 = 197.66468811035156;
    double a1 = 19.98349380493164;

    double p2 = -219.092739;
    double q2 = 48.692085;
    double v2 = 118.13329315185547;
    double a2 = 0.19568365812301636;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(twt);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(q2, svA2.getQ(), tol);
    BOOST_CHECK_CLOSE(v2, svA2.getU(), tol);
    BOOST_CHECK_SMALL(std::abs(a2 - svA2.getA()), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testTwoWindingsTransformerWithoutRtc) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.setR(0.43);
    twt.setX(15.90);
    twt.setG(0.0);
    twt.setB(0.0);

    twt.getPhaseTapChanger().getCurrentStep().setRho(0.98);
    twt.getPhaseTapChanger().getCurrentStep().setAlpha(-5.0);
    twt.getPhaseTapChanger().getCurrentStep().setR(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setX(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setG(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setB(0.0);

    twt.setRatedU1(230.0);
    twt.setRatedU2(138.0);

    double tol = 0.0001;
    double p1 = 220.644832;
    double q1 = 8.699260;
    double v1 = 197.66468811035156;
    double a1 = 19.98349380493164;

    double p2 = -219.092739;
    double q2 = 48.692085;
    double v2 = 118.13329315185547;
    double a2 = 0.19568365812301636;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(twt);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(q2, svA2.getQ(), tol);
    BOOST_CHECK_CLOSE(v2, svA2.getU(), tol);
    BOOST_CHECK_SMALL(std::abs(a2 - svA2.getA()), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testTwoWindingsTransformerWithoutPtc) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NHV2_NLOAD");
    twt.setR(0.43);
    twt.setX(15.90);
    twt.setG(0.0);
    twt.setB(0.0);

    twt.getRatioTapChanger().getCurrentStep().setRho(0.98);
    twt.getRatioTapChanger().getCurrentStep().setR(0.0);
    twt.getRatioTapChanger().getCurrentStep().setX(0.0);
    twt.getRatioTapChanger().getCurrentStep().setG(0.0);
    twt.getRatioTapChanger().getCurrentStep().setB(0.0);

    twt.setRatedU1(230.0);
    twt.setRatedU2(138.0);

    double tol = 0.0001;
    double p1 = 220.644832;
    double q1 = 8.699260;
    double v1 = 197.66468811035156;
    double a1 = 19.98349380493164;

    double p2 = -219.092739124819760;
    double q2 = 48.692081198528110;
    double v2 = 118.133298648525750;
    double a2 = 5.195684102383955;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(twt);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(q2, svA2.getQ(), tol);
    BOOST_CHECK_CLOSE(v2, svA2.getU(), tol);
    BOOST_CHECK_CLOSE(a2, svA2.getA(), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testHalfLine) {
    Network network = createComponentsTestNetworkBB();
    auto& tieLine = dynamic_cast<TieLine&>(network.getLine("TL_VL4_VL6"));
    TieLine::HalfLine& halfLine = tieLine.getHalf1();
    halfLine.setR(0.15);
    halfLine.setX(0.25);
    halfLine.setG1(0.01);
    halfLine.setB1(0.0020);
    halfLine.setG2(0.01);
    halfLine.setB2(0.0020);

    double tol = 0.0001;
    double p1 = 485.306701;
    double q1 = 48.537745;
    double v1 = 138.0;
    double a1 = 0.0;

    double p2 = -104.996276;
    double q2 = -123.211145;
    double v2 = 137.5232696533203;
    double a2 = -0.18332427740097046;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);

    BOOST_CHECK_CLOSE(p2, svA1.otherSideP(halfLine), tol);
    BOOST_CHECK_CLOSE(q2, svA1.otherSideQ(halfLine), tol);
    BOOST_CHECK_CLOSE(v2, svA1.otherSideU(halfLine), tol);
    BOOST_CHECK_CLOSE(a2, svA1.otherSideA(halfLine), tol);

    BOOST_CHECK_CLOSE(p1, svB2.otherSideP(halfLine), tol);
    BOOST_CHECK_CLOSE(q1, svB2.otherSideQ(halfLine), tol);
    BOOST_CHECK_CLOSE(v1, svB2.otherSideU(halfLine), tol);
    BOOST_CHECK_SMALL(svB2.otherSideA(halfLine), tol);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
