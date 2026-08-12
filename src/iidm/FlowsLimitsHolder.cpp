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

void FlowsLimitsHolder::addSelectedOperationalLimitsGroupsByPredicate(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate) {
    std::list<std::string> groupsIds;
    for(const auto& olg : getOperationalLimitsGroups()) {
        if(operationalLimitsGroupIdPredicate(olg.getId())) {
            groupsIds.push_back(olg.getId());
        }
    }

    addSelectedOperationalLimitsGroups(groupsIds);
}

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

stdcxx::const_range<ActivePowerLimits> FlowsLimitsHolder::getAllSelectedActivePowerLimits() const {
    const auto& getLimitsOfType = [](const OperationalLimitsGroup& opl) {
        return opl.getActivePowerLimits();
    };
    const auto& hasLimits = [](const stdcxx::CReference<ActivePowerLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::CReference<ActivePowerLimits>, ActivePowerLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::range<ActivePowerLimits> FlowsLimitsHolder::getAllSelectedActivePowerLimits() {
    const auto& getLimitsOfType = [](OperationalLimitsGroup& opl) {
        return opl.getActivePowerLimits();
    };
    const auto& hasLimits = [](const stdcxx::Reference<ActivePowerLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ActivePowerLimits>, ActivePowerLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::CReference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimitsFromId(const std::string & groupId) const {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getActivePowerLimits() : stdcxx::CReference<ActivePowerLimits>();
}
stdcxx::Reference<ActivePowerLimits> FlowsLimitsHolder::getActivePowerLimitsFromId(const std::string & groupId) {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getActivePowerLimits() : stdcxx::Reference<ActivePowerLimits>();
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

stdcxx::const_range<ApparentPowerLimits> FlowsLimitsHolder::getAllSelectedApparentPowerLimits() const {
    const auto& getLimitsOfType = [](const OperationalLimitsGroup& opl) {
        return opl.getApparentPowerLimits();
    };
    const auto& hasLimits = [](const stdcxx::CReference<ApparentPowerLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::CReference<ApparentPowerLimits>, ApparentPowerLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::range<ApparentPowerLimits> FlowsLimitsHolder::getAllSelectedApparentPowerLimits() {
    const auto& getLimitsOfType = [](OperationalLimitsGroup& opl) {
        return opl.getApparentPowerLimits();
    };
    const auto& hasLimits = [](const stdcxx::Reference<ApparentPowerLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<ApparentPowerLimits>, ApparentPowerLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::CReference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimitsFromId(const std::string & groupId) const {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getApparentPowerLimits() : stdcxx::CReference<ApparentPowerLimits>();
}
stdcxx::Reference<ApparentPowerLimits> FlowsLimitsHolder::getApparentPowerLimitsFromId(const std::string & groupId) {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getApparentPowerLimits() : stdcxx::Reference<ApparentPowerLimits>();
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

stdcxx::const_range<CurrentLimits> FlowsLimitsHolder::getAllSelectedCurrentLimits() const {
    const auto& getLimitsOfType = [](const OperationalLimitsGroup& opl) {
        return opl.getCurrentLimits();
    };
    const auto& hasLimits = [](const stdcxx::CReference<CurrentLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::CReference<CurrentLimits>, CurrentLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::range<CurrentLimits> FlowsLimitsHolder::getAllSelectedCurrentLimits() {
    const auto& getLimitsOfType = [](OperationalLimitsGroup& opl) {
        return opl.getCurrentLimits();
    };
    const auto& hasLimits = [](const stdcxx::Reference<CurrentLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<CurrentLimits>, CurrentLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::CReference<CurrentLimits> FlowsLimitsHolder::getCurrentLimitsFromId(const std::string & groupId) const {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getCurrentLimits() : stdcxx::CReference<CurrentLimits>();
}
stdcxx::Reference<CurrentLimits> FlowsLimitsHolder::getCurrentLimitsFromId(const std::string & groupId) {
    auto opl = getOperationalLimitsGroup(groupId);
    return static_cast<bool>(opl) ? opl.get().getCurrentLimits() : stdcxx::Reference<CurrentLimits>();
}

stdcxx::const_range<LoadingLimits> FlowsLimitsHolder::getAllSelectedLoadingLimits(const LimitType& type) const {
    const auto& getLimitsOfType = [type](const OperationalLimitsGroup& opl) {
        return opl.getOperationalLimits<LoadingLimits>(type);
    };
    const auto& hasLimits = [](const stdcxx::CReference<LoadingLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::CReference<LoadingLimits>, LoadingLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
}
stdcxx::range<LoadingLimits> FlowsLimitsHolder::getAllSelectedLoadingLimits(const LimitType& type) {
    const auto& getLimitsOfType = [type](OperationalLimitsGroup& opl) {
        return opl.getOperationalLimits<LoadingLimits>(type);
    };
    const auto& hasLimits = [](const stdcxx::Reference<LoadingLimits>& ref) {
        return static_cast<bool>(ref);
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<LoadingLimits>, LoadingLimits>;

    return getAllSelectedOperationalLimitsGroups() | boost::adaptors::transformed(getLimitsOfType)
                                                   | boost::adaptors::filtered(hasLimits)
                                                   | boost::adaptors::transformed(mapper);
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
