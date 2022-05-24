/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/LinkData.hpp>

#include <powsybl/math/ComplexUtils.hpp>

namespace powsybl {

namespace iidm {

namespace LinkData {

BranchAdmittanceMatrix::BranchAdmittanceMatrix(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, 
                                               const std::complex<double>& y22) :
    y11(y11),
    y12(y12),
    y21(y21),
    y22(y22) {
}

BranchAdmittanceMatrix calculateBranchAdmittance(double r, double x, double ratio1, double angle1,
                                                 double ratio2, double angle2, const std::complex<double>& ysh1, const std::complex<double>& ysh2) {
    std::complex<double> a1 = std::polar(ratio1, angle1);
    std::complex<double> a2 = std::polar(ratio2, angle2);

    std::complex<double> ytr;
    if (r != 0.0 || x != 0.0) {
        ytr = std::complex<double>(1) / std::complex<double>(r, x);
    }

    BranchAdmittanceMatrix branchAdmittance;

    branchAdmittance.y11 = (ytr + ysh1) / (std::conj(a1) * a1);
    branchAdmittance.y12 = (- ytr) / (std::conj(a1) * a2);
    branchAdmittance.y21 = (- ytr) / (std::conj(a2) * a1);
    branchAdmittance.y22 = (ytr + ysh2) / (std::conj(a2) * a2);

    return branchAdmittance;
}

Flow flowBothEnds(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22, double u1, 
                  double theta1, double u2, double theta2) {

    const std::complex<double>& v1 = math::ComplexUtils::polar2Complex(u1, theta1);
    const std::complex<double>& v2 = math::ComplexUtils::polar2Complex(u2, theta2);

    return flowBothEnds(y11, y12, y21, y22, v1, v2);
}

Flow flowBothEnds(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22, 
                  const std::complex<double>& v1, const std::complex<double>& v2) {

    Flow flow;
    const std::complex<double>& ift = y12 * v2 + y11 * v1;
    flow.fromTo = std::conj(ift) * v1;

    const std::complex<double>& itf = y21 * v1 + y22 * v2;
    flow.toFrom = std::conj(itf) * v2;

    return flow;
}

std::complex<double> flowYshunt(const std::complex<double>& ysh, double u, double theta) {
    std::complex<double> v = math::ComplexUtils::polar2Complex(u, theta);

    return std::conj(ysh) * std::conj(v) * v;
}

double getFixedX(double x, double epsilonX, bool applyReactanceCorrection) {
    return std::abs(x) < epsilonX && applyReactanceCorrection ? epsilonX : x;
}

double getPhaseAngleClockDegrees(int phaseAngleClock) {
    double phaseAngleClockDegree = std::remainder(phaseAngleClock * 30.0, 360.0);
    if (phaseAngleClockDegree > 180.0) {
        phaseAngleClockDegree -= 360.0;
    }
    return phaseAngleClockDegree;
}

std::complex<double> kronAntenna(const std::complex<double>& y11, const std::complex<double>& y12, const std::complex<double>& y21, const std::complex<double>& y22, 
                                 bool isOpenFrom) {
    std::complex<double> ysh(0.0, 0.0);

    if (isOpenFrom) {
        if (y11 != std::complex<double>(0.0, 0.0)) {
            ysh = y22 - (y21 * y12 / y11);
        }
    }
    else {
        if (y22 != std::complex<double>(0.0, 0.0)) {
            ysh = y11 - (y12 * y21 / y22);
        }
    }
    return ysh;
}

BranchAdmittanceMatrix kronChain(const BranchAdmittanceMatrix& firstAdm, const Branch::Side& firstChainNodeSide, const BranchAdmittanceMatrix& secondAdm, 
                                 const Branch::Side& secondChainNodeSide) {
    BranchAdmittanceMatrix admittance;

    std::complex<double> yFirst11;
    std::complex<double> yFirst1C;
    std::complex<double> yFirstC1;
    std::complex<double> yFirstCC;
    if (firstChainNodeSide == Branch::Side::TWO) {
        yFirst11 = firstAdm.y11;
        yFirst1C = firstAdm.y12;
        yFirstC1 = firstAdm.y21;
        yFirstCC = firstAdm.y22;
    } else {
        yFirst11 = firstAdm.y22;
        yFirst1C = firstAdm.y21;
        yFirstC1 = firstAdm.y12;
        yFirstCC = firstAdm.y11;
    }

    std::complex<double> ySecond22;
    std::complex<double> ySecond2C;
    std::complex<double> ySecondC2;
    std::complex<double> ySecondCC;
    if (secondChainNodeSide == Branch::Side::TWO) {
        ySecond22 = secondAdm.y11;
        ySecond2C = secondAdm.y12;
        ySecondC2 = secondAdm.y21;
        ySecondCC = secondAdm.y22;
    } else {
        ySecond22 = secondAdm.y22;
        ySecond2C = secondAdm.y21;
        ySecondC2 = secondAdm.y12;
        ySecondCC = secondAdm.y11;
    }

    admittance.y11 = yFirst11 - (yFirst1C * yFirstC1 / (yFirstCC + ySecondCC));
    admittance.y12 = - yFirst1C * ySecondC2 / (yFirstCC + ySecondCC);
    admittance.y21 = - ySecond2C * yFirstC1 / (yFirstCC + ySecondCC);
    admittance.y22 = ySecond22 - (ySecond2C * ySecondC2 / (yFirstCC + ySecondCC));

    return admittance;
}

}  // namespace LinkData

}  // namespace iidm

}  // namespace powsybl
