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

SV::SV(double p, double q, double u, double a, const Branch::Side& side) :
    m_p(p),
    m_q(q),
    m_u(u),
    m_a(a),
    m_side(side) {
}

std::complex<double> SV::flowAtEnd1(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& vEnd2) {
    return std::conj((adm.y11 * vEnd1 + adm.y12 * vEnd2) * std::conj(vEnd1));
}

std::complex<double> SV::flowAtEnd2(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& vEnd2) {
    return std::conj((adm.y21 * vEnd1 + adm.y22 * vEnd2) * std::conj(vEnd2));
}

double SV::getA() const {
    return m_a;
}

double SV::getAlpha(const TwoWindingsTransformer& twt) {
    double alpha = 0.0;
    if (twt.hasPhaseTapChanger()) {
        alpha = twt.getPhaseTapChanger().getCurrentStep().getAlpha();
    }
    return alpha * stdcxx::toRadians;
}

double SV::getB(const TwoWindingsTransformer& twt) {
    double b = twt.getB();  // WARNING: bug in java here
    if (twt.hasRatioTapChanger()) {
        b = b * (1 + twt.getRatioTapChanger().getCurrentStep().getB() / 100);
    }
    if (twt.hasPhaseTapChanger()) {
        b = b * (1 + twt.getPhaseTapChanger().getCurrentStep().getB() / 100);
    }
    return b;
}

double SV::getG(const TwoWindingsTransformer& twt) {
    double g = twt.getG();
    if (twt.hasRatioTapChanger()) {
        g = g * (1 + twt.getRatioTapChanger().getCurrentStep().getG() / 100);
    }
    if (twt.hasPhaseTapChanger()) {
        g = g * (1 + twt.getPhaseTapChanger().getCurrentStep().getG() / 100);
    }
    return g;
}

double SV::getP() const {
    return m_p;
}

double SV::getQ() const {
    return m_q;
}

double SV::getR(const TwoWindingsTransformer& twt) {
    double r = twt.getR();
    if (twt.hasRatioTapChanger()) {
        r = r * (1 + twt.getRatioTapChanger().getCurrentStep().getR() / 100);
    }
    if (twt.hasPhaseTapChanger()) {
        r = r * (1 + twt.getPhaseTapChanger().getCurrentStep().getR() / 100);
    }
    return r;
}

double SV::getRho(const TwoWindingsTransformer& twt) {
    double rho = twt.getRatedU2() / twt.getRatedU1();
    if (twt.hasRatioTapChanger()) {
        rho = rho * twt.getRatioTapChanger().getCurrentStep().getRho();
    }
    if (twt.hasPhaseTapChanger()) {
        rho = rho * twt.getPhaseTapChanger().getCurrentStep().getRho();
    }
    return rho;
}

const Branch::Side& SV::getSide() const {
    return m_side;
}

double SV::getU() const {
    return m_u;
}

double SV::getX(const TwoWindingsTransformer& twt) {
    double x = twt.getX();
    if (twt.hasRatioTapChanger()) {
        x = x * (1 + twt.getRatioTapChanger().getCurrentStep().getX() / 100);
    }
    if (twt.hasPhaseTapChanger()) {
        x = x * (1 + twt.getPhaseTapChanger().getCurrentStep().getX() / 100);
    }
    return x;
}

SV SV::otherSide(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const {
    LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(r, x, 1 / rho, -alpha, 1.0, 0.0,
                                                                             std::complex<double>(g1, b1), std::complex<double>(g2, b2));
    return otherSide(adm);
}

SV SV::otherSide(const TwoWindingsTransformer& twt) const {
    return otherSide(getR(twt), getX(twt), getG(twt), getB(twt), 0.0, 0.0, getRho(twt), getAlpha(twt));
}

SV SV::otherSide(const TwoWindingsTransformer& twt, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSide(getR(twt), getX(twt), getG(twt) * 0.5, getB(twt) * 0.5, getG(twt) * 0.5, getB(twt) * 0.5, getRho(twt), getAlpha(twt));
    }
    return otherSide(twt);
}

SV SV::otherSide(const Line& line) const {
    return otherSide(line.getR(), line.getX(), line.getG1(), line.getB1(), line.getG2(), line.getB2(), 1.0, 0.0);
}

SV SV::otherSide(const DanglingLine& dl) const {
    return otherSide(dl.getR(), dl.getX(), dl.getG(), dl.getB(), 0.0, 0.0, 1.0, 0.0);
}

SV SV::otherSide(const DanglingLine& dl, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSide(dl.getR(), dl.getX(), dl.getG() * 0.5, dl.getB() * 0.5, dl.getG() * 0.5, dl.getB() * 0.5, 1.0, 0.0);
    }
    return otherSide(dl);
}

SV SV::otherSide(const LinkData::BranchAdmittanceMatrix& adm) const {
    std::complex<double> v;
    std::complex<double> s;
    Branch::Side otherSide;
    if (m_side == Branch::Side::ONE) {
        std::complex<double> v1 = std::polar(m_u, m_a * stdcxx::toRadians);
        std::complex<double> s1(m_p, m_q);
        v = voltageAtEnd2(adm, v1, s1);
        s = flowAtEnd2(adm, v1, v);
        otherSide = Branch::Side::TWO;
    } else {
        std::complex<double> v2 = std::polar(m_u, m_a * stdcxx::toRadians);
        std::complex<double> s2(m_p, m_q);
        v = voltageAtEnd1(adm, v2, s2);
        s = flowAtEnd1(adm, v, v2);
        otherSide = Branch::Side::ONE;
    }
    return {std::real(s), std::imag(s), std::abs(v), std::arg(v) * stdcxx::toDegrees, otherSide};
}

double SV::otherSideA(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const {
    LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(r, x, 1 / rho, -alpha, 1.0, 0.0,
                                                                               std::complex<double>(g1, b1), std::complex<double>(g2, b2));
    return otherSideA(adm);
}

double SV::otherSideA(const TieLine::HalfLine& halfLine) const {
    return otherSideA(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0, 0.0);
}

double SV::otherSideA(const DanglingLine& dl) const {
    return otherSideA(dl.getR(), dl.getX(), dl.getG(), dl.getB(), 0.0, 0.0, 1.0, 0.0);
}

double SV::otherSideA(const DanglingLine& dl, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSideA(dl.getR(), dl.getX(), dl.getG() * 0.5, dl.getB() * 0.5, dl.getG() * 0.5, dl.getB() * 0.5, 1.0, 0.0);
    }
    return otherSideA(dl);
}

double SV::otherSideA(const LinkData::BranchAdmittanceMatrix& adm) const {
    return std::arg(otherSideV(adm)) * stdcxx::toDegrees;
}

double SV::otherSideP(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const {
    LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(r, x, 1 / rho, -alpha, 1.0, 0.0,
                                                                               std::complex<double>(g1, b1), std::complex<double>(g2, b2));
    return otherSideP(adm);
}

double SV::otherSideP(const TieLine::HalfLine& halfLine) const {
    return otherSideP(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0, 0.0);
}

double SV::otherSideP(const DanglingLine& dl) const {
    return otherSideP(dl.getR(), dl.getX(), dl.getG(), dl.getB(), 0.0, 0.0, 1.0, 0.0);
}

double SV::otherSideP(const DanglingLine& dl, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSideP(dl.getR(), dl.getX(), dl.getG() * 0.5, dl.getB() * 0.5, dl.getG() * 0.5, dl.getB() * 0.5, 1.0, 0.0);
    }
    return otherSideP(dl);
}

double SV::otherSideP(const LinkData::BranchAdmittanceMatrix& adm) const {
    return otherSide(adm).getP();
}

double SV::otherSideQ(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const {
    LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(r, x, 1 / rho, -alpha, 1.0, 0.0,
                                                                               std::complex<double>(g1, b1), std::complex<double>(g2, b2));
    return otherSideQ(adm);
}

double SV::otherSideQ(const TieLine::HalfLine& halfLine) const {
    return otherSideQ(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0, 0.0);
}

double SV::otherSideQ(const DanglingLine& dl) const {
    return otherSideQ(dl.getR(), dl.getX(), dl.getG(), dl.getB(), 0.0, 0.0, 1.0, 0.0);
}

double SV::otherSideQ(const DanglingLine& dl, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSideQ(dl.getR(), dl.getX(), dl.getG() * 0.5, dl.getB() * 0.5, dl.getG() * 0.5, dl.getB() * 0.5, 1.0, 0.0);
    }
    return otherSideQ(dl);
}

double SV::otherSideQ(const LinkData::BranchAdmittanceMatrix& adm) const {
    return otherSide(adm).getQ();
}

double SV::otherSideU(double r, double x, double g1, double b1, double g2, double b2, double rho, double alpha) const {
    LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(r, x, 1 / rho, -alpha, 1.0, 0.0,
                                                                               std::complex<double>(g1, b1), std::complex<double>(g2, b2));
    return otherSideU(adm);
}

double SV::otherSideU(const TieLine::HalfLine& halfLine) const {
    return otherSideU(halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2(), 1.0, 0.0);
}

double SV::otherSideU(const DanglingLine& dl) const {
    return otherSideU(dl.getR(), dl.getX(), dl.getG(), dl.getB(), 0.0, 0.0, 1.0, 0.0);
}

double SV::otherSideU(const DanglingLine& dl, bool splitShuntAdmittance) const {
    if (splitShuntAdmittance) {
        return otherSideU(dl.getR(), dl.getX(), dl.getG() * 0.5, dl.getB() * 0.5, dl.getG() * 0.5, dl.getB() * 0.5, 1.0, 0.0);
    }
    return otherSideU(dl);
}

double SV::otherSideU(const LinkData::BranchAdmittanceMatrix& adm) const {
    return std::abs(otherSideV(adm));
}

std::complex<double> SV::otherSideV(const LinkData::BranchAdmittanceMatrix& adm) const {
    std::complex<double> v;
    if (m_side == Branch::Side::ONE) {
        std::complex<double> v1 = std::polar(m_u, m_a * stdcxx::toRadians);
        std::complex<double> s1(m_p, m_q);
        v = voltageAtEnd2(adm, v1, s1);
    } else {
        std::complex<double> v2 = std::polar(m_u, m_a * stdcxx::toRadians);
        std::complex<double> s2(m_p, m_q);
        v = voltageAtEnd1(adm, v2, s2);
    }
    return v;
}

std::complex<double> SV::voltageAtEnd1(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd2, const std::complex<double>& sEnd2) {
    return (std::conj(sEnd2) / std::conj(vEnd2) - adm.y22 * vEnd2) / adm.y21;
}

std::complex<double> SV::voltageAtEnd2(const LinkData::BranchAdmittanceMatrix& adm, const std::complex<double>& vEnd1, const std::complex<double>& sEnd1) {
    return (std::conj(sEnd1) / std::conj(vEnd1) - adm.y11 * vEnd1) / adm.y12;
}

}  // namespace iidm

}  // namespace powsybl
