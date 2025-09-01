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
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
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

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createNodeBreakerDanglingLineNetwork() {
    Network network("twoBusesWithLineAndDanglingLine", "test");
    double vn = 225.0;

    // First substation
    Substation& s1 = network.newSubstation()
            .setId("S1")
            .add();
    VoltageLevel& s1vl1 = s1.newVoltageLevel()
            .setId("S1VL1")
            .setNominalV(vn)
            .setLowVoltageLimit(vn * 0.9)
            .setHighVoltageLimit(vn * 1.1)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();

    s1vl1.getNodeBreakerView().newBusbarSection()
            .setId("S1VL1_BBS0A")
            .setName("S1VL1_BBS0A")
            .setNode(0)
            .add();
    s1vl1.getNodeBreakerView().newInternalConnection()
            .setNode1(0)
            .setNode2(1)
            .add();
    s1vl1.getNodeBreakerView().newInternalConnection()
            .setNode1(0)
            .setNode2(2)
            .add();

    Generator& generator1 = s1vl1.newGenerator()
            .setId("S1VL1-Generator")
            .setEnergySource(EnergySource::HYDRO)
            .setMinP(-500.0)
            .setMaxP(500.0)
            .setVoltageRegulatorOn(true)
            .setTargetP(80.0)
            .setTargetV(vn)
            .setTargetQ(10.0)
            .setNode(1)
            .add();
    generator1.newMinMaxReactiveLimits()
            .setMinQ(-500.0)
            .setMaxQ(500.0);
    generator1.getTerminal().setP(-80.0).setQ(-10.0);


    // Second substation
    Substation& s2 = network.newSubstation()
            .setId("S2")
            .add();
    VoltageLevel& s2vl1 = s2.newVoltageLevel()
            .setId("S2VL1")
            .setNominalV(vn)
            .setLowVoltageLimit(vn * 0.9)
            .setHighVoltageLimit(vn * 1.1)
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .add();

    s2vl1.getNodeBreakerView().newBusbarSection()
            .setId("S2VL1_BBS0")
            .setName("S2VL1_BBS0")
            .setNode(0)
            .add();
    s2vl1.getNodeBreakerView().newInternalConnection()
            .setNode1(0)
            .setNode2(1)
            .add();
    s2vl1.getNodeBreakerView().newInternalConnection()
            .setNode1(0)
            .setNode2(2)
            .add();
    s2vl1.getNodeBreakerView().newInternalConnection()
            .setNode1(0)
            .setNode2(3)
            .add();
            
    Load& load = s2vl1.newLoad()
            .setId("S2VL1-Load")
            .setLoadType(LoadType::UNDEFINED)
            .setP0(45.0)
            .setQ0(9.0)
            .setNode(1)
            .add();
    load.getTerminal().setP(45.0).setQ(9.0);

    network.getVoltageLevel("S2VL1").newDanglingLine()
            .setId("Dl-3")
            .setR(0.01)
            .setX(2.0)
            .setG(0.0)
            .setB(0.0005)
            .setP0(70.0)
            .setQ0(10.0)
            .setUcteXnodeCode("ucteNode")
            .setNode(3)
            .setEnsureIdUnicity(false)
            .add();

    // Line between both substations
    network.newLine()
            .setId("Line-2-2")
            .setR(0.01)
            .setX(2.0)
            .setG1(0.0)
            .setB1(0.0005)
            .setG2(0.0)
            .setB2(0.0005)
            .setNode1(2)
            .setVoltageLevel1("S1VL1")
            .setNode2(2)
            .setVoltageLevel2("S2VL1")
            .add();

    return network;
}

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

    SV svB1_noSplit = svB2.otherSide(dl, false);
    BOOST_CHECK_CLOSE(p1, svB1_noSplit.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1_noSplit.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1_noSplit.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1_noSplit.getA(), tol);

    SV svB1_split = svB2.otherSide(dl, true);
    BOOST_CHECK_CLOSE(164.26909345746856, svB1_split.getP(), tol);
    BOOST_CHECK_CLOSE(-65.013195498595877, svB1_split.getQ(), tol);
    BOOST_CHECK_CLOSE(124.29901066736439, svB1_split.getU(), tol);
    BOOST_CHECK_CLOSE(13.624022208160760, svB1_split.getA(), tol);
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
    BOOST_CHECK_EQUAL(Branch::Side::TWO, svA2.getSide());
    BOOST_CHECK_SMALL(std::abs(a2 - svA2.getA()), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);

    SV svB1_noSplit = svB2.otherSide(twt, false);
    BOOST_CHECK_CLOSE(p1, svB1_noSplit.getP(), tol);
    BOOST_CHECK_CLOSE(q1, svB1_noSplit.getQ(), tol);
    BOOST_CHECK_CLOSE(v1, svB1_noSplit.getU(), tol);
    BOOST_CHECK_CLOSE(a1, svB1_noSplit.getA(), tol);

    SV svB1_split = svB2.otherSide(twt, true);
    BOOST_CHECK_CLOSE(220.64483202933764, svB1_split.getP(), tol);
    BOOST_CHECK_CLOSE(8.6992618987675883, svB1_split.getQ(), tol);
    BOOST_CHECK_CLOSE(197.66467838729912, svB1_split.getU(), tol);
    BOOST_CHECK_CLOSE(19.983494806925435, svB1_split.getA(), tol);
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

BOOST_AUTO_TEST_CASE(testDCLine) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");
    line.setR(0.0);
    line.setX(5.917E-4);
    line.setG1(0.01);
    line.setB1(0.0020);
    line.setG2(0.01);
    line.setB2(0.0020);
    line.getTerminal1().getVoltageLevel().setNominalV(1.0);
    line.getTerminal2().getVoltageLevel().setNominalV(1.0);

    double tol = 0.0001;
    double p1 = 148.70937259543686;
    double q1 = stdcxx::nan();
    double v1 = stdcxx::nan();
    double a1 = 0.0;

    double p2 = -148.70937259543686;
    double q2 = stdcxx::nan();
    double v2 = stdcxx::nan();
    double a2 = -5.041532173036991;

    SV svA1(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(line);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(a2, svA2.getA(), tol);

    SV svB2(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(line);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_SMALL(svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testDCTwoWindingsTransformer) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NHV2_NLOAD");
    twt.setR(0.0043);
    twt.setX(0.0055618);
    twt.setG(0.0);
    twt.setB(0.0);

    twt.getRatioTapChanger().getCurrentStep().setRho(1.0);
    twt.getRatioTapChanger().getCurrentStep().setR(0.0);
    twt.getRatioTapChanger().getCurrentStep().setX(0.0);
    twt.getRatioTapChanger().getCurrentStep().setG(0.0);
    twt.getRatioTapChanger().getCurrentStep().setB(0.0);

    twt.setRatedU1(0.969);
    twt.setRatedU2(1.0);

    twt.getTerminal2().getVoltageLevel().setNominalV(1.0);

    double tol = 0.0001;
    double p1 = 1.4792780985886924;
    double q1 = stdcxx::nan();
    double v1 = stdcxx::nan();
    double a1 = -10.76932587556957;

    double p2 = -1.4792780985886924;
    double q2 = stdcxx::nan();
    double v2 = stdcxx::nan();
    double a2 = -11.226110634252219;

    SV svA1 = SV(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(twt);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(a2, svA2.getA(), tol);

    SV svB2 = SV(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testDCPhaseShifter) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.setR(0.0043);
    twt.setX(0.0020912);
    twt.setG(0.0);
    twt.setB(0.0);

    twt.getPhaseTapChanger().getCurrentStep().setRho(1.0);
    twt.getPhaseTapChanger().getCurrentStep().setAlpha(10.0);
    twt.getPhaseTapChanger().getCurrentStep().setR(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setX(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setG(0.0);
    twt.getPhaseTapChanger().getCurrentStep().setB(0.0);

    twt.setRatedU1(0.978);
    twt.setRatedU2(1.0);

    twt.getTerminal2().getVoltageLevel().setNominalV(1.0);

    double tol = 0.0001;
    double p1 = 58.02489256054598;
    double q1 = stdcxx::nan();
    double v1 = stdcxx::nan();
    double a1 = -10.76932587556957;

    double p2 = -58.02489256054598;
    double q2 = stdcxx::nan();
    double v2 = stdcxx::nan();
    double a2 = -7.56873858064591;

    SV svA1 = SV(p1, q1, v1, a1, Branch::Side::ONE);
    SV svA2 = svA1.otherSide(twt);
    BOOST_CHECK_CLOSE(p2, svA2.getP(), tol);
    BOOST_CHECK_CLOSE(a2, svA2.getA(), tol);

    SV svB2 = SV(p2, q2, v2, a2, Branch::Side::TWO);
    SV svB1 = svB2.otherSide(twt);
    BOOST_CHECK_CLOSE(p1, svB1.getP(), tol);
    BOOST_CHECK_CLOSE(a1, svB1.getA(), tol);
}

BOOST_AUTO_TEST_CASE(testOlfRealNetwork) {
    Network network = createNodeBreakerDanglingLineNetwork();
    Line& line = network.getLine("Line-2-2");
    DanglingLine& dl = network.getDanglingLine("Dl-3");
    Bus& bus1 = network.getBusBreakerView().getBus("S1VL1_0");
    Bus& bus2 = network.getBusBreakerView().getBus("S2VL1_0");

    // Voltages at the buses
    bus1.setV(225.0).setAngle(0.0);
    bus2.setV(225.2726820000).setAngle(-0.2603514491);

    line.getTerminal1().setP(115.003788).setQ(-56.302621);
    line.getTerminal2().setP(-115.000986).setQ(6.176675);

    dl.getTerminal().setP(70.000986).setQ(-15.176675);

    double tol = 0.00001;
    SV svL1 = SV(line.getTerminal1().getP(), line.getTerminal1().getQ(), bus1.getV(), bus1.getAngle(), Branch::Side::ONE);
    SV svL1other = svL1.otherSide(line);
    BOOST_CHECK_CLOSE(line.getTerminal2().getP(), svL1other.getP(), tol);
    BOOST_CHECK_CLOSE(line.getTerminal2().getQ(), svL1other.getQ(), tol);
    BOOST_CHECK_CLOSE(bus2.getV(), svL1other.getU(), tol);
    BOOST_CHECK_CLOSE(bus2.getAngle(), svL1other.getA(), tol);

    SV svL2 = SV(line.getTerminal2().getP(), line.getTerminal2().getQ(), bus2.getV(), bus2.getAngle(), Branch::Side::TWO);
    SV svL2other = svL2.otherSide(line);
    BOOST_CHECK_CLOSE(line.getTerminal1().getP(), svL2other.getP(), tol);
    BOOST_CHECK_CLOSE(line.getTerminal1().getQ(), svL2other.getQ(), tol);
    BOOST_CHECK_CLOSE(bus1.getV(), svL2other.getU(), tol);
    BOOST_CHECK_SMALL(svL2other.getA(), tol);

    BOOST_CHECK_CLOSE(line.getTerminal1().getP(), svL2.otherSideP(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1.0, 0.0), tol);
    BOOST_CHECK_CLOSE(line.getTerminal1().getQ(), svL2.otherSideQ(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1.0, 0.0), tol);
    BOOST_CHECK_CLOSE(bus1.getV(), svL2.otherSideU(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1.0, 0.0), tol);
    BOOST_CHECK_SMALL(svL2.otherSideA(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1.0, 0.0), tol);

    SV svDl1 = SV(dl.getTerminal().getP(), dl.getTerminal().getQ(), bus2.getV(), bus2.getAngle(), Branch::Side::ONE);
    SV svDl1other = svDl1.otherSide(dl);
    BOOST_CHECK_CLOSE(-dl.getP0(), svDl1other.getP(), tol);
    BOOST_CHECK_CLOSE(-dl.getQ0(), svDl1other.getQ(), tol);
    BOOST_CHECK_CLOSE(225.1798987500, svDl1other.getU(), tol);
    BOOST_CHECK_CLOSE(-0.4183680524, svDl1other.getA(), tol);

    BOOST_CHECK_SMALL(std::abs(-dl.getP0() - dl.getBoundary().getP()) / std::abs(dl.getBoundary().getP()), tol); // issue with BOOST_CHECK_CLOSE

}

BOOST_AUTO_TEST_CASE(testDcOlfRealNetwork) {
    Network network = createNodeBreakerDanglingLineNetwork();
    Line &line = network.getLine("Line-2-2");
    DanglingLine &dl = network.getDanglingLine("Dl-3");
    Bus &bus1 = network.getBusBreakerView().getBus("S1VL1_0");
    Bus &bus2 = network.getBusBreakerView().getBus("S2VL1_0");

    // Voltages at the buses
    bus1.setAngle(0.0);
    bus2.setAngle(-0.26030675136807774);
    line.getTerminal1().setP(115.0);
    line.getTerminal2().setP(-115.0);
    dl.getTerminal().setP(70.0);

    double tol = 0.00001;
    SV svL1 = SV(line.getTerminal1().getP(), line.getTerminal1().getQ(), bus1.getV(), bus1.getAngle(), Branch::Side::ONE);
    SV svL1other = svL1.otherSide(line);
    BOOST_CHECK_CLOSE(line.getTerminal2().getP(), svL1other.getP(), tol);
    BOOST_CHECK_CLOSE(bus2.getAngle(), svL1other.getA(), tol);

    SV svL2 = SV(line.getTerminal2().getP(), line.getTerminal2().getQ(), bus2.getV(), bus2.getAngle(), Branch::Side::TWO);
    SV svL2other = svL2.otherSide(line);
    BOOST_CHECK_CLOSE(line.getTerminal1().getP(), svL2other.getP(), tol);
    BOOST_CHECK_SMALL(svL2other.getA(), tol);

    SV svDl1 = SV(dl.getTerminal().getP(), dl.getTerminal().getQ(), bus2.getV(), bus2.getAngle(), Branch::Side::ONE);
    SV svDl1other = svDl1.otherSide(dl);
    BOOST_CHECK_CLOSE(-dl.getP0(), svDl1other.getP(), tol);
    BOOST_CHECK_CLOSE(-0.4187543391573424, svDl1other.getA(), tol);

    BOOST_CHECK_CLOSE(-dl.getP0(), dl.getBoundary().getP(), tol);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
