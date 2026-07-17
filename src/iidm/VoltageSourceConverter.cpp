/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageSourceConverter.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

VoltageSourceConverter::VoltageSourceConverter(const std::string& id, const std::string& name, bool fictitious, unsigned long variantArraySize, 
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const ControlMode& controlMode, double targetP, double targetVdc, 
                    bool voltageRegulatorOn, double reactivePowerSetpoint, double voltageSetpoint) :
    Identifiable(id, name, fictitious),
    AcDcConverter(variantArraySize, idleLoss, switchingLoss, resistiveLoss, pccTerminal, controlMode, targetP, targetVdc),
    m_voltageRegulatorOn(variantArraySize, voltageRegulatorOn),
    m_reactivePowerSetpoint(variantArraySize, reactivePowerSetpoint),
    m_voltageSetpoint(variantArraySize, voltageSetpoint) {
}

const IdentifiableType& VoltageSourceConverter::getType() const {
    static IdentifiableType s_type = IdentifiableType::VOLTAGE_SOURCE_CONVERTER;
    return s_type;
}

const std::string& VoltageSourceConverter::getTypeDescription() const {
    static std::string s_typeDescription = "AC/DC Voltage Source Converter";
    return s_typeDescription;
}

VoltageSourceConverter& VoltageSourceConverter::setIdleLoss(double idleLoss) {
    AcDcConverter::setIdleLoss(idleLoss);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setSwitchingLoss(double switchingLoss) {
    AcDcConverter::setSwitchingLoss(switchingLoss);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setResistiveLoss(double resistiveLoss) {
    AcDcConverter::setResistiveLoss(resistiveLoss);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal) {
    AcDcConverter::setPccTerminal(pccTerminal);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setControlMode(const AcDcConverter::ControlMode& controlMode) {
    AcDcConverter::setControlMode(controlMode);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setTargetP(double targetP) {
    AcDcConverter::setTargetP(targetP);
    return *this;
}
VoltageSourceConverter& VoltageSourceConverter::setTargetVdc(double targetVdc) {
    AcDcConverter::setTargetVdc(targetVdc);
    return *this;
}

void VoltageSourceConverter::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    AcDcConverter::allocateVariantArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_reactivePowerSetpoint[index] = m_reactivePowerSetpoint[sourceIndex];
        m_voltageSetpoint[index] = m_voltageSetpoint[sourceIndex];
    }
}

void VoltageSourceConverter::deleteVariantArrayElement(unsigned long index) {
    AcDcConverter::deleteVariantArrayElement(index);
}

void VoltageSourceConverter::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    AcDcConverter::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() + number, m_reactivePowerSetpoint[sourceIndex]);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() + number, m_voltageSetpoint[sourceIndex]);
}

void VoltageSourceConverter::reduceVariantArraySize(unsigned long number) {
    AcDcConverter::reduceVariantArraySize(number);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() - number);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() - number);
}

bool VoltageSourceConverter::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn.at(getNetwork().getVariantIndex());
}

VoltageSourceConverter& VoltageSourceConverter::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getVoltageSetpoint(), getReactivePowerSetpoint(), getNetwork().getMinimumValidationLevel());
    m_voltageRegulatorOn[getNetwork().getVariantIndex()] = voltageRegulatorOn;
    getNetwork().invalidateValidationLevel();
    return *this;
}

double VoltageSourceConverter::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(getNetwork().getVariantIndex());
}

VoltageSourceConverter& VoltageSourceConverter::setVoltageSetpoint(double voltageSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), voltageSetpoint, getReactivePowerSetpoint(), getNetwork().getMinimumValidationLevel());
    m_voltageSetpoint[getNetwork().getVariantIndex()] = voltageSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

double VoltageSourceConverter::getReactivePowerSetpoint() const {
    return m_reactivePowerSetpoint.at(getNetwork().getVariantIndex());
}

VoltageSourceConverter& VoltageSourceConverter::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), getVoltageSetpoint(), reactivePowerSetpoint, getNetwork().getMinimumValidationLevel());
    m_reactivePowerSetpoint[getNetwork().getVariantIndex()] = reactivePowerSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

void VoltageSourceConverter::onReferencedRemoval(Terminal& removedReference) {
    AcDcConverter::onReferencedRemoval(removedReference);
    m_voltageRegulatorOn.assign(m_voltageRegulatorOn.size(), false);
}


}  // namespace iidm

}  // namespace powsybl
