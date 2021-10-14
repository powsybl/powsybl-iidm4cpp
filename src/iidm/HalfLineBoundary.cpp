/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "HalfLineBoundary.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HalfLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/util/SV.hpp>

namespace powsybl {

namespace iidm {

namespace half_line {

Boundary::Boundary(tie_line::HalfLine& halfline, const Branch::Side& side) :
    m_parent(halfline),
    m_side(side) {
}

double Boundary::getAngle() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), m_side).otherSideA(m_parent);
}

const Connectable& Boundary::getConnectable() const {
    return m_parent.getParent();
}

Connectable& Boundary::getConnectable() {
    return m_parent.getParent();
}

double Boundary::getP() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), m_side).otherSideP(m_parent);
}

double Boundary::getQ() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), m_side).otherSideQ(m_parent);
}

stdcxx::optional<Branch::Side> Boundary::getSide() const {
    return m_side;
}

const TieLine& Boundary::getTieLine() const {
    return dynamic_cast<const TieLine&>(getConnectable());
}

TieLine& Boundary::getTieLine() {
    return dynamic_cast<TieLine&>(getConnectable());
}

double Boundary::getV() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), m_side).otherSideU(m_parent);
}

const VoltageLevel& Boundary::getVoltageLevel() const {
    return getTieLine().getTerminal(m_side).getVoltageLevel();
}

VoltageLevel& Boundary::getVoltageLevel() {
    return getTieLine().getTerminal(m_side).getVoltageLevel();
}

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl
