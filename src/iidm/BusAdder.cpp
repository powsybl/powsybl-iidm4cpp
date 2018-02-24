/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusAdder.hpp"
#include "ConfiguredBus.hpp"

namespace powsybl {

namespace iidm {

BusAdder::BusAdder(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

Bus& BusAdder::add() {
    std::unique_ptr<ConfiguredBus> ptrBus = stdcxx::make_unique<ConfiguredBus>(m_id, m_voltageLevel);
    return m_voltageLevel.addBus(std::move(ptrBus));
}

Network& BusAdder::getNetwork() {
    return m_voltageLevel.getNetwork();
}

const std::string& BusAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Bus";

    return s_typeDescription;
}

}

}
