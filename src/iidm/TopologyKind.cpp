/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyKind.hpp>

#include <array>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

static const std::array<std::string, 2>& getTopologyKindNames() {
    static std::array<std::string, 2> s_topologyKindNames {{
        "NODE_BREAKER",
        "BUS_BREAKER"
    }};
    return s_topologyKindNames;
}

}  // namespace iidm

namespace logging {

/**
 * toString template specialization for TopologyKind
 */
template <>
std::string toString(const iidm::TopologyKind& value) {
    return toString(iidm::getTopologyKindNames(), value);
}

}  // namespace logging

namespace iidm {

TopologyKind getTopologyKind(const std::string& topologyKindName) {
    const auto& names = getTopologyKindNames();
    const auto& it = std::find(names.cbegin(), names.cend(), topologyKindName);
    if (it == names.cend()) {
        throw PowsyblException(logging::format("Unable to retrieve topology kind from '%1%'", topologyKindName));
    }
    return static_cast<TopologyKind>(it - names.cbegin());
}

std::string getTopologyKindName(const TopologyKind& topologyKind) {
    return logging::toString(topologyKind);
}

std::ostream& operator<<(std::ostream& stream, const iidm::TopologyKind& topologyKind) {
    stream << logging::toString(topologyKind);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
