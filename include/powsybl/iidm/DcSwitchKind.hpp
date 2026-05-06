/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCSWITCHKIND_HPP
#define POWSYBL_IIDM_DCSWITCHKIND_HPP

#include <string>

namespace powsybl {

namespace iidm {

enum class DcSwitchKind : unsigned char {
    BREAKER,
    DISCONNECTOR
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCSWITCHKIND_HPP
