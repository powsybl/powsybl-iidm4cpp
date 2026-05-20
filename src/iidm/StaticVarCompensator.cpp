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

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

StaticVarCompensator::StaticVarCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
        double bMin, double bMax, double voltageSetpoint, double reactivePowerSetpoint, 
        const RegulationMode& regulationMode, bool regulating, stdcxx::Reference<Terminal>& regulatingTerminal) :
    Identifiable(id, name, fictitious),
    m_bMin(checkBmin(*this, bMin)),
    m_bMax(checkBmax(*this, bMax)),
    m_voltageSetpoint(network.getVariantManager().getVariantArraySize(), voltageSetpoint),
    m_reactivePowerSetpoint(network.getVariantManager().getVariantArraySize(), reactivePowerSetpoint),
    m_regulatingTerminal(regulatingTerminal),
    m_regulationMode(network.getVariantManager().getVariantArraySize(), regulationMode),
    m_regulationOn(network.getVariantManager().getVariantArraySize(), regulating) {
    if(static_cast<bool>(m_regulatingTerminal)) {
        m_regulatingTerminal.get().registerReferrer(*this);
    }
    ValidationLevel vl = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (stdcxx::isInstanceOf<Network>(network)) {
        auto& n = dynamic_cast<Network&>(network);
        vl = n.getMinimumValidationLevel();
    }
    checkSvcRegulator(*this, regulating, voltageSetpoint, reactivePowerSetpoint, regulationMode, vl);
}

void StaticVarCompensator::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_voltageSetpoint[index] = m_voltageSetpoint[sourceIndex];
        m_reactivePowerSetpoint[index] = m_reactivePowerSetpoint[sourceIndex];
        m_regulationMode[index] = m_regulationMode[sourceIndex];
        m_regulationOn[index] = m_regulationOn[sourceIndex];
    }
}

void StaticVarCompensator::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_voltageSetpoint.resize(m_voltageSetpoint.size() + number, m_voltageSetpoint[sourceIndex]);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() + number, m_reactivePowerSetpoint[sourceIndex]);
    m_regulationMode.resize(m_regulationMode.size() + number, m_regulationMode[sourceIndex]);
    m_regulationOn.resize(m_regulationOn.size() + number, m_regulationOn[sourceIndex]);
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
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
}

Terminal& StaticVarCompensator::getRegulatingTerminal() {
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
}

const StaticVarCompensator::RegulationMode& StaticVarCompensator::getRegulationMode() const {
    return m_regulationMode.at(getNetwork().getVariantIndex());
}

const IdentifiableType& StaticVarCompensator::getType() const {
    static IdentifiableType s_type = IdentifiableType::STATIC_VAR_COMPENSATOR;
    return s_type;
}

const std::string& StaticVarCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "Static var compensator";

    return s_typeDescription;
}

double StaticVarCompensator::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(getNetwork().getVariantIndex());
}

bool StaticVarCompensator::isRegulating() const {
    return m_regulationOn.at(getNetwork().getVariantIndex());
}

void StaticVarCompensator::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_voltageSetpoint.resize(m_voltageSetpoint.size() - number);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() - number);
    m_regulationMode.resize(m_regulationMode.size() - number);
    m_regulationOn.resize(m_regulationOn.size() - number);
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
    checkSvcRegulator(*this, isRegulating(), getVoltageSetpoint(), reactivePowerSetpoint, getRegulationMode(), getNetwork().getMinimumValidationLevel());
    m_reactivePowerSetpoint[getNetwork().getVariantIndex()] = reactivePowerSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulating(bool regulating) {
    checkSvcRegulator(*this, regulating, getVoltageSetpoint(), getReactivePowerSetpoint(), getRegulationMode(), getNetwork().getMinimumValidationLevel());
    m_regulationOn[getNetwork().getVariantIndex()] = regulating;
    getNetwork().invalidateValidationLevel();
    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    checkRegulatingTerminal(*this, regulatingTerminal, getNetwork());
    if(static_cast<bool>(m_regulatingTerminal)) {
        m_regulatingTerminal.get().unregisterReferrer(*this);
        m_regulatingTerminal = stdcxx::Reference<Terminal>();
    }
    if(static_cast<bool>(regulatingTerminal)) {
        m_regulatingTerminal = regulatingTerminal;
        m_regulatingTerminal.get().registerReferrer(*this);
    }
    return *this;
}

StaticVarCompensator& StaticVarCompensator::setRegulationMode(const RegulationMode& regulationMode) {
    checkSvcRegulator(*this, isRegulating(), getVoltageSetpoint(), getReactivePowerSetpoint(), regulationMode, getNetwork().getMinimumValidationLevel());
    m_regulationMode[getNetwork().getVariantIndex()] = regulationMode;
    getNetwork().invalidateValidationLevel();
    return *this;
}

StaticVarCompensator& StaticVarCompensator::setVoltageSetpoint(double voltageSetpoint) {
    checkSvcRegulator(*this, isRegulating(), voltageSetpoint, getReactivePowerSetpoint(), getRegulationMode(), getNetwork().getMinimumValidationLevel());
    m_voltageSetpoint[getNetwork().getVariantIndex()] = voltageSetpoint;
    getNetwork().invalidateValidationLevel();
    return *this;
}

void StaticVarCompensator::remove() {
    if(static_cast<bool>(m_regulatingTerminal)){
        m_regulatingTerminal.get().unregisterReferrer(*this);
    }
    Injection::remove();
}

void StaticVarCompensator::onReferencedRemoval(Terminal& /*removedReference*/) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<StaticVarCompensator>();

    //At least one variant set in Voltage regulation ?
    bool bUseVoltageRegulation = std::find(m_regulationMode.begin(), m_regulationMode.end(), RegulationMode::VOLTAGE) != m_regulationMode.end();

    if(static_cast<bool>(m_regulatingTerminal)) {
        Terminal& oldRegulatingTerminal = m_regulatingTerminal.get();
        Terminal& localTerminal = getTerminal();

        auto bus = oldRegulatingTerminal.getBusView().getBus();
        auto localBus = localTerminal.getBusView().getBus();
        if (bUseVoltageRegulation && static_cast<bool>(bus) && stdcxx::areSame(bus, localBus)) {
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
    m_regulationOn.assign(m_regulationOn.size(), false);
    m_regulationMode.assign(m_regulationMode.size(), RegulationMode::VOLTAGE);
}

void StaticVarCompensator::onReferencedReplacement(Terminal& /*oldReference*/, Terminal& newReference) {
    checkRegulatingTerminal(*this, stdcxx::ref(newReference), getNetwork());
    m_regulatingTerminal = newReference;
    newReference.registerReferrer(*this);
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<StaticVarCompensator::RegulationMode>() {
    static std::initializer_list<std::string> s_staticVarCompensatorRegulationNames {
        "VOLTAGE",
        "REACTIVE_POWER"
    };
    return s_staticVarCompensatorRegulationNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
