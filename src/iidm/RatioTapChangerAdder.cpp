/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>

#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

RatioTapChangerAdder::StepAdder::StepAdder(RatioTapChangerAdder& parent) :
    m_parent(parent) {
}

RatioTapChangerAdder& RatioTapChangerAdder::StepAdder::endStep() {
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    return m_parent.addStep(m_rho, m_r, m_x, m_g, m_b);
}

RatioTapChangerAdder::StepAdder& RatioTapChangerAdder::StepAdder::setB(double b) {
    m_b = b;
    return *this;
}

RatioTapChangerAdder::StepAdder& RatioTapChangerAdder::StepAdder::setG(double g) {
    m_g = g;
    return *this;
}

RatioTapChangerAdder::StepAdder& RatioTapChangerAdder::StepAdder::setR(double r) {
    m_r = r;
    return *this;
}

RatioTapChangerAdder::StepAdder& RatioTapChangerAdder::StepAdder::setRho(double rho) {
    m_rho = rho;
    return *this;
}

RatioTapChangerAdder::StepAdder& RatioTapChangerAdder::StepAdder::setX(double x) {
    m_x = x;
    return *this;
}

RatioTapChangerAdder::RatioTapChangerAdder(RatioTapChangerHolder& parent) :
    m_parent(parent) {
}

RatioTapChanger& RatioTapChangerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<RatioTapChangerAdder>();

    checkOptional(m_parent, m_tapPosition, "tap position is not set");
    if (m_steps.empty()) {
        throw ValidationException(m_parent, "ratio tap changer should have at least one step");
    }
    long highTapPosition = m_lowTapPosition + m_steps.size() - 1;
    checkTapPosition(m_parent, *m_tapPosition, m_lowTapPosition, highTapPosition);
    checkRatioTapChangerRegulation(m_parent, m_regulating, m_loadTapChangingCapabilities, m_regulationTerminal, m_targetV, getNetwork());
    checkTargetDeadband(m_parent, "ratio tap changer", m_regulating, m_targetDeadband);

    std::unique_ptr<RatioTapChanger> ptrRatioTapChanger = stdcxx::make_unique<RatioTapChanger>(m_parent, m_lowTapPosition, m_steps, m_regulationTerminal,
                                                                                               m_loadTapChangingCapabilities, *m_tapPosition, m_regulating, m_targetV, m_targetDeadband);

    bool wasRegulating = m_parent.hasRatioTapChanger() && m_parent.getRatioTapChanger().isRegulating();
    unsigned long count = m_parent.getRegulatingTapChangerCount() - (wasRegulating ? 1 : 0);
    checkOnlyOneTapChangerRegulatingEnabled(m_parent, count, m_regulating);

    if (m_parent.hasPhaseTapChanger()) {
        logger.warn("%1% both Ratio and Phase Tap Changer are defined", m_parent.getMessageHeader());
    }

    m_parent.setRatioTapChanger(std::move(ptrRatioTapChanger));

    return m_parent.getRatioTapChanger();
}

RatioTapChangerAdder& RatioTapChangerAdder::addStep(double rho, double r, double x, double g, double b) {
    m_steps.emplace_back(rho, r, x, g, b);
    return *this;
}

RatioTapChangerAdder::StepAdder RatioTapChangerAdder::beginStep() {
    return StepAdder(*this);
}

Network& RatioTapChangerAdder::getNetwork() {
    return m_parent.getNetwork();
}

Validable& RatioTapChangerAdder::getValidable() {
    return m_parent;
}

RatioTapChangerAdder& RatioTapChangerAdder::setLoadTapChangingCapabilities(bool loadTapChangingCapabilities) {
    m_loadTapChangingCapabilities = loadTapChangingCapabilities;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setLowTapPosition(long lowTapPosition) {
    m_lowTapPosition = lowTapPosition;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setRegulating(bool regulating) {
    m_regulating = regulating;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    m_regulationTerminal = regulationTerminal;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setTapPosition(long tapPosition) {
    m_tapPosition = tapPosition;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setTargetDeadband(double targetDeadband) {
    m_targetDeadband = targetDeadband;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
