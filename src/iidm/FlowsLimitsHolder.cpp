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
#include <powsybl/iidm/util/LoadingLimitsUtil.hpp>

namespace powsybl {

namespace iidm {

stdcxx::CReference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimits() const {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getActivePowerLimits();
    }
    return stdcxx::CReference<ActivePowerLimits>();
}

stdcxx::Reference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimits() {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getActivePowerLimits();
    }
    return stdcxx::Reference<ActivePowerLimits>();
}

stdcxx::CReference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimits() const {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getApparentPowerLimits();
    }
    return stdcxx::CReference<ApparentPowerLimits>();
}

stdcxx::Reference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimits() {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getApparentPowerLimits();
    }
    return stdcxx::Reference<ApparentPowerLimits>();
}

stdcxx::CReference<CurrentLimits> FlowsLimitsHolder::getCurrentLimits() const {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getCurrentLimits();
    }
    return stdcxx::CReference<CurrentLimits>();
}

stdcxx::Reference<CurrentLimits> FlowsLimitsHolder::getCurrentLimits() {
    auto selectedLimitsGroup = getSelectedOperationalLimitsGroup();
    if(static_cast<bool>(selectedLimitsGroup)) {
        return selectedLimitsGroup.get().getCurrentLimits();
    }
    return stdcxx::Reference<CurrentLimits>();
}

ActivePowerLimitsAdder FlowsLimitsHolder::newActivePowerLimits(const ActivePowerLimits& activePowerLimits) {
    ActivePowerLimitsAdder adder = newActivePowerLimits();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, activePowerLimits);
    return adder;
}

ApparentPowerLimitsAdder FlowsLimitsHolder::newApparentPowerLimits(const ApparentPowerLimits& apparentPowerLimits) {
    ApparentPowerLimitsAdder adder = newApparentPowerLimits();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, apparentPowerLimits);
    return adder;
}

CurrentLimitsAdder FlowsLimitsHolder::newCurrentLimits(const CurrentLimits& currentLimits) {
    CurrentLimitsAdder adder = newCurrentLimits();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, currentLimits);
    return adder;
}

OperationalLimitsGroup& FlowsLimitsHolder::getOrCreateSelectedOperationalLimitsGroup(const std::string& id) {
    stdcxx::Reference<OperationalLimitsGroup> refLimitsGroup = getOperationalLimitsGroup(id);
    OperationalLimitsGroup& limitsGroup = static_cast<bool>(refLimitsGroup) ? refLimitsGroup.get() : newOperationalLimitsGroup(id);
    setSelectedOperationalLimitsGroup(id);
    return limitsGroup;
}

}  // namespace iidm

}  // namespace powsybl
