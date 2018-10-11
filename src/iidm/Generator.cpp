/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Generator.hpp>

#include <powsybl/iidm/StateManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Generator::Generator(powsybl::iidm::MultiStateObject& network, const std::string& id, const std::string& name,
                     const powsybl::iidm::EnergySource& energySource, double minP, double maxP, bool voltageRegulatorOn,
                     const stdcxx::Reference<powsybl::iidm::Terminal>& regulatingTerminal, double activePowerSetpoint,
                     double reactivePowerSetpoint, double voltageSetpoint, double ratedS) :
    Injection(id, name, ConnectableType::GENERATOR),
    m_energySource(energySource),
    m_minP(minP),
    m_maxP(maxP),
    m_ratedS(ratedS),
    m_regulatingTerminal(regulatingTerminal),
    m_voltageRegulatorOn(network.getStateManager().getStateArraySize(), voltageRegulatorOn),
    m_activePowerSetpoint(network.getStateManager().getStateArraySize(), activePowerSetpoint),
    m_reactivePowerSetpoint(network.getStateManager().getStateArraySize(), reactivePowerSetpoint),
    m_voltageSetpoint(network.getStateManager().getStateArraySize(), voltageSetpoint) {
}

void Generator::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateStateArrayElement(indexes, sourceIndex);

    for (unsigned long index : indexes) {
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_activePowerSetpoint[index] = m_activePowerSetpoint[sourceIndex];
        m_reactivePowerSetpoint[index] = m_reactivePowerSetpoint[sourceIndex];
        m_voltageSetpoint[index] = m_voltageSetpoint[sourceIndex];
    }
}

void Generator::deleteStateArrayElement(unsigned long index) {
    Injection::deleteStateArrayElement(index);
}

void Generator::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendStateArraySize(initStateArraySize, number, sourceIndex);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() + number, m_activePowerSetpoint[sourceIndex]);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() + number, m_reactivePowerSetpoint[sourceIndex]);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() + number, m_voltageSetpoint[sourceIndex]);
}

double Generator::getActivePowerSetpoint() const {
    return m_activePowerSetpoint.at(m_network.get().getStateIndex());
}

const EnergySource& Generator::getEnergySource() const {
    return m_energySource;
}

double Generator::getMaxP() const {
    return m_maxP;
}

double Generator::getMinP() const {
    return m_minP;
}

double Generator::getRatedS() const {
    return m_ratedS;
}

double Generator::getReactivePowerSetpoint() const {
    return m_reactivePowerSetpoint.at(m_network.get().getStateIndex());
}

const stdcxx::Reference<Terminal>& Generator::getRegulatingTerminal() const {
    return m_regulatingTerminal;
}

stdcxx::Reference<Terminal> Generator::getRegulatingTerminal() {
    return m_regulatingTerminal;
}

double Generator::getTargetP() const {
    return getActivePowerSetpoint();
}

double Generator::getTargetQ() const {
    return getReactivePowerSetpoint();
}

double Generator::getTargetV() const {
    return getVoltageSetpoint();
}

const std::string& Generator::getTypeDescription() const {
    static std::string s_typeDescription = "Generator";

    return s_typeDescription;
}

double Generator::getVoltageSetpoint() const {
    return m_voltageSetpoint.at(m_network.get().getStateIndex());
}

bool Generator::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn.at(m_network.get().getStateIndex());
}

void Generator::reduceStateArraySize(unsigned long number) {
    Injection::reduceStateArraySize(number);

    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_activePowerSetpoint.resize(m_activePowerSetpoint.size() - number);
    m_reactivePowerSetpoint.resize(m_reactivePowerSetpoint.size() - number);
    m_voltageSetpoint.resize(m_voltageSetpoint.size() - number);
}

Generator& Generator::setActivePowerSetpoint(double activePowerSetpoint) {
    m_activePowerSetpoint[m_network.get().getStateIndex()] = checkActivePowerSetpoint(*this, activePowerSetpoint);
    return *this;
}

Generator& Generator::setEnergySource(const EnergySource& energySource) {
    m_energySource = energySource;
    return *this;
}

Generator& Generator::setMaxP(double maxP) {
    checkMaxP(*this, maxP);
    checkActiveLimits(*this, m_minP, maxP);
    m_maxP = maxP;
    return *this;
}

Generator& Generator::setMinP(double minP) {
    checkMinP(*this, minP);
    checkActiveLimits(*this, minP, m_maxP);
    m_minP = minP;
    return *this;
}

Generator& Generator::setRatedS(double ratedS) {
    m_ratedS = checkRatedS(*this, ratedS);
    return *this;
}

Generator& Generator::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), getVoltageSetpoint(), reactivePowerSetpoint);
    m_reactivePowerSetpoint[m_network.get().getStateIndex()] = reactivePowerSetpoint;
    return *this;
}

Generator& Generator::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    if (static_cast<bool>(regulatingTerminal)) {
        checkRegulatingTerminal(*this, regulatingTerminal.get(), getNetwork());
        m_regulatingTerminal = regulatingTerminal;
    } else {
        m_regulatingTerminal = stdcxx::ref(getTerminal());
    }
    return *this;
}

Generator& Generator::setTargetP(double activePowerSetpoint) {
    return setActivePowerSetpoint(activePowerSetpoint);
}

Generator& Generator::setTargetQ(double reactivePowerSetpoint) {
    return setReactivePowerSetpoint(reactivePowerSetpoint);
}

Generator& Generator::setTargetV(double voltageSetpoint) {
    return setVoltageSetpoint(voltageSetpoint);
}

Generator& Generator::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getTargetV(), getTargetQ());
    m_voltageRegulatorOn[m_network.get().getStateIndex()] = voltageRegulatorOn;
    return *this;
}

Generator& Generator::setVoltageSetpoint(double voltageSetpoint) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), voltageSetpoint, getReactivePowerSetpoint());
    m_voltageSetpoint[m_network.get().getStateIndex()] = voltageSetpoint;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
