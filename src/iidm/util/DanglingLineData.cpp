/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/DanglingLineData.hpp>

#include <complex>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/util/LinkData.hpp>
#include <powsybl/math/ComplexUtils.hpp>

namespace powsybl {

namespace iidm {

DanglingLineData::DanglingLineData(const DanglingLine& danglingLine) :
    DanglingLineData(danglingLine, true) {
}

DanglingLineData::DanglingLineData(const DanglingLine& danglingLine, bool splitShuntAdmittance) :
    m_id(danglingLine.getId()),
    m_r(danglingLine.getR()),
    m_x(danglingLine.getX()),
    m_g1(splitShuntAdmittance ? danglingLine.getG() * 0.5 : danglingLine.getG()),
    m_g2(splitShuntAdmittance ? danglingLine.getG() * 0.5 : 0.0),
    m_b1(splitShuntAdmittance ? danglingLine.getB() * 0.5 : danglingLine.getB()),
    m_b2(splitShuntAdmittance ? danglingLine.getB() * 0.5 : 0.0),
    m_u1(getV(danglingLine)),
    m_theta1(getTheta(danglingLine)),
    m_p0(danglingLine.getP0()),
    m_q0(danglingLine.getQ0()) {

    if (!valid(m_u1, m_theta1)) {
        return;
    }

    const std::complex<double>& v1 = math::ComplexUtils::polar2Complex(m_u1, m_theta1);

    std::complex<double> vBoundaryBus(stdcxx::nan(), stdcxx::nan());
    if (m_p0 == 0.0 && m_q0 == 0.0) {
        LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(m_r, m_x, 1.0, 0.0, 1.0, 0.0, std::complex<double>(m_g1, m_b1), std::complex<double>(m_g2, m_b2));
        vBoundaryBus = (- adm.y21 * v1) / adm.y22;
    } else {
        // Two buses Loadflow
        std::complex<double> sBoundary(-m_p0, -m_q0);
        std::complex<double> ytr = 1.0 / std::complex<double>(m_r, m_x);
        std::complex<double> ysh2(m_g2, m_b2);
        std::complex<double> zt = 1.0 / (ytr + ysh2);
        std::complex<double> v0 = ytr * v1 / (ytr + ysh2);
        double v02 = std::abs(v0) * std::abs(v0);

        std::complex<double> sigma = zt * std::conj(sBoundary) / v02;
        double d = 0.25 + std::real(sigma) - std::imag(sigma) * std::imag(sigma);
        // d < 0 Collapsed network
        if (d >= 0) {
            vBoundaryBus = std::complex<double>(0.5 + std::sqrt(d), std::imag(sigma)) * v0;
        }
    }

    m_boundaryBusU = std::abs(vBoundaryBus);
    m_boundaryBusTheta = std::arg(vBoundaryBus);
}

double DanglingLineData::getBoundaryBusTheta() const {
    return m_boundaryBusTheta;
}

double DanglingLineData::getBoundaryBusU() const {
    return m_boundaryBusU;
}

const std::string& DanglingLineData::getId() const {
    return m_id;
}

double DanglingLineData::getTheta(const DanglingLine& danglingLine) {
    return danglingLine.getTerminal().isConnected() ? stdcxx::toRadians * danglingLine.getTerminal().getBusView().getBus().get().getAngle() : stdcxx::nan();
}

double DanglingLineData::getV(const DanglingLine& danglingLine) {
    return danglingLine.getTerminal().isConnected() ? danglingLine.getTerminal().getBusView().getBus().get().getV() : stdcxx::nan();
}

bool DanglingLineData::valid(double v, double theta) {
    if (std::isnan(v) || v <= 0.0) {
        return false;
    }
    return !std::isnan(theta);
}

}  // namespace iidm

}  // namespace powsybl
