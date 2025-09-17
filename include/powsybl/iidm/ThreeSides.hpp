/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREESIDES_HPP
#define POWSYBL_IIDM_THREESIDES_HPP

#include <ostream>
#include <cstdint>

namespace powsybl {

namespace iidm {

enum class ThreeSides : std::uint8_t {
    UNDEFINED = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3
};

std::ostream& operator<<(std::ostream& stream, const ThreeSides& threeSide);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREESIDES_HPP
