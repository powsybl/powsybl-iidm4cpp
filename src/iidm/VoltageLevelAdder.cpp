/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

#include "BusBreakerVoltageLevel.hpp"
#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

VoltageLevelAdder::VoltageLevelAdder(Substation& substation) :
    m_substation(substation) {
}

VoltageLevel& VoltageLevelAdder::add() {
    // TODO(thiebarr) : check that there are not another voltage level with same base voltage
    checkNominalVoltage(*this, m_nominalVoltage);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
    checkOptional(*this, m_topologyKind, "TopologyKind is not set");

    std::unique_ptr<VoltageLevel> ptrVoltageLevel;
    switch (*m_topologyKind) {
        case TopologyKind::NODE_BREAKER:
            ptrVoltageLevel = stdcxx::make_unique<NodeBreakerVoltageLevel>(checkAndGetUniqueId(), getName(), m_substation, m_nominalVoltage, m_lowVoltageLimit, m_highVoltageLimit);
            break;

        case TopologyKind::BUS_BREAKER:
            ptrVoltageLevel = stdcxx::make_unique<BusBreakerVoltageLevel>(checkAndGetUniqueId(), getName(), m_substation, m_nominalVoltage, m_lowVoltageLimit, m_highVoltageLimit);
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected TopologyKind value: %1%", *m_topologyKind));
    }

    auto& voltageLevel = getNetwork().checkAndAdd<VoltageLevel>(std::move(ptrVoltageLevel));
    m_substation.addVoltageLevel(voltageLevel);

    return voltageLevel;
}

const Network& VoltageLevelAdder::getNetwork() const {
    return m_substation.getNetwork();
}

Network& VoltageLevelAdder::getNetwork() {
    return m_substation.getNetwork();
}

const std::string& VoltageLevelAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Voltage level";

    return s_typeDescription;
}

VoltageLevelAdder& VoltageLevelAdder::setHighVoltageLimit(double highVoltageLimit) {
    m_highVoltageLimit = highVoltageLimit;
    return *this;
}

VoltageLevelAdder& VoltageLevelAdder::setLowVoltageLimit(double lowVoltageLimit) {
    m_lowVoltageLimit = lowVoltageLimit;
    return *this;
}

VoltageLevelAdder& VoltageLevelAdder::setNominalVoltage(double nominalVoltage) {
    m_nominalVoltage = nominalVoltage;
    return *this;
}

VoltageLevelAdder& VoltageLevelAdder::setTopologyKind(const TopologyKind& topologyKind) {
    m_topologyKind = topologyKind;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
