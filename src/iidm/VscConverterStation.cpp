/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VscConverterStation.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>

namespace powsybl {

namespace iidm {

VscConverterStation::VscConverterStation(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, double lossFactor, bool voltageRegulatorOn, double reactivePowerSetpoint, double voltageSetpoint) :
    HvdcConverterStation(id, name, fictitious, lossFactor),
    m_voltageRegulatorOn(network.getVariantManager().getVariantArraySize(), voltageRegulatorOn),
    m_reactivePowerSetpoint(network.getVariantManager().getVariantArraySize(), reactivePowerSetpoint),
    m_voltageSetpoint(network.getVariantManager().getVariantArraySize(), voltageSetpoint) {
    checkVoltageControl(*this, voltageRegulatorOn, voltageSetpoint, reactivePowerSetpoint);
}

void VscConverterStation::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_reactivePowerSetpoint[index] = m_reactivePowerSetpoint[sourceIndex];
        m_voltageSetpoint[index] = m_voltageSetpoint[sourceIndex];
    }
}

void VscConverterStation::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() + number, m_reactivePowerSetpoint[sourceIndex]);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() + number, m_voltageSetpoint[sourceIndex]);
}

HvdcConverterStation::HvdcType VscConverterStation::getHvdcType() const {
    return HvdcType::VSC;
}

const std::string& VscConverterStation::getTypeDescription() const {
    static std::string s_typeDescription = "vscConverterStation";

    return s_typeDescription;
}

double VscConverterStation::getReactivePowerSetpoint() const {
    return m_reactivePowerSetpoint.at(getNetwork().getVariantIndex());
}

double VscConverterStation::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(getNetwork().getVariantIndex());
}

bool VscConverterStation::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn.at(getNetwork().getVariantIndex());
}

void VscConverterStation::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() - number);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() - number);
}

VscConverterStation& VscConverterStation::setFictitious(bool fictitious) {
    HvdcConverterStation::setFictitious(fictitious);
    return *this;
}

VscConverterStation& VscConverterStation::setLossFactor(double lossFactor) {
    return dynamic_cast<VscConverterStation&>(HvdcConverterStation::setLossFactor(lossFactor));
}

VscConverterStation& VscConverterStation::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), getVoltageSetpoint(), reactivePowerSetpoint);
    m_reactivePowerSetpoint[getNetwork().getVariantIndex()] = reactivePowerSetpoint;
    return *this;
}

VscConverterStation& VscConverterStation::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getVoltageSetpoint(), getReactivePowerSetpoint());
    m_voltageRegulatorOn[getNetwork().getVariantIndex()] = voltageRegulatorOn;
    return *this;
}

VscConverterStation& VscConverterStation::setVoltageSetpoint(double voltageSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), voltageSetpoint, getReactivePowerSetpoint());
    m_voltageSetpoint[getNetwork().getVariantIndex()] = voltageSetpoint;
    return *this;
}
}  // namespace iidm

}  // namespace powsybl
