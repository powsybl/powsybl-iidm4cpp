/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelState.hpp"

#include <powsybl/stdcxx/make_unique.hpp>

#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

StateImpl::StateImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

std::unique_ptr<StateImpl> StateImpl::copy() const {
    return stdcxx::make_unique<StateImpl>(m_voltageLevel);
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
