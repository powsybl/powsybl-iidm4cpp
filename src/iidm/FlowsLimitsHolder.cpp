/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/FlowsLimitsHolder.hpp>

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

FlowsLimitsHolder::FlowsLimitsHolder(Identifiable& identifiable, std::string&& attributeName) :
    m_operationalLimitsHolder(identifiable, std::move(attributeName)) {
}

FlowsLimitsHolder::FlowsLimitsHolder(std::string&& attributeName) :
    m_operationalLimitsHolder(std::move(attributeName)) {
}

stdcxx::CReference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::CReference<CurrentLimits> FlowsLimitsHolder::getCurrentLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> FlowsLimitsHolder::getCurrentLimits() {
    return m_operationalLimitsHolder.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::const_range<OperationalLimits> FlowsLimitsHolder::getOperationalLimits() const {
    return m_operationalLimitsHolder.getOperationalLimits();
}

stdcxx::range<OperationalLimits> FlowsLimitsHolder::getOperationalLimits() {
    return m_operationalLimitsHolder.getOperationalLimits();
}

ActivePowerLimitsAdder FlowsLimitsHolder::newActivePowerLimits() {
    return m_operationalLimitsHolder.newActivePowerLimits();
}

ApparentPowerLimitsAdder FlowsLimitsHolder::newApparentPowerLimits() {
    return m_operationalLimitsHolder.newApparentPowerLimits();
}

CurrentLimitsAdder FlowsLimitsHolder::newCurrentLimits() {
    return m_operationalLimitsHolder.newCurrentLimits();
}

void FlowsLimitsHolder::setIdentifiable(Identifiable& identifiable) {
    m_operationalLimitsHolder.setIdentifiable(identifiable);
}

}  // namespace iidm

}  // namespace powsybl
