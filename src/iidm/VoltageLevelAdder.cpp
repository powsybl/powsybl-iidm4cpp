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

VoltageLevelAdder::VoltageLevelAdder(Network& network) :
    m_network(network) {
}

VoltageLevelAdder::VoltageLevelAdder(Substation& substation) :
    m_substation(substation) {
}

VoltageLevel& VoltageLevelAdder::add() {
    // TODO(thiebarr) : check that there are not another voltage level with same base voltage
    checkNominalVoltage(*this, m_nominalV);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
    checkOptional(*this, m_topologyKind, "TopologyKind is not set");

    stdcxx::Reference<VoltageLevel> voltageLevel;
    switch (*m_topologyKind) {
        case TopologyKind::NODE_BREAKER:
            voltageLevel = stdcxx::ref<VoltageLevel>(getNetwork().checkAndAdd<NodeBreakerVoltageLevel>(
                stdcxx::make_unique<NodeBreakerVoltageLevel>(checkAndGetUniqueId(), getName(), isFictitious(), m_substation, m_network, m_nominalV, m_lowVoltageLimit, m_highVoltageLimit)));
            break;

        case TopologyKind::BUS_BREAKER:
            voltageLevel = stdcxx::ref<VoltageLevel>(getNetwork().checkAndAdd<BusBreakerVoltageLevel>(
                stdcxx::make_unique<BusBreakerVoltageLevel>(checkAndGetUniqueId(), getName(), isFictitious(), m_substation, m_network, m_nominalV, m_lowVoltageLimit, m_highVoltageLimit)));
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected TopologyKind value: %1%", *m_topologyKind));
    }

    if (static_cast<bool>(m_substation)) {
        m_substation.get().addVoltageLevel(voltageLevel);
    }
    return voltageLevel;
}

const Network& VoltageLevelAdder::getNetwork() const {
    if (static_cast<bool>(m_network)) {
        return m_network.get();
    }
    if (static_cast<bool>(m_substation)) {
        return m_substation.get().getNetwork();
    }
    throw PowsyblException("Voltage level has no container");
}

Network& VoltageLevelAdder::getNetwork() {
    return const_cast<Network&>(static_cast<const VoltageLevelAdder*>(this)->getNetwork());
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

VoltageLevelAdder& VoltageLevelAdder::setNominalV(double nominalV) {
    m_nominalV = nominalV;
    return *this;
}

VoltageLevelAdder& VoltageLevelAdder::setTopologyKind(const TopologyKind& topologyKind) {
    m_topologyKind = topologyKind;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
