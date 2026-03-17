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

stdcxx::const_range<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroups1() const {
    return m_operationalLimitsHolder1.getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroups1() {
    return m_operationalLimitsHolder1.getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& AbstractConnectableBranch::getSelectedOperationalLimitsGroupId1() const {
    return m_operationalLimitsHolder1.getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroup1(const std::string& id) const {
    return m_operationalLimitsHolder1.getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroup1(const std::string& id) {
    return m_operationalLimitsHolder1.getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> AbstractConnectableBranch::getSelectedOperationalLimitsGroup1() const {
    return m_operationalLimitsHolder1.getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> AbstractConnectableBranch::getSelectedOperationalLimitsGroup1() {
    return m_operationalLimitsHolder1.getSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::newOperationalLimitsGroup1(const std::string& id) {
    return m_operationalLimitsHolder1.newOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::setSelectedOperationalLimitsGroup1(const std::string& id) {
    m_operationalLimitsHolder1.setSelectedOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::removeOperationalLimitsGroup1(const std::string& id) {
    m_operationalLimitsHolder1.removeOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::cancelSelectedOperationalLimitsGroup1() {
    m_operationalLimitsHolder1.cancelSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::getOrCreateSelectedOperationalLimitsGroup1() {
    return m_operationalLimitsHolder1.getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::getOrCreateSelectedOperationalLimitsGroup1(const std::string& id) {
    return Branch::getOrCreateSelectedOperationalLimitsGroup1(id);
}

stdcxx::const_range<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroups2() const {
    return m_operationalLimitsHolder2.getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroups2() {
    return m_operationalLimitsHolder2.getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& AbstractConnectableBranch::getSelectedOperationalLimitsGroupId2() const {
    return m_operationalLimitsHolder2.getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroup2(const std::string& id) const {
    return m_operationalLimitsHolder2.getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> AbstractConnectableBranch::getOperationalLimitsGroup2(const std::string& id) {
    return m_operationalLimitsHolder2.getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> AbstractConnectableBranch::getSelectedOperationalLimitsGroup2() const {
    return m_operationalLimitsHolder2.getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> AbstractConnectableBranch::getSelectedOperationalLimitsGroup2() {
    return m_operationalLimitsHolder2.getSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::newOperationalLimitsGroup2(const std::string& id) {
    return m_operationalLimitsHolder2.newOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::setSelectedOperationalLimitsGroup2(const std::string& id) {
    m_operationalLimitsHolder2.setSelectedOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::removeOperationalLimitsGroup2(const std::string& id) {
    m_operationalLimitsHolder2.removeOperationalLimitsGroup(id);
}
void AbstractConnectableBranch::cancelSelectedOperationalLimitsGroup2() {
    m_operationalLimitsHolder2.cancelSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::getOrCreateSelectedOperationalLimitsGroup2() {
    return m_operationalLimitsHolder2.getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& AbstractConnectableBranch::getOrCreateSelectedOperationalLimitsGroup2(const std::string& id) {
    return Branch::getOrCreateSelectedOperationalLimitsGroup2(id);
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
    return getOrCreateSelectedOperationalLimitsGroup1().newActivePowerLimits();
}

ActivePowerLimitsAdder AbstractConnectableBranch::newActivePowerLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newActivePowerLimits();
}

ApparentPowerLimitsAdder AbstractConnectableBranch::newApparentPowerLimits1() {
    return getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits();
}

ApparentPowerLimitsAdder AbstractConnectableBranch::newApparentPowerLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newApparentPowerLimits();
}

CurrentLimitsAdder AbstractConnectableBranch::newCurrentLimits1() {
    return getOrCreateSelectedOperationalLimitsGroup1().newCurrentLimits();
}

CurrentLimitsAdder AbstractConnectableBranch::newCurrentLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newCurrentLimits();
}

void AbstractConnectableBranch::copyOperationalLimits(const AbstractConnectableBranch& copiedBranch) {

    for(const auto& copyLimitsGroup : copiedBranch.getOperationalLimitsGroups1()) {
        OperationalLimitsGroup& newLimitsGroup1 = newOperationalLimitsGroup1(copyLimitsGroup.getId());

        if(static_cast<bool>(copyLimitsGroup.getCurrentLimits())) {
            newLimitsGroup1.newCurrentLimits(copyLimitsGroup.getCurrentLimits().get()).add();
        }
        if(static_cast<bool>(copyLimitsGroup.getActivePowerLimits())) {
            newLimitsGroup1.newActivePowerLimits(copyLimitsGroup.getActivePowerLimits().get()).add();
        }
        if(static_cast<bool>(copyLimitsGroup.getApparentPowerLimits())) {
            newLimitsGroup1.newApparentPowerLimits(copyLimitsGroup.getApparentPowerLimits().get()).add();
        }
    }
    if(copiedBranch.getSelectedOperationalLimitsGroupId1().has_value()) {
        setSelectedOperationalLimitsGroup1(copiedBranch.getSelectedOperationalLimitsGroupId1().get());
    }

    for(const auto& copyLimitsGroup : copiedBranch.getOperationalLimitsGroups2()) {
        OperationalLimitsGroup& newLimitsGroup2 = newOperationalLimitsGroup2(copyLimitsGroup.getId());

        if(static_cast<bool>(copyLimitsGroup.getCurrentLimits())) {
            newLimitsGroup2.newCurrentLimits(copyLimitsGroup.getCurrentLimits().get()).add();
        }
        if(static_cast<bool>(copyLimitsGroup.getActivePowerLimits())) {
            newLimitsGroup2.newActivePowerLimits(copyLimitsGroup.getActivePowerLimits().get()).add();
        }
        if(static_cast<bool>(copyLimitsGroup.getApparentPowerLimits())) {
            newLimitsGroup2.newApparentPowerLimits(copyLimitsGroup.getApparentPowerLimits().get()).add();
        }
    }
    if(copiedBranch.getSelectedOperationalLimitsGroupId2().has_value()) {
        setSelectedOperationalLimitsGroup2(copiedBranch.getSelectedOperationalLimitsGroupId2().get());
    }

}


}  // namespace iidm

}  // namespace powsybl
