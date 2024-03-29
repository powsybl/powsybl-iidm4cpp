/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_LINKDATA_HPP
#define POWSYBL_IIDM_UTIL_LINKDATA_HPP

#include <complex>

namespace powsybl {

namespace iidm {

namespace LinkData {

struct Flow {
public:
    std::complex<double> fromTo;

    std::complex<double> toFrom;
};

struct BranchAdmittanceMatrix {
public:
    std::complex<double> y11;

    std::complex<double> y12;

    std::complex<double> y21;

    std::complex<double> y22;
};

BranchAdmittanceMatrix calculateBranchAdmittance(double r, double x, double ratio1, double angle1,
                                                 double ratio2, double angle2, const std::complex<double>& ysh1, const std::complex<double>& ysh2);

}  // namespace LinkData

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_LINKDATA_HPP
