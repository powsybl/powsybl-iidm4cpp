/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineGenerationAdder.hpp>

#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace dangling_line {

DanglingLineGenerationAdder::DanglingLineGenerationAdder(DanglingLineAdder& parent, stdcxx::optional<DanglingLineGenerationAdder>& adder) :
    m_parent(parent),
    m_adder(adder) {
}

// NOLINTNEXTLINE
DanglingLineGenerationAdder& DanglingLineGenerationAdder::operator=(const DanglingLineGenerationAdder& adder) {
    m_parent = adder.m_parent;
    m_adder = adder.m_adder;
    m_minP = adder.m_minP;
    m_maxP = adder.m_maxP;
    m_targetP = adder.m_targetP;
    m_targetQ = adder.m_targetQ;
    m_targetV = adder.m_targetV;
    m_voltageRegulationOn = adder.m_voltageRegulationOn;
    return *this;
}

DanglingLineAdder& DanglingLineGenerationAdder::add() {
    checkActivePowerLimits(m_parent, m_minP, m_maxP);
    checkActivePowerSetpoint(m_parent, m_targetP);
    checkVoltageControl(m_parent, m_voltageRegulationOn, m_targetV, m_targetQ);
    m_adder = *this;
    return m_parent;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setMaxP(double maxP) {
    m_maxP = maxP;
    return *this;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setMinP(double minP) {
    m_minP = minP;
    return *this;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setTargetP(double targetP) {
    m_targetP = targetP;
    return *this;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setTargetQ(double targetQ) {
    m_targetQ = targetQ;
    return *this;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

DanglingLineGenerationAdder& DanglingLineGenerationAdder::setVoltageRegulationOn(bool voltageRegulationOn) {
    m_voltageRegulationOn = voltageRegulationOn;
    return *this;
}

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl
