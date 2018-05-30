/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MATH_HPP
#define POWSYBL_STDCXX_MATH_HPP

#include <limits>

namespace stdcxx {

template <typename T = double>
inline T nan() {
    return std::numeric_limits<T>::quiet_NaN();
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_MATH_HPP
