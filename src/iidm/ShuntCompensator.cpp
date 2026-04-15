/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensator.hpp>

#include <powsybl/iidm/ShuntCompensatorModel.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

ShuntCompensator::ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, std::unique_ptr<ShuntCompensatorModel>&& model,
                                   unsigned long currentSectionCount, const stdcxx::optional<unsigned long>& solvedSectionCount, stdcxx::Reference<Terminal>& regulatingTerminal, bool voltageRegulatorOn, double targetV, double targetDeadband) :
    Identifiable(id, name, fictitious),
    m_model(std::move(model)),
    m_sectionCount(network.getVariantManager().getVariantArraySize(), currentSectionCount),
    m_solvedSectionCount(network.getVariantManager().getVariantArraySize(), solvedSectionCount),
    m_regulatingTerminal(regulatingTerminal),
    m_voltageRegulatorOn(network.getVariantManager().getVariantArraySize(), voltageRegulatorOn),
    m_targetV(network.getVariantManager().getVariantArraySize(), targetV),
    m_targetDeadband(network.getVariantManager().getVariantArraySize(), targetDeadband) {
        if(static_cast<bool>(m_regulatingTerminal)) {
            m_regulatingTerminal.get().registerReferrer(*this);
        }

    m_model->attach(*this);

    checkSolvedSection(*this, solvedSectionCount, m_model->getMaximumSectionCount());
}

void ShuntCompensator::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_sectionCount[index] = m_sectionCount[sourceIndex];
        m_solvedSectionCount[index] = m_solvedSectionCount[sourceIndex];
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_targetV[index] = m_targetV[sourceIndex];
        m_targetDeadband[index] = m_targetDeadband[sourceIndex];
    }
}

void ShuntCompensator::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_sectionCount.resize(m_sectionCount.size() + number, m_sectionCount[sourceIndex]);
    m_solvedSectionCount.resize(m_solvedSectionCount.size() + number, m_solvedSectionCount[sourceIndex]);
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
    m_targetDeadband.resize(m_targetDeadband.size() + number, m_targetDeadband[sourceIndex]);
}

double ShuntCompensator::getB() const {
    return m_model->getB(m_sectionCount[getNetwork().getVariantIndex()]);
}

double ShuntCompensator::getB(unsigned long sectionCount) const {
    return m_model->getB(sectionCount);
}

double ShuntCompensator::getG() const {
    return m_model->getG(m_sectionCount[getNetwork().getVariantIndex()]);
}

double ShuntCompensator::getG(unsigned long sectionCount) const {
    return m_model->getG(sectionCount);
}

unsigned long ShuntCompensator::getMaximumSectionCount() const {
    return m_model->getMaximumSectionCount();
}

const ShuntCompensatorModel& ShuntCompensator::getModel() const {
    return *m_model;
}

ShuntCompensatorModel& ShuntCompensator::getModel() {
    return *m_model;
}

const ShuntCompensatorModelType& ShuntCompensator::getModelType() const {
    return m_model->getType();
}

const Terminal& ShuntCompensator::getRegulatingTerminal() const {
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
}

Terminal& ShuntCompensator::getRegulatingTerminal() {
    return (static_cast<bool>(m_regulatingTerminal)) ? m_regulatingTerminal.get() : getTerminal();
}

unsigned long ShuntCompensator::getSectionCount() const {
    return m_sectionCount[getNetwork().getVariantIndex()];
}

stdcxx::optional<unsigned long> ShuntCompensator::getSolvedSectionCount() const {
    return m_solvedSectionCount[getNetwork().getVariantIndex()];
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

const IdentifiableType& ShuntCompensator::getType() const {
    static IdentifiableType s_type = IdentifiableType::SHUNT_COMPENSATOR;
    return s_type;
}

const std::string& ShuntCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "Shunt compensator";

    return s_typeDescription;
}

void ShuntCompensator::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_sectionCount.resize(m_sectionCount.size() - number);
    m_solvedSectionCount.resize(m_solvedSectionCount.size() - number);
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
    m_targetDeadband.resize(m_targetDeadband.size() - number);
}

ShuntCompensator& ShuntCompensator::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
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

ShuntCompensator& ShuntCompensator::setSectionCount(unsigned long sectionCount) {
    checkSections(*this, sectionCount, m_model->getMaximumSectionCount(), getNetwork().getMinimumValidationLevel());
    unsigned long variantIndex = getNetwork().getVariantIndex();
    m_sectionCount[variantIndex] = sectionCount;
    getNetwork().invalidateValidationLevel();
    return *this;
}

ShuntCompensator& ShuntCompensator::setSolvedSectionCount(unsigned long solvedSectionCount) {
    checkSolvedSection(*this, solvedSectionCount, m_model->getMaximumSectionCount());
    unsigned long variantIndex = getNetwork().getVariantIndex();
    m_solvedSectionCount[variantIndex] = solvedSectionCount;
    return *this;
}
ShuntCompensator& ShuntCompensator::unsetSolvedSectionCount() {
    unsigned long variantIndex = getNetwork().getVariantIndex();
    m_solvedSectionCount[variantIndex].reset();
    return *this;
}

ShuntCompensator& ShuntCompensator::setTargetDeadband(double targetDeadband) {
    checkTargetDeadband(*this, "shunt compensator", isVoltageRegulatorOn(), targetDeadband, getNetwork().getMinimumValidationLevel());
    m_targetDeadband[getNetwork().getVariantIndex()] = targetDeadband;
    getNetwork().invalidateValidationLevel();
    return *this;
}

ShuntCompensator& ShuntCompensator::setTargetV(double targetV) {
    checkVoltageControl(*this, isVoltageRegulatorOn(), targetV, getNetwork().getMinimumValidationLevel());
    m_targetV[getNetwork().getVariantIndex()] = targetV;
    getNetwork().invalidateValidationLevel();
    return *this;
}

ShuntCompensator& ShuntCompensator::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    checkVoltageControl(*this, voltageRegulatorOn, getTargetV(), getNetwork().getMinimumValidationLevel());
    checkTargetDeadband(*this, "shunt compensator", voltageRegulatorOn, m_targetDeadband[getNetwork().getVariantIndex()], getNetwork().getMinimumValidationLevel());
    m_voltageRegulatorOn[getNetwork().getVariantIndex()] = voltageRegulatorOn;
    getNetwork().invalidateValidationLevel();
    return *this;
}

void ShuntCompensator::remove() {
    if(static_cast<bool>(m_regulatingTerminal)){
        m_regulatingTerminal.get().unregisterReferrer(*this);
    }
    Injection::remove();
}

void ShuntCompensator::onReferencedRemoval(Terminal& /*removedReference*/) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ShuntCompensator>();
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

void ShuntCompensator::onReferencedReplacement(Terminal& oldReference, Terminal& newReference) {
    checkRegulatingTerminal(*this, stdcxx::ref(newReference), getNetwork());
    if(static_cast<bool>(m_regulatingTerminal) && stdcxx::areSame(m_regulatingTerminal.get(), oldReference)) {
        m_regulatingTerminal = newReference;
        newReference.registerReferrer(*this);
    }
}

void ShuntCompensator::applySolvedValues() {
    setSectionCountToSolvedSectionCount();
}
void ShuntCompensator::setSectionCountToSolvedSectionCount() {
    auto solvedValue = getSolvedSectionCount();
    if(solvedValue.has_value()) {
        setSectionCount(solvedValue.get());
    }
}

}  // namespace iidm

}  // namespace powsybl
