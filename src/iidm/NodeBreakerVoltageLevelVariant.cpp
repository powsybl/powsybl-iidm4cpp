/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelVariant.hpp"

#include <powsybl/stdcxx/make_unique.hpp>

#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

VariantImpl::VariantImpl(NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_calculatedBusBreakerTopology(voltageLevel),
    m_calculatedBusTopology(voltageLevel) {

}

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_voltageLevel);
}

CalculatedBusBreakerTopology& VariantImpl::getCalculatedBusBreakerTopology() {
    return m_calculatedBusBreakerTopology;
}

CalculatedBusTopology& VariantImpl::getCalculatedBusTopology() {
    return m_calculatedBusTopology;
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
