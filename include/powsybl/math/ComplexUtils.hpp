/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_COMPLEXUTILS_HPP
#define POWSYBL_MATH_COMPLEXUTILS_HPP

#include <complex>

namespace powsybl {

namespace math {

namespace ComplexUtils {

std::complex<double> polar2Complex(double r, double theta);

}  // namespace ComplexUtils

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_COMPLEXUTILS_HPP
