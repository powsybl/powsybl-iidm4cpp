/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELGRAPH_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELGRAPH_HPP

#include <powsybl/math/UndirectedGraph.hpp>

namespace powsybl {

namespace iidm {

class NodeTerminal;
class Switch;

namespace node_breaker_voltage_level {

typedef math::UndirectedGraph<NodeTerminal, Switch> Graph;

}  // node_breaker_voltage_level

}  // iidm

}  // powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELGRAPH_HPP
