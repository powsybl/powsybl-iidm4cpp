/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/iidm/util/TieLineUtil.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TieLineTestSuite)

    struct NodeSv {
        double v;
        double a;
    };
    struct LineSv {
        double p1;
        double q1;
        double p2;
        double q2;
        double v1;
        double v2;

        double getI1() const {
            return std::hypot(p1, q1) / (std::sqrt(3.0) * v1 / 1000.0);
        }
        double getI2() const {
            return std::hypot(p2, q2) / (std::sqrt(3.0) * v2 / 1000.0);
        }
    };
    struct CaseSv {
        NodeSv node1;
        NodeSv node2;
        NodeSv nodeBoundary;
        LineSv line1;
        LineSv line2;
    };
    
    // Line1 from node1 to nodeBoundary, Line2 from nodeBoundary to node2
    static CaseSv createCase0() {
        NodeSv node1 = NodeSv{1.06000000, 0.0 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{1.05913402, -0.01700730 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{1.04546576, -0.04168907 * stdcxx::toDegrees};

        LineSv line1 = LineSv{0.32101578, -0.16210107, -0.26328124, 0.00991455, node1.v, nodeBoundary.v};
        LineSv line2 = LineSv{0.26328124, -0.00991455, -0.21700000, -0.12700000, nodeBoundary.v, node2.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    // Line1 from node1 to nodeBoundary, Line2 from node2 to nodeBoundary
    static CaseSv createCase1() {
        NodeSv node1 = NodeSv{1.06000000, 0.0 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{1.05916756, -0.01702560 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{1.04216358, -0.03946400 * stdcxx::toDegrees};

        LineSv line1 = LineSv{0.32116645, -0.16274609, -0.26342655, 0.01056498, node1.v, nodeBoundary.v};
        LineSv line2 = LineSv{-0.21700000, -0.12700000, 0.26342655, -0.01056498, node2.v, nodeBoundary.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    // Line1 from nodeBoundary to node1, Line2 from nodeBoundary to node2
    static CaseSv createCase2() {
        NodeSv node1 = NodeSv{1.06000000, 0.0 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{1.05998661, -0.01660626 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{1.04634503, -0.04125738 * stdcxx::toDegrees};

        LineSv line1 = LineSv{-0.26335112, 0.01016197, 0.32106283, -0.16270573, nodeBoundary.v, node1.v};
        LineSv line2 = LineSv{0.26335112, -0.01016197, -0.21700000, -0.12700000, nodeBoundary.v, node2.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    // Line1 from nodeBoundary to node1, Line2 from node2 to nodeBoundary
    static CaseSv createCase3() {
        NodeSv node1 = NodeSv{1.06000000, 0.0 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{1.06002014, -0.01662448 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{1.04304009, -0.03903205 * stdcxx::toDegrees};

        LineSv line1 = LineSv{-0.26349561, 0.01081185, 0.32121215, -0.16335034, nodeBoundary.v, node1.v};
        LineSv line2 = LineSv{-0.21700000, -0.12700000, 0.26349561, -0.01081185, node2.v, nodeBoundary.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    // Line1 from node1 to nodeBoundary, Line2 from nodeBoundary to node2
    // Different nominal voltage at node1 and node2
    static CaseSv createCaseDifferentNominalVoltageAtEnds() {
        NodeSv node1 = NodeSv{145.2861673277147, -0.01745197 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{145.42378472578227, -0.02324020 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{231.30269602522478, -0.02818192 * stdcxx::toDegrees};

        LineSv line1 = LineSv{11.729938, -8.196614, -11.713527, 1.301712, node1.v, nodeBoundary.v};
        LineSv line2 = LineSv{11.713527, -1.301712, -11.700000, -6.700000, nodeBoundary.v, node2.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    // Line1 from node1 to nodeBoundary, Line2 from nodeBoundary to node2
    // Different nominal voltage at node1 and node2
    // NaN values for voltages - simulates DC LF
    static CaseSv createCaseNaNVoltages() {
        NodeSv node1 = NodeSv{stdcxx::nan(), -0.01745197 * stdcxx::toDegrees};
        NodeSv nodeBoundary = NodeSv{stdcxx::nan(), -0.02324020 * stdcxx::toDegrees};
        NodeSv node2 = NodeSv{stdcxx::nan(), -0.02818192 * stdcxx::toDegrees};

        LineSv line1 = LineSv{11.729938, -8.196614, -11.713527, 1.301712, node1.v, nodeBoundary.v};
        LineSv line2 = LineSv{11.713527, -1.301712, -11.700000, -6.700000, nodeBoundary.v, node2.v};
        return CaseSv{node1, node2, nodeBoundary, line1, line2};
    }

    struct LineInitialModel {
        double r;
        double x;
        double g1;
        double b1;
        double g2;
        double b2;
    };

    class TieLineInitialModel {
    public:
        LineInitialModel half1;
        LineInitialModel half2;
        LineInitialModel tieLine;

        TieLineInitialModel(const LineInitialModel& halfModel1, const TwoSides& half1Boundary, const LineInitialModel& halfModel2, const TwoSides& half2Boundary) :
            half1(halfModel1),
            half2(halfModel2) {
        
            LinkData::BranchAdmittanceMatrix adm1 = LinkData::calculateBranchAdmittance(half1.r, half1.x, 1.0, 0.0, 1.0, 0.0,
                std::complex<double>(half1.g1, half1.b1), std::complex<double>(half1.g2, half1.b2));
            LinkData::BranchAdmittanceMatrix adm2 = LinkData::calculateBranchAdmittance(half2.r, half2.x, 1.0, 0.0, 1.0, 0.0,
                std::complex<double>(half2.g1, half2.b1), std::complex<double>(half2.g2, half2.b2));

            LinkData::BranchAdmittanceMatrix adm = LinkData::kronChain(adm1, half1Boundary, adm2, half2Boundary);
            tieLine = { (std::complex<double>(-1.0) / adm.y12).real(),
                        (std::complex<double>(-1.0) / adm.y12).imag(),
                        (adm.y11 + adm.y12).real(),
                        (adm.y11 + adm.y12).imag(),
                        (adm.y22 + adm.y21).real(),
                        (adm.y22 + adm.y21).imag()};
        }
    };

    static double getOtherSideP(const LineSv& line, const TwoSides& boundarySide) {
        if (boundarySide == TwoSides::ONE) {
            return line.p2;
        } else {
            return line.p1;
        }
    }

    static double getOtherSideQ(const LineSv& line, const TwoSides& boundarySide) {
        if (boundarySide == TwoSides::ONE) {
            return line.q2;
        } else {
            return line.q1;
        }
    }

    static double getP(const LineSv& line, const TwoSides& boundarySide) {
        if (boundarySide == TwoSides::ONE) {
            return line.p1;
        } else {
            return line.p2;
        }
    }

    static double getQ(const LineSv& line, const TwoSides& boundarySide) {
        if (boundarySide == TwoSides::ONE) {
            return line.q1;
        } else {
            return line.q2;
        }
    }

    static double getI(const LineSv& line, const TwoSides& boundarySide) {
        if (boundarySide == TwoSides::ONE) {
            return line.getI1();
        } else {
            return line.getI2();
        }
    }

    static SV initialSv1(const CaseSv& initialCase, const TieLineInitialModel& tlim, const TwoSides& /*half1Boundary*/, const TwoSides& half2Boundary) {
        return SV(getOtherSideP(initialCase.line2, half2Boundary),
            getOtherSideQ(initialCase.line2, half2Boundary),
            initialCase.node2.v, initialCase.node2.a, TwoSides::TWO).otherSide(tlim.tieLine.r, tlim.tieLine.x,
                tlim.tieLine.g1, tlim.tieLine.b1, tlim.tieLine.g2, tlim.tieLine.b2, 1.0, 0.0);
    }

    static SV initialSv2(const CaseSv& initialCase, const TieLineInitialModel& tlim, const TwoSides& half1Boundary, const TwoSides& /*half2Boundary*/) {
        return SV(getOtherSideP(initialCase.line1, half1Boundary),
            getOtherSideQ(initialCase.line1, half1Boundary),
            initialCase.node1.v, initialCase.node1.a, TwoSides::ONE).otherSide(tlim.tieLine.r, tlim.tieLine.x,
                tlim.tieLine.g1, tlim.tieLine.b1, tlim.tieLine.g2, tlim.tieLine.b2, 1.0, 0.0);
    }

    static SV initialHalf1SvBoundary(const CaseSv& initialCase, const TieLineInitialModel& tlim, const TwoSides& half1Boundary) {
        return SV(getOtherSideP(initialCase.line1, half1Boundary),
            getOtherSideQ(initialCase.line1, half1Boundary),
            initialCase.node1.v, initialCase.node1.a,
            (half1Boundary==TwoSides::ONE) ? TwoSides::TWO : TwoSides::ONE).otherSide(tlim.half1.r,
                tlim.half1.x, tlim.half1.g1, tlim.half1.b1, tlim.half1.g2, tlim.half1.b2, 1.0, 0.0);
    }

    static SV initialHalf2SvBoundary(const CaseSv& initialCase, const TieLineInitialModel& tlim, const TwoSides& half2Boundary) {
        return SV(getOtherSideP(initialCase.line2, half2Boundary),
            getOtherSideQ(initialCase.line2, half2Boundary),
            initialCase.node2.v, initialCase.node2.a,
            (half2Boundary==TwoSides::ONE) ? TwoSides::TWO : TwoSides::ONE).otherSide(tlim.half2.r,
                tlim.half2.x, tlim.half2.g1, tlim.half2.b1, tlim.half2.g2, tlim.half2.b2, 1.0, 0.0);
    }

    static TieLineInitialModel initialModelCase(const TwoSides& half1Boundary, const TwoSides& half2Boundary) {
        return TieLineInitialModel {
            LineInitialModel {0.019, 0.059, 0.02, 0.075, 0.03, 0.065}, half1Boundary,
            LineInitialModel {0.038, 0.118, 0.015, 0.050, 0.025, 0.080}, half2Boundary };
    }

    static TieLineInitialModel initialModelDifferentVlCase(const TwoSides& half1Boundary, const TwoSides& half2Boundary) {
        return TieLineInitialModel {
            LineInitialModel{2.1672071999999996, 9.5543748, 0.0, 1.648813274522159E-4, 0.0, 1.648813274522159E-4}, half1Boundary,
            LineInitialModel{3.1513680000000006, 14.928011999999999, 0.008044414674299755, -0.03791520949675112,
                -0.005046041932060755, 0.023978278075869598}, half2Boundary };
    }



    // We define an error by value to adjust the case. The error is calculated by difference between
    // the calculated value with both models, the initial model of the case and the current model of the boundaryLine
    // Errors are due to the boundaryLine model (it does not allow shunt admittance at both ends)
    static bool compare(const SV& sv, const NodeSv& nodeSv, const LineSv& lineSv, const TwoSides& boundarySide, const SV& initialModelSv) {
        double tol = 0.00001;
        double errorP = initialModelSv.getP() - sv.getP();
        double errorQ = initialModelSv.getQ() - sv.getQ();
        double errorU = initialModelSv.getU() - sv.getU();
        double errorA = initialModelSv.getA() - sv.getA();
        if (std::abs(sv.getP() - getOtherSideP(lineSv, boundarySide)) > tol + std::abs(errorP)) {
            return false;
        }
        if (std::abs(sv.getQ() - getOtherSideQ(lineSv, boundarySide)) > tol + std::abs(errorQ)) {
            return false;
        }
        if (std::abs(sv.getU() - nodeSv.v) > tol + std::abs(errorU)) {
            return false;
        }
        if (std::abs(sv.getA() - nodeSv.a) > tol + std::abs(errorA)) {
            return false;
        }
        return true;
    }

    // We define an error to adjust the case. The error is calculated by difference between
    // the calculated value with both models, the initial model of the case and the current model of the boundaryLine
    // Errors are due to the boundaryLine model (it does not allow shunt admittance at both ends)
    static bool compare(double expected, double actual, double initialActual) {
        double tol = 0.00001;
        double error = initialActual - actual;
        if (std::abs(actual - expected) > tol + std::abs(error)) {
            return false;
        }
        return true;
    }

    static void createBus(VoltageLevel& voltageLevel, const std::string& id) {
        voltageLevel.getBusBreakerView()
            .newBus()
            .setName(id)
            .setId(id)
            .add();
    }

    static Network createNetworkWithTieLine(const TwoSides& boundarySide1, const TwoSides& boundarySide2, const CaseSv& caseSv) {
        Network network("TieLine-BusBreaker", "test");

        Substation& s1 = network.newSubstation()
                .setId("S1")
                .add();
        VoltageLevel& s1vl1 = s1.newVoltageLevel()
                .setId("S1VL1")
                .setNominalV(1.0)
                .setLowVoltageLimit(0.95)
                .setHighVoltageLimit(1.05)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();

        createBus(s1vl1, "S1VL1-BUS");

        Substation& s2 = network.newSubstation()
            .setId("S2")
            .add();
        VoltageLevel& s2vl1 = s2.newVoltageLevel()
            .setId("S2VL1")
            .setNominalV(1.0)
            .setLowVoltageLimit(0.95)
            .setHighVoltageLimit(1.05)
            .setTopologyKind(TopologyKind::BUS_BREAKER)
            .add();

        createBus(s2vl1, "S2VL1-BUS");

        // The initial parameters for AcLineSegment 1 are R = 0.019, X = 0.059, G1 = 0.02, B1 = 0.075, G2 = 0.03, B2 = 0.065
        // The initial parameters for AcLineSegment 2 are R = 0.038, X = 0.118, G1 = 0.015,B1 = 0.050, G2 = 0.025,B2 = 0.080
        // AcLinesegment 1 must be reoriented if boundary side is at end 1
        // AcLinesegment 2 must be reoriented if boundary side is at end 2
        // Current model does not allow shunt admittances at both ends, so it does not make sense to reorient the AcLineSegments
        BoundaryLine& bl1 = s1vl1.newBoundaryLine()
                .setBus("S1VL1-BUS")
                .setId(Enum::toString(boundarySide1))
                .setEnsureIdUnicity(true)
                .setName(Enum::toString(boundarySide1))
                .setP0(0.0)
                .setQ0(0.0)
                .setR(0.019)
                .setX(0.059)
                .setG(0.05)
                .setB(0.14)
                .add();
        BoundaryLine& bl2 = s2vl1.newBoundaryLine()
                .setBus("S2VL1-BUS")
                .setId(Enum::toString(boundarySide2))
                .setEnsureIdUnicity(true)
                .setName(Enum::toString(boundarySide2))
                .setP0(0.0)
                .setQ0(0.0)
                .setR(0.038)
                .setX(0.118)
                .setG(0.04)
                .setB(0.13)
                .setPairingKey("key")
                .add();

        TieLine& tieLine = network.newTieLine()
            .setId(Enum::toString(boundarySide1) + " + " + Enum::toString(boundarySide2))
            .setName(Enum::toString(boundarySide1) + " + " + Enum::toString(boundarySide2))
            .setBoundaryLine1(bl1.getId())
            .setBoundaryLine2(bl2.getId())
            .add();
        tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().setV(caseSv.node1.v);
        tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().setAngle(caseSv.node1.a);
        tieLine.getBoundaryLine1().getTerminal().setP(getOtherSideP(caseSv.line1, boundarySide1));
        tieLine.getBoundaryLine1().getTerminal().setQ(getOtherSideQ(caseSv.line1, boundarySide1));

        tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().setV(caseSv.node2.v);
        tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().setAngle(caseSv.node2.a);
        tieLine.getBoundaryLine2().getTerminal().setP(getOtherSideP(caseSv.line2, boundarySide2));
        tieLine.getBoundaryLine2().getTerminal().setQ(getOtherSideQ(caseSv.line2, boundarySide2));

        return network;
    }
    static Network createNetworkWithTieLineWithDifferentNominalVoltageAtEnds(const TwoSides& boundarySide1, const TwoSides& boundarySide2, const CaseSv& caseSv) {
        Network network ("TieLine-BusBreaker", "test");

        Substation& s1 = network.newSubstation()
                .setId("S1")
                .add();
        VoltageLevel& s1vl1 = s1.newVoltageLevel()
                .setId("S1VL1")
                .setNominalV(138.0)
                .setLowVoltageLimit(110.0)
                .setHighVoltageLimit(150.0)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();

        createBus(s1vl1, "S1VL1-BUS");

        Substation& s2 = network.newSubstation()
            .setId("S2")
            .add();
        VoltageLevel& s2vl1 = s2.newVoltageLevel()
            .setId("S2VL1")
            .setNominalV(220.0)
            .setLowVoltageLimit(195.0)
            .setHighVoltageLimit(240.0)
            .setTopologyKind(TopologyKind::BUS_BREAKER)
            .add();

        createBus(s2vl1, "S2VL1-BUS");

        // The initial parameters for AcLineSegment 1 are R = 2.1672071999999996, X = 9.5543748, G1 = 0.0, B1 = 1.648813274522159E-4, G2 = 0.0, B2 = 1.648813274522159E-4
        // AcLinesegment 1 must be reoriented if boundary side is at end 1
        // Current model does not allow shunt admittances at both ends, so it does not make sense to reorient it

        // The initial parameters for AcLineSegment 2 are R = 3.1513680000000006, X = 14.928011999999999, G1 = 0.008044414674299755, B1 = -0.03791520949675112, G2 = -0.005046041932060755, B2 = 0.023978278075869598
        // AcLinesegment 2 must be reoriented if boundary side is at end 2
        // Current model does not allow shunt admittances at both ends, so it does not make sense to reorient it
        BoundaryLine& bl1 = s1vl1.newBoundaryLine()
                .setBus("S1VL1-BUS")
                .setId(Enum::toString(boundarySide1))
                .setName(Enum::toString(boundarySide1))
                .setP0(0.0)
                .setQ0(0.0)
                .setR(2.1672071999999996)
                .setX(9.5543748)
                .setG(0.0)
                .setB(0.00032976265)
                .setPairingKey("key")
                .add();
        BoundaryLine& bl2 = s2vl1.newBoundaryLine()
                .setBus("S2VL1-BUS")
                .setId(Enum::toString(boundarySide2))
                .setName(Enum::toString(boundarySide2))
                .setP0(0.0)
                .setQ0(0.0)
                .setR(3.1513680000000006)
                .setX(14.928011999999999)
                .setG(0.00299837274)
                .setB(-0.01393693142)
                .add();

        TieLine& tieLine = network.newTieLine()
                .setId(Enum::toString(boundarySide1) + " + " + Enum::toString(boundarySide2))
                .setName(Enum::toString(boundarySide1) + " + " + Enum::toString(boundarySide2))
                .setBoundaryLine1(bl1.getId())
                .setBoundaryLine2(bl2.getId())
                .add();

        tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().setV(caseSv.node1.v);
        tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().setAngle(caseSv.node1.a);
        tieLine.getBoundaryLine1().getTerminal().setP(getOtherSideP(caseSv.line1, boundarySide1));
        tieLine.getBoundaryLine1().getTerminal().setQ(getOtherSideQ(caseSv.line1, boundarySide1));

        tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().setV(caseSv.node2.v);
        tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().setAngle(caseSv.node2.a);
        tieLine.getBoundaryLine2().getTerminal().setP(getOtherSideP(caseSv.line2, boundarySide2));
        tieLine.getBoundaryLine2().getTerminal().setQ(getOtherSideQ(caseSv.line2, boundarySide2));

        return network;
    }


BOOST_AUTO_TEST_CASE(tieLineTest0) {
    // Line1 from node1 to boundaryNode, Line2 from boundaryNode to node2
    CaseSv caseSv0 = createCase0();
    Network n = createNetworkWithTieLine(TwoSides::TWO, TwoSides::ONE, caseSv0);

    TieLine& tieLine = n.getTieLine("TWO + ONE");
    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv0, initialModelCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv2, caseSv0.node2, caseSv0.line2, TwoSides::ONE, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv0, initialModelCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv1, caseSv0.node1, caseSv0.line1, TwoSides::TWO, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv0, initialModelCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv0.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv0.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv0.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv0.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv0.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv0, initialModelCase(TwoSides::TWO, TwoSides::ONE), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv0.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv0.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv0.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv0.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv0.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));
    
}
BOOST_AUTO_TEST_CASE(tieLineTest1) {
    // Line1 from node1 to boundaryNode, Line2 from node2 to boundaryNode
    CaseSv caseSv1 = createCase1();
    Network n = createNetworkWithTieLine(TwoSides::TWO, TwoSides::TWO, caseSv1);
    TieLine& tieLine = n.getTieLine("TWO + TWO");

    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv1, initialModelCase(TwoSides::TWO, TwoSides::TWO), TwoSides::TWO, TwoSides::TWO);
    BOOST_CHECK(compare(sv2, caseSv1.node2, caseSv1.line2, TwoSides::TWO, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv1, initialModelCase(TwoSides::TWO, TwoSides::TWO), TwoSides::TWO, TwoSides::TWO);
    BOOST_CHECK(compare(sv1, caseSv1.node1, caseSv1.line1, TwoSides::TWO, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv1, initialModelCase(TwoSides::TWO, TwoSides::TWO), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv1.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv1.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv1.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv1.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv1.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv1, initialModelCase(TwoSides::TWO, TwoSides::TWO), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv1.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv1.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv1.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv1.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv1.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));
    
}
BOOST_AUTO_TEST_CASE(tieLineTest2) {
    // Line1 from boundaryNode to node1, Line2 from boundaryNode to node2
    CaseSv caseSv2 = createCase2();
    Network n = createNetworkWithTieLine(TwoSides::ONE, TwoSides::ONE, caseSv2);
    TieLine& tieLine = n.getTieLine("ONE + ONE");

    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv2, initialModelCase(TwoSides::ONE, TwoSides::ONE), TwoSides::ONE, TwoSides::ONE);
    BOOST_CHECK(compare(sv2, caseSv2.node2, caseSv2.line2, TwoSides::ONE, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv2, initialModelCase(TwoSides::ONE, TwoSides::ONE), TwoSides::ONE, TwoSides::ONE);
    BOOST_CHECK(compare(sv1, caseSv2.node1, caseSv2.line1, TwoSides::ONE, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv2, initialModelCase(TwoSides::ONE, TwoSides::ONE), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv2.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv2.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv2.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv2.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv2.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv2, initialModelCase(TwoSides::ONE, TwoSides::ONE), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv2.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv2.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv2.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv2.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv2.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));
    
}
BOOST_AUTO_TEST_CASE(tieLineTest3) {
    // Line1 from boundaryNode to node1, Line2 from node2 to boundaryNode
    CaseSv caseSv3 = createCase3();
    Network n = createNetworkWithTieLine(TwoSides::ONE, TwoSides::TWO, caseSv3);
    TieLine& tieLine = n.getTieLine("ONE + TWO");

    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv3, initialModelCase(TwoSides::ONE, TwoSides::TWO), TwoSides::ONE, TwoSides::TWO);
    BOOST_CHECK(compare(sv2, caseSv3.node2, caseSv3.line2, TwoSides::TWO, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv3, initialModelCase(TwoSides::ONE, TwoSides::TWO), TwoSides::ONE, TwoSides::TWO);
    BOOST_CHECK(compare(sv1, caseSv3.node1, caseSv3.line1, TwoSides::ONE, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv3, initialModelCase(TwoSides::ONE, TwoSides::TWO), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv3.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv3.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv3.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv3.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv3.line1, TwoSides::ONE), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv3, initialModelCase(TwoSides::ONE, TwoSides::TWO), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv3.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv3.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv3.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv3.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv3.line2, TwoSides::TWO), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));
    
}
BOOST_AUTO_TEST_CASE(tieLineWithDifferentNominalVoltageAtEndsTest) {
    // Line1 from node1 to boundaryNode, Line2 from boundaryNode to node2
    CaseSv caseSv = createCaseDifferentNominalVoltageAtEnds();
    Network n = createNetworkWithTieLineWithDifferentNominalVoltageAtEnds(TwoSides::TWO, TwoSides::ONE, caseSv);
    TieLine& tieLine = n.getTieLine("TWO + ONE");

    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv2, caseSv.node2, caseSv.line2, TwoSides::ONE, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
            tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
            TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv1, caseSv.node1, caseSv.line1, TwoSides::TWO, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));

}

BOOST_AUTO_TEST_CASE(tieLineWithNaNVoltagesTest) {

    // Line1 from node1 to boundaryNode, Line2 from boundaryNode to node2
    CaseSv caseSv = createCaseNaNVoltages();
    Network n = createNetworkWithTieLineWithDifferentNominalVoltageAtEnds(TwoSides::TWO, TwoSides::ONE, caseSv);
    TieLine& tieLine = n.getTieLine("TWO + ONE");

    SV sv2 = SV(tieLine.getBoundaryLine1().getTerminal().getP(), tieLine.getBoundaryLine1().getTerminal().getQ(),
                tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(),
                tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(),
                TwoSides::ONE).otherSide(tieLine);
    SV isv2 = initialSv2(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv2, caseSv.node2, caseSv.line2, TwoSides::ONE, isv2));

    SV sv1 = SV(tieLine.getBoundaryLine2().getTerminal().getP(), tieLine.getBoundaryLine2().getTerminal().getQ(),
                tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(),
                tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(),
                TwoSides::TWO).otherSide(tieLine);
    SV isv1 = initialSv1(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO, TwoSides::ONE);
    BOOST_CHECK(compare(sv1, caseSv.node1, caseSv.line1, TwoSides::TWO, isv1));

    SV isvHalf1 = initialHalf1SvBoundary(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::TWO);
    BOOST_CHECK(compare(caseSv.nodeBoundary.v, tieLine.getBoundaryLine1().getBoundary().getV(), isvHalf1.getU()));
    BOOST_CHECK(compare(caseSv.nodeBoundary.a, tieLine.getBoundaryLine1().getBoundary().getAngle(), isvHalf1.getA()));
    BOOST_CHECK(compare(getP(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getP(), isvHalf1.getP()));
    BOOST_CHECK(compare(getQ(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getQ(), isvHalf1.getQ()));
    BOOST_CHECK(compare(getI(caseSv.line1, TwoSides::TWO), tieLine.getBoundaryLine1().getBoundary().getI(), isvHalf1.getI()));

    SV isvHalf2 = initialHalf2SvBoundary(caseSv, initialModelDifferentVlCase(TwoSides::TWO, TwoSides::ONE), TwoSides::ONE);
    BOOST_CHECK(compare(caseSv.nodeBoundary.v, tieLine.getBoundaryLine2().getBoundary().getV(), isvHalf2.getU()));
    BOOST_CHECK(compare(caseSv.nodeBoundary.a, tieLine.getBoundaryLine2().getBoundary().getAngle(), isvHalf2.getA()));
    BOOST_CHECK(compare(getP(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getP(), isvHalf2.getP()));
    BOOST_CHECK(compare(getQ(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getQ(), isvHalf2.getQ()));
    BOOST_CHECK(compare(getI(caseSv.line2, TwoSides::ONE), tieLine.getBoundaryLine2().getBoundary().getI(), isvHalf2.getI()));
}

BOOST_AUTO_TEST_CASE(tieLineTestZeroImpedanceBl1) {
    // Line1 from node1 to boundaryNode, Line2 from node2 to boundaryNode
    CaseSv caseSv0 = createCase0();
    Network n = createNetworkWithTieLine(TwoSides::TWO, TwoSides::TWO, caseSv0);
    TieLine& tieLine = n.getTieLine("TWO + TWO");
    //set boundaryLine1 with zeroImpedance 
    tieLine.getBoundaryLine1().setR(0.0).setX(0.0).setG(0.0).setB(0.0);

    double tol = 1.0e-6;
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getR(), tieLine.getR(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getX(), tieLine.getX(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getG(), tieLine.getG2(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getB(), tieLine.getB2(), tol);
    BOOST_CHECK_CLOSE(0.0, tieLine.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getB1(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(), TieLineUtil::getBoundaryV(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(), TieLineUtil::getBoundaryAngle(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);

}
BOOST_AUTO_TEST_CASE(tieLineTestZeroImpedanceBl2) {
    // Line1 from node1 to boundaryNode, Line2 from node2 to boundaryNode
    CaseSv caseSv0 = createCase0();
    Network n = createNetworkWithTieLine(TwoSides::TWO, TwoSides::TWO, caseSv0);
    TieLine& tieLine = n.getTieLine("TWO + TWO");
    //set boundaryLine2 with zeroImpedance 
    tieLine.getBoundaryLine2().setR(0.0).setX(0.0).setG(0.0).setB(0.0);

    double tol = 1.0e-6;
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getR(), tieLine.getR(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getX(), tieLine.getX(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getG(), tieLine.getG1(), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getB(), tieLine.getB1(), tol);
    BOOST_CHECK_CLOSE(0.0, tieLine.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getB2(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getV(), TieLineUtil::getBoundaryV(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine2().getTerminal().getBusView().getBus().get().getAngle(), TieLineUtil::getBoundaryAngle(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);
    
}
BOOST_AUTO_TEST_CASE(tieLineTestZeroImpedanceBl1AndBl2) {
    // Line1 from node1 to boundaryNode, Line2 from node2 to boundaryNode
    CaseSv caseSv0 = createCase0();
    Network n = createNetworkWithTieLine(TwoSides::TWO, TwoSides::TWO, caseSv0);
    TieLine& tieLine = n.getTieLine("TWO + TWO");
    tieLine.getBoundaryLine1().setR(0.0).setX(0.0).setG(0.0).setB(0.0);
    tieLine.getBoundaryLine2().setR(0.0).setX(0.0).setG(0.0).setB(0.0);

    double tol = 1.0e-6;
    BOOST_CHECK_CLOSE(0.0, tieLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getB2(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getV(), TieLineUtil::getBoundaryV(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);
    BOOST_CHECK_CLOSE(tieLine.getBoundaryLine1().getTerminal().getBusView().getBus().get().getAngle(), TieLineUtil::getBoundaryAngle(tieLine.getBoundaryLine1(), tieLine.getBoundaryLine2()), tol);
    
}

BOOST_AUTO_TEST_SUITE_END()



}  // namespace iidm

}  // namespace powsybl
