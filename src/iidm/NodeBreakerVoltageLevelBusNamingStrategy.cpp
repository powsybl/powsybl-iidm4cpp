/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelBusNamingStrategy.hpp"

#include <powsybl/stdcxx/format.hpp>

#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusNamingStrategy::BusNamingStrategy(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

std::string BusNamingStrategy::getId(const std::vector<unsigned long>& nodes) {
    const auto& iter = std::min_element(nodes.cbegin(), nodes.cend());
    return stdcxx::format("%1%_%2%", m_voltageLevel.getId(), *iter);
}

std::string BusNamingStrategy::getName(const std::vector<unsigned long>& nodes) {
    if (!m_voltageLevel.getOptionalName().empty()) {
        const auto& iter = std::min_element(nodes.cbegin(), nodes.cend());
        return stdcxx::format("%1%_%2%", m_voltageLevel.getOptionalName(), *iter);
    }

    return "";
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
