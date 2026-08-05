/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/BoundaryLineUtil.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/util/LinkData.hpp>

namespace powsybl {

namespace iidm {

namespace BoundaryLineUtil {

    namespace{
        bool validVTheta(double v, double theta) {
            if (std::isnan(v) || v <= 0.0) {
                return false;
            }
            return !std::isnan(theta);
        }

        std::complex<double> getBoundaryBus(const BoundaryLine& boundaryLine) {

            double u1 = getV(boundaryLine);
            double theta1 = getTheta(boundaryLine);
            if (!validVTheta(u1, theta1)) {
                return std::complex<double>(stdcxx::nan(), stdcxx::nan());
            }
            if(zeroImpedance(boundaryLine)) {
                return std::polar(u1, theta1);
            }

            // BoundaryLine model has shunt admittance on network side only, so it is not split between both sides.
            std::complex<double> c1 = std::polar(u1, theta1);
            std::complex<double> cBoundaryBus = std::complex<double>(stdcxx::nan(), stdcxx::nan());
        
            if (boundaryLine.getP0() == 0.0 && boundaryLine.getQ0() == 0.0) {
                LinkData::BranchAdmittanceMatrix adm = LinkData::calculateBranchAdmittance(boundaryLine.getR(), boundaryLine.getX(), 1.0, 0.0, 1.0, 0.0, 
                                                std::complex<double>(boundaryLine.getG(), boundaryLine.getB()), std::complex<double>(0.0, 0.0));
                cBoundaryBus = - adm.y21 * c1 / adm.y22;
            } else {
                // Two buses Loadflow
                std::complex<double> sBoundary = std::complex<double>(-boundaryLine.getP0(), -boundaryLine.getQ0());
                std::complex<double> zt = std::complex<double>(boundaryLine.getR(), boundaryLine.getX());
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

bool useHypothesis(const BoundaryLine& boundaryLine) {
    return !boundaryLine.isPaired() && !std::isnan(boundaryLine.getP0()) && !std::isnan(boundaryLine.getQ0()) && !boundaryLine.getGeneration();
}

bool zeroImpedance(const BoundaryLine& boundaryLine) {
    return boundaryLine.getR() == 0.0 && boundaryLine.getX() == 0.0;
}

double getBoundaryBusU(const BoundaryLine& boundaryLine) {
    std::complex<double> cBoundaryBus = getBoundaryBus(boundaryLine);
    return std::abs(cBoundaryBus);
}

double getBoundaryBusTheta(const BoundaryLine& boundaryLine) {
    std::complex<double> cBoundaryBus = getBoundaryBus(boundaryLine);
    return std::arg(cBoundaryBus);
}

double getV(const BoundaryLine& boundaryLine) {
    return boundaryLine.getTerminal().isConnected() ? boundaryLine.getTerminal().getBusView().getBus().get().getV() : stdcxx::nan();
}

double getTheta(const BoundaryLine& boundaryLine) {
    return boundaryLine.getTerminal().isConnected() ? (boundaryLine.getTerminal().getBusView().getBus().get().getAngle() * stdcxx::toRadians) : stdcxx::nan();
}

}  // namespace BoundaryLineUtil

}  // namespace iidm

}  // namespace powsybl

