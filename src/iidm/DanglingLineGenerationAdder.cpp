/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineGenerationAdder.hpp>

#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/DanglingLineGeneration.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace dangling_line {

GenerationAdder::GenerationAdder(DanglingLineAdder& parent) :
    m_parent(parent) {
}

DanglingLineAdder& GenerationAdder::add() {
    checkActivePowerLimits(m_parent, m_minP, m_maxP);

    Network& network = m_parent.getNetwork();
    network.setValidationLevelIfGreaterThan(checkActivePowerSetpoint(m_parent, m_targetP, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkVoltageControl(m_parent, m_voltageRegulationOn, m_targetV, m_targetQ, network.getMinimumValidationLevel()));
    
    m_parent.setGenerationAdder(*this);
    return m_parent;
}

std::unique_ptr<Generation> GenerationAdder::build() const {
    return stdcxx::make_unique<Generation>(m_parent.getNetwork(), m_minP, m_maxP, m_targetP, m_targetQ, m_targetV, m_voltageRegulationOn);
}

GenerationAdder& GenerationAdder::setMaxP(double maxP) {
    m_maxP = maxP;
    return *this;
}

GenerationAdder& GenerationAdder::setMinP(double minP) {
    m_minP = minP;
    return *this;
}

GenerationAdder& GenerationAdder::setTargetP(double targetP) {
    m_targetP = targetP;
    return *this;
}

GenerationAdder& GenerationAdder::setTargetQ(double targetQ) {
    m_targetQ = targetQ;
    return *this;
}

GenerationAdder& GenerationAdder::setTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

GenerationAdder& GenerationAdder::setVoltageRegulationOn(bool voltageRegulationOn) {
    m_voltageRegulationOn = voltageRegulationOn;
    return *this;
}

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl
