/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VALIDATIONLEVEL_HPP
#define POWSYBL_IIDM_VALIDATIONLEVEL_HPP

#include <cstdint>

namespace powsybl
{

namespace iidm
{

enum class ValidationLevel : std::uint8_t {
    UNVALID,
    EQUIPMENT,
    STEADY_STATE_HYPOTHESIS
};

namespace validationLevel {

inline const ValidationLevel& min(const ValidationLevel& vl1, const ValidationLevel& vl2) {
    return vl1 >= vl2 ? vl2 : vl1; 
}

}   // namespace validationLevel

}   // namespace iidm

}   // namespace powsybl

#endif  // POWSYBL_IIDM_VALIDATIONLEVEL_HPP
