/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OperationalLimitsGroups.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Identifiable.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

namespace powsybl {

namespace iidm {

const char* const OperationalLimitsGroups::DEFAULT_SELECTED_OPERATIONAL_LIMITS_GROUP_ID = "DEFAULT";

OperationalLimitsGroups::OperationalLimitsGroups(Identifiable& identifiable, const std::string& attributeName) :
    m_identifiable(identifiable),
    m_attributeName(attributeName) {
}

OperationalLimitsGroups::OperationalLimitsGroups(const std::string& attributeName) :
    m_attributeName(attributeName) {
}

void OperationalLimitsGroups::setIdentifiable(Identifiable& identifiable) {
    m_identifiable = identifiable;
}

stdcxx::const_range<OperationalLimitsGroup> OperationalLimitsGroups::getOperationalLimitsGroups() const {
    return boost::adaptors::values(m_operationalLimitsGroupById) | boost::adaptors::indirected;
}
stdcxx::range<OperationalLimitsGroup> OperationalLimitsGroups::getOperationalLimitsGroups() {
    return boost::adaptors::values(m_operationalLimitsGroupById) | boost::adaptors::indirected;
}

stdcxx::optional<std::string> OperationalLimitsGroups::getSelectedOperationalLimitsGroupId() const {
    return (!m_selectedLimitsGroupIds.empty()) ? m_selectedLimitsGroupIds.back() : stdcxx::optional<std::string>();
}
const std::list<std::string>& OperationalLimitsGroups::getAllSelectedOperationalLimitsGroupIds() const {
    return m_selectedLimitsGroupIds;
}

stdcxx::CReference<OperationalLimitsGroup> OperationalLimitsGroups::getOperationalLimitsGroup(const std::string& id) const {
    const auto& it = m_operationalLimitsGroupById.find(id);
    return it != m_operationalLimitsGroupById.cend() ? stdcxx::cref(*it->second) : stdcxx::CReference<OperationalLimitsGroup>();
}
stdcxx::Reference<OperationalLimitsGroup> OperationalLimitsGroups::getOperationalLimitsGroup(const std::string& id) {
    auto it = m_operationalLimitsGroupById.find(id);
    return it != m_operationalLimitsGroupById.end() ? stdcxx::ref(*it->second) : stdcxx::Reference<OperationalLimitsGroup>();
}

stdcxx::CReference<OperationalLimitsGroup> OperationalLimitsGroups::getSelectedOperationalLimitsGroup() const {
    return getSelectedOperationalLimitsGroupId().has_value() ? getOperationalLimitsGroup(getSelectedOperationalLimitsGroupId().get()) : stdcxx::CReference<OperationalLimitsGroup>();
}
stdcxx::Reference<OperationalLimitsGroup> OperationalLimitsGroups::getSelectedOperationalLimitsGroup() {
    return getSelectedOperationalLimitsGroupId().has_value() ? getOperationalLimitsGroup(getSelectedOperationalLimitsGroupId().get()) : stdcxx::Reference<OperationalLimitsGroup>();
}

stdcxx::const_range<OperationalLimitsGroup> OperationalLimitsGroups::getAllSelectedOperationalLimitsGroups() const {
    const auto& isSelected = [this](const OperationalLimitsGroup& opl) {
        return isSelectedOperationalLimitsGroup(opl.getId());
    };
    return getOperationalLimitsGroups() | boost::adaptors::filtered(isSelected);
}
stdcxx::range<OperationalLimitsGroup> OperationalLimitsGroups::getAllSelectedOperationalLimitsGroups() {
    const auto& isSelected = [this](const OperationalLimitsGroup& opl) {
        return isSelectedOperationalLimitsGroup(opl.getId());
    };
    return getOperationalLimitsGroups() | boost::adaptors::filtered(isSelected);
}

OperationalLimitsGroup& OperationalLimitsGroups::newOperationalLimitsGroup(const std::string& id) {
    if(!m_identifiable) {
        throw PowsyblException("OperationalLimitsGroup's Identifiable owner not set");
    }

    std::shared_ptr<OperationalLimitsGroup> operationalLimitsGroupPtr = std::make_shared<OperationalLimitsGroup>(id, m_identifiable, m_attributeName);

    m_operationalLimitsGroupById[id] = operationalLimitsGroupPtr;
    return *m_operationalLimitsGroupById.find(id)->second;
}

void OperationalLimitsGroups::setSelectedOperationalLimitsGroup(const std::string& id) {
    if(id.empty()){
        return;
    }
    if(!getOperationalLimitsGroup(id)) {
        throw PowsyblException(stdcxx::format("No operational limits group with given id (%1%) exists, can't be assigned as selected", id));
    }

    //Unselect everyone
    m_selectedLimitsGroupIds.clear();
    //Select only the given id
    m_selectedLimitsGroupIds.push_back(id);
}

void OperationalLimitsGroups::addSelectedOperationalLimitsGroups(const std::list<std::string>& ids) {
    if(ids.empty()){
        return;
    }

    bool idIsNull = false;
    std::list<std::string> nonExistingGroup;
    logging::Logger& logger = logging::LoggerFactory::getLogger<OperationalLimitsGroups>();

    for (const auto& id : ids){
        if(id.empty()) {
            idIsNull = true;
            logger.error("One of the provided ID was null");
            continue;
        }
        if(!getOperationalLimitsGroup(id)) {
            nonExistingGroup.push_back(id);
            logger.error("No operational limits group with given ID (%1%) exists, can't be assigned as a selected group of %2%", id, m_attributeName);
        }
    }
    
    if(idIsNull){
        throw PowsyblException("One or more of the provided IDs for the group selection were null, none of the provided groups were selected");
    }
    if(!nonExistingGroup.empty()) {
        throw PowsyblException(stdcxx::format("The following IDs did not correspond to an existing group, they cannot be selected : %1%", stdcxx::toString(nonExistingGroup)));
    }

    // all given IDs are non null and a group of that ID exists
    for (const auto& id : ids) {
        //reset as last selected
        m_selectedLimitsGroupIds.remove(id);
        m_selectedLimitsGroupIds.push_back(id);
    }
}

void OperationalLimitsGroups::removeOperationalLimitsGroup(const std::string& id) {
    if(id.empty()) {
        return;
    }
    if(isSelectedOperationalLimitsGroup(id)) {
        deselectOperationalLimitsGroups({id});
    }
    m_operationalLimitsGroupById.erase(id);
}

void OperationalLimitsGroups::cancelSelectedOperationalLimitsGroup() {
    m_selectedLimitsGroupIds.clear();
}

void OperationalLimitsGroups::deselectOperationalLimitsGroups(const std::list<std::string>& ids) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<OperationalLimitsGroups>();
    
    for (const auto& id : ids) {
        if(id.empty()) {
            logger.warn("One of the provided ID was null");
            continue;
        }
        if(!getOperationalLimitsGroup(id)) {
            logger.warn("The ID %1% did not correspond to any existing group of %2%.", id, m_attributeName);
            continue;
        }
        m_selectedLimitsGroupIds.remove(id);
    }
}

OperationalLimitsGroup& OperationalLimitsGroups::getOrCreateSelectedOperationalLimitsGroup(const std::string& id) {
    return FlowsLimitsHolder::getOrCreateSelectedOperationalLimitsGroup(id);
}

ActivePowerLimitsAdder OperationalLimitsGroups::newActivePowerLimits() {
    return ActivePowerLimitsAdder(getOrCreateSelectedOperationalLimitsGroup());
}

ApparentPowerLimitsAdder OperationalLimitsGroups::newApparentPowerLimits() {
    return ApparentPowerLimitsAdder(getOrCreateSelectedOperationalLimitsGroup());
}

CurrentLimitsAdder OperationalLimitsGroups::newCurrentLimits() {
    return CurrentLimitsAdder(getOrCreateSelectedOperationalLimitsGroup());
}


OperationalLimitsGroup& OperationalLimitsGroups::getOrCreateSelectedOperationalLimitsGroup() {
    stdcxx::Reference<OperationalLimitsGroup> selectedGroup = getSelectedOperationalLimitsGroup();
    if(!selectedGroup) {
        std::string defaultId = DEFAULT_SELECTED_OPERATIONAL_LIMITS_GROUP_ID;
        auto defaultGroup = getOperationalLimitsGroup(defaultId);
        if(!defaultGroup) {
            selectedGroup = newOperationalLimitsGroup(defaultId);
        } else {
            selectedGroup = defaultGroup;
        }
        setSelectedOperationalLimitsGroup(defaultId);
    }

    return selectedGroup.get();
}

bool OperationalLimitsGroups::isSelectedOperationalLimitsGroup(const std::string& groupId) const {
    if(m_selectedLimitsGroupIds.empty() || groupId.empty()) {
        return false;
    }

    const auto& rslt = std::find(m_selectedLimitsGroupIds.cbegin(), m_selectedLimitsGroupIds.cend(), groupId);
    return rslt!=m_selectedLimitsGroupIds.cend();
}


}  // namespace iidm

}  // namespace powsybl
