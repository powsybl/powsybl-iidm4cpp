/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SV_HPP
#define POWSYBL_IIDM_SV_HPP

#include <complex>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/util/LinkData.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;
class Line;
class TwoWindingsTransformer;

namespace tie_line {

class HalfLine;

}  // namespace tie_line

class SV {
public:
    SV(double p, double q, double u, double a, const Branch::Side& side);

    double getA() const;

    const Branch::Side& getSide() const;

    double getP() const;

    double getQ() const;

    double getU() const;

    SV otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    SV otherSide(const TwoWindingsTransformer& twt) const;

    SV otherSide(const TwoWindingsTransformer& twt, bool splitShuntAdmittance) const;

    SV otherSide(const Line& line) const;

    SV otherSide(const DanglingLine& dl) const;

    SV otherSide(const DanglingLine& dl, bool splitShuntAdmittance) const;

    double otherSideA(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideA(const tie_line::HalfLine& halfLine) const;

    double otherSideA(const DanglingLine& dl) const;

    double otherSideA(const DanglingLine& dl, bool splitShuntAdmittance) const;

    double otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideP(const tie_line::HalfLine& halfLine) const;

    double otherSideP(const DanglingLine& dl) const;

    double otherSideP(const DanglingLine& dl, bool splitShuntAdmittance) const;

    double otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideQ(const tie_line::HalfLine& halfLine) const;

    double otherSideQ(const DanglingLine& dl) const;

    double otherSideQ(const DanglingLine& dl, bool splitShuntAdmittance) const;

    double otherSideU(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideU(const tie_line::HalfLine& halfLine) const;

    double otherSideU(const DanglingLine& dl) const;

    double otherSideU(const DanglingLine& dl, bool splitShuntAdmittance) const;

private:
    // Get S1 from Y11.V1 + Y12.V2 = S1* / V1*
    static std::complex<double> flowAtEnd1(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& vEnd2);

    // Get S2 from Y21.V1 + Y22.V2 = S2* / V2*
    static std::complex<double> flowAtEnd2(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& vEnd2);

    static double getAlpha(const TwoWindingsTransformer& twt);

    static double getB(const TwoWindingsTransformer& twt);

    static double getG(const TwoWindingsTransformer& twt);

    static double getR(const TwoWindingsTransformer& twt);

    static double getRho(const TwoWindingsTransformer& twt);

    static double getX(const TwoWindingsTransformer& twt);

    // Get V1 from Y21.V1 + Y22.V2 = S2* / V2*
    static std::complex<double> voltageAtEnd1(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd2, const std::complex<double>& sEnd2);

    // Get V2 from Y11.V1 + Y12.V2 = S1* / V1*
    static std::complex<double> voltageAtEnd2(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& sEnd1);

private:
    SV otherSide(const LinkData::BranchAdmittanceMatrix& adm) const;

    double otherSideA(const LinkData::BranchAdmittanceMatrix& adm) const;

    double otherSideP(const LinkData::BranchAdmittanceMatrix& adm) const;

    double otherSideQ(const LinkData::BranchAdmittanceMatrix& adm) const;

    double otherSideU(const LinkData::BranchAdmittanceMatrix& adm) const;

    std::complex<double> otherSideV(const LinkData::BranchAdmittanceMatrix& adm) const;

private:
    double m_p;

    double m_q;

    double m_u;

    double m_a;

    Branch::Side m_side;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SV_HPP
