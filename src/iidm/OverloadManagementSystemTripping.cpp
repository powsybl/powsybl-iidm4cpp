/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace overload_management_system {

Tripping::Tripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction) :
    m_overloadManagementSystemId(overloadManagementSystemId),
    m_key(key) {
    setName(name);
    setCurrentLimit(currentLimit);
    setOpenAction(openAction);
}

const std::string& Tripping::getKey() const {
    return m_key;
}

const std::string& Tripping::getNameOrKey() const {
    if(!m_name.empty()) {
        return m_name;
    }
    return m_key;
}

Tripping& Tripping::setName(const std::string& name) {
    m_name = name;
    return *this;
}

double Tripping::getCurrentLimit() const {
    return m_currentLimit;
}

Tripping& Tripping::setCurrentLimit(double currentLimit) {
    if(std::isnan(currentLimit)) {
        throw ValidationException(*this, "Current limit is mandatory.");
    } else if(std::islessequal(currentLimit, 0.0)) {
        throw ValidationException(*this, "Current limit must be positive.");
    }
    m_currentLimit = currentLimit;
    return *this;
}

bool Tripping::isOpenAction() const {
    return m_openAction;
}

Tripping& Tripping::setOpenAction(bool open) {
    m_openAction = open;
    return *this;
}

std::string Tripping::getMessageHeader() const  {
    return stdcxx::format("Overload management system '%1%' - %2%:", m_overloadManagementSystemId, getTrippingAttribute());
}

std::string Tripping::getTrippingAttribute() const {
    return stdcxx::format("tripping '%1%'", m_key);
}


SwitchTripping::SwitchTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& switchToOperateId) :
    Tripping(overloadManagementSystemId, key, name, currentLimit, openAction) {
    setSwitchToOperateId(switchToOperateId);
}

const Tripping::Type& SwitchTripping::getType() const {
    static Tripping::Type s_trippingType = Tripping::Type::SWITCH_TRIPPING;
    return s_trippingType;
}

const std::string& SwitchTripping::getSwitchToOperateId() const {
    return m_switchToOperateId;
}

SwitchTripping& SwitchTripping::setSwitchToOperateId(const std::string& switchToOperateId) {
    if(switchToOperateId.empty()) {
        throw ValidationException(*this, "Switch Id is mandatory.");
    }
    m_switchToOperateId = switchToOperateId;
    return *this;
}


BranchTripping::BranchTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& branchToOperateId, const TwoSides& side) :
    Tripping(overloadManagementSystemId, key, name, currentLimit, openAction) {
    setBranchToOperateId(branchToOperateId);
    setSideToOperate(side);
}

const Tripping::Type& BranchTripping::getType() const {
    static Tripping::Type s_trippingType = Tripping::Type::BRANCH_TRIPPING;
    return s_trippingType;
}

const std::string& BranchTripping::getBranchToOperateId() const {
    return m_branchToOperateId;
}

BranchTripping& BranchTripping::setBranchToOperateId(const std::string& branchToOperateId) {
    if(branchToOperateId.empty()) {
        throw ValidationException(*this, "Branch Id is mandatory.");
    }
    m_branchToOperateId = branchToOperateId;
    return *this;
}

const TwoSides& BranchTripping::getSideToOperate() const {
    return m_side;
}

BranchTripping& BranchTripping::setSideToOperate(const TwoSides& side) {
    switch (side) {
        case TwoSides::ONE:
        case TwoSides::TWO:
            break;
        case TwoSides::UNDEFINED:
        default:
            throw ValidationException(*this, stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
    m_side = side;
    return *this;
}


ThreeWindingsTransformerTripping::ThreeWindingsTransformerTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& threeWindingsTransformerId, const ThreeSides& side) :
    Tripping(overloadManagementSystemId, key, name, currentLimit, openAction) {
    setThreeWindingsTransformerToOperateId(threeWindingsTransformerId);
    setSideToOperate(side);
}

const Tripping::Type& ThreeWindingsTransformerTripping::getType() const  {
    static Tripping::Type s_trippingType = Tripping::Type::THREE_WINDINGS_TRANSFORMER_TRIPPING;
    return s_trippingType;
}

const std::string& ThreeWindingsTransformerTripping::getThreeWindingsTransformerToOperateId() const {
    return m_transformerToOperateId;
}

ThreeWindingsTransformerTripping& ThreeWindingsTransformerTripping::setThreeWindingsTransformerToOperateId(const std::string& threeWindingsTransformerId) {
    if(threeWindingsTransformerId.empty()) {
        throw ValidationException(*this, "Three windings transformer Id is mandatory.");
    }
    m_transformerToOperateId = threeWindingsTransformerId;
    return *this;
}

const ThreeSides& ThreeWindingsTransformerTripping::getSideToOperate() const {
    return m_side;
}

ThreeWindingsTransformerTripping& ThreeWindingsTransformerTripping::setSideToOperate(const ThreeSides& side) {
    switch (side) {
        case ThreeSides::ONE:
        case ThreeSides::TWO:
        case ThreeSides::THREE:
            break;
        case ThreeSides::UNDEFINED:
        default:
            throw ValidationException(*this, stdcxx::format("Unexpected ThreeSides value: %1%", side));
    }
    m_side = side;
    return *this;
}

}  // namespace overload_management_system

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<overload_management_system::Tripping::Type>() {
    static std::initializer_list<std::string> s_typeNames {
        "BRANCH_TRIPPING",
        "THREE_WINDINGS_TRANSFORMER_TRIPPING",
        "SWITCH_TRIPPING"
    };
    return s_typeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
