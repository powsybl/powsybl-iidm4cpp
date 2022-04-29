/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/TwtData.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>
#include <powsybl/math/ComplexUtils.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

const std::string UNEXPECTED_SIDE = "Unexpected side";

TwtData::TwtData(const ThreeWindingsTransformer& twt, double epsilonX, bool applyReactanceCorrection) :
    TwtData(twt, 0, 0, epsilonX, applyReactanceCorrection, false) {
}

TwtData::TwtData(const ThreeWindingsTransformer& twt, double epsilonX, bool applyReactanceCorrection, bool twtSplitShuntAdmittance) :
    TwtData(twt, 0, 0, epsilonX, applyReactanceCorrection, twtSplitShuntAdmittance) {
}

TwtData::TwtData(const ThreeWindingsTransformer& twt, int phaseAngleClock2, int phaseAngleClock3, double epsilonX, bool applyReactanceCorrection, 
                 bool twtSplitShuntAdmittance) :
    m_id(twt.getId()),
    m_p1(twt.getLeg1().getTerminal().getP()),
    m_q1(twt.getLeg1().getTerminal().getQ()),
    m_p2(twt.getLeg2().getTerminal().getP()),
    m_q2(twt.getLeg2().getTerminal().getQ()),
    m_p3(twt.getLeg3().getTerminal().getP()),
    m_q3(twt.getLeg3().getTerminal().getQ()),
    m_u1(getV(twt.getLeg1())),
    m_theta1(getTheta(twt.getLeg1())),
    m_u2(getV(twt.getLeg2())),
    m_theta2(getTheta(twt.getLeg2())),
    m_u3(getV(twt.getLeg3())),
    m_theta3(getTheta(twt.getLeg3())),
    m_r1(getR(twt.getLeg1())),
    m_x1(LinkData::getFixedX(getX(twt.getLeg1()), epsilonX, applyReactanceCorrection)),
    m_r2(getR(twt.getLeg2())),
    m_x2(LinkData::getFixedX(getX(twt.getLeg2()), epsilonX, applyReactanceCorrection)),
    m_r3(getR(twt.getLeg3())),
    m_x3(LinkData::getFixedX(getX(twt.getLeg3()), epsilonX, applyReactanceCorrection)),
    m_g11(getG1(twt.getLeg1(), twtSplitShuntAdmittance)),
    m_b11(getB1(twt.getLeg1(), twtSplitShuntAdmittance)),
    m_g12(getG2(twt.getLeg1(), twtSplitShuntAdmittance)),
    m_b12(getB2(twt.getLeg1(), twtSplitShuntAdmittance)),
    m_g21(getG1(twt.getLeg2(), twtSplitShuntAdmittance)),
    m_b21(getB1(twt.getLeg2(), twtSplitShuntAdmittance)),
    m_g22(getG2(twt.getLeg2(), twtSplitShuntAdmittance)),
    m_b22(getB2(twt.getLeg2(), twtSplitShuntAdmittance)),
    m_g31(getG1(twt.getLeg3(), twtSplitShuntAdmittance)),
    m_b31(getB1(twt.getLeg3(), twtSplitShuntAdmittance)),
    m_g32(getG2(twt.getLeg3(), twtSplitShuntAdmittance)),
    m_b32(getB2(twt.getLeg3(), twtSplitShuntAdmittance)),
    m_alpha1(alpha(twt.getLeg1())),
    m_alpha2(alpha(twt.getLeg2())),
    m_alpha3(alpha(twt.getLeg3())),
    m_ratedU1(twt.getLeg1().getRatedU()),
    m_ratedU2(twt.getLeg2().getRatedU()),
    m_ratedU3(twt.getLeg3().getRatedU()),
    m_connected1(twt.getLeg1().getTerminal().isConnected()),
    m_connected2(twt.getLeg2().getTerminal().isConnected()),
    m_connected3(twt.getLeg3().getTerminal().isConnected()),
    m_mainComponent1(isMainComponent(twt.getLeg1())),
    m_mainComponent2(isMainComponent(twt.getLeg2())),
    m_mainComponent3(isMainComponent(twt.getLeg3())),
    m_phaseAngleClock2(phaseAngleClock2),
    m_phaseAngleClock3(phaseAngleClock3),
    m_ratedU0(twt.getRatedU0()) {
    m_rho1 = rho(twt.getLeg1(), m_ratedU0);
    m_rho2 = rho(twt.getLeg2(), m_ratedU0);
    m_rho3 = rho(twt.getLeg3(), m_ratedU0);

    double rhof = 1.0;
    double alphaf = 0.0;

    double angle1 = -m_alpha1;
    double angle2 = -m_alpha2 - stdcxx::toRadians * LinkData::getPhaseAngleClockDegrees(phaseAngleClock2);
    double angle3 = -m_alpha3 - stdcxx::toRadians * LinkData::getPhaseAngleClockDegrees(phaseAngleClock3);
    double anglef = -alphaf;

    LinkData::BranchAdmittanceMatrix branchAdmittanceLeg1 = LinkData::calculateBranchAdmittance(m_r1, m_x1, 1 / m_rho1, angle1, 1 / rhof, anglef, std::complex<double>(m_g11, m_b11), std::complex<double>(m_g12, m_b12));

    LinkData::BranchAdmittanceMatrix branchAdmittanceLeg2 = LinkData::calculateBranchAdmittance(m_r2, m_x2, 1 / m_rho2, angle2, 1 / rhof, anglef, std::complex<double>(m_g21, m_b21), std::complex<double>(m_g22, m_b22));

    LinkData::BranchAdmittanceMatrix branchAdmittanceLeg3 = LinkData::calculateBranchAdmittance(m_r3, m_x3, 1 / m_rho3, angle3, 1 / rhof, anglef, std::complex<double>(m_g31, m_b31), std::complex<double>(m_g32, m_b32));

    // Assume the ratedU at the star bus is equal to ratedU of Leg1

    if (m_connected1 && m_connected2 && m_connected3 && valid(m_u1, m_theta1) && valid(m_u2, m_theta2) && valid(m_u3, m_theta3)) {

        calculateThreeConnectedLegsFlowAndStarBusVoltage(m_u1, m_theta1, m_u2, m_theta2, m_u3, m_theta3, branchAdmittanceLeg1,
                                                         branchAdmittanceLeg2, branchAdmittanceLeg3);
    } else if (m_connected1 && m_connected2 && valid(m_u1, m_theta1) && valid(m_u2, m_theta2)) {

        LinkData::Flow flow = calculateTwoConnectedLegsFlow(m_u1, m_theta1, m_u2, m_theta2,
                                                            branchAdmittanceLeg1, branchAdmittanceLeg2, branchAdmittanceLeg3);
        m_computedP1 = std::real(flow.fromTo);
        m_computedQ1 = std::imag(flow.fromTo);
        m_computedP2 = std::real(flow.toFrom);
        m_computedQ2 = std::imag(flow.toFrom);
        m_computedP3 = 0.0;
        m_computedQ3 = 0.0;

        std::complex<double> v0 = calculateTwoConnectedLegsStarBusVoltage(m_u1, m_theta1, m_u2, m_theta2,
                                                                          branchAdmittanceLeg1, branchAdmittanceLeg2, branchAdmittanceLeg3);
        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else if (m_connected1 && m_connected3 && valid(m_u1, m_theta1) && valid(m_u3, m_theta3)) {

        LinkData::Flow flow = calculateTwoConnectedLegsFlow(m_u1, m_theta1, m_u3, m_theta3,
                                                            branchAdmittanceLeg1, branchAdmittanceLeg3, branchAdmittanceLeg2);
        m_computedP1 = std::real(flow.fromTo);
        m_computedQ1 = std::imag(flow.fromTo);
        m_computedP2 = 0.0;
        m_computedQ2 = 0.0;
        m_computedP3 = std::real(flow.toFrom);
        m_computedQ3 = std::imag(flow.toFrom);

        std::complex<double> v0 = calculateTwoConnectedLegsStarBusVoltage(m_u1, m_theta1, m_u3, m_theta3,
                                                                          branchAdmittanceLeg1, branchAdmittanceLeg3, branchAdmittanceLeg2);

        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else if (m_connected2 && m_connected3 && valid(m_u2, m_theta2) && valid(m_u3, m_theta3)) {

        LinkData::Flow flow = calculateTwoConnectedLegsFlow(m_u2, m_theta2, m_u3, m_theta3,
                                                            branchAdmittanceLeg2, branchAdmittanceLeg3, branchAdmittanceLeg1);
        m_computedP1 = 0.0;
        m_computedQ1 = 0.0;
        m_computedP2 = std::real(flow.fromTo);
        m_computedQ2 = std::imag(flow.fromTo);
        m_computedP3 = std::real(flow.toFrom);
        m_computedQ3 = std::imag(flow.toFrom);

        std::complex<double> v0 = calculateTwoConnectedLegsStarBusVoltage(m_u2, m_theta2, m_u3, m_theta3,
                                                                          branchAdmittanceLeg2, branchAdmittanceLeg3, branchAdmittanceLeg1);
        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else if (m_connected1 && valid(m_u1, m_theta1)) {

        std::complex<double> flow = calculateOneConnectedLegFlow(m_u1, m_theta1, branchAdmittanceLeg1,
                                                                 branchAdmittanceLeg2, branchAdmittanceLeg3);
        m_computedP1 = std::real(flow);
        m_computedQ1 = std::imag(flow);
        m_computedP2 = 0.0;
        m_computedQ2 = 0.0;
        m_computedP3 = 0.0;
        m_computedQ3 = 0.0;

        std::complex<double> v0 = calculateOneConnectedLegStarBusVoltage(m_u1, m_theta1, branchAdmittanceLeg1,
                                                                         branchAdmittanceLeg2, branchAdmittanceLeg3);

        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else if (m_connected2 && valid(m_u2, m_theta2)) {

        std::complex<double> flow = calculateOneConnectedLegFlow(m_u2, m_theta2, branchAdmittanceLeg2,
                                                                 branchAdmittanceLeg1, branchAdmittanceLeg3);

        m_computedP1 = 0.0;
        m_computedQ1 = 0.0;
        m_computedP2 = std::real(flow);
        m_computedQ2 = std::imag(flow);
        m_computedP3 = 0.0;
        m_computedQ3 = 0.0;

        std::complex<double> v0 = calculateOneConnectedLegStarBusVoltage(m_u2, m_theta2, branchAdmittanceLeg2,
                                                                         branchAdmittanceLeg1, branchAdmittanceLeg3);
        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else if (m_connected3 && valid(m_u3, m_theta3)) {

        std::complex<double> flow = calculateOneConnectedLegFlow(m_u3, m_theta3, branchAdmittanceLeg3,
                                                                 branchAdmittanceLeg1, branchAdmittanceLeg2);

        m_computedP1 = 0.0;
        m_computedQ1 = 0.0;
        m_computedP2 = 0.0;
        m_computedQ2 = 0.0;
        m_computedP3 = std::real(flow);
        m_computedQ3 = std::imag(flow);

        std::complex<double> v0 = calculateOneConnectedLegStarBusVoltage(m_u3, m_theta3, branchAdmittanceLeg3,
                                                                         branchAdmittanceLeg1, branchAdmittanceLeg2);
        m_starU = std::abs(v0);
        m_starTheta = std::arg(v0);
    } else {

        m_computedP1 = stdcxx::nan();
        m_computedQ1 = stdcxx::nan();
        m_computedP2 = stdcxx::nan();
        m_computedQ2 = stdcxx::nan();
        m_computedP3 = stdcxx::nan();
        m_computedQ3 = stdcxx::nan();

        m_starU = stdcxx::nan();
        m_starTheta = stdcxx::nan();
    }
}

double TwtData::alpha(const ThreeWindingsTransformer::Leg& leg) {
    return leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getAlpha() : 0.0;
}

std::complex<double> TwtData::calculateOneConnectedLegFlow(double u, double theta,
                                                           const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg,
                                                           const LinkData::BranchAdmittanceMatrix& admittanceMatrixFirstOpenLeg,
                                                           const LinkData::BranchAdmittanceMatrix& admittanceMatrixSecondOpenLeg) const {

    std::complex<double> ysh = calculateOneConnectedLegShunt(admittanceMatrixLeg,
                                                             admittanceMatrixFirstOpenLeg, admittanceMatrixSecondOpenLeg);

    return LinkData::flowYshunt(ysh, u, theta);
}

std::complex<double> TwtData::calculateOneConnectedLegShunt(const LinkData::BranchAdmittanceMatrix& closeLeg,
                                                            const LinkData::BranchAdmittanceMatrix& firstOpenLeg,
                                                            const LinkData::BranchAdmittanceMatrix& secondOpenLeg) const {
    std::complex<double> ysh1 = LinkData::kronAntenna(firstOpenLeg.y11, firstOpenLeg.y12, firstOpenLeg.y21, firstOpenLeg.y22, true);
    std::complex<double> ysh2 = LinkData::kronAntenna(secondOpenLeg.y11, secondOpenLeg.y12, secondOpenLeg.y21, secondOpenLeg.y22, true);
    std::complex<double> y22 = closeLeg.y22 + ysh1 + ysh2;

    return LinkData::kronAntenna(closeLeg.y11, closeLeg.y12, closeLeg.y21, y22, false);
}

std::complex<double> TwtData::calculateOneConnectedLegStarBusVoltage(double u, double theta,
                                                                     const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg,
                                                                     const LinkData::BranchAdmittanceMatrix& admittanceMatrixFirstOpenLeg,
                                                                     const LinkData::BranchAdmittanceMatrix& admittanceMatrixSecondOpenLeg) const {

    std::complex<double> v = math::ComplexUtils::polar2Complex(u, theta);

    std::complex<double> ysh1O = LinkData::kronAntenna(admittanceMatrixFirstOpenLeg.y11, admittanceMatrixFirstOpenLeg.y12,
                                                       admittanceMatrixFirstOpenLeg.y21, admittanceMatrixFirstOpenLeg.y22, true);
    std::complex<double> ysh2O = LinkData::kronAntenna(admittanceMatrixSecondOpenLeg.y11, admittanceMatrixSecondOpenLeg.y12,
                                                       admittanceMatrixSecondOpenLeg.y21, admittanceMatrixSecondOpenLeg.y22, true);

    return (-admittanceMatrixLeg.y21 * v) / (admittanceMatrixLeg.y22 + ysh1O + ysh2O);
}

void TwtData::calculateThreeConnectedLegsFlowAndStarBusVoltage(double u1, double theta1, double u2, double theta2,
                                                               double u3, double theta3,
                                                               const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg1,
                                                               const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg2,
                                                               const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg3) {
    std::complex<double> v1 = math::ComplexUtils::polar2Complex(u1, theta1);
    std::complex<double> v2 = math::ComplexUtils::polar2Complex(u2, theta2);
    std::complex<double> v3 = math::ComplexUtils::polar2Complex(u3, theta3);

    std::complex<double> v0 = -(branchAdmittanceLeg1.y21 * v1 + branchAdmittanceLeg2.y21 * v2 + branchAdmittanceLeg3.y21 * v3)
        / (branchAdmittanceLeg1.y22 + branchAdmittanceLeg2.y22 + branchAdmittanceLeg3.y22);

    LinkData::Flow flowLeg1 = LinkData::flowBothEnds(branchAdmittanceLeg1.y11, branchAdmittanceLeg1.y12,
                                                     branchAdmittanceLeg1.y21, branchAdmittanceLeg1.y22, v1, v0);

    LinkData::Flow flowLeg2 = LinkData::flowBothEnds(branchAdmittanceLeg2.y11, branchAdmittanceLeg2.y12,
                                                     branchAdmittanceLeg2.y21, branchAdmittanceLeg2.y22, v2, v0);

    LinkData::Flow flowLeg3 = LinkData::flowBothEnds(branchAdmittanceLeg3.y11, branchAdmittanceLeg3.y12,
                                                     branchAdmittanceLeg3.y21, branchAdmittanceLeg3.y22, v3, v0);

    m_computedP1 = std::real(flowLeg1.fromTo);
    m_computedQ1 = std::imag(flowLeg1.fromTo);
    m_computedP2 = std::real(flowLeg2.fromTo);
    m_computedQ2 = std::imag(flowLeg2.fromTo);
    m_computedP3 = std::real(flowLeg3.fromTo);
    m_computedQ3 = std::imag(flowLeg3.fromTo);

    m_starU = std::abs(v0);
    m_starTheta = std::arg(v0);
}

LinkData::BranchAdmittanceMatrix TwtData::calculateTwoConnectedLegsAdmittance(const LinkData::BranchAdmittanceMatrix& firstCloseLeg,
                                                                              const LinkData::BranchAdmittanceMatrix& secondCloseLeg,
                                                                              const LinkData::BranchAdmittanceMatrix& openLeg) const {

    std::complex<double> ysh = LinkData::kronAntenna(openLeg.y11, openLeg.y12, openLeg.y21, openLeg.y22, true);
    LinkData::BranchAdmittanceMatrix secondCloseLegMod(secondCloseLeg.y11, secondCloseLeg.y12, secondCloseLeg.y21, secondCloseLeg.y22 + ysh);
    return LinkData::kronChain(firstCloseLeg, Branch::Side::TWO, secondCloseLegMod, Branch::Side::TWO);
}

LinkData::Flow TwtData::calculateTwoConnectedLegsFlow(double u1, double theta1, double u2, double theta2,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg1,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg2,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixOpenLeg) const {

    std::complex<double> v1 = math::ComplexUtils::polar2Complex(u1, theta1);
    std::complex<double> v2 = math::ComplexUtils::polar2Complex(u2, theta2);

    LinkData::BranchAdmittanceMatrix admittance = calculateTwoConnectedLegsAdmittance(admittanceMatrixLeg1,
                                                                                      admittanceMatrixLeg2, admittanceMatrixOpenLeg);

    return LinkData::flowBothEnds(admittance.y11, admittance.y12, admittance.y21, admittance.y22, v1, v2);
}

std::complex<double> TwtData::calculateTwoConnectedLegsStarBusVoltage(double u1, double theta1, double u2, double theta2,
                                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg1,
                                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg2,
                                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixOpenLeg) const {

    std::complex<double> v1 = math::ComplexUtils::polar2Complex(u1, theta1);
    std::complex<double> v2 = math::ComplexUtils::polar2Complex(u2, theta2);

    std::complex<double> yshO = LinkData::kronAntenna(admittanceMatrixOpenLeg.y11, admittanceMatrixOpenLeg.y12, admittanceMatrixOpenLeg.y21, admittanceMatrixOpenLeg.y22, true);
    return -(admittanceMatrixLeg1.y21 * v1 + (admittanceMatrixLeg2.y21 * v2))
        / (admittanceMatrixLeg1.y22 + admittanceMatrixLeg2.y22 + yshO);
}

double TwtData::getB1(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_b11;
        case ThreeWindingsTransformer::Side::TWO:
            return m_b21;
        case ThreeWindingsTransformer::Side::THREE:
            return m_b31;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getB1(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance) {
    return getValue(twtSplitShuntAdmittance ? leg.getB() / 2 : leg.getB(),
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getB() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getB() : 0.0);
}

double TwtData::getB2(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_b12;
        case ThreeWindingsTransformer::Side::TWO:
            return m_b22;
        case ThreeWindingsTransformer::Side::THREE:
            return m_b32;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getB2(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance) {
    return getValue(twtSplitShuntAdmittance ? leg.getB() / 2 : 0.0,
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getB() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getB() : 0.0);
}

double TwtData::getComputedP(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_computedP1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_computedP2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_computedP3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getComputedQ(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_computedQ1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_computedQ2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_computedQ3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getG1(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_g11;
        case ThreeWindingsTransformer::Side::TWO:
            return m_g21;
        case ThreeWindingsTransformer::Side::THREE:
            return m_g31;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getG1(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance) {
    return getValue(twtSplitShuntAdmittance ? leg.getG() / 2 : leg.getG(),
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getG() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getG() : 0.0);
}

double TwtData::getG2(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_g12;
        case ThreeWindingsTransformer::Side::TWO:
            return m_g22;
        case ThreeWindingsTransformer::Side::THREE:
            return m_g32;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getG2(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance) {
    return getValue(twtSplitShuntAdmittance ? leg.getG() / 2 : 0.0,
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getG() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getG() : 0.0);
}

const std::string& TwtData::getId() const {
    return m_id;
}

double TwtData::getP(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_p1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_p2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_p3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

int TwtData::getPhaseAngleClock2() const {
    return m_phaseAngleClock2;
}

int TwtData::getPhaseAngleClock3() const {
    return m_phaseAngleClock3;
}

double TwtData::getQ(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_q1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_q2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_q3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getR(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_r1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_r2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_r3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getR(const ThreeWindingsTransformer::Leg& leg) {
    return getValue(leg.getR(),
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getR() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getR() : 0.0);
}

double TwtData::getRatedU(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_ratedU1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_ratedU2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_ratedU3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getRatedU0() const {
    return m_ratedU0;
}

double TwtData::getStarTheta() const {
    return m_starTheta;
}

double TwtData::getStarU() const {
    return m_starU;
}

double TwtData::getTheta(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_theta1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_theta2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_theta3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getTheta(const ThreeWindingsTransformer::Leg& leg) {
    return leg.getTerminal().isConnected() ? stdcxx::toRadians * leg.getTerminal().getBusView().getBus().get().getAngle() : stdcxx::nan();
}

double TwtData::getU(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_u1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_u2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_u3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getV(const ThreeWindingsTransformer::Leg& leg) {
    return leg.getTerminal().isConnected() ? leg.getTerminal().getBusView().getBus().get().getV() : stdcxx::nan();
}

double TwtData::getValue(double initialValue, double rtcStepValue, double ptcStepValue) {
    return initialValue * (1 + rtcStepValue / 100) * (1 + ptcStepValue / 100);
}

double TwtData::getX(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_x1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_x2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_x3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

double TwtData::getX(const ThreeWindingsTransformer::Leg& leg) {
    return getValue(leg.getX(),
                    leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getX() : 0.0,
                    leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getX() : 0.0);
}

bool TwtData::isConnected(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_connected1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_connected2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_connected3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

bool TwtData::isMainComponent(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_mainComponent1;
        case ThreeWindingsTransformer::Side::TWO:
            return m_mainComponent2;
        case ThreeWindingsTransformer::Side::THREE:
            return m_mainComponent3;
        default:
            throw AssertionError(UNEXPECTED_SIDE + ": " + Enum::toString(side));
    }
}

bool TwtData::isMainComponent(const ThreeWindingsTransformer::Leg& leg) {
    const auto& bus = leg.getTerminal().getBusView().getBus();
    const auto& connectableBus = leg.getTerminal().getBusView().getConnectableBus();
    bool connectableMainComponent = connectableBus && connectableBus.get().isInMainConnectedComponent();
    return bus ? bus.get().isInMainConnectedComponent() : connectableMainComponent;
}

double TwtData::rho(const ThreeWindingsTransformer::Leg& leg, double ratedU0) {
    double rho = ratedU0 / leg.getRatedU();
    rho *= leg.getOptionalRatioTapChanger() ? leg.getRatioTapChanger().getCurrentStep().getRho() : 1.0;
    rho *= leg.getOptionalPhaseTapChanger() ? leg.getPhaseTapChanger().getCurrentStep().getRho() : 1.0;
    return rho;
}

bool TwtData::valid(double voltage, double theta) {
    if (std::isnan(voltage) || voltage <= 0.0) {
        return false;
    }
    return !std::isnan(theta);
}

}  // namespace iidm

}  // namespace powsybl
