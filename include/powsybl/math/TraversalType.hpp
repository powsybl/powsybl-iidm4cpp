/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_TRAVERSALTYPE_HPP
#define POWSYBL_MATH_TRAVERSALTYPE_HPP

#include <cstdint>

namespace powsybl {

namespace math {

enum class TraversalType : std::uint8_t {
    DEPTH_FIRST,
    BREADTH_FIRST
};

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_TRAVERSALTYPE_HPP
