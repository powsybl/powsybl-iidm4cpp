/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelBusNamingStrategy.hpp"

#include <powsybl/logging/MessageFormat.hpp>

#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusNamingStrategy::BusNamingStrategy(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_counter(0) {

}

std::string BusNamingStrategy::getName() {
    std::lock_guard<std::mutex> lock(m_mutex);

    return logging::format("%1%_%2%", m_voltageLevel.getId(), (m_counter++));
}

}  // node_breaker_voltage_level

}  // iidm

}  // powsybl
