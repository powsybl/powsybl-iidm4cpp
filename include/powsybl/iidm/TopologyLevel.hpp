/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TOPOLOGYLEVEL_HPP
#define POWSYBL_IIDM_TOPOLOGYLEVEL_HPP

#include <iosfwd>

#include <powsybl/iidm/TopologyKind.hpp>

namespace powsybl {

namespace iidm {

enum class TopologyLevel {
    NODE_BREAKER,
    BUS_BREAKER,
    BUS_BRANCH
};

TopologyLevel getMinTopologyLevel(const TopologyKind& topologyKind, const TopologyLevel& topologyLevel);

TopologyKind getTopologyKind(const TopologyLevel& topologyLevel);

std::ostream& operator<<(std::ostream& stream, const TopologyLevel& topologyLevel);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TOPOLOGYLEVEL_HPP
