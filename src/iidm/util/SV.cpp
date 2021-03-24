/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/SV.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

SV::SV(double p, double q, double u, double a) :
    m_p(p),
    m_q(q),
    m_u(u),
    m_a(a),
    m_s1(p, q),
    m_u1(std::polar(u, stdcxx::toRadians(a))),
    m_v1(m_u1 / sqrt(3.0)),  // v1=u1/sqrt(3)
    m_i1(std::conj(m_s1 / (m_v1 * 3.0))) {  // i1=conj(s1/(3*v1))
}

std::complex<double> SV::computeS2(const std::complex<double>& y1, const std::complex<double>& y2, const std::complex<double>& z, double rho) const {
    std::complex<double> v1p = m_v1 * rho;  // v1p=v1*rho
    std::complex<double> i1p = m_i1 / rho;  // i1p=i1/rho

    std::complex<double> i2p = i1p - (y1 * v1p);  // i2p=i1p-y1*v1p
    std::complex<double> v2 = v1p - (z * i2p);  // v2p=v1p-z*i2
    std::complex<double> i2 = i2p - (y2 * v2);  // i2=i2p-y2*v2
    return v2 * 3.0 * std::conj(i2);  // s2=3*v2*conj(i2)
}

std::complex<double> SV::computeS2(const std::complex<double>& y, const std::complex<double>& z, double rho) const {
    return computeS2(y, std::complex<double>(), z, rho);
}

std::complex<double> SV::computeU2(const std::complex<double>& y1, const std::complex<double>& /*y2*/, const std::complex<double>& z, double rho) const {
    std::complex<double> v1p = m_v1 * rho;  // v1p=v1*rho
    std::complex<double> i1p = m_i1 / rho;  // i1p=i1/rho

    std::complex<double> i2p = i1p - (y1 * v1p);  // i2p=i1p-y1*v1p
    std::complex<double> v2 = v1p - (z * i2p);  // v2p=v1p-z*i2
    return v2 * std::sqrt(3.0);
}

std::complex<double> SV::computeU2(const std::complex<double>& y, const std::complex<double>& z, double rho) const {
    return computeU2(y, std::complex<double>(0, 0), z, rho);
}

double SV::getA() const {
    return m_a;
}

double SV::getB(const TwoWindingsTransformer& twt) {
    return twt.getB() * (twt.getOptionalRatioTapChanger() ? 1 + twt.getOptionalRatioTapChanger().get().getCurrentStep().getB() / 100 : 1.0);
}

double SV::getG(const TwoWindingsTransformer& twt) {
    return twt.getG() * (twt.getOptionalRatioTapChanger() ? 1 + twt.getOptionalRatioTapChanger().get().getCurrentStep().getG() / 100 : 1.0);
}

double SV::getP() const {
    return m_p;
}

double SV::getQ() const {
    return m_q;
}

double SV::getR(const TwoWindingsTransformer& twt) {
    return twt.getR() * (twt.getOptionalRatioTapChanger() ? 1 + twt.getOptionalRatioTapChanger().get().getCurrentStep().getR() / 100 : 1.0);
}

double SV::getRho(const TwoWindingsTransformer& twt) {
    return twt.getRatedU2() / twt.getRatedU1() * (twt.getOptionalRatioTapChanger() ? 1 + twt.getOptionalRatioTapChanger().get().getCurrentStep().getRho() / 100 : 1.0);
}

double SV::getU() const {
    return m_u;
}

double SV::getX(const TwoWindingsTransformer& twt) {
    return twt.getX() * (twt.getOptionalRatioTapChanger() ? 1 + twt.getOptionalRatioTapChanger().get().getCurrentStep().getX() / 100 : 1.0);
}

SV SV::otherSide(double r, double x, double g, double b, double rho) const {
    std::complex<double> z(r, x);  // z=r+jx
    std::complex<double> y(g, b);  // y=g+jb
    std::complex<double> u2 = computeU2(y, z, rho);
    std::complex<double> s2 = computeS2(y, z, rho);
    return {-s2.real(), -s2.imag(), std::abs(u2), stdcxx::toDegrees(std::arg(u2))};
}

SV SV::otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z (r, x);  // z=r+jx
    std::complex<double> y1(g1, b1);  // y1=g1+jb1
    std::complex<double> y2(g2, b2);  // y2=g2+jb2
    std::complex<double> u2 = computeU2(y1, y2, z, rho);
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return {-s2.real(), -s2.imag(), std::abs(u2), stdcxx::toDegrees(std::arg(u2))};
}

SV SV::otherSide(const TwoWindingsTransformer& twt) const {
    return otherSide(getR(twt), getX(twt), getG(twt), getB(twt), getRho(twt));
}

SV SV::otherSide(const Line& l) const {
    return otherSide(l.getR(), l.getX(), l.getG1() + l.getG2(), l.getB1() + l.getB2(), 1);
}

SV SV::otherSide(const DanglingLine& dl) const {
    return otherSide(dl.getR(), dl.getX(), dl.getG() / 2.0, dl.getB() / 2.0, dl.getG() / 2.0, dl.getB() / 2.0, 1);
}

double SV::otherSideA(double r, double x, double g1, double b1, double rho) const {
    std::complex<double> z(r, x);  // z=r+jx
    std::complex<double> y1(g1, b1);  // y1=g1+jb1
    std::complex<double> u2 = computeU2(y1, std::complex<double>(), z, rho);
    return stdcxx::toDegrees(std::arg(u2));
}

double SV::otherSideA(const TieLine::HalfLine& hl) const {
    return otherSideA(hl.getR(), hl.getX(), hl.getG1(), hl.getB1(), 1.0);
}

double SV::otherSideA(const DanglingLine& dl) const {
    return otherSideA(dl.getR(), dl.getX(), dl.getG() / 2.0, dl.getB() / 2.0, 1.0);
}

double SV::otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z(r, x);  // z=r+jx
    std::complex<double> y1(g1, b1);  // y1=g1+jb1
    std::complex<double> y2(g2, b2);  // y2=g2+jb2
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return -s2.real();
}

double SV::otherSideP(const DanglingLine& dl) const {
    return otherSideP(dl.getR(), dl.getX(), dl.getG() / 2.0, dl.getB() / 2.0, dl.getG() / 2.0, dl.getB() / 2.0, 1);
}

double SV::otherSideP(const TieLine::HalfLine& hl) const {
    return otherSideP(hl.getR(), hl.getX(), hl.getG1(), hl.getB1(), hl.getG2(), hl.getB2(), 1.0);
}

double SV::otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z (r, x);  // z=r+jx
    std::complex<double> y1(g1, b1);  // y1=g1+jb1
    std::complex<double> y2(g2, b2);  // y2=g2+jb2
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return -s2.imag();
}

double SV::otherSideQ(const TieLine::HalfLine& hl) const {
    return otherSideQ(hl.getR(), hl.getX(), hl.getG1(), hl.getB1(), hl.getG2(), hl.getB2(), 1.0);
}

double SV::otherSideQ(const DanglingLine& dl) const {
    return otherSideQ(dl.getR(), dl.getX(), dl.getG() / 2.0, dl.getB() / 2.0, dl.getG() / 2.0, dl.getB() / 2.0, 1);
}

double SV::otherSideU(double r, double x, double g1, double b1, double rho) const {
    std::complex<double> z(r, x);  // z=r+jx
    std::complex<double> y1(g1, b1);  // y1=g1+jb1
    std::complex<double> u2 = computeU2(y1, std::complex<double>(), z, rho);
    return std::abs(u2);
}

double SV::otherSideU(const TieLine::HalfLine& hl) const {
    return otherSideU(hl.getR(), hl.getX(), hl.getG1(), hl.getB1(), 1.0);
}

double SV::otherSideU(const DanglingLine& dl) const {
    return otherSideU(dl.getR(), dl.getX(), dl.getG() / 2.0, dl.getB() / 2.0, 1);
}

SV SV::otherSideY1Y2(const Line& l) const {
    return otherSide(l.getR(), l.getX(), l.getG1(), l.getB1(), l.getG2(), l.getB2(), 1);
}

}  // namespace iidm

}  // namespace powsybl
