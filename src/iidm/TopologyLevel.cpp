/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyLevel.hpp>

#include <array>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

const std::array<std::string, 3>& getTopologyLevelNames() {
    static std::array<std::string, 3> s_topologyLevelNames {{
        "NODE_BREAKER",
        "BUS_BREAKER",
        "BUS_BRANCH"
    }};
    return s_topologyLevelNames;
}

}  // namespace iidm

namespace logging {

/**
 * toString template specialization for TopologyLevel
 */
template <>
std::string toString(const iidm::TopologyLevel& value) {
    return toString(iidm::getTopologyLevelNames(), value);
}

}  // namespace logging

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
            return TopologyKind::BUS_BREAKER;

        case TopologyLevel::BUS_BRANCH:
        default:
            throw PowsyblException(logging::format("No topology kind associated to topology level %1%", topologyLevel));
    }
}

std::ostream& operator<<(std::ostream& stream, const TopologyLevel& topologyLevel) {
    stream << logging::toString(topologyLevel);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
