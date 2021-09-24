/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/LinkData.hpp>

namespace powsybl {

namespace iidm {

namespace LinkData {

BranchAdmittanceMatrix calculateBranchAdmittance(double r, double x, double ratio1, double angle1,
                                                 double ratio2, double angle2, const std::complex<double>& ysh1, const std::complex<double>& ysh2) {
    std::complex<double> a1 = std::polar(ratio1, angle1);
    std::complex<double> a2 = std::polar(ratio2, angle2);

    std::complex<double> ytr;
    if (r == 0.0 && x == 0.0) {
        ytr = std::complex<double>();
    } else {
        ytr = std::complex<double>(1) / std::complex<double>(r, x);
    }

    BranchAdmittanceMatrix branchAdmittance;

    branchAdmittance.y11 = (ytr + ysh1) / (std::conj(a1) * a1);
    branchAdmittance.y12 = (- ytr) / (std::conj(a1) * a2);
    branchAdmittance.y21 = (- ytr) / (std::conj(a2) * a1);
    branchAdmittance.y22 = (ytr + ysh2) / (std::conj(a2) * a2);

    return branchAdmittance;
}

BranchAdmittanceMatrix::Flow flowBothEnds(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22,
                  double u1, double theta1, double u2, double theta2) {

    std::complex<double> v1 = std::polar(u1, theta1);
    std::complex<double> v2 = std::polar(u2, theta2);

    return flowBothEnds(y11, y12, y21, y22, v1, v2);
}

BranchAdmittanceMatrix::Flow flowBothEnds(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22,
                                          const std::complex<double>& v1, const std::complex<double>& v2) {
    BranchAdmittanceMatrix::Flow flow;
    std::complex<double> ift = y12 * v2 + y11 * v1;
    flow.fromTo = std::conj(ift) * v1;

    std::complex<double> itf = y21 * v1 + y22 * v2;
    flow.toFrom = std::conj(itf) * v2;

    return flow;
}

std::complex<double> flowYshunt(const std::complex<double>& ysh, double u, double theta) {
    std::complex<double> v = std::polar(u, theta);

    return std::conj(ysh) * std::conj(v) * v;
}

double getFixedX(double x, double epsilonX, bool applyReactanceCorrection) {
    return std::abs(x) < epsilonX && applyReactanceCorrection ? epsilonX : x;
}

double getPhaseAngleClockDegrees(int phaseAngleClock) {
    double phaseAngleClockDegree = 0.0;
    phaseAngleClockDegree += phaseAngleClock * 30.0;
    phaseAngleClockDegree = std::fmod(phaseAngleClockDegree, 360.0);
    if (phaseAngleClockDegree > 180.0) {
        phaseAngleClockDegree -= 360.0;
    }
    return phaseAngleClockDegree;
}

std::complex<double> kronAntenna(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22, bool isOpenFrom) {
    std::complex<double> ysh;

    if (isOpenFrom) {
        if (y11 != std::complex<double>()) {
    ysh = y22 - (y21 * y12 / y11);
}
    } else {
        if (y22 != std::complex<double>()) {
            ysh = y11 - (y12 * y21 / y22);
        }
    }
    return ysh;
}

BranchAdmittanceMatrix kronChain(const std::complex<double>& yFirstConnected11, const std::complex<double>& yFirstConnected12,
                                 const std::complex<double>& yFirstConnected21, const std::complex<double>& yFirstConnected22, const std::complex<double>& ySecondConnected11,
                                 const std::complex<double>& ySecondConnected12, const std::complex<double>& ySecondConnected21, const std::complex<double>& ySecondConnected22) {
    BranchAdmittanceMatrix admittance;

    admittance.y11 = (yFirstConnected11 - (yFirstConnected21 * yFirstConnected12) / (yFirstConnected22 + ySecondConnected22));
    admittance.y12 = ySecondConnected21 * yFirstConnected12 / (- (yFirstConnected22 + ySecondConnected22));
    admittance.y21 = yFirstConnected21 * ySecondConnected12 / (- (yFirstConnected22 + ySecondConnected22));
    admittance.y22 = ySecondConnected11 - (ySecondConnected21 * ySecondConnected12 / (yFirstConnected22 + ySecondConnected22));

    return admittance;
}

}  // namespace LinkData

}  // namespace iidm

}  // namespace powsybl
