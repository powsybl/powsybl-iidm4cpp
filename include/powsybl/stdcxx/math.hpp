/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MATH_HPP
#define POWSYBL_STDCXX_MATH_HPP

#include <cmath>
#include <limits>
#include <sstream>

#include <powsybl/stdcxx/export.hpp>

namespace stdcxx {

template <typename T = double>
inline bool isEqual(const T& v1, const T& v2) {
    return !std::islessgreater(v1, v2);
}

template <typename T = double>
inline T nan() {
    return std::numeric_limits<T>::quiet_NaN();
}

template <typename T>
IIDM_DECLSPEC std::string to_string(const T& value) {
    std::ostringstream out;
    out.precision(std::numeric_limits<T>::max_digits10);
    out << value;
    return out.str();
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_MATH_HPP
