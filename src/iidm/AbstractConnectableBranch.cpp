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

const FlowsLimitsHolder& AbstractConnectableBranch::getOperationalLimitsHolder1() const {
    return m_operationalLimitsHolder1;
}
FlowsLimitsHolder& AbstractConnectableBranch::getOperationalLimitsHolder1()  {
    return m_operationalLimitsHolder1;
}
const FlowsLimitsHolder& AbstractConnectableBranch::getOperationalLimitsHolder2() const {
    return m_operationalLimitsHolder2;
}
FlowsLimitsHolder& AbstractConnectableBranch::getOperationalLimitsHolder2() {
    return m_operationalLimitsHolder2;
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
        for (const auto& propertyName : copyLimitsGroup.getPropertyNames()) {
            newLimitsGroup1.setProperty(propertyName, copyLimitsGroup.getProperty(propertyName));
        }
    }
    cancelSelectedOperationalLimitsGroup1();
    addSelectedOperationalLimitsGroups1(copiedBranch.getAllSelectedOperationalLimitsGroupIds1());

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
        for (const auto& propertyName : copyLimitsGroup.getPropertyNames()) {
            newLimitsGroup2.setProperty(propertyName, copyLimitsGroup.getProperty(propertyName));
        }
    }
    cancelSelectedOperationalLimitsGroup2();
    addSelectedOperationalLimitsGroups2(copiedBranch.getAllSelectedOperationalLimitsGroupIds2());

}


}  // namespace iidm

}  // namespace powsybl
