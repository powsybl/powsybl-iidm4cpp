/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SV_HPP
#define POWSYBL_IIDM_SV_HPP

#include <complex>

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
    static double getB(const TwoWindingsTransformer& twt);

    static double getG(const TwoWindingsTransformer& twt);

    static double getR(const TwoWindingsTransformer& twt);

    static double getRho(const TwoWindingsTransformer& twt);

    static double getX(const TwoWindingsTransformer& twt);

public:
    SV(double p, double q, double u, double a);

    double getA() const;

    double getP() const;

    double getQ() const;

    double getU() const;

    SV otherSide(double r, double x, double g, double b, double rho) const;

    SV otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho) const;

    SV otherSide(const TwoWindingsTransformer& twt) const;

    SV otherSide(const Line& line) const;

    SV otherSide(const DanglingLine& danglingLine) const;

    double otherSideA(double r, double x, double g1, double b1, double rho) const;

    double otherSideA(const tie_line::HalfLine& halfLine) const;

    double otherSideA(const DanglingLine& danglingLine) const;

    double otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho) const;

    double otherSideP(const DanglingLine& danglingLine) const;

    double otherSideP(const tie_line::HalfLine& halfLine) const;

    double otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho) const;

    double otherSideQ(const tie_line::HalfLine& halfLine) const;

    double otherSideQ(const DanglingLine& danglingLine) const;

    double otherSideU(double r, double x, double g1, double b1, double rho) const;

    double otherSideU(const tie_line::HalfLine& halfLine) const;

    double otherSideU(const DanglingLine& danglingLine) const;

    SV otherSideY1Y2(const Line& line) const;

private:
    std::complex<double> computeS2(const std::complex<double>& y1, const std::complex<double>& y2, const std::complex<double>& z, double rho) const;

    std::complex<double> computeS2(const std::complex<double>& y, const std::complex<double>& z, double rho) const;

    std::complex<double> computeU2(const std::complex<double>& y1, const std::complex<double>& y2, const std::complex<double>& z, double rho) const;

    std::complex<double> computeU2(const std::complex<double>& y, const std::complex<double>& z, double rho) const;

private:
    double m_p;

    double m_q;

    double m_u;

    double m_a;

    std::complex<double> m_s1;

    std::complex<double> m_u1;

    std::complex<double> m_v1;

    std::complex<double> m_i1;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SV_HPP
