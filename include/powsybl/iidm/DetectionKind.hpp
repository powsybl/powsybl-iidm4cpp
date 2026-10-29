/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DETECTIONKIND_HPP
#define POWSYBL_IIDM_DETECTIONKIND_HPP

#include <cstdint>

namespace powsybl {

namespace iidm {

enum class DetectionKind : std::uint8_t {
    HIGH,
    LOW
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DETECTIONKIND_HPP
