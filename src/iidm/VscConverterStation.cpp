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

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

VscConverterStation::VscConverterStation(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, double lossFactor, bool voltageRegulatorOn, double reactivePowerSetpoint, double voltageSetpoint, stdcxx::Reference<Terminal>& regulatingTerminal) :
    Identifiable(id, name, fictitious),
    HvdcConverterStation(lossFactor),
    m_voltageRegulatorOn(network.getVariantManager().getVariantArraySize(), voltageRegulatorOn),
    m_reactivePowerSetpoint(network.getVariantManager().getVariantArraySize(), reactivePowerSetpoint),
    m_voltageSetpoint(network.getVariantManager().getVariantArraySize(), voltageSetpoint),
    m_regulatingTerminal(regulatingTerminal) {
    if(static_cast<bool>(m_regulatingTerminal)) {
        m_regulatingTerminal.get().registerReferrer(*this);
    }
    ValidationLevel vl = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (stdcxx::isInstanceOf<Network>(network)) {
        auto& n = dynamic_cast<Network&>(network);
        vl = n.getMinimumValidationLevel();
    }
    checkVoltageControl(*this, voltageRegulatorOn, voltageSetpoint, reactivePowerSetpoint, vl);
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

const Terminal& VscConverterStation::getRegulatingTerminal() const {
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
}

Terminal& VscConverterStation::getRegulatingTerminal() {
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
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

VscConverterStation& VscConverterStation::setLossFactor(double lossFactor) {
    return dynamic_cast<VscConverterStation&>(HvdcConverterStation::setLossFactor(lossFactor));
}

VscConverterStation& VscConverterStation::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), getVoltageSetpoint(), reactivePowerSetpoint, getNetwork().getMinimumValidationLevel());
    m_reactivePowerSetpoint[getNetwork().getVariantIndex()] = reactivePowerSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

VscConverterStation& VscConverterStation::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    checkRegulatingTerminal(*this, regulatingTerminal, getNetwork());

    if(static_cast<bool>(m_regulatingTerminal)){
        m_regulatingTerminal.get().unregisterReferrer(*this);
        m_regulatingTerminal = stdcxx::Reference<Terminal>();
    }
    if(static_cast<bool>(regulatingTerminal)) {
        m_regulatingTerminal = regulatingTerminal;
        m_regulatingTerminal.get().registerReferrer(*this);
    }

    return *this;
}

VscConverterStation& VscConverterStation::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getVoltageSetpoint(), getReactivePowerSetpoint(), getNetwork().getMinimumValidationLevel());
    m_voltageRegulatorOn[getNetwork().getVariantIndex()] = voltageRegulatorOn;
    getNetwork().invalidateValidationLevel();
    return *this;
}

VscConverterStation& VscConverterStation::setVoltageSetpoint(double voltageSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), voltageSetpoint, getReactivePowerSetpoint(), getNetwork().getMinimumValidationLevel());
    m_voltageSetpoint[getNetwork().getVariantIndex()] = voltageSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

void VscConverterStation::remove() {
    if(static_cast<bool>(m_regulatingTerminal)){
        m_regulatingTerminal.get().unregisterReferrer(*this);
    }
    HvdcConverterStation::remove();
}

void VscConverterStation::onReferencedRemoval(Terminal& /*removedReference*/) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<VscConverterStation>();
    if(static_cast<bool>(m_regulatingTerminal)) {
        Terminal& oldRegulatingTerminal = m_regulatingTerminal.get();
        Terminal& localTerminal = getTerminal();

        auto bus = oldRegulatingTerminal.getBusView().getBus();
        auto localBus = localTerminal.getBusView().getBus();
        if (static_cast<bool>(bus) && stdcxx::areSame(bus, localBus)) {
            // local voltage regulation, we keep the regulating status, and re-locate the regulation at the regulated equipment
            logger.warn(stdcxx::format("Connectable %1% was a local voltage regulation point for %2%. Regulation terminal is re-located at %3%.",
                        oldRegulatingTerminal.getConnectable().get().getId(), getId(), getId()));
        
            m_regulatingTerminal = localTerminal;
            return;
        } else {
            logger.warn(stdcxx::format("Connectable %1% was a regulating terminal for (%2%). Regulation is deactivated",
                        oldRegulatingTerminal.getConnectable().get().getId(), getId()));
            m_regulatingTerminal = stdcxx::Reference<Terminal>();
        }
    }

    m_voltageRegulatorOn.assign(m_voltageRegulatorOn.size(), false);
}

void VscConverterStation::onReferencedReplacement(Terminal& /*oldReference*/, Terminal& newReference) {
    checkRegulatingTerminal(*this, stdcxx::ref(newReference), getNetwork());
    m_regulatingTerminal = newReference;
    newReference.registerReferrer(*this);
}

}  // namespace iidm

}  // namespace powsybl
