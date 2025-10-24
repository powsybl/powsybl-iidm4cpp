/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>

#include <powsybl/iidm/Substation.hpp>

namespace powsybl {

namespace iidm {

OverloadManagementSystemAdder::OverloadManagementSystemAdder(Substation& substation) :
    m_substation(substation) {

}

OverloadManagementSystem& OverloadManagementSystemAdder::add() {
    std::string id = checkAndGetUniqueId();

    if(!static_cast<bool>(m_substation)){
        throw PowsyblException(stdcxx::format("Overload management system %1% has no substation", id));
    }

    //Build Overload management system
    std::unique_ptr<OverloadManagementSystem> ptrOverloadManagementSystem = std::unique_ptr<OverloadManagementSystem>(new OverloadManagementSystem(m_substation, id, getName(), isFictitious(),
        m_enabled, m_monitoredElementId, m_monitoredSide));

    //Add trippings:
    std::set<std::string> knownTrippingKeys;
    for(auto& trippingAdderPtr : m_trippingAdders) {
        std::unique_ptr<OverloadManagementSystem::Tripping> trippingPtr = createTripping(trippingAdderPtr, id, knownTrippingKeys);
        if(static_cast<bool>(ptrOverloadManagementSystem) && static_cast<bool>(trippingPtr)){
            ptrOverloadManagementSystem->addTripping(std::move(trippingPtr));
        }
    }

    auto& overloadManagementSystem = getNetwork().checkAndAdd<OverloadManagementSystem>(std::move(ptrOverloadManagementSystem));
    m_substation.get().addOverloadManagement(overloadManagementSystem);

    return overloadManagementSystem;
}

std::shared_ptr<overload_management_system::SwitchTrippingAdder> OverloadManagementSystemAdder::newSwitchTripping() {
    std::shared_ptr<overload_management_system::SwitchTrippingAdder> switchTrippingAdder = overload_management_system::SwitchTrippingAdder::create(*this);
    return switchTrippingAdder;
}
std::shared_ptr<overload_management_system::BranchTrippingAdder> OverloadManagementSystemAdder::newBranchTripping() {
    std::shared_ptr<overload_management_system::BranchTrippingAdder> branchTrippingAdder = overload_management_system::BranchTrippingAdder::create(*this);
    return branchTrippingAdder;
}
std::shared_ptr<overload_management_system::ThreeWindingsTransformerTrippingAdder> OverloadManagementSystemAdder::newThreeWindingsTransformerTripping() {
    std::shared_ptr<overload_management_system::ThreeWindingsTransformerTrippingAdder> transformerTrippingAdder = overload_management_system::ThreeWindingsTransformerTrippingAdder::create(*this);
    return transformerTrippingAdder;
}

OverloadManagementSystemAdder& OverloadManagementSystemAdder::setEnabled(bool enabled) {
    m_enabled = enabled;
    return *this;
}

OverloadManagementSystemAdder& OverloadManagementSystemAdder::setMonitoredElementId(const std::string& monitoredElementId) {
    m_monitoredElementId = monitoredElementId;
    return *this;
}

OverloadManagementSystemAdder& OverloadManagementSystemAdder::setMonitoredElementSide(const ThreeSides& monitoredElementSide) {
    m_monitoredSide = monitoredElementSide;
    return *this;
}

const Network& OverloadManagementSystemAdder::getNetwork() const {
    return m_substation.get().getNetwork();
}

Network& OverloadManagementSystemAdder::getNetwork() {
    return m_substation.get().getNetwork();
}

const std::string& OverloadManagementSystemAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Overload management system";
    return s_typeDescription;
}

std::unique_ptr<OverloadManagementSystem::Tripping> OverloadManagementSystemAdder::createTripping(std::shared_ptr<TrippingAdder>& trippingAdder, const std::string& overloadManagementSystemId, 
                        std::set<std::string>& knownTrippingKeys) {
    std::unique_ptr<OverloadManagementSystem::Tripping> trippingPtr;

    if(static_cast<bool>(trippingAdder)) {
        std::string key = trippingAdder->m_key;
        if(knownTrippingKeys.find(key) != knownTrippingKeys.end()) {
            throw ValidationException(*trippingAdder, 
                stdcxx::format("key '%1%' is already used for another tripping in the overload management system '%2%'.",
                    key, overloadManagementSystemId));
        }
        knownTrippingKeys.insert(key);

        trippingPtr = trippingAdder->buildTripping(overloadManagementSystemId);
    }

    return trippingPtr;
}

}  // namespace iidm

}  // namespace powsybl
