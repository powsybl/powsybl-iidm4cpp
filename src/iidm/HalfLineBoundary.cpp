/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HalfLineBoundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/util/SV.hpp>

namespace powsybl {

namespace iidm {

namespace half_line {

Boundary::Boundary(TieLine& tieLine, const Branch::Side& side, const TerminalSupplier& terminalSupplier) :
    m_parent(tieLine),
    m_side(side),
    m_terminalSupplier(terminalSupplier) {
}

double Boundary::getAngle() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideA(m_parent.getHalf(m_side));
}

const Connectable& Boundary::getConnectable() const {
    return m_parent;
}

Connectable& Boundary::getConnectable() {
    return m_parent;
}

double Boundary::getP() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideP(m_parent.getHalf(m_side));
}

double Boundary::getQ() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideQ(m_parent.getHalf(m_side));
}

stdcxx::optional<Branch::Side> Boundary::getSide() const {
    return m_side;
}

double Boundary::getV() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b)).otherSideU(m_parent.getHalf(m_side));
}

const VoltageLevel& Boundary::getVoltageLevel() const {
    return m_parent.getTerminal(m_side).getVoltageLevel();
}

VoltageLevel& Boundary::getVoltageLevel() {
    return m_parent.getTerminal(m_side).getVoltageLevel();
}

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl
