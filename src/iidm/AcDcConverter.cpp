/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AcDcConverter.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

AcDcConverter::AcDcConverter(unsigned long variantArraySize,
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const AcDcConverter::ControlMode& controlMode,
                    double targetP, double targetVdc) :
    m_idleLoss(idleLoss),
    m_switchingLoss(switchingLoss),
    m_resistiveLoss(resistiveLoss),
    m_pccRegulatingTerminal(pccTerminal),
    m_controlMode(variantArraySize, controlMode),
    m_targetP(variantArraySize, targetP),
    m_targetVdc(variantArraySize, targetVdc) {
    if(static_cast<bool>(m_pccRegulatingTerminal)) {
        m_pccRegulatingTerminal.get().registerReferrer(*this);
    }
}

const Network& AcDcConverter::getNetwork() const {
    return Connectable::getNetwork();
}
Network& AcDcConverter::getNetwork() {
    return Connectable::getNetwork();
}
const Network& AcDcConverter::getParentNetwork() const {
    return Connectable::getParentNetwork();
}
Network& AcDcConverter::getParentNetwork() {
    return Connectable::getParentNetwork();
}

void AcDcConverter::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);
    for (unsigned long terminalIndex = 0 ; terminalIndex < getTerminalsCount(); terminalIndex++) {
        Connectable::getTerminal(terminalIndex).allocateVariantArrayElement(indexes, sourceIndex);
    }
    for (unsigned long dcterminalIndex = 0 ; dcterminalIndex < getDcTerminalsCount(); dcterminalIndex++) {
        DcConnectable::getDcTerminal(dcterminalIndex).allocateVariantArrayElement(indexes, sourceIndex);
    }
    for (unsigned long index : indexes) {
        m_controlMode[index] = m_controlMode[sourceIndex];
        m_targetP[index] = m_targetP[sourceIndex];
        m_targetVdc[index] = m_targetVdc[sourceIndex];
    }
}

void AcDcConverter::deleteVariantArrayElement(unsigned long index) {
    Identifiable::deleteVariantArrayElement(index);
    for (unsigned long terminalIndex = 0 ; terminalIndex < getTerminalsCount(); terminalIndex++) {
        Connectable::getTerminal(terminalIndex).deleteVariantArrayElement(index);
    }
    for (unsigned long dcterminalIndex = 0 ; dcterminalIndex < getDcTerminalsCount(); dcterminalIndex++) {
        DcConnectable::getDcTerminal(dcterminalIndex).deleteVariantArrayElement(index);
    }
}

void AcDcConverter::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    for (unsigned long terminalIndex = 0 ; terminalIndex < getTerminalsCount(); terminalIndex++) {
        Connectable::getTerminal(terminalIndex).extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    for (unsigned long dcterminalIndex = 0 ; dcterminalIndex < getDcTerminalsCount(); dcterminalIndex++) {
        DcConnectable::getDcTerminal(dcterminalIndex).extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }

    m_controlMode.resize(m_controlMode.size() + number, m_controlMode[sourceIndex]);
    m_targetP.resize(m_targetP.size() + number, m_targetP[sourceIndex]);
    m_targetVdc.resize(m_targetVdc.size() + number, m_targetVdc[sourceIndex]);
}

void AcDcConverter::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);
    for (unsigned long terminalIndex = 0 ; terminalIndex < getTerminalsCount(); terminalIndex++) {
        Connectable::getTerminal(terminalIndex).reduceVariantArraySize(number);
    }
    for (unsigned long dcterminalIndex = 0 ; dcterminalIndex < getDcTerminalsCount(); dcterminalIndex++) {
        DcConnectable::getDcTerminal(dcterminalIndex).reduceVariantArraySize(number);
    }

    m_controlMode.resize(m_controlMode.size() - number);
    m_targetP.resize(m_targetP.size() - number);
    m_targetVdc.resize(m_targetVdc.size() - number);
}

void AcDcConverter::onReferencedRemoval(Terminal& /*removedReference*/) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<AcDcConverter>();

    if(static_cast<bool>(m_pccRegulatingTerminal)) {
        logger.warn(stdcxx::format("Connectable %1% was a regulating terminal for (%2%). Regulation is deactivated",
                        m_pccRegulatingTerminal.get().getConnectable().get().getId(), getId()));
        m_pccRegulatingTerminal = stdcxx::Reference<Terminal>();
    }
    m_controlMode.assign(m_controlMode.size(), ControlMode::V_DC);
}
void AcDcConverter::onReferencedReplacement(Terminal& /*oldReference*/, Terminal& newReference) {
    checkAcDcConverterPccTerminal(*this, static_cast<bool>(getTerminal2()), stdcxx::ref(newReference), getTerminal1().getVoltageLevel());
    m_pccRegulatingTerminal = newReference;
    newReference.registerReferrer(*this);
}

const Terminal& AcDcConverter::getTerminal1() const {
    return Connectable::getTerminal(0);
}

Terminal& AcDcConverter::getTerminal1() {
    return Connectable::getTerminal(0);
}

stdcxx::CReference<Terminal> AcDcConverter::getTerminal2() const {
    if(getTerminalsCount()>1) {
        return stdcxx::cref(Connectable::getTerminal(1));
    }
    return stdcxx::CReference<Terminal>();
}

stdcxx::Reference<Terminal> AcDcConverter::getTerminal2() {
    if(getTerminalsCount()>1) {
        return stdcxx::ref(Connectable::getTerminal(1));
    }
    return stdcxx::Reference<Terminal>();
}

const Terminal& AcDcConverter::getTerminal(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getTerminal1();

        case TwoSides::TWO: {
            auto terminal2Ref = getTerminal2();
            if(!terminal2Ref) {
                throw PowsyblException(stdcxx::format("AC/DC Converter '%1%' does not have a second AC Terminal", getId()));
            }
            return terminal2Ref.get();
        }

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

Terminal& AcDcConverter::getTerminal(const TwoSides& side) {
    return const_cast<Terminal&>(static_cast<const AcDcConverter*>(this)->getTerminal(side));
}

const DcTerminal& AcDcConverter::getDcTerminal1() const {
    return DcConnectable::getDcTerminal(0);
}

DcTerminal& AcDcConverter::getDcTerminal1() {
    return DcConnectable::getDcTerminal(0);
}

const DcTerminal& AcDcConverter::getDcTerminal2() const {
    return DcConnectable::getDcTerminal(1);
}

DcTerminal& AcDcConverter::getDcTerminal2() {
    return DcConnectable::getDcTerminal(1);
}

const DcTerminal& AcDcConverter::getDcTerminal(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getDcTerminal1();

        case TwoSides::TWO:
            return getDcTerminal2();

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

DcTerminal& AcDcConverter::getDcTerminal(const TwoSides& side) {
    return const_cast<DcTerminal&>(static_cast<const AcDcConverter*>(this)->getDcTerminal(side));
}

TwoSides AcDcConverter::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(getTerminal1(), terminal)) {
        return TwoSides::ONE;
    }
    
    auto terminal2Ref = getTerminal2();
    if(static_cast<bool>(terminal2Ref) && stdcxx::areSame(terminal2Ref.get(), terminal)) {
        return TwoSides::TWO;
    }
    
    throw PowsyblException(stdcxx::format("The terminal is not connected to this AC/DC Converter"));
    return TwoSides::UNDEFINED;
}

TwoSides AcDcConverter::getSide(const DcTerminal& terminal) const {
    if (stdcxx::areSame(terminal, getDcTerminal1())) {
        return TwoSides::ONE;
    } else if (stdcxx::areSame(terminal, getDcTerminal2())) {
        return TwoSides::TWO;
    }

    throw AssertionError("The DC terminal is not connected to this AC/DC Converter");
    return TwoSides::UNDEFINED;
}


AcDcConverter& AcDcConverter::setIdleLoss(double idleLoss) {
    checkDoubleParamPositive(*this, idleLoss, "idleLoss");
    m_idleLoss = idleLoss;
    return *this;
}

double AcDcConverter::getIdleLoss() const {
    return m_idleLoss;
}

AcDcConverter& AcDcConverter::setSwitchingLoss(double switchingLoss) {
    checkDoubleParamPositive(*this, switchingLoss, "switchingLoss");
    m_switchingLoss = switchingLoss;
    return *this;
}

double AcDcConverter::getSwitchingLoss() const {
    return m_switchingLoss;
}

AcDcConverter& AcDcConverter::setResistiveLoss(double resistiveLoss) {
    checkDoubleParamPositive(*this, resistiveLoss, "resistiveLoss");
    m_resistiveLoss = resistiveLoss;
    return *this;
}

double AcDcConverter::getResistiveLoss() const {
    return m_resistiveLoss;
}

AcDcConverter& AcDcConverter::setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal) {
    checkAcDcConverterPccTerminal(*this, static_cast<bool>(getTerminal2()), pccTerminal, getTerminal1().getVoltageLevel());

    if(static_cast<bool>(m_pccRegulatingTerminal)) {
        m_pccRegulatingTerminal.get().unregisterReferrer(*this);
        m_pccRegulatingTerminal = stdcxx::Reference<Terminal>();
    }
    if(static_cast<bool>(pccTerminal)) {
        m_pccRegulatingTerminal = pccTerminal;
        m_pccRegulatingTerminal.get().registerReferrer(*this);
    }

    return *this;
}

const Terminal& AcDcConverter::getPccTerminal() const {
    return (static_cast<bool>(m_pccRegulatingTerminal)) ? m_pccRegulatingTerminal.get() : getTerminal1();
}

Terminal& AcDcConverter::getPccTerminal() {
    return (static_cast<bool>(m_pccRegulatingTerminal)) ? m_pccRegulatingTerminal.get() : getTerminal1();
}

AcDcConverter& AcDcConverter::setControlMode(const ControlMode& controlMode) {
    Network& n = getNetwork();
    checkAcDcConverterControl(*this, controlMode, getTargetP(), getTargetVdc(), n.getMinimumValidationLevel());
    m_controlMode[n.getVariantIndex()] = controlMode;
    n.invalidateValidationLevel();
    return *this;
}

const AcDcConverter::ControlMode& AcDcConverter::getControlMode() const {
    return m_controlMode.at(getNetwork().getVariantIndex());
}

AcDcConverter& AcDcConverter::setTargetP(double targetP) {
    Network& n = getNetwork();
    checkAcDcConverterControl(*this, getControlMode(), targetP, getTargetVdc(), n.getMinimumValidationLevel());
    m_targetP[n.getVariantIndex()] = targetP;
    n.invalidateValidationLevel();
    return *this;
}

double AcDcConverter::getTargetP() const {
    return m_targetP.at(getNetwork().getVariantIndex());
}

AcDcConverter& AcDcConverter::setTargetVdc(double targetVdc) {
    Network& n = getNetwork();
    checkAcDcConverterControl(*this, getControlMode(), getTargetP(), targetVdc, n.getMinimumValidationLevel());
    m_targetVdc[n.getVariantIndex()] = targetVdc;
    n.invalidateValidationLevel();
    return *this;
}

double AcDcConverter::getTargetVdc() const {
    return m_targetVdc.at(getNetwork().getVariantIndex());
}


void AcDcConverter::remove() {
    if(static_cast<bool>(m_pccRegulatingTerminal)){
        m_pccRegulatingTerminal.get().unregisterReferrer(*this);
    }

    //no use of ReferrerManager<DcTerminal> so we do not have to worry about dcTerminals owned by DcConnectable :
    Connectable::remove();
}



namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<AcDcConverter::ControlMode>() {
    static std::initializer_list<std::string> s_acdcConverterControlModeNames {
        "P_PCC",
        "V_DC"
    };
    return s_acdcConverterControlModeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
