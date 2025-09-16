/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#include <powsybl/iidm/VoltageAngleLimit.hpp>

#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

// class OperationalLimitsOwner;

VoltageAngleLimit::VoltageAngleLimit(const std::string& id, Terminal& terminalFrom, Terminal& terminalTo, double lowLimit, double highLimit) :
    m_id(id),
    m_fromTerminal(terminalFrom),
    m_toTerminal(terminalTo),
    m_lowLimit(lowLimit),
    m_highLimit(highLimit) {
}

const LimitType& VoltageAngleLimit::getLimitType() const {
    static LimitType s_type = LimitType::VOLTAGE_ANGLE;
    return s_type;
}
void VoltageAngleLimit::remove() {
    //do nothing
}

const std::string& VoltageAngleLimit::getId() const {
    return m_id;
}

const Terminal& VoltageAngleLimit::getTerminalFrom() const {
    return m_fromTerminal.get(); 
}
Terminal& VoltageAngleLimit::getTerminalFrom() {
    return m_fromTerminal.get(); 
}

const Terminal& VoltageAngleLimit::getTerminalTo() const {
    return m_toTerminal.get(); 
}
Terminal& VoltageAngleLimit::getTerminalTo() {
    return m_toTerminal.get(); 
}

double VoltageAngleLimit::getLowLimit() const {
    return m_lowLimit;
}
double VoltageAngleLimit::getHighLimit() const {
    return m_highLimit;
}

}  // namespace iidm

}  // namespace powsybl
