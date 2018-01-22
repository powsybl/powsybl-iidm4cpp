/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NodeBreakerVoltageLevel.hpp>

namespace powsybl {

namespace iidm {

NodeBreakerVoltageLevel::NodeBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                                 double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit) {
}

void NodeBreakerVoltageLevel::attach(Terminal& /*terminal*/, bool /*test*/) {
    // TODO
}

void NodeBreakerVoltageLevel::clean() {
    // TODO
}

bool NodeBreakerVoltageLevel::connect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

void NodeBreakerVoltageLevel::detach(Terminal& /*terminal*/) {
    // TODO
}

bool NodeBreakerVoltageLevel::disconnect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

const TopologyKind& NodeBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::NODE_BREAKER;

    return s_topologyKind;
}

void NodeBreakerVoltageLevel::invalidateCache() {
    // TODO
}

}

}
