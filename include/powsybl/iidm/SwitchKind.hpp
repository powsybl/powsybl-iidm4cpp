/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCHKIND_HPP
#define POWSYBL_IIDM_SWITCHKIND_HPP

namespace powsybl {

namespace iidm {

enum class SwitchKind {
    BREAKER,
    DISCONNECTOR,
    LOAD_BREAK_SWITCH
};

}

}

#endif  // POWSYBL_IIDM_SWITCHKIND_HPP
