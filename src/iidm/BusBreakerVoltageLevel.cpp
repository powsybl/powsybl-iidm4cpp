/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusBreakerVoltageLevel.hpp>

#include "ConfiguredBus.hpp"

namespace powsybl {

namespace iidm {

BusBreakerVoltageLevel::BusBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                                               double nominalVoltage, double lowVoltageLimit, double highVoltagelimit) :
    VoltageLevel(id, name, substation, nominalVoltage, lowVoltageLimit, highVoltagelimit) {
}

Bus& BusBreakerVoltageLevel::addBus(std::unique_ptr<ConfiguredBus>&& ptrBus) {
    ConfiguredBus& bus = getNetwork().checkAndAdd(std::move(ptrBus));

    // TODO

    return bus;
}

void BusBreakerVoltageLevel::attach(Terminal& /*terminal*/, bool /*test*/) {
    // TODO
}

void BusBreakerVoltageLevel::clean() {
    // TODO
}

bool BusBreakerVoltageLevel::connect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

void BusBreakerVoltageLevel::detach(Terminal& /*terminal*/) {
    // TODO
}

bool BusBreakerVoltageLevel::disconnect(Terminal& /*terminal*/) {
    // TODO: need topological graph
    return true;
}

const TopologyKind& BusBreakerVoltageLevel::getTopologyKind() const {
    static TopologyKind s_topologyKind = TopologyKind::BUS_BREAKER;

    return s_topologyKind;
}

void BusBreakerVoltageLevel::invalidateCache() {
    // TODO
}

}

}
