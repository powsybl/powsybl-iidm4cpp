/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractConnectableBranch.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

AbstractConnectableBranch::AbstractConnectableBranch() :
    m_operationalLimitsHolder1(*this, "limits1"),
    m_operationalLimitsHolder2(*this, "limits2") {
}

stdcxx::CReference<ActivePowerLimits> AbstractConnectableBranch::getActivePowerLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> AbstractConnectableBranch::getActivePowerLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ActivePowerLimits> AbstractConnectableBranch::getActivePowerLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> AbstractConnectableBranch::getActivePowerLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ApparentPowerLimits> AbstractConnectableBranch::getApparentPowerLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> AbstractConnectableBranch::getApparentPowerLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::CReference<ApparentPowerLimits> AbstractConnectableBranch::getApparentPowerLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> AbstractConnectableBranch::getApparentPowerLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}


stdcxx::CReference<CurrentLimits> AbstractConnectableBranch::getCurrentLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> AbstractConnectableBranch::getCurrentLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::CReference<CurrentLimits> AbstractConnectableBranch::getCurrentLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> AbstractConnectableBranch::getCurrentLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

const Terminal& AbstractConnectableBranch::getTerminal1() const {
    return getTerminals().at(0).get();
}

Terminal& AbstractConnectableBranch::getTerminal1() {
    return getTerminals().at(0).get();
}

const Terminal& AbstractConnectableBranch::getTerminal2() const {
    return getTerminals().at(1).get();
}

Terminal& AbstractConnectableBranch::getTerminal2() {
    return getTerminals().at(1).get();
}

ActivePowerLimitsAdder AbstractConnectableBranch::newActivePowerLimits1() {
    return m_operationalLimitsHolder1.newActivePowerLimits();
}

ActivePowerLimitsAdder AbstractConnectableBranch::newActivePowerLimits2() {
    return m_operationalLimitsHolder2.newActivePowerLimits();
}

ApparentPowerLimitsAdder AbstractConnectableBranch::newApparentPowerLimits1() {
    return m_operationalLimitsHolder1.newApparentPowerLimits();
}

ApparentPowerLimitsAdder AbstractConnectableBranch::newApparentPowerLimits2() {
    return m_operationalLimitsHolder2.newApparentPowerLimits();
}

CurrentLimitsAdder AbstractConnectableBranch::newCurrentLimits1() {
    return m_operationalLimitsHolder1.newCurrentLimits();
}

CurrentLimitsAdder AbstractConnectableBranch::newCurrentLimits2() {
    return m_operationalLimitsHolder2.newCurrentLimits();
}


}  // namespace iidm

}  // namespace powsybl
