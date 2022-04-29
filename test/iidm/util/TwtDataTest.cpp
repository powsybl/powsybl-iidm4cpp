/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <limits>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/util/TwtData.hpp>

namespace powsybl {

namespace iidm {

double P1 = 99.218431;
double Q1 = 2.7304328;
double P2 = -216.19819;
double Q2 = -85.368180;
double P3 = 118;
double Q3 = 92.612077;

double U1 = 412.989001;
double ANGLE1 = -6.78071;
double U2 = 224.315268;
double ANGLE2 = -8.77012;
double U3 = 21.987;
double ANGLE3 = -6.6508;

double STAR_U = 412.662007016922757;
double STAR_ANGLE = -7.353686938578365;

double R1 = 0.898462;
double X1 = 17.204128;
double G11 = 0;
double B11 = 2.4375E-6;
double G12 = 0;
double B12 = 0;
double RATED_U1 = 400;
double R2 = 1.070770247933884;
double X2 = 19.6664;
double G21 = 0;
double B21 = 0;
double G22 = 0;
double B22 = 0;
double RATED_U2 = 220;
double R3 = 4.837006802721089;
double X3 = 21.76072562358277;
double G31 = 0;
double B31 = 0;
double G32 = 0;
double B32 = 0;
double RATED_U3 = 21;
int PHASE_ANGLE_CLOCK_2 = 0;
int PHASE_ANGLE_CLOCK_3 = 0;
double RATED_U0 = RATED_U1;

Network createTwtDataTestNetwork() {
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

    vl1.newLoad()
        .setId("LOAD1")
        .setName("LOAD1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    vl2.newLoad()
        .setId("LOAD2")
        .setName("LOAD2_NAME")
        .setBus(vl2Bus1.getId())
        .setConnectableBus(vl2Bus1.getId())
        .setLoadType(LoadType::UNDEFINED)
        .setP0(60.0)
        .setQ0(70.0)
        .add();

    VoltageLevel& vl3 = substation.newVoltageLevel()
        .setId("VL3")
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL1_VL2_VL3")
        .setName("3WT_VL1_VL2_VL3_NAME")
        .setRatedU0(RATED_U0)
        .newLeg1()
            .setR(1.3)
            .setX(1.4)
            .setG(1.6)
            .setB(1.7)
            .setRatedU(1.1)
            .setRatedS(2.2)
            .setVoltageLevel(vl1.getId())
            .setBus(vl1Bus1.getId())
            .setConnectableBus(vl1Bus1.getId())
        .add()
        .newLeg2()
            .setR(2.3)
            .setX(2.4)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(2.1)
            .setVoltageLevel(vl2.getId())
            .setBus(vl2Bus1.getId())
            .setConnectableBus(vl2Bus1.getId())
        .add()
        .newLeg3()
            .setR(3.3)
            .setX(3.4)
            .setG(0.0)
            .setB(0.0)
            .setRatedU(3.1)
            .setVoltageLevel(vl3.getId())
            .setBus(vl3Bus1.getId())
            .setConnectableBus(vl3Bus1.getId())
        .add()
        .add();

    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");

    twt.getLeg1().getTerminal().getBusBreakerView().getBus().get().setV(U1);
    twt.getLeg1().getTerminal().getBusBreakerView().getBus().get().setAngle(ANGLE1);
    twt.getLeg1().getTerminal().setP(P1);
    twt.getLeg1().getTerminal().setQ(Q1);
    twt.getLeg1().setR(R1);
    twt.getLeg1().setX(X1);
    twt.getLeg1().setRatedU(RATED_U1);
    twt.getLeg1().setB(B11 + B12);
    twt.getLeg1().setG(G11 + G12);

    twt.getLeg2().getTerminal().getBusBreakerView().getBus().get().setV(U2);
    twt.getLeg2().getTerminal().getBusBreakerView().getBus().get().setAngle(ANGLE2);
    twt.getLeg2().getTerminal().setP(P2);
    twt.getLeg2().getTerminal().setQ(Q2);
    twt.getLeg2().setR(R2);
    twt.getLeg2().setX(X2);
    twt.getLeg2().setRatedU(RATED_U2);
    twt.getLeg2().setB(B21 + B22);
    twt.getLeg2().setG(G21 + G22);

    twt.getLeg3().getTerminal().getBusBreakerView().getBus().get().setV(U3);
    twt.getLeg3().getTerminal().getBusBreakerView().getBus().get().setAngle(ANGLE3);
    twt.getLeg3().getTerminal().setP(P3);
    twt.getLeg3().getTerminal().setQ(Q3);
    twt.getLeg3().setR(R3);
    twt.getLeg3().setX(X3);
    twt.getLeg3().setRatedU(RATED_U3);
    twt.getLeg3().setB(B31 + B32);
    twt.getLeg3().setG(G31 + G32);

    return network;
}

struct T3xFlow {
    double p1 = stdcxx::nan();
    double q1 = stdcxx::nan();
    double p2 = stdcxx::nan();
    double q2 = stdcxx::nan();
    double p3 = stdcxx::nan();
    double q3 = stdcxx::nan();
};

bool sameFlow(const T3xFlow& expected, const T3xFlow& actual) {
    double tol = 0.00001;
    return !((!std::isnan(expected.p1) && std::isnan(actual.p1)) ||
        (!std::isnan(expected.q1) && std::isnan(actual.q1)) ||
        (!std::isnan(expected.p2) && std::isnan(actual.p2)) ||
        (!std::isnan(expected.q2) && std::isnan(actual.q2)) ||
        (!std::isnan(expected.p3) && std::isnan(actual.p3)) ||
        (!std::isnan(expected.q3) && std::isnan(actual.q3)) ||
        std::abs(expected.p1 - actual.p1) > tol ||
        std::abs(expected.q1 - actual.q1) > tol ||
        std::abs(expected.p2 - actual.p2) > tol ||
        std::abs(expected.q2 - actual.q2) > tol ||
        std::abs(expected.p3 - actual.p3) > tol ||
        std::abs(expected.q3 - actual.q3) > tol);
}

bool t3xCompareFlow(const TwtData& twtData, double p1, double q1, double p2, double q2, double p3, double q3) {
    T3xFlow actual;
    actual.p1 = twtData.getComputedP(ThreeWindingsTransformer::Side::ONE);
    actual.q1 = twtData.getComputedQ(ThreeWindingsTransformer::Side::ONE);
    actual.p2 = twtData.getComputedP(ThreeWindingsTransformer::Side::TWO);
    actual.q2 = twtData.getComputedQ(ThreeWindingsTransformer::Side::TWO);
    actual.p3 = twtData.getComputedP(ThreeWindingsTransformer::Side::THREE);
    actual.q3 = twtData.getComputedQ(ThreeWindingsTransformer::Side::THREE);

    T3xFlow expected;
    expected.p1 = p1;
    expected.q1 = q1;
    expected.p2 = p2;
    expected.q2 = q2;
    expected.p3 = p3;
    expected.q3 = q3;

    return sameFlow(expected, actual);
}

bool t3xCompareStarBusVoltage(const TwtData& twtData, double starU, double starAngle) {
    double tol = 0.00001;
    return !(std::isnan(twtData.getStarU()) && !std::isnan(starU)) ||
        (std::isnan(twtData.getStarTheta()) && !std::isnan(starAngle)) ||
        std::abs(twtData.getStarU() - starU) > tol ||
        std::abs(stdcxx::toDegrees * twtData.getStarTheta() - starAngle) > tol;
}

BOOST_AUTO_TEST_SUITE(TwtDataTestSuite)

BOOST_AUTO_TEST_CASE(test) {
    Network network = createTwtDataTestNetwork();
    const ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    TwtData twtData(twt, 0, false, false);

    BOOST_CHECK_CLOSE(P1, twtData.getComputedP(ThreeWindingsTransformer::Side::ONE), 0.3);
    BOOST_CHECK_CLOSE(Q1, twtData.getComputedQ(ThreeWindingsTransformer::Side::ONE), 0.7); // 2.7304328 != 2.7471471852083678
    BOOST_CHECK_CLOSE(P2, twtData.getComputedP(ThreeWindingsTransformer::Side::TWO), 0.3);
    BOOST_CHECK_CLOSE(Q2, twtData.getComputedQ(ThreeWindingsTransformer::Side::TWO), 0.3);
    BOOST_CHECK_CLOSE(P3, twtData.getComputedP(ThreeWindingsTransformer::Side::THREE), 0.3);
    BOOST_CHECK_CLOSE(Q3, twtData.getComputedQ(ThreeWindingsTransformer::Side::THREE), 0.3);

    BOOST_CHECK_CLOSE(STAR_U, twtData.getStarU(), 0.0001);
    BOOST_CHECK_CLOSE(STAR_ANGLE, stdcxx::toDegrees * twtData.getStarTheta(), 0.0001);
}

BOOST_AUTO_TEST_CASE(testSplitShuntAdmittance) {
    Network network = createTwtDataTestNetwork();
    const ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    TwtData twtData(twt, 0, false, true);

    bool ok = t3xCompareFlow(twtData, 99.231950, 2.876479, -216.194348, -85.558437, 117.981856, 92.439531);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd1End2End3Connected) {
    Network network = createTwtDataTestNetwork();
    const ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    TwtData twtData(twt, 0, false);

    bool ok = t3xCompareFlow(twtData, 99.227288294050, 2.747147185208, -216.195866533486, -85.490493190353, 117.988318295633, 92.500849015581);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 412.66200701692287, -7.353686938578365);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd2End3Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg1().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, 0.0, 0.0, -164.099476216398, -81.835885442800, 165.291731946141, 89.787051339157);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 412.29478568401856, -7.700275244269859);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd1End3Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg2().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, -18.723067158829, -59.239225729782, 0.0, 0.0, 18.851212571411, 59.694062940578);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 415.4806896701992, -6.690799426080698);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd1End2Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg3().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, 161.351352526949, 51.327798049323, -161.019856627996, -45.536840365345, 0.0, 0.0);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 410.53566804098494, -7.703116461849692);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd1Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg2().getTerminal().disconnect();
    twt.getLeg3().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, 0.0, -0.415739792683, 0.0, 0.0, 0.0, 0.0);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 412.9890009999999, -6.78071000000000);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd2Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg1().getTerminal().disconnect();
    twt.getLeg3().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, 0.0, 0.0, 0.000001946510, -0.405486077928, 0.0, 0.0);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 407.8654944214268, -8.77026956158324);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd3Connected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg1().getTerminal().disconnect();
    twt.getLeg2().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, 0.0, 0.0, 0.0, 0.0, 0.000005977974, -0.427562118410);
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, 418.82221596280823, -6.65147559975559);
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(testEnd1End2End3Disconnected) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");
    twt.getLeg1().getTerminal().disconnect();
    twt.getLeg2().getTerminal().disconnect();
    twt.getLeg3().getTerminal().disconnect();

    TwtData twtData(twt, 0, false);
    bool ok = t3xCompareFlow(twtData, stdcxx::nan(), stdcxx::nan(), stdcxx::nan(), stdcxx::nan(), stdcxx::nan(), stdcxx::nan());
    BOOST_CHECK(ok);

    ok = t3xCompareStarBusVoltage(twtData, stdcxx::nan(), stdcxx::nan());
    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_CASE(checkInputs) {
    Network network = createTwtDataTestNetwork();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT_VL1_VL2_VL3");

    BOOST_CHECK_CLOSE(2.4375e-6, TwtData(twt, 0, false).getB1(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getB1(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getB1(ThreeWindingsTransformer::Side::THREE));

    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getB2(ThreeWindingsTransformer::Side::ONE));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getB2(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getB2(ThreeWindingsTransformer::Side::THREE));

    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG1(ThreeWindingsTransformer::Side::ONE));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG1(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG1(ThreeWindingsTransformer::Side::THREE));

    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG2(ThreeWindingsTransformer::Side::ONE));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG2(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK_EQUAL(0.0, TwtData(twt, 0, false).getG2(ThreeWindingsTransformer::Side::THREE));

    BOOST_CHECK_EQUAL("3WT_VL1_VL2_VL3", TwtData(twt, 0, false).getId());

    BOOST_CHECK_EQUAL(0, TwtData(twt, 0, false).getPhaseAngleClock2());
    BOOST_CHECK_EQUAL(0, TwtData(twt, 0, false).getPhaseAngleClock3());

    BOOST_CHECK_CLOSE(2.7304328, TwtData(twt, 0, false).getQ(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-85.36818, TwtData(twt, 0, false).getQ(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(92.612077, TwtData(twt, 0, false).getQ(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(0.898462, TwtData(twt, 0, false).getR(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.070770247933884, TwtData(twt, 0, false).getR(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.837006802721089, TwtData(twt, 0, false).getR(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(400.0, TwtData(twt, 0, false).getRatedU(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(220.0, TwtData(twt, 0, false).getRatedU(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(21.0, TwtData(twt, 0, false).getRatedU(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(400, TwtData(twt, 0, false).getRatedU0(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(-0.1183457151229047, TwtData(twt, 0, false).getTheta(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.1530674697950051, TwtData(twt, 0, false).getTheta(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.11607835789163888, TwtData(twt, 0, false).getTheta(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(412.989001, TwtData(twt, 0, false).getU(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(224.315268, TwtData(twt, 0, false).getU(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(21.987, TwtData(twt, 0, false).getU(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(17.204128, TwtData(twt, 0, false).getX(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(19.6664, TwtData(twt, 0, false).getX(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(21.76072562358277, TwtData(twt, 0, false).getX(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(99.218431, TwtData(twt, 0, false).getP(ThreeWindingsTransformer::Side::ONE), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-216.19819, TwtData(twt, 0, false).getP(ThreeWindingsTransformer::Side::TWO), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(118.0, TwtData(twt, 0, false).getP(ThreeWindingsTransformer::Side::THREE), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::ONE));
    BOOST_CHECK(TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::ONE));
    twt.getLeg1().getTerminal().disconnect();
    BOOST_CHECK(!TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::ONE));
    BOOST_CHECK(!TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::ONE));

    BOOST_CHECK(TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK(TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::TWO));
    twt.getLeg2().getTerminal().disconnect();
    BOOST_CHECK(!TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::TWO));
    BOOST_CHECK(!TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::TWO));

    BOOST_CHECK(TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::THREE));
    BOOST_CHECK(TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::THREE));
    twt.getLeg3().getTerminal().disconnect();
    BOOST_CHECK(!TwtData(twt, 0, false).isConnected(ThreeWindingsTransformer::Side::THREE));
    BOOST_CHECK(!TwtData(twt, 0, false).isMainComponent(ThreeWindingsTransformer::Side::THREE));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
