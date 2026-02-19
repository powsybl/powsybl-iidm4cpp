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

        std::complex<double> getBoundaryBus(const DanglingLine& danglingLine) {

            double u1 = getV(danglingLine);
            double theta1 = getTheta(danglingLine);
            if (!validVTheta(u1, theta1)) {
                return std::complex<double>(stdcxx::nan(), stdcxx::nan());
            }
            if(zeroImpedance(danglingLine)) {
                return std::polar(u1, theta1);
            }

            // DanglingLine model has shunt admittance on network side only, so it is not split between both sides.
            std::complex<double> c1 = std::polar(u1, theta1);
            std::complex<double> cBoundaryBus = std::complex<double>(stdcxx::nan(), stdcxx::nan());
        
            if (danglingLine.getP0() == 0.0 && danglingLine.getQ0() == 0.0) {
                LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(danglingLine.getR(), danglingLine.getX(), 1.0, 0.0, 1.0, 0.0, 
                                                std::complex<double>(danglingLine.getG(), danglingLine.getB()), std::complex<double>(0.0, 0.0));
                cBoundaryBus = - adm.y21 * c1 / adm.y22;
            } else {
                // Two buses Loadflow
                std::complex<double> sBoundary = std::complex<double>(-danglingLine.getP0(), -danglingLine.getQ0());
                std::complex<double> zt = std::complex<double>(danglingLine.getR(), danglingLine.getX());
                double v12 = std::abs(c1) * std::abs(c1);

                std::complex<double> sigma = zt * std::conj(sBoundary) * (std::complex<double>(1.0) / v12);
                double d = 0.25 + sigma.real() - sigma.imag() * sigma.imag();
                // d < 0 Collapsed network
                if (d >= 0) {
                    cBoundaryBus = std::complex<double>(0.5 + std::sqrt(d), sigma.imag()) * c1;
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
    std::complex<double> cBoundaryBus = getBoundaryBus(danglingLine);
    return std::abs(cBoundaryBus);
}

double getBoundaryBusTheta(const DanglingLine& danglingLine) {
    std::complex<double> cBoundaryBus = getBoundaryBus(danglingLine);
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

