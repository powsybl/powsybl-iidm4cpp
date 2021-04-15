/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/SV.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/TieLine.hpp>
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
    m_u1(std::polar(u, a * stdcxx::toRadians)),
    m_v1(m_u1 / sqrt(3.0)),
    m_i1(std::conj(m_s1 / (m_v1 * 3.0))) {
}

std::complex<double> SV::computeS2(const std::complex<double>& y1, const std::complex<double>& y2, const std::complex<double>& z, double rho) const {
    std::complex<double> v1p = m_v1 * rho;
    std::complex<double> i1p = m_i1 / rho;

    std::complex<double> i2p = i1p - (y1 * v1p);
    std::complex<double> v2 = v1p - (z * i2p);
    std::complex<double> i2 = i2p - (y2 * v2);
    return v2 * 3.0 * std::conj(i2);
}

std::complex<double> SV::computeS2(const std::complex<double>& y, const std::complex<double>& z, double rho) const {
    return computeS2(y, std::complex<double>(), z, rho);
}

std::complex<double> SV::computeU2(const std::complex<double>& y1, const std::complex<double>& /*y2*/, const std::complex<double>& z, double rho) const {
    std::complex<double> v1p = m_v1 * rho;
    std::complex<double> i1p = m_i1 / rho;

    std::complex<double> i2p = i1p - (y1 * v1p);
    std::complex<double> v2 = v1p - (z * i2p);
    return v2 * std::sqrt(3.0);
}

std::complex<double> SV::computeU2(const std::complex<double>& y, const std::complex<double>& z, double rho) const {
    return computeU2(y, std::complex<double>(0, 0), z, rho);
}

double SV::getA() const {
    return m_a;
}

double SV::getB(const TwoWindingsTransformer& twt) {
    return twt.getB() * (twt.hasRatioTapChanger() ? 1 + twt.getRatioTapChanger().getCurrentStep().getB() / 100 : 1.0);
}

double SV::getG(const TwoWindingsTransformer& twt) {
    return twt.getG() * (twt.hasRatioTapChanger() ? 1 + twt.getRatioTapChanger().getCurrentStep().getG() / 100 : 1.0);
}

double SV::getP() const {
    return m_p;
}

double SV::getQ() const {
    return m_q;
}

double SV::getR(const TwoWindingsTransformer& twt) {
    return twt.getR() * (twt.hasRatioTapChanger() ? 1 + twt.getRatioTapChanger().getCurrentStep().getR() / 100 : 1.0);
}

double SV::getRho(const TwoWindingsTransformer& twt) {
    return twt.getRatedU2() / twt.getRatedU1() * (twt.hasRatioTapChanger() ? 1 + twt.getRatioTapChanger().getCurrentStep().getRho() / 100 : 1.0);
}

double SV::getU() const {
    return m_u;
}

double SV::getX(const TwoWindingsTransformer& twt) {
    return twt.getX() * (twt.hasRatioTapChanger() ? 1 + twt.getRatioTapChanger().getCurrentStep().getX() / 100 : 1.0);
}

SV SV::otherSide(double r, double x, double g, double b, double rho) const {
    std::complex<double> z(r, x);
    std::complex<double> y(g, b);
    std::complex<double> u2 = computeU2(y, z, rho);
    std::complex<double> s2 = computeS2(y, z, rho);
    return {-s2.real(), -s2.imag(), std::abs(u2), std::arg(u2) * stdcxx::toDegrees};
}

SV SV::otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z (r, x);
    std::complex<double> y1(g1, b1);
    std::complex<double> y2(g2, b2);
    std::complex<double> u2 = computeU2(y1, y2, z, rho);
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return {-s2.real(), -s2.imag(), std::abs(u2), std::arg(u2) * stdcxx::toDegrees};
}

SV SV::otherSide(const TwoWindingsTransformer& twt) const {
    return otherSide(getR(twt), getX(twt), getG(twt), getB(twt), getRho(twt));
}

SV SV::otherSide(const Line& line) const {
    return otherSide(line.getR(), line.getX(), line.getG1() + line.getG2(), line.getB1() + line.getB2(), 1);
}

SV SV::otherSide(const DanglingLine& danglingLine) const {
    return otherSide(danglingLine.getR(), danglingLine.getX(), danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, 1);
}

double SV::otherSideA(double r, double x, double g1, double b1, double rho) const {
    std::complex<double> z(r, x);
    std::complex<double> y1(g1, b1);
    std::complex<double> u2 = computeU2(y1, std::complex<double>(), z, rho);
    return std::arg(u2) * stdcxx::toDegrees;
}

double SV::otherSideA(const TieLine::HalfLine& halfLine) const {
    return otherSideA(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), 1.0);
}

double SV::otherSideA(const DanglingLine& danglingLine) const {
    return otherSideA(danglingLine.getR(), danglingLine.getX(), danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, 1.0);
}

double SV::otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z(r, x);
    std::complex<double> y1(g1, b1);
    std::complex<double> y2(g2, b2);
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return -s2.real();
}

double SV::otherSideP(const DanglingLine& danglingLine) const {
    return otherSideP(danglingLine.getR(), danglingLine.getX(), danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, 1);
}

double SV::otherSideP(const TieLine::HalfLine& halfLine) const {
    return otherSideP(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0);
}

double SV::otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho) const {
    std::complex<double> z (r, x);
    std::complex<double> y1(g1, b1);
    std::complex<double> y2(g2, b2);
    std::complex<double> s2 = computeS2(y1, y2, z, rho);
    return -s2.imag();
}

double SV::otherSideQ(const TieLine::HalfLine& halfLine) const {
    return otherSideQ(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0);
}

double SV::otherSideQ(const DanglingLine& danglingLine) const {
    return otherSideQ(danglingLine.getR(), danglingLine.getX(), danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, 1);
}

double SV::otherSideU(double r, double x, double g1, double b1, double rho) const {
    std::complex<double> z(r, x);
    std::complex<double> y1(g1, b1);
    std::complex<double> u2 = computeU2(y1, std::complex<double>(), z, rho);
    return std::abs(u2);
}

double SV::otherSideU(const TieLine::HalfLine& halfLine) const {
    return otherSideU(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), 1.0);
}

double SV::otherSideU(const DanglingLine& danglingLine) const {
    return otherSideU(danglingLine.getR(), danglingLine.getX(), danglingLine.getG() / 2.0, danglingLine.getB() / 2.0, 1);
}

SV SV::otherSideY1Y2(const Line& line) const {
    return otherSide(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1);
}

}  // namespace iidm

}  // namespace powsybl
