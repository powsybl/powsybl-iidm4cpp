/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusAdder.hpp>

#include "BusBreakerTopologyModel.hpp"
#include "ConfiguredBus.hpp"

namespace powsybl {

namespace iidm {

BusAdder::BusAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

Bus& BusAdder::add() {
    return addOnTopology(m_voltageLevel.getTopologyModel<BusBreakerTopologyModel>());
}

Bus& BusAdder::addOnTopology(BusBreakerTopologyModel& topologyModel) {
    std::unique_ptr<ConfiguredBus> ptrBus = stdcxx::make_unique<ConfiguredBus>(checkAndGetUniqueId(), getName(), isFictitious(), m_voltageLevel);
    return topologyModel.addBus(std::move(ptrBus));
}

const Network& BusAdder::getNetwork() const {
    return m_voltageLevel.getNetwork();
}

Network& BusAdder::getNetwork() {
    return m_voltageLevel.getNetwork();
}

const std::string& BusAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Bus";

    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
