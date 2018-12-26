/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/logging/MessageFormat.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace logging {

/**
 * toString template specialization for RegulationMode
 */
template <>
std::string toString(const iidm::StaticVarCompensator::RegulationMode& value) {
    static std::array<std::string, 3> s_regulationModeNames {{
                                                               "VOLTAGE",
                                                               "REACTIVE_POWER",
                                                               "OFF"
                                                           }};

    unsigned int val = static_cast<unsigned int>(value);
    return (val < s_regulationModeNames.size()) ? s_regulationModeNames.at(val) : format("%1%", val);
}

}  // namespace logging

namespace iidm {

StaticVarCompensator::StaticVarCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name,
        double bMin, double bMax, double voltageSetpoint, double reactivePowerSetpoint, const RegulationMode& regulationMode) :
    Injection(id, name, ConnectableType::STATIC_VAR_COMPENSATOR),
    m_network(network),
    m_bMin(checkBmin(*this, bMin)),
    m_bMax(checkBmax(*this, bMax)),
    m_voltageSetpoint(network.getVariantManager().getVariantArraySize(), voltageSetpoint),
    m_reactivePowerSetpoint(network.getVariantManager().getVariantArraySize(), reactivePowerSetpoint),
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
    return m_reactivePowerSetpoint.at(m_network.get().getVariantIndex());
}

const StaticVarCompensator::RegulationMode& StaticVarCompensator::getRegulationMode() const {
    return m_regulationMode.at(m_network.get().getVariantIndex());
}

const std::string& StaticVarCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "staticVarCompensator";

    return s_typeDescription;
}

double StaticVarCompensator::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(m_network.get().getVariantIndex());
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
    m_reactivePowerSetpoint[m_network.get().getVariantIndex()] = reactivePowerSetpoint;

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulationMode(const RegulationMode& regulationMode) {
    checkSvcRegulator(*this, getVoltageSetpoint(), getReactivePowerSetpoint(), regulationMode);
    m_regulationMode[m_network.get().getVariantIndex()] = regulationMode;

    return *this;
}

StaticVarCompensator& StaticVarCompensator::setVoltageSetpoint(double voltageSetpoint) {
    checkSvcRegulator(*this, voltageSetpoint, getReactivePowerSetpoint(), getRegulationMode());
    m_voltageSetpoint[m_network.get().getVariantIndex()] = voltageSetpoint;

    return *this;
}

std::ostream& operator<<(std::ostream& stream, const StaticVarCompensator::RegulationMode& mode) {
    stream << logging::toString(mode);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
