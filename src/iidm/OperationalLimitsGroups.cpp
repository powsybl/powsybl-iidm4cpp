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

const stdcxx::optional<std::string>& OperationalLimitsGroups::getSelectedOperationalLimitsGroupId() const {
    return m_selectedLimitsGroupId;
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
    return m_selectedLimitsGroupId.has_value() ? getOperationalLimitsGroup(*m_selectedLimitsGroupId) : stdcxx::CReference<OperationalLimitsGroup>();
}
stdcxx::Reference<OperationalLimitsGroup> OperationalLimitsGroups::getSelectedOperationalLimitsGroup() {
    return m_selectedLimitsGroupId.has_value() ? getOperationalLimitsGroup(*m_selectedLimitsGroupId) : stdcxx::Reference<OperationalLimitsGroup>();
}

stdcxx::Reference<OperationalLimitsGroup> OperationalLimitsGroups::newOperationalLimitsGroup(const std::string& id) {
    if(!m_identifiable) {
        throw PowsyblException("OperationalLimitsGroup's Identifiable owner not set");
    }

    std::shared_ptr<OperationalLimitsGroup> operationalLimitsGroupPtr = std::make_shared<OperationalLimitsGroup>(id, m_identifiable, m_attributeName);

    m_operationalLimitsGroupById[id] = operationalLimitsGroupPtr;
    return stdcxx::ref(*m_operationalLimitsGroupById.find(id)->second);
}

void OperationalLimitsGroups::setSelectedOperationalLimitsGroup(const std::string& id) {
    if(id.empty()){
        cancelSelectedOperationalLimitsGroup();
        return;
    }
    if(m_selectedLimitsGroupId.has_value() && *m_selectedLimitsGroupId==id) {
        return;
    }

    //update selected group id in each group
    for (auto it : m_operationalLimitsGroupById) {
        it.second->setSelectedGroupId(id);
    }

    if(!getOperationalLimitsGroup(id)) {
        throw PowsyblException("no group with given id exists, can't be assigned as selected");
    }
    m_selectedLimitsGroupId = id;
}

void OperationalLimitsGroups::removeOperationalLimitsGroup(const std::string& id) {
    if(id.empty()) {
        return;
    }
    m_operationalLimitsGroupById.erase(id);

    if(m_selectedLimitsGroupId.has_value() && *m_selectedLimitsGroupId==id) {
        cancelSelectedOperationalLimitsGroup();
    }
}

void OperationalLimitsGroups::cancelSelectedOperationalLimitsGroup() {
    //update selected groupe id in each group
    for (auto it : m_operationalLimitsGroupById) {
        it.second->cancelSelectedGroupId();
    }
    m_selectedLimitsGroupId.reset();
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


}  // namespace iidm

}  // namespace powsybl
