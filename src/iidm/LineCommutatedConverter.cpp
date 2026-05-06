/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LineCommutatedConverter.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

LineCommutatedConverter::LineCommutatedConverter(const std::string& id, const std::string& name, bool fictitious, unsigned long variantArraySize, 
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const ControlMode& controlMode, double targetP, double targetVdc, 
                    const ReactiveModel& reactiveModel, double powerFactor) :
    Identifiable(id, name, fictitious),
    AcDcConverter(variantArraySize, idleLoss, switchingLoss, resistiveLoss, pccTerminal, controlMode, targetP, targetVdc),
    m_reactiveModel(reactiveModel),
    m_powerFactor(powerFactor) {
}

const IdentifiableType& LineCommutatedConverter::getType() const {
    static IdentifiableType s_type = IdentifiableType::LINE_COMMUTATED_CONVERTER;
    return s_type;
}

const std::string& LineCommutatedConverter::getTypeDescription() const {
    static std::string s_typeDescription = "AC/DC Line Commutated Converter";
    return s_typeDescription;
}

LineCommutatedConverter& LineCommutatedConverter::setIdleLoss(double idleLoss) {
    AcDcConverter::setIdleLoss(idleLoss);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setSwitchingLoss(double switchingLoss) {
    AcDcConverter::setSwitchingLoss(switchingLoss);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setResistiveLoss(double resistiveLoss) {
    AcDcConverter::setResistiveLoss(resistiveLoss);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal) {
    AcDcConverter::setPccTerminal(pccTerminal);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setControlMode(const AcDcConverter::ControlMode& controlMode) {
    AcDcConverter::setControlMode(controlMode);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setTargetP(double targetP) {
    AcDcConverter::setTargetP(targetP);
    return *this;
}
LineCommutatedConverter& LineCommutatedConverter::setTargetVdc(double targetVdc) {
    AcDcConverter::setTargetVdc(targetVdc);
    return *this;
}

const LineCommutatedConverter::ReactiveModel& LineCommutatedConverter::getReactiveModel() const {
    return m_reactiveModel;
}
LineCommutatedConverter& LineCommutatedConverter::setReactiveModel(const LineCommutatedConverter::ReactiveModel& reactiveModel) {
    m_reactiveModel = checkLccReactiveModel(*this, reactiveModel);
    return *this;
}

double LineCommutatedConverter::getPowerFactor() const {
    return m_powerFactor;
}
LineCommutatedConverter& LineCommutatedConverter::setPowerFactor(double powerFactor) {
    m_powerFactor = checkPowerFactorPositive(*this, powerFactor);
    return *this;
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<LineCommutatedConverter::ReactiveModel>() {
    static std::initializer_list<std::string> s_lccReactiveModelNames {
        "FIXED_POWER_FACTOR",
        "CALCULATED_POWER_FACTOR"
    };
    return s_lccReactiveModelNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
