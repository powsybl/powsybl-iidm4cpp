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
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

enum class TopologyLevel : unsigned char {
    NODE_BREAKER,
    BUS_BREAKER,
    BUS_BRANCH
};

IIDM_DECLSPEC TopologyLevel getMinTopologyLevel(const TopologyKind& topologyKind, const TopologyLevel& topologyLevel);

IIDM_DECLSPEC TopologyKind getTopologyKind(const TopologyLevel& topologyLevel);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TOPOLOGYLEVEL_HPP
