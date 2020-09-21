/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/GenerationAdder.hpp>

#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace dangling_line_views {

GenerationAdder::GenerationAdder(DanglingLineAdder& danglingLineAdder) :
    m_danglingLineAdder(danglingLineAdder) {
}

DanglingLineAdder& GenerationAdder::add() {
    checkActivePowerLimits(m_danglingLineAdder, m_minP, m_maxP);
    checkActivePowerSetpoint(m_danglingLineAdder, m_targetP);
    checkVoltageControl(m_danglingLineAdder, m_voltageRegulationOn, m_targetV, m_targetQ);
    return m_danglingLineAdder;
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

}  // namespace dangling_line_views

}  // namespace iidm

}  // namespace powsybl
