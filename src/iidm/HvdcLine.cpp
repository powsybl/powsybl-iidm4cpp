/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace logging {

/**
 * toString template specialization for ConvertersMode
 */
template <>
std::string toString(const iidm::HvdcLine::ConvertersMode& value) {
    static std::array<std::string, 2> s_convertersModeNames {{
        "SIDE_1_RECTIFIER_SIDE_2_INVERTER",
        "SIDE_1_INVERTER_SIDE_2_RECTIFIER"
    }};

    return toString(s_convertersModeNames, value);
}

}  // namespace logging

namespace iidm {

HvdcLine::HvdcLine(Network& network, const std::string& id, const std::string& name, double r, double nominalVoltage, double maxP,
                   const ConvertersMode& convertersMode, double activePowerSetpoint, HvdcConverterStation& converterStation1, HvdcConverterStation& converterStation2) :
    Identifiable(id, name),
    m_network(network),
    m_converterStation1(converterStation1),
    m_converterStation2(converterStation2),
    m_r(checkR(*this, r)),
    m_nominalVoltage(checkNominalVoltage(*this, nominalVoltage)),
    m_maxP(checkMaxP(*this, maxP)),
    m_convertersMode(network.getVariantManager().getVariantArraySize(), checkConvertersMode(*this, convertersMode)),
    m_activePowerSetpoint(network.getVariantManager().getVariantArraySize(), checkActivePowerSetpoint(*this, activePowerSetpoint)) {
}

void HvdcLine::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_convertersMode[index] = m_convertersMode[sourceIndex];
        m_activePowerSetpoint[index] = m_activePowerSetpoint[sourceIndex];
    }
}

void HvdcLine::deleteVariantArrayElement(unsigned long /*index*/) {
    // nothing to do
}

void HvdcLine::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_convertersMode.resize(m_convertersMode.size() + number, m_convertersMode[sourceIndex]);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() + number, m_activePowerSetpoint[sourceIndex]);
}

double HvdcLine::getActivePowerSetpoint() const {
    return m_activePowerSetpoint.at(getNetwork().getVariantIndex());
}

const HvdcLine::ConvertersMode& HvdcLine::getConvertersMode() const {
    return m_convertersMode.at(getNetwork().getVariantIndex());
}

const stdcxx::Reference<HvdcConverterStation>& HvdcLine::getConverterStation1() const {
    return m_converterStation1;
}

stdcxx::Reference<HvdcConverterStation>& HvdcLine::getConverterStation1() {
    return m_converterStation1;
}

const stdcxx::Reference<HvdcConverterStation>& HvdcLine::getConverterStation2() const {
    return m_converterStation2;
}

stdcxx::Reference<HvdcConverterStation>& HvdcLine::getConverterStation2() {
    return m_converterStation2;
}

double HvdcLine::getMaxP() const {
    return m_maxP;
}

const Network& HvdcLine::getNetwork() const {
    return m_network.get();
}

Network& HvdcLine::getNetwork() {
    return m_network.get();
}

double HvdcLine::getNominalVoltage() const {
    return m_nominalVoltage;
}

double HvdcLine::getR() const {
    return m_r;
}

const std::string& HvdcLine::getTypeDescription() const {
    static std::string s_typeDescription = "hvdcLine";

    return s_typeDescription;
}

void HvdcLine::reduceVariantArraySize(unsigned long number) {
    m_convertersMode.resize(m_convertersMode.size() - number);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() - number);
}

void HvdcLine::remove() {
    getNetwork().remove(*this);
}

HvdcLine& HvdcLine::setActivePowerSetpoint(double activePowerSetpoint) {
    m_activePowerSetpoint[getNetwork().getVariantIndex()] = checkActivePowerSetpoint(*this, activePowerSetpoint);

    return *this;
}

HvdcLine& HvdcLine::setConvertersMode(const ConvertersMode& mode) {
    m_convertersMode[getNetwork().getVariantIndex()] = checkConvertersMode(*this, mode);

    return *this;
}

HvdcLine& HvdcLine::setMaxP(double maxP) {
    m_maxP = checkMaxP(*this, maxP);

    return *this;
}

HvdcLine& HvdcLine::setNominalVoltage(double nominalVoltage) {
    m_nominalVoltage = checkNominalVoltage(*this, nominalVoltage);

    return *this;
}

HvdcLine& HvdcLine::setR(double r) {
    m_r = checkR(*this, r);

    return *this;
}

std::ostream& operator<<(std::ostream& stream, const HvdcLine::ConvertersMode& mode) {
    stream << logging::toString(mode);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
