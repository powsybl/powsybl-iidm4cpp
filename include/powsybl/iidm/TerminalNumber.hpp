/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINALNUMBER_HPP
#define POWSYBL_IIDM_TERMINALNUMBER_HPP

#include <ostream>
#include <cstdint>

namespace powsybl {

namespace iidm {

enum class TerminalNumber : std::uint8_t {
    UNDEFINED = 0,
    ONE = 1,
    TWO = 2
};

std::ostream& operator<<(std::ostream& stream, const TerminalNumber& num);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINALNUMBER_HPP
