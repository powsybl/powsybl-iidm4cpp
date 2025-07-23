/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HvdcLine.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

HvdcLine::HvdcLine(Network& network, const std::string& id, const std::string& name, bool fictitious, double r, double nominalV, double maxP,
                   const ConvertersMode& convertersMode, double activePowerSetpoint, HvdcConverterStation& converterStation1, HvdcConverterStation& converterStation2) :
    Identifiable(id, name, fictitious),
    m_converterStation1(attach(converterStation1)),
    m_converterStation2(attach(converterStation2)),
    m_r(checkR(*this, r)),
    m_nominalV(checkNominalVoltage(*this, nominalV)),
    m_maxP(checkHvdcMaxP(*this, maxP)),
    m_convertersMode(network.getVariantManager().getVariantArraySize(), convertersMode),
    m_activePowerSetpoint(network.getVariantManager().getVariantArraySize(), activePowerSetpoint) {
    checkConvertersMode(*this, convertersMode, network.getMinimumValidationLevel());
    checkHvdcActivePowerSetpoint(*this, activePowerSetpoint, network.getMinimumValidationLevel());
}

void HvdcLine::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_convertersMode[index] = m_convertersMode[sourceIndex];
        m_activePowerSetpoint[index] = m_activePowerSetpoint[sourceIndex];
    }
}

HvdcConverterStation& HvdcLine::attach(HvdcConverterStation& converterStation) {
    converterStation.setHvdcLine(stdcxx::ref(*this));
    return converterStation;
}

void HvdcLine::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_convertersMode.resize(m_convertersMode.size() + number, m_convertersMode[sourceIndex]);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() + number, m_activePowerSetpoint[sourceIndex]);
}

double HvdcLine::getActivePowerSetpoint() const {
    return m_activePowerSetpoint.at(getNetwork().getVariantIndex());
}

const HvdcLine::ConvertersMode& HvdcLine::getConvertersMode() const {
    return m_convertersMode.at(getNetwork().getVariantIndex());
}

stdcxx::CReference<HvdcConverterStation> HvdcLine::getConverterStation(const HvdcLine::Side& side) const {
    return side == Side::ONE ? getConverterStation1() : getConverterStation2();
}

stdcxx::Reference<HvdcConverterStation> HvdcLine::getConverterStation(const HvdcLine::Side& side) {
    return side == Side::ONE ? getConverterStation1() : getConverterStation2();
}

stdcxx::CReference<HvdcConverterStation> HvdcLine::getConverterStation1() const {
    return stdcxx::cref(m_converterStation1);
}

stdcxx::Reference<HvdcConverterStation> HvdcLine::getConverterStation1() {
    return m_converterStation1;
}

stdcxx::CReference<HvdcConverterStation> HvdcLine::getConverterStation2() const {
    return stdcxx::cref(m_converterStation2);
}

stdcxx::Reference<HvdcConverterStation> HvdcLine::getConverterStation2() {
    return m_converterStation2;
}

double HvdcLine::getMaxP() const {
    return m_maxP;
}

const Network& HvdcLine::getNetwork() const {
    if (m_converterStation1) {
        return m_converterStation1.get().getNetwork();
    }
    if (m_converterStation2) {
        return m_converterStation2.get().getNetwork();
    }

    throw PowsyblException(getId() + " is not attached to a network");
}

Network& HvdcLine::getNetwork() {
    return const_cast<Network&>(static_cast<const HvdcLine*>(this)->getNetwork());
}

double HvdcLine::getNominalV() const {
    return m_nominalV;
}

double HvdcLine::getR() const {
    return m_r;
}

const IdentifiableType& HvdcLine::getType() const {
    static IdentifiableType s_type = IdentifiableType::HVDC_LINE;
    return s_type;
}

const std::string& HvdcLine::getTypeDescription() const {
    static std::string s_typeDescription = "hvdcLine";

    return s_typeDescription;
}

void HvdcLine::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_convertersMode.resize(m_convertersMode.size() - number);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() - number);
}

void HvdcLine::remove() {
    Network& network = getNetwork();

    // Detach converter stations
    m_converterStation1.get().setHvdcLine(stdcxx::ref<HvdcLine>());
    m_converterStation2.get().setHvdcLine(stdcxx::ref<HvdcLine>());
    m_converterStation1 = stdcxx::ref<HvdcConverterStation>();
    m_converterStation2 = stdcxx::ref<HvdcConverterStation>();

    network.remove(*this);
}

HvdcLine& HvdcLine::setActivePowerSetpoint(double activePowerSetpoint) {
    checkHvdcActivePowerSetpoint(*this, activePowerSetpoint, getNetwork().getMinimumValidationLevel());
    m_activePowerSetpoint[getNetwork().getVariantIndex()] = activePowerSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

HvdcLine& HvdcLine::setConvertersMode(const ConvertersMode& mode) {
    checkConvertersMode(*this, mode, getNetwork().getMinimumValidationLevel());
    m_convertersMode[getNetwork().getVariantIndex()] = mode;
    getNetwork().invalidateValidationLevel();
    return *this;
}

HvdcLine& HvdcLine::setMaxP(double maxP) {
    m_maxP = checkHvdcMaxP(*this, maxP);

    return *this;
}

HvdcLine& HvdcLine::setNominalV(double nominalV) {
    m_nominalV = checkNominalVoltage(*this, nominalV);

    return *this;
}

HvdcLine& HvdcLine::setR(double r) {
    m_r = checkR(*this, r);

    return *this;
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<HvdcLine::ConvertersMode>() {
    static std::initializer_list<std::string> s_sideNames {
        "SIDE_1_RECTIFIER_SIDE_2_INVERTER",
        "SIDE_1_INVERTER_SIDE_2_RECTIFIER"
    };
    return s_sideNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
