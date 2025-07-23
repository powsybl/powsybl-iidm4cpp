/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerStep.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

PhaseTapChangerAdder::StepAdder::StepAdder(PhaseTapChangerAdder& parent) :
    m_parent(parent) {
}

PhaseTapChangerAdder& PhaseTapChangerAdder::StepAdder::endStep() {
    checkOptional(m_parent.getValidable(), m_alpha, "step alpha is not set");
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    return m_parent.addStep(m_alpha, m_rho, m_r, m_x, m_g, m_b);
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setAlpha(double alpha) {
    m_alpha = alpha;
    return *this;
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setB(double b) {
    m_b = b;
    return *this;
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setG(double g) {
    m_g = g;
    return *this;
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setR(double r) {
    m_r = r;
    return *this;
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setRho(double rho) {
    m_rho = rho;
    return *this;
}

PhaseTapChangerAdder::StepAdder& PhaseTapChangerAdder::StepAdder::setX(double x) {
    m_x = x;
    return *this;
}

PhaseTapChangerAdder::PhaseTapChangerAdder(PhaseTapChangerHolder& parent) :
    m_parent(parent) {
}

PhaseTapChanger& PhaseTapChangerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<PhaseTapChangerAdder>();
    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkOptional(m_parent, m_tapPosition, "tap position is not set", network.getMinimumValidationLevel()));
    if (m_steps.empty()) {
        throw ValidationException(m_parent, "a phase tap changer should have at least one step");
    }
    long highTapPosition = m_lowTapPosition + m_steps.size() - 1;
    if(m_tapPosition) {
        network.setValidationLevelIfGreaterThan(checkTapPosition(m_parent, *m_tapPosition, m_lowTapPosition, highTapPosition, network.getMinimumValidationLevel()));
    }
    network.setValidationLevelIfGreaterThan(checkPhaseTapChangerRegulation(m_parent, m_regulationMode, m_regulationValue, m_regulating, stdcxx::cref(m_regulationTerminal.get()), network, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkTargetDeadband(m_parent, "phase tap changer", m_regulating, m_targetDeadband, network.getMinimumValidationLevel()));

    std::unique_ptr<PhaseTapChanger> ptrPhaseTapChanger = stdcxx::make_unique<PhaseTapChanger>(m_parent, m_lowTapPosition, m_steps, m_regulationTerminal,
                                                                                               *m_tapPosition, m_regulating, m_regulationMode, m_regulationValue, m_targetDeadband);

    bool wasRegulating = m_parent.hasPhaseTapChanger() && m_parent.getPhaseTapChanger().isRegulating();
    unsigned long count = m_parent.getRegulatingTapChangerCount() - (wasRegulating ? 1 : 0);
    network.setValidationLevelIfGreaterThan(checkOnlyOneTapChangerRegulatingEnabled(m_parent, count, m_regulating, network.getMinimumValidationLevel()));

    if (m_parent.hasRatioTapChanger()) {
        logger.warn("%1% both Ratio and Phase Tap Changer are defined", m_parent.getMessageHeader());
    }

    m_parent.setPhaseTapChanger(std::move(ptrPhaseTapChanger));

    return m_parent.getPhaseTapChanger();
}

PhaseTapChangerAdder& PhaseTapChangerAdder::addStep(double alpha, double rho, double r, double x, double g, double b) {
    m_steps.emplace_back(alpha, rho, r, x, g, b);
    return *this;
}

PhaseTapChangerAdder::StepAdder PhaseTapChangerAdder::beginStep() {
    return StepAdder(*this);
}

Network& PhaseTapChangerAdder::getNetwork() {
    return m_parent.getNetwork();
}

Validable& PhaseTapChangerAdder::getValidable() {
    return m_parent;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setLowTapPosition(long lowTapPosition) {
    m_lowTapPosition = lowTapPosition;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setRegulating(bool regulating) {
    m_regulating = regulating;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setRegulationMode(const PhaseTapChanger::RegulationMode& regulationMode) {
    m_regulationMode = regulationMode;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    m_regulationTerminal = regulationTerminal;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setRegulationValue(double regulationValue) {
    m_regulationValue = regulationValue;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setTapPosition(long tapPosition) {
    m_tapPosition = tapPosition;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerAdder::setTargetDeadband(double targetDeadband) {
    m_targetDeadband = targetDeadband;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
