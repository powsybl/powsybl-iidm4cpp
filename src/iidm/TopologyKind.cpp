/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyKind.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
IIDM_DECLSPEC const std::initializer_list<std::string>& getNames<TopologyKind>() {
    static std::initializer_list<std::string> s_topologyKindNames {
        "NODE_BREAKER",
        "BUS_BREAKER"
    };
    return s_topologyKindNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
