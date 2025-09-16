/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageAngleLimitAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

// class VoltageAngleLimitAdder : public OperationalLimitsAdder<VoltageAngleLimitAdder> {

VoltageAngleLimitAdder::VoltageAngleLimitAdder(Network& network) :
    m_network(network) {
}

VoltageAngleLimitAdder& VoltageAngleLimitAdder::setId(const std::string& id) {
    m_id = id;
    return *this;
}

VoltageAngleLimitAdder& VoltageAngleLimitAdder::from(const stdcxx::Reference<Terminal>& terminal) {
    m_terminalFrom = terminal;
    return *this;
}
VoltageAngleLimitAdder& VoltageAngleLimitAdder::to(const stdcxx::Reference<Terminal>& terminal) {
    m_terminalTo = terminal;
    return *this;
}

VoltageAngleLimitAdder& VoltageAngleLimitAdder::setLowLimit(double lowLimit) {
    m_lowLimit = lowLimit;
    return *this;
}
VoltageAngleLimitAdder& VoltageAngleLimitAdder::setHighLimit(double highLimit) {
    m_highLimit = highLimit;
    return *this;
}

VoltageAngleLimit& VoltageAngleLimitAdder::add() {

    checkNotEmpty(m_id, "Voltage angle limit id is mandatory.");
    if (m_network.getVoltageAngleLimitsIndex().find(m_id) != m_network.getVoltageAngleLimitsIndex().end()) {
        throw PowsyblException(stdcxx::format("The network %1% already contains a voltage angle limit with the id '%2%'", m_network.getId(), m_id));
    }
    if (!std::isnan(m_lowLimit) && !std::isnan(m_highLimit) && m_lowLimit >= m_highLimit) {
        throw PowsyblException("Voltage angle low limit must be lower than the high limit.");
    }
    if(!m_terminalFrom || !m_terminalTo) {
        throw PowsyblException("Voltage angle limit must be connected to terminals.");
    }

    Terminal& terminalFrom = m_terminalFrom;
    Terminal& terminalTo = m_terminalTo;

    auto pairRslt = m_network.getVoltageAngleLimitsIndex().emplace(m_id, VoltageAngleLimit(m_id,
        terminalFrom, terminalTo, m_lowLimit, m_highLimit));

    return pairRslt.first->second;
}

}  // namespace iidm

}  // namespace powsybl
