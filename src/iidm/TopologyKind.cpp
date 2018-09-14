/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyKind.hpp>

#include <array>

#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

/**
 * toString template specialization for TopologyKind
 */
template <>
std::string toString(const iidm::TopologyKind& value) {
    static std::array<std::string, 2> s_topologyKindNames {{
                                                               "NODE_BREAKER",
                                                               "BUS_BREAKER"
                                                           }};

    return s_topologyKindNames.at(static_cast<unsigned int>(value));
}

}  // namespace logging

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const iidm::TopologyKind& topologyKind) {
    stream << logging::toString(topologyKind);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
