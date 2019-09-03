/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SwitchKind.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

const std::array<std::string, 3>& getSwitchKindNames() {
    static std::array<std::string, 3> s_switchKindNames {{
        "BREAKER",
        "DISCONNECTOR",
        "LOAD_BREAK_SWITCH"
    }};
    return s_switchKindNames;
}

SwitchKind getSwitchKind(const std::string& switchKindName) {
    const auto& names = getSwitchKindNames();
    const auto& it = std::find(names.cbegin(), names.cend(), switchKindName);
    if (it == names.cend()) {
        throw PowsyblException(logging::format("Unable to retrieve switch kind from '%1%'", switchKindName));
    }
    return static_cast<SwitchKind>(it - names.cbegin());
}

std::string getSwitchKindName(const SwitchKind& switchKind) {
    return logging::toString(getSwitchKindNames(), switchKind);
}

}  // namespace iidm

}  // namespace powsybl

