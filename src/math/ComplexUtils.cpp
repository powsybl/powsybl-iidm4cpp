/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/ComplexUtils.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace math {

namespace ComplexUtils {

std::complex<double> polar2Complex(double r, double theta) {
    if (r < 0.0) {
        throw PowsyblException(stdcxx::format("negative complex module %1%", r));
    }
    return std::complex<double>(r * std::cos(theta), r * std::sin(theta));
}

}  // namespace ComplexUtils

}  // namespace math

}  // namespace powsybl
