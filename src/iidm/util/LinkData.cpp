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

BranchAdmittanceMatrix kronChain(const BranchAdmittanceMatrix& firstAdm, const TwoSides& firstChainNodeSide,
                                 const BranchAdmittanceMatrix& secondAdm, const TwoSides& secondChainNodeSide) {

    BranchAdmittanceMatrix admittance;

    std::complex<double> yFirst11;
    std::complex<double> yFirst1C;
    std::complex<double> yFirstC1;
    std::complex<double> yFirstCC;
    if (firstChainNodeSide == TwoSides::TWO) {
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
    if (secondChainNodeSide == TwoSides::TWO){
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
