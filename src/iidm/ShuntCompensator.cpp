/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensator.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>

namespace powsybl {

namespace iidm {

ShuntCompensator::ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
    double bPerSection, unsigned long maximumSectionCount, unsigned long currentSectionCount, Terminal& terminal,
    bool voltageRegulatorOn, double targetV, double targetDeadband) :
    Injection(id, name, fictitious, ConnectableType::SHUNT_COMPENSATOR),
    m_bPerSection(checkbPerSection(*this, bPerSection)),
    m_maximumSectionCount(maximumSectionCount),
    m_currentSectionCount(network.getVariantManager().getVariantArraySize(), currentSectionCount),
    m_regulatingTerminal(terminal),
    m_voltageRegulatorOn(network.getVariantManager().getVariantArraySize(), voltageRegulatorOn),
    m_targetV(network.getVariantManager().getVariantArraySize(), targetV),
    m_targetDeadband(network.getVariantManager().getVariantArraySize(), targetDeadband) {
    checkSections(*this, currentSectionCount, maximumSectionCount);
}

void ShuntCompensator::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_currentSectionCount[index] = m_currentSectionCount[sourceIndex];
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_targetV[index] = m_targetV[sourceIndex];
        m_targetDeadband[index] = m_targetDeadband[sourceIndex];
    }
}

void ShuntCompensator::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_currentSectionCount.resize(m_currentSectionCount.size() + number, m_currentSectionCount[sourceIndex]);
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
    m_targetDeadband.resize(m_targetDeadband.size() + number, m_targetDeadband[sourceIndex]);
}

double ShuntCompensator::getbPerSection() const {
    return m_bPerSection;
}

double ShuntCompensator::getCurrentB() const {
    return m_bPerSection * getCurrentSectionCount();
}

unsigned long ShuntCompensator::getCurrentSectionCount() const {
    return m_currentSectionCount.at(getNetwork().getVariantIndex());
}

double ShuntCompensator::getMaximumB() const {
    return m_bPerSection * m_maximumSectionCount;
}

unsigned long ShuntCompensator::getMaximumSectionCount() const {
    return m_maximumSectionCount;
}

const Terminal& ShuntCompensator::getRegulatingTerminal() const {
    return m_regulatingTerminal.get();
}

Terminal& ShuntCompensator::getRegulatingTerminal() {
    return m_regulatingTerminal.get();
}

double ShuntCompensator::getTargetDeadband() const {
    return m_targetDeadband[getNetwork().getVariantIndex()];
}

double ShuntCompensator::getTargetV() const {
    return m_targetV[getNetwork().getVariantIndex()];
}

bool ShuntCompensator::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn[getNetwork().getVariantIndex()];
}

const std::string& ShuntCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "Shunt compensator";

    return s_typeDescription;
}

void ShuntCompensator::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_currentSectionCount.resize(m_currentSectionCount.size() - number);
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
    m_targetDeadband.resize(m_targetDeadband.size() - number);
}

ShuntCompensator& ShuntCompensator::setbPerSection(double bPerSection) {
    m_bPerSection = checkbPerSection(*this, bPerSection);

    return *this;
}

ShuntCompensator& ShuntCompensator::setCurrentSectionCount(unsigned long currentSectionCount) {
    checkSections(*this, currentSectionCount, m_maximumSectionCount);
    m_currentSectionCount[getNetwork().getVariantIndex()] = currentSectionCount;

    return *this;
}

ShuntCompensator& ShuntCompensator::setFictitious(bool fictitious) {
    Injection::setFictitious(fictitious);
    return *this;
}

ShuntCompensator& ShuntCompensator::setMaximumSectionCount(unsigned long maximumSectionCount) {
    checkSections(*this, getCurrentSectionCount(), maximumSectionCount);
    m_maximumSectionCount = maximumSectionCount;

    return *this;
}

ShuntCompensator& ShuntCompensator::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    checkRegulatingTerminal(*this, regulatingTerminal, getNetwork());
    m_regulatingTerminal = regulatingTerminal ? regulatingTerminal.get() : getTerminal();

    return *this;
}

ShuntCompensator& ShuntCompensator::setTargetDeadband(double targetDeadband) {
    checkTargetDeadband(*this, "shunt compensator", isVoltageRegulatorOn(), targetDeadband);
    m_targetDeadband[getNetwork().getVariantIndex()] = targetDeadband;

    return *this;
}

ShuntCompensator& ShuntCompensator::setTargetV(double targetV) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), targetV);
    m_targetV[getNetwork().getVariantIndex()] = targetV;

    return *this;
}

ShuntCompensator& ShuntCompensator::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getTargetV());
    checkTargetDeadband(*this, "shunt compensator", voltageRegulatorOn, m_targetDeadband[getNetwork().getVariantIndex()]);
    m_voltageRegulatorOn[getNetwork().getVariantIndex()] = voltageRegulatorOn;

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
