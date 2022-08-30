/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StaticVarCompensator.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

StaticVarCompensator::StaticVarCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
        double bMin, double bMax, double voltageSetpoint, double reactivePowerSetpoint, const RegulationMode& regulationMode, Terminal& regulatingTerminal) :
    Injection(id, name, fictitious),
    m_bMin(checkBmin(*this, bMin)),
    m_bMax(checkBmax(*this, bMax)),
    m_voltageSetpoint(network.getVariantManager().getVariantArraySize(), voltageSetpoint),
    m_reactivePowerSetpoint(network.getVariantManager().getVariantArraySize(), reactivePowerSetpoint),
    m_regulatingTerminal(regulatingTerminal),
    m_regulationMode(network.getVariantManager().getVariantArraySize(), regulationMode) {
    checkSvcRegulator(*this, voltageSetpoint, reactivePowerSetpoint, regulationMode);
}

void StaticVarCompensator::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_voltageSetpoint[index] = m_voltageSetpoint[sourceIndex];
        m_reactivePowerSetpoint[index] = m_reactivePowerSetpoint[sourceIndex];
        m_regulationMode[index] = m_regulationMode[sourceIndex];
    }
}

void StaticVarCompensator::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_voltageSetpoint.resize(m_voltageSetpoint.size() + number, m_voltageSetpoint[sourceIndex]);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() + number, m_reactivePowerSetpoint[sourceIndex]);
    m_regulationMode.resize(m_regulationMode.size() + number, m_regulationMode[sourceIndex]);
}

double StaticVarCompensator::getBmax() const {
    return m_bMax;
}

double StaticVarCompensator::getBmin() const {
    return m_bMin;
}

double StaticVarCompensator::getReactivePowerSetpoint() const {
    return m_reactivePowerSetpoint.at(getNetwork().getVariantIndex());
}

const Terminal& StaticVarCompensator::getRegulatingTerminal() const {
    return m_regulatingTerminal.get();
}

Terminal& StaticVarCompensator::getRegulatingTerminal() {
    return m_regulatingTerminal.get();
}

const StaticVarCompensator::RegulationMode& StaticVarCompensator::getRegulationMode() const {
    return m_regulationMode.at(getNetwork().getVariantIndex());
}

const IdentifiableType& StaticVarCompensator::getType() const {
    static IdentifiableType s_type = IdentifiableType::STATIC_VAR_COMPENSATOR;
    return s_type;
}

const std::string& StaticVarCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "staticVarCompensator";

    return s_typeDescription;
}

double StaticVarCompensator::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(getNetwork().getVariantIndex());
}

void StaticVarCompensator::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_voltageSetpoint.resize(m_voltageSetpoint.size() - number);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() - number);
    m_regulationMode.resize(m_regulationMode.size() - number);
}

StaticVarCompensator& StaticVarCompensator::setBmax(double bMax) {
    m_bMax = checkBmax(*this, bMax);

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setBmin(double bMin) {
    m_bMin = checkBmin(*this, bMin);

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    checkSvcRegulator(*this, getVoltageSetpoint(), reactivePowerSetpoint, getRegulationMode());
    m_reactivePowerSetpoint[getNetwork().getVariantIndex()] = reactivePowerSetpoint;

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    checkRegulatingTerminal(*this, regulatingTerminal, getNetwork());
    m_regulatingTerminal = regulatingTerminal ? regulatingTerminal : getTerminal();
    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulationMode(const RegulationMode& regulationMode) {
    checkSvcRegulator(*this, getVoltageSetpoint(), getReactivePowerSetpoint(), regulationMode);
    m_regulationMode[getNetwork().getVariantIndex()] = regulationMode;

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setVoltageSetpoint(double voltageSetpoint) {
    checkSvcRegulator(*this, voltageSetpoint, getReactivePowerSetpoint(), getRegulationMode());
    m_voltageSetpoint[getNetwork().getVariantIndex()] = voltageSetpoint;

    return *this;
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<StaticVarCompensator::RegulationMode>() {
    static std::initializer_list<std::string> s_staticVarCompensatorRegulationNames {
        "VOLTAGE",
        "REACTIVE_POWER",
        "OFF"
    };
    return s_staticVarCompensatorRegulationNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
