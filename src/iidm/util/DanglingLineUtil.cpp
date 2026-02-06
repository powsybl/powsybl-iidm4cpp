/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/DanglingLineUtil.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/util/LinkData.hpp>

namespace powsybl {

namespace iidm {

namespace DanglingLineUtil {

    namespace{
        bool validVTheta(double v, double theta) {
            if (std::isnan(v) || v <= 0.0) {
                return false;
            }
            return !std::isnan(theta);
        }

        std::complex<double> getBoundaryBus(const DanglingLine& danglingLine, bool splitShuntAdmittance) {

            double u1 = getV(danglingLine);
            double theta1 = getTheta(danglingLine);
            if (!validVTheta(u1, theta1)) {
                return std::complex<double>(stdcxx::nan(), stdcxx::nan());
            }
            if(zeroImpedance(danglingLine)) {
                return std::polar(u1, theta1);
            }

            double g1 = splitShuntAdmittance ? danglingLine.getG() * 0.5 : danglingLine.getG();
            double b1 = splitShuntAdmittance ? danglingLine.getB() * 0.5 : danglingLine.getB();
            double g2 = splitShuntAdmittance ? danglingLine.getG() * 0.5 : 0.0;
            double b2 = splitShuntAdmittance ? danglingLine.getB() * 0.5 : 0.0;

            std::complex<double> c1 = std::polar(u1, theta1);
            std::complex<double> cBoundaryBus = std::complex<double>(stdcxx::nan(), stdcxx::nan());
        
            if (danglingLine.getP0() == 0.0 && danglingLine.getQ0() == 0.0) {
                LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(danglingLine.getR(), danglingLine.getX(), 1.0, 0.0, 1.0, 0.0, 
                                                std::complex<double>(g1, b1), std::complex<double>(g2, b2));
                cBoundaryBus = - adm.y21 * c1 / adm.y22;
            } else {
                // Two buses Loadflow
                std::complex<double> sBoundary = std::complex<double>(-danglingLine.getP0(), -danglingLine.getQ0());
                std::complex<double> ytr = std::complex<double>(1.0) / std::complex<double>(danglingLine.getR(), danglingLine.getX());
                std::complex<double> ysh2 = std::complex<double>(g2, b2);
                std::complex<double> zt = std::complex<double>(1.0) / (ytr + ysh2);
                std::complex<double> v0 = ytr * c1 / (ytr + ysh2);
                double v02 = std::abs(v0) * std::abs(v0);

                std::complex<double> sigma = zt * std::conj(sBoundary) * (std::complex<double>(1.0) / v02);
                double d = 0.25 + sigma.real() - sigma.imag() * sigma.imag();
                // d < 0 Collapsed network
                if (d >= 0) {
                    cBoundaryBus = std::complex<double>(0.5 + std::sqrt(d), sigma.imag()) * v0;
                }
            }

            return cBoundaryBus;
        }
    }  //anonymous namespace

bool useHypothesis(const DanglingLine& danglingLine) {
    return !danglingLine.isPaired() && !std::isnan(danglingLine.getP0()) && !std::isnan(danglingLine.getQ0()) && !danglingLine.getGeneration();
}

bool zeroImpedance(const DanglingLine& danglingLine) {
    return danglingLine.getR() == 0.0 && danglingLine.getX() == 0.0;
}

double getBoundaryBusU(const DanglingLine& danglingLine) {
    std::complex<double> cBoundaryBus = getBoundaryBus(danglingLine, true);
    return std::abs(cBoundaryBus);
}

double getBoundaryBusTheta(const DanglingLine& danglingLine) {
    std::complex<double> cBoundaryBus = getBoundaryBus(danglingLine, true);
    return std::arg(cBoundaryBus);
}

double getV(const DanglingLine& danglingLine) {
    return danglingLine.getTerminal().isConnected() ? danglingLine.getTerminal().getBusView().getBus().get().getV() : stdcxx::nan();
}

double getTheta(const DanglingLine& danglingLine) {
    return danglingLine.getTerminal().isConnected() ? (danglingLine.getTerminal().getBusView().getBus().get().getAngle() * stdcxx::toRadians) : stdcxx::nan();
}

}  // namespace DanglingLineUtil

}  // namespace iidm

}  // namespace powsybl

