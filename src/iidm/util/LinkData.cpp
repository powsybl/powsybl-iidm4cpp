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

}  // namespace LinkData

}  // namespace iidm

}  // namespace powsybl
