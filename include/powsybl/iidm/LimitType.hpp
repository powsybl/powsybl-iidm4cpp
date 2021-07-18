/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LIMITTYPE_HPP
#define POWSYBL_IIDM_LIMITTYPE_HPP

namespace powsybl {

namespace iidm {

enum class LimitType : unsigned char {
    ACTIVE_POWER,
    APPARENT_POWER,
    CURRENT,
    VOLTAGE
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LIMITTYPE_HPP
