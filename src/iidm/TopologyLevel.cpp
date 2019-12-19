/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyLevel.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

TopologyLevel getMinTopologyLevel(const TopologyKind& topologyKind, const TopologyLevel& topologyLevel) {
    const TopologyLevel& level = topologyKind == TopologyKind::NODE_BREAKER ? TopologyLevel::NODE_BREAKER : TopologyLevel::BUS_BREAKER;
    return std::max(level, topologyLevel);
}

TopologyKind getTopologyKind(const TopologyLevel& topologyLevel) {
    switch (topologyLevel) {
        case TopologyLevel::NODE_BREAKER:
            return TopologyKind::NODE_BREAKER;

        case TopologyLevel::BUS_BREAKER:
        case TopologyLevel::BUS_BRANCH:
            return TopologyKind::BUS_BREAKER;

        default:
            throw PowsyblException(logging::format("No topology kind associated to topology level %1%", topologyLevel));
    }
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<TopologyLevel>() {
    static std::initializer_list<std::string> s_topologyLevelNames {
        "NODE_BREAKER",
        "BUS_BREAKER",
        "BUS_BRANCH"
    };
    return s_topologyLevelNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
