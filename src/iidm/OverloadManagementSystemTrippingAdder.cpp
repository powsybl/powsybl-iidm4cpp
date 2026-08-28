/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OverloadManagementSystemTrippingAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/OverloadManagementSystemAdder.hpp>
#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace overload_management_system {

std::string TrippingAdder::getMessageHeader() const {
    return stdcxx::format("Overload management system in substation '%1%':  - %2%:", m_parent.m_substation.get().getId(), getTrippingAttribute());
}

OverloadManagementSystemAdder& TrippingAdder::add() {
    m_parent.m_trippingAdders.push_back(shared_from_this());
    return m_parent;
}

TrippingAdder& TrippingAdder::setKey(const std::string& key) {
    m_key = key;
    return *this;
}
TrippingAdder& TrippingAdder::setName(const std::string& name) {
    m_name = name;
    return *this;
}
TrippingAdder& TrippingAdder::setCurrentLimit(double currentLimit) {
    m_currentLimit = currentLimit;
    return *this;
}
TrippingAdder& TrippingAdder::setOpenAction(bool open) {
    m_openAction = open;
    return *this;
}

TrippingAdder::TrippingAdder(OverloadManagementSystemAdder& parent) :
    m_parent(parent) {
}

std::string TrippingAdder::getTrippingAttribute() const {
    return stdcxx::format("tripping '%1%'", m_key);
}
stdcxx::Reference<Identifiable> TrippingAdder::checkElementId(const std::string& monitoredElementId, const std::string& attributeName) {
    stdcxx::Reference<Identifiable> foundElement;
    if(monitoredElementId.empty()) {
        throw ValidationException(*this, stdcxx::format("%1% is not set", attributeName));
    }
    foundElement = m_parent.m_substation.get().getNetwork().find<Identifiable>(monitoredElementId);
    if(!static_cast<bool>(foundElement)) {
        throw ValidationException(*this, stdcxx::format("Identifiable (%1%) not found in this network (%2%)", monitoredElementId, m_parent.m_substation.get().getNetwork().getId()));
    }
    return foundElement;
}



const Tripping::Type& SwitchTrippingAdder::getType() const {
    static Tripping::Type s_trippingType = Tripping::Type::SWITCH_TRIPPING;
    return s_trippingType;
}

SwitchTrippingAdder& SwitchTrippingAdder::setSwitchToOperateId(const std::string& switchId) {
    m_switchId = switchId;
    return *this;
}

SwitchTrippingAdder::SwitchTrippingAdder(OverloadManagementSystemAdder& parent) :
    TrippingAdder(parent) {
}

std::unique_ptr<Tripping> SwitchTrippingAdder::buildTripping(const std::string& overloadManagementSystemId) {
    checkSwitchId(m_switchId);
    std::unique_ptr<Tripping> ptrSwitchTripping = std::unique_ptr<Tripping>(new SwitchTripping(overloadManagementSystemId,
                                        m_key, m_name, m_currentLimit, m_openAction, m_switchId));
    copyPropertiesTo(*ptrSwitchTripping);
    return ptrSwitchTripping;
}

stdcxx::Reference<Identifiable> SwitchTrippingAdder::checkSwitchId(const std::string& monitoredElementId) {
    stdcxx::Reference<Identifiable> foundElement = checkElementId(monitoredElementId, "switchId");
    if (!stdcxx::isInstanceOf<Switch>(foundElement)) {
        throw ValidationException(*this, stdcxx::format("Element (%1%) is of type : %2% (%3% expected)", monitoredElementId, stdcxx::demangle(foundElement),stdcxx::demangle<Switch>()));
    }
    return foundElement;
}




const Tripping::Type& BranchTrippingAdder::getType() const {
    static Tripping::Type s_trippingType = Tripping::Type::BRANCH_TRIPPING;
    return s_trippingType;
}

BranchTrippingAdder& BranchTrippingAdder::setBranchToOperateId(const std::string& branchId) {
    m_branchId = branchId;
    return *this;
}

BranchTrippingAdder& BranchTrippingAdder::setSideToOperate(const TwoSides& side) {
    m_side = side;
    return *this;
}

BranchTrippingAdder::BranchTrippingAdder(OverloadManagementSystemAdder& parent) :
    TrippingAdder(parent) {
}

std::unique_ptr<Tripping> BranchTrippingAdder::buildTripping(const std::string& overloadManagementSystemId) {
    checkBranchId(m_branchId);

    std::unique_ptr<Tripping> ptrBranchTripping = std::unique_ptr<Tripping>(new BranchTripping(overloadManagementSystemId,
                                        m_key, m_name, m_currentLimit, m_openAction, m_branchId, m_side));
    copyPropertiesTo(*ptrBranchTripping);
    return ptrBranchTripping;
}

stdcxx::Reference<Identifiable> BranchTrippingAdder::checkBranchId(const std::string& monitoredElementId) {
    stdcxx::Reference<Identifiable> foundElement = checkElementId(monitoredElementId, "branchId");
    if (!stdcxx::isInstanceOf<Branch>(foundElement)) {
        throw ValidationException(*this, stdcxx::format("Element (%1%) is of type : %2% (%3% expected)", monitoredElementId, stdcxx::demangle(foundElement),stdcxx::demangle<Branch>()));
    }
    return foundElement;
}



const Tripping::Type& ThreeWindingsTransformerTrippingAdder::getType() const {
    static Tripping::Type s_trippingType = Tripping::Type::THREE_WINDINGS_TRANSFORMER_TRIPPING;
    return s_trippingType;
}

ThreeWindingsTransformerTrippingAdder& ThreeWindingsTransformerTrippingAdder::setThreeWindingsTransformerToOperateId(const std::string& transformerId) {
    m_transformerId = transformerId;
    return *this;
}

ThreeWindingsTransformerTrippingAdder& ThreeWindingsTransformerTrippingAdder::setSideToOperate(const ThreeSides& side) {
    m_side = side;
    return *this;
}

ThreeWindingsTransformerTrippingAdder::ThreeWindingsTransformerTrippingAdder(OverloadManagementSystemAdder& parent) :
    TrippingAdder(parent) {
}

std::unique_ptr<Tripping> ThreeWindingsTransformerTrippingAdder::buildTripping(const std::string& overloadManagementSystemId) {
    checkThreeWindingsTransformerId(m_transformerId);
    std::unique_ptr<Tripping> ptrTransformerTripping = std::unique_ptr<Tripping>(new ThreeWindingsTransformerTripping(overloadManagementSystemId,
                                        m_key, m_name, m_currentLimit, m_openAction, m_transformerId, m_side));
    copyPropertiesTo(*ptrTransformerTripping);
    return ptrTransformerTripping;
}

stdcxx::Reference<Identifiable> ThreeWindingsTransformerTrippingAdder::checkThreeWindingsTransformerId(const std::string& monitoredElementId) {
    stdcxx::Reference<Identifiable> foundElement = checkElementId(monitoredElementId, "threeWindingsTransformerId");
    if (!stdcxx::isInstanceOf<ThreeWindingsTransformer>(foundElement)) {
        throw ValidationException(*this, stdcxx::format("Element (%1%) is of type : %2% (%3% expected)", monitoredElementId, stdcxx::demangle(foundElement.get()),stdcxx::demangle<ThreeWindingsTransformer>()));
    }
    return foundElement;
}



}  // namespace overload_management_system

}  // namespace iidm

}  // namespace powsybl
