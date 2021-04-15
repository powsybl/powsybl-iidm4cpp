/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineBoundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace dangling_line {

Boundary::Boundary(DanglingLine& parent) :
    m_parent(parent) {
}

double Boundary::getAngle() const {
    const Terminal& t = m_parent.getTerminal();
    const stdcxx::CReference<Bus>& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideA(m_parent);
}

const Connectable& Boundary::getConnectable() const {
    return m_parent;
}

Connectable& Boundary::getConnectable() {
    return m_parent;
}

double Boundary::getP() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideP(m_parent);
}

double Boundary::getQ() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideQ(m_parent);
}

stdcxx::optional<Branch::Side> Boundary::getSide() const {
    return {};
}

double Boundary::getV() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideU(m_parent);
}

const VoltageLevel& Boundary::getVoltageLevel() const {
    return m_parent.getTerminal().getVoltageLevel();
}

VoltageLevel& Boundary::getVoltageLevel() {
    return m_parent.getTerminal().getVoltageLevel();
}

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl
