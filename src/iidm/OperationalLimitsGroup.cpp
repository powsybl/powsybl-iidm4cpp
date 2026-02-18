/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OperationalLimitsGroup.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

OperationalLimitsGroup::OperationalLimitsGroup(const std::string& id, Identifiable& identifiable, const std::string& attributeName, const std::string& selectedGroupId) :
    OperationalLimitsGroup(id, identifiable, attributeName) {
    m_selectedGroupId = selectedGroupId;
}

OperationalLimitsGroup::OperationalLimitsGroup(const std::string& id, Identifiable& identifiable, const std::string& attributeName) :
    m_id(id),
    m_identifiable(identifiable),
    m_attributeName(attributeName) {
}

std::string OperationalLimitsGroup::getMessageHeader() const {
    return m_identifiable.getMessageHeader();
}

const Validable& OperationalLimitsGroup::getValidable() const {
    return m_identifiable;
}

const Network& OperationalLimitsGroup::getNetwork() const {
    return m_identifiable.getNetwork();
}
Network& OperationalLimitsGroup::getNetwork() {
    return m_identifiable.getNetwork();
}

const std::string& OperationalLimitsGroup::getId() const {
    return m_id;
}

void OperationalLimitsGroup::setSelectedGroupId(const std::string& selectedGroupId) {
    if(selectedGroupId.empty()){
        cancelSelectedGroupId();
        return;
    }
    m_selectedGroupId = selectedGroupId;
}
void OperationalLimitsGroup::cancelSelectedGroupId() {
    m_selectedGroupId.reset();
}

stdcxx::CReference<ActivePowerLimits> OperationalLimitsGroup::getActivePowerLimits() const {
    return getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}
stdcxx::Reference<ActivePowerLimits> OperationalLimitsGroup::getActivePowerLimits() {
    return getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}
stdcxx::CReference<ApparentPowerLimits> OperationalLimitsGroup::getApparentPowerLimits() const {
    return getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}
stdcxx::Reference<ApparentPowerLimits> OperationalLimitsGroup::getApparentPowerLimits() {
    return getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}
stdcxx::CReference<CurrentLimits> OperationalLimitsGroup::getCurrentLimits() const {
    return getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}
stdcxx::Reference<CurrentLimits> OperationalLimitsGroup::getCurrentLimits() {
    return getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

ActivePowerLimitsAdder OperationalLimitsGroup::newActivePowerLimits() {
    return ActivePowerLimitsAdder(*this);
}
ApparentPowerLimitsAdder OperationalLimitsGroup::newApparentPowerLimits() {
    return ApparentPowerLimitsAdder(*this);
}
CurrentLimitsAdder OperationalLimitsGroup::newCurrentLimits() {
    return CurrentLimitsAdder(*this);
}
ActivePowerLimitsAdder OperationalLimitsGroup::newActivePowerLimits(const ActivePowerLimits& limits) {
    return ActivePowerLimitsAdder(*this, limits);
}
ApparentPowerLimitsAdder OperationalLimitsGroup::newApparentPowerLimits(const ApparentPowerLimits& limits) {
    return ApparentPowerLimitsAdder(*this, limits);
}
CurrentLimitsAdder OperationalLimitsGroup::newCurrentLimits(const CurrentLimits& limits) {
    return CurrentLimitsAdder(*this, limits);
}

stdcxx::Reference<OperationalLimits> OperationalLimitsGroup::setOperationalLimits(const LimitType& limitType, const std::shared_ptr<OperationalLimits>& operationalLimits) {
    if (!operationalLimits) {
        m_operationalLimits.erase(limitType);
    } else {
        m_operationalLimits[limitType] = operationalLimits;
        return stdcxx::ref(*m_operationalLimits.find(limitType)->second);
    }
    return stdcxx::ref<OperationalLimits>();
}

void OperationalLimitsGroup::removeCurrentLimits() {
    removeOperationalLimits(LimitType::CURRENT);
}
void OperationalLimitsGroup::removeActivePowerLimits() {
    removeOperationalLimits(LimitType::ACTIVE_POWER);
}
void OperationalLimitsGroup::removeApparentPowerLimits() {
    removeOperationalLimits(LimitType::APPARENT_POWER);
}
void OperationalLimitsGroup::removeOperationalLimits(const LimitType& type) {
    m_operationalLimits.erase(type);
}

bool OperationalLimitsGroup::isEmpty() const {
    return m_operationalLimits.empty();
}

}  // namespace iidm

}  // namespace powsybl
