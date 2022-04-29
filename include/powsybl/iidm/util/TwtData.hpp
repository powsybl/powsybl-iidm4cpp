/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_TWTDATA_HPP
#define POWSYBL_IIDM_UTIL_TWTDATA_HPP

#include <complex>
#include <string>

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/util/LinkData.hpp>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformer;

class TwtData {
public:
    TwtData(const ThreeWindingsTransformer& twt, double epsilonX, bool applyReactanceCorrection);

    TwtData(const ThreeWindingsTransformer& twt, double epsilonX, bool applyReactanceCorrection, bool twtSplitShuntAdmittance);

    TwtData(const ThreeWindingsTransformer& twt, int phaseAngleClock2, int phaseAngleClock3, double epsilonX, bool applyReactanceCorrection, 
            bool twtSplitShuntAdmittance);

    double getB1(const ThreeWindingsTransformer::Side& side) const;

    double getB2(const ThreeWindingsTransformer::Side& side) const;

    double getComputedP(const ThreeWindingsTransformer::Side& side) const;

    double getComputedQ(const ThreeWindingsTransformer::Side& side) const;

    double getG1(const ThreeWindingsTransformer::Side& side) const;

    double getG2(const ThreeWindingsTransformer::Side& side) const;

    const std::string& getId() const;

    double getP(const ThreeWindingsTransformer::Side& side) const;

    double getQ(const ThreeWindingsTransformer::Side& side) const;

    double getR(const ThreeWindingsTransformer::Side& side) const;

    double getRatedU(const ThreeWindingsTransformer::Side& side) const;

    double getStarTheta() const;

    double getStarU() const;

    double getTheta(const ThreeWindingsTransformer::Side& side) const;

    double getU(const ThreeWindingsTransformer::Side& side) const;

    double getX(const ThreeWindingsTransformer::Side& side) const;

    bool isConnected(const ThreeWindingsTransformer::Side& side) const;

    bool isMainComponent(const ThreeWindingsTransformer::Side& side) const;

    int getPhaseAngleClock2() const;

    int getPhaseAngleClock3() const;

    double getRatedU0() const;
    
private:
    static double alpha(const ThreeWindingsTransformer::Leg& leg);

    static double getB1(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance);

    static double getB2(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance);

    static double getG1(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance);

    static double getG2(const ThreeWindingsTransformer::Leg& leg, bool twtSplitShuntAdmittance);

    static double getR(const ThreeWindingsTransformer::Leg& leg);

    static double getTheta(const ThreeWindingsTransformer::Leg& leg);

    static double getV(const ThreeWindingsTransformer::Leg& leg);

    static double getValue(double initialValue, double rtcStepValue, double ptcStepValue);

    static double getX(const ThreeWindingsTransformer::Leg& leg);

    static bool isMainComponent(const ThreeWindingsTransformer::Leg& leg);

    static double rho(const ThreeWindingsTransformer::Leg& leg, double ratedU0);

    static bool valid(double voltage, double theta);

private:
    std::complex<double> calculateOneConnectedLegFlow(double u, double theta,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixFirstOpenLeg,
                                                      const LinkData::BranchAdmittanceMatrix& admittanceMatrixSecondOpenLeg) const;

    std::complex<double> calculateOneConnectedLegShunt(const LinkData::BranchAdmittanceMatrix& closeLeg,
                                                       const LinkData::BranchAdmittanceMatrix& firstOpenLeg,
                                                       const LinkData::BranchAdmittanceMatrix& secondOpenLeg) const;

    std::complex<double> calculateOneConnectedLegStarBusVoltage(double u, double theta,
                                                                const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg,
                                                                const LinkData::BranchAdmittanceMatrix& admittanceMatrixFirstOpenLeg,
                                                                const LinkData::BranchAdmittanceMatrix& admittanceMatrixSecondOpenLeg) const;

    void calculateThreeConnectedLegsFlowAndStarBusVoltage(double u1, double theta1, double u2, double theta2,
                                                          double u3, double theta3,
                                                          const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg1,
                                                          const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg2,
                                                          const LinkData::BranchAdmittanceMatrix& branchAdmittanceLeg3);

    LinkData::BranchAdmittanceMatrix calculateTwoConnectedLegsAdmittance(const LinkData::BranchAdmittanceMatrix& firstCloseLeg,
                                                                         const LinkData::BranchAdmittanceMatrix& secondCloseLeg,
                                                                         const LinkData::BranchAdmittanceMatrix& openLeg) const;

    LinkData::Flow calculateTwoConnectedLegsFlow(double u1, double theta1, double u2, double theta2,
                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg1,
                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg2,
                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixOpenLeg) const;

    std::complex<double> calculateTwoConnectedLegsStarBusVoltage(double u1, double theta1, double u2, double theta2,
                                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg1,
                                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixLeg2,
                                                                 const LinkData::BranchAdmittanceMatrix& admittanceMatrixOpenLeg) const;
    
private:
    std::string m_id;

    double m_p1;
    double m_q1;
    double m_p2;
    double m_q2;
    double m_p3;
    double m_q3;

    double m_u1;
    double m_theta1;
    double m_u2;
    double m_theta2;
    double m_u3;
    double m_theta3;

    double m_r1;
    double m_x1;
    double m_r2;
    double m_x2;
    double m_r3;
    double m_x3;

    double m_g11;
    double m_b11;
    double m_g12;
    double m_b12;
    double m_g21;
    double m_b21;
    double m_g22;
    double m_b22;
    double m_g31;
    double m_b31;
    double m_g32;
    double m_b32;

    double m_rho1;
    double m_alpha1;
    double m_rho2;
    double m_alpha2;
    double m_rho3;
    double m_alpha3;

    double m_ratedU1;
    double m_ratedU2;
    double m_ratedU3;

    bool m_connected1;
    bool m_connected2;
    bool m_connected3;
    bool m_mainComponent1;
    bool m_mainComponent2;
    bool m_mainComponent3;

    double m_computedP1;
    double m_computedQ1;
    double m_computedP2;
    double m_computedQ2;
    double m_computedP3;
    double m_computedQ3;

    double m_starU;
    double m_starTheta;

    int m_phaseAngleClock2;
    int m_phaseAngleClock3;
    double m_ratedU0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_TWTDATA_HPP
