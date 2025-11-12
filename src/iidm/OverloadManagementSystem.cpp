/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OverloadManagementSystem.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/format.hpp>

#include <boost/range/adaptors.hpp>

namespace powsybl {

namespace iidm {

OverloadManagementSystem::OverloadManagementSystem(const stdcxx::Reference<Substation>& substation, const std::string& id, const std::string& name, bool fictitious, bool enabled,
                            const std::string& monitoredElementId, const ThreeSides& monitoredSide) :
    AutomationSystem(substation.get().getNetwork(), id, name, fictitious, enabled),
    m_substation(substation),
    m_monitoredElementId(monitoredElementId),
    m_monitoredSide(monitoredSide) {
    checkMonitoredElementId(monitoredElementId);
}

void OverloadManagementSystem::checkMonitoredElementId(const std::string& monitoredElementId) {
    if(monitoredElementId.empty()) {
        throw ValidationException(*this, "monitoredElementId is not set");
    }
    stdcxx::Reference<Identifiable> monitoredElement = getNetwork().find<Identifiable>(monitoredElementId);
    if(!static_cast<bool>(monitoredElement)) {
        throw ValidationException(*this, stdcxx::format("Identifiable (%1%) not found in this network (%2%)", monitoredElementId, getNetwork().getId()));
    }
}

const Network& OverloadManagementSystem::getNetwork() const {
    if (static_cast<bool>(m_substation)) {
        return m_substation.get().getNetwork();
    }
    throw PowsyblException(stdcxx::format("Overload management system %1% has no substation", getId()));
}
Network& OverloadManagementSystem::getNetwork() {
    return const_cast<Network&>(static_cast<const OverloadManagementSystem*>(this)->getNetwork());
}

const Network& OverloadManagementSystem::getParentNetwork() const {
    if (static_cast<bool>(m_substation)) {
        return m_substation.get().getParentNetwork();
    }
    throw PowsyblException(stdcxx::format("Overload management system %1% has no substation", getId()));
}
Network& OverloadManagementSystem::getParentNetwork() {
    return const_cast<Network&>(static_cast<const OverloadManagementSystem*>(this)->getParentNetwork());
}

const IdentifiableType& OverloadManagementSystem::getType() const {
    static IdentifiableType s_type = IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM;
    return s_type;
}

const std::string& OverloadManagementSystem::getTypeDescription() const {
    static std::string s_typeDescription = "Overload management system";
    return s_typeDescription;
}

void OverloadManagementSystem::remove() {
    if (static_cast<bool>(m_substation)) {
        m_substation.get().remove(*this);
    }
    getNetwork().remove(*this);
}

stdcxx::CReference<Substation> OverloadManagementSystem::getSubstation() const {
    return stdcxx::cref(m_substation);
}

stdcxx::Reference<Substation> OverloadManagementSystem::getSubstation() {
    return m_substation;
}

const std::string& OverloadManagementSystem::getMonitoredElementId() const {
    return m_monitoredElementId;
}

const ThreeSides& OverloadManagementSystem::getMonitoredSide() const {
    return m_monitoredSide;
}

std::vector<std::reference_wrapper<OverloadManagementSystem::Tripping>> OverloadManagementSystem::getTrippings() {
    std::vector<std::reference_wrapper<OverloadManagementSystem::Tripping>> trippings;
    trippings.reserve(m_trippings.size());
    for (const auto& tripping : m_trippings) {
        trippings.push_back(std::ref(*tripping));
    }
    return trippings;
}
stdcxx::const_range<OverloadManagementSystem::Tripping> OverloadManagementSystem::getTrippings() const {
    return m_trippings | boost::adaptors::indirected;
}

OverloadManagementSystem& OverloadManagementSystem::addTripping(std::unique_ptr<Tripping>&& tripping) {
    m_trippings.emplace_back(std::move(tripping));
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
