/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/InternalConnectionAdder.hpp>

#include "NodeBreakerVoltageLevel.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_view {

InternalConnectionAdder::InternalConnectionAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

void InternalConnectionAdder::add() {
    checkOptional(m_voltageLevel, m_node1, "first connection node is not set");
    checkOptional(m_voltageLevel, m_node2, "second connection node is not set");

    auto& voltageLevel = dynamic_cast<NodeBreakerVoltageLevel&>(m_voltageLevel);
    voltageLevel.addInternalConnection(*m_node1, *m_node2);
}

InternalConnectionAdder& InternalConnectionAdder::setNode1(unsigned long node1) {
    m_node1 = node1;
    return *this;
}

InternalConnectionAdder& InternalConnectionAdder::setNode2(unsigned long node2) {
    m_node2 = node2;
    return *this;
}

}  // namespace node_breaker_view

}  // namespace iidm

}  // namespace powsybl
