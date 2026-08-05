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

class BoundaryLine;
class Line;
class TwoWindingsTransformer;
class TieLine;

namespace tie_line {

}  // namespace tie_line

class SV {
public:
    SV(double p, double q, double u, double a, const TwoSides& side);

    double getA() const;

    double getI() const;

    const TwoSides& getSide() const;

    double getP() const;

    double getQ() const;

    double getU() const;

    SV otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    SV otherSide(const TwoWindingsTransformer& twt) const;

    SV otherSide(const TwoWindingsTransformer& twt, bool splitShuntAdmittance) const;

    SV otherSide(const Line& line) const;

    SV otherSide(const TieLine& tieLine) const;

    SV otherSide(const BoundaryLine& bl) const;

    SV otherSide(const BoundaryLine& bl, bool splitShuntAdmittance) const;

    double otherSideA(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideA(const BoundaryLine& bl) const;

    double otherSideA(const BoundaryLine& bl, bool splitShuntAdmittance) const;

    double otherSideI(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideI(const BoundaryLine& bl) const;

    double otherSideI(const BoundaryLine& bl, bool splitShuntAdmittance) const;

    double otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideP(const BoundaryLine& bl) const;

    double otherSideP(const BoundaryLine& bl, bool splitShuntAdmittance) const;

    double otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideQ(const BoundaryLine& bl) const;

    double otherSideQ(const BoundaryLine& bl, bool splitShuntAdmittance) const;

    double otherSideU(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const;

    double otherSideU(const BoundaryLine& bl) const;

    double otherSideU(const BoundaryLine& bl, bool splitShuntAdmittance) const;

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

    bool isAllDataForCalculatingOtherSide() const;

    bool isAllDataForCalculatingOterSideDcApproximation(double zBase) const;

private:
    SV otherSide(const LinkData::BranchAdmittanceMatrix& adm) const;

    SV otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha, double zb) const;

    SV otherSideDcApproximation(double x, double ratio, double angle, double zb, bool useRatio) const;

private:
    double m_p;

    double m_q;

    double m_u;

    double m_a;

    TwoSides m_side;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SV_HPP
