/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/AbstractHalfLineBoundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HalfLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/util/SV.hpp>

namespace powsybl {

namespace iidm {

namespace util {

namespace half_line {

AbstractHalfLineBoundary::AbstractHalfLineBoundary(tie_line::HalfLine& halfline, const Branch::Side& side) :
    m_parent(halfline),
    m_side(side) {
}

double AbstractHalfLineBoundary::getAngle() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), Boundary::getV(b), Boundary::getAngle(b), m_side).otherSideA(m_parent);
}

double AbstractHalfLineBoundary::getP() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), Boundary::getV(b), Boundary::getAngle(b), m_side).otherSideP(m_parent);
}

const tie_line::HalfLine& AbstractHalfLineBoundary::getParent() const {
    return m_parent;
}

tie_line::HalfLine& AbstractHalfLineBoundary::getParent() {
    return m_parent;
}

double AbstractHalfLineBoundary::getQ() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), Boundary::getV(b), Boundary::getAngle(b), m_side).otherSideQ(m_parent);
}

stdcxx::optional<Branch::Side> AbstractHalfLineBoundary::getSide() const {
    return m_side;
}

const TieLine& AbstractHalfLineBoundary::getTieLine() const {
    return dynamic_cast<const TieLine&>(getConnectable());
}

TieLine& AbstractHalfLineBoundary::getTieLine() {
    return dynamic_cast<TieLine&>(getConnectable());
}

double AbstractHalfLineBoundary::getV() const {
    const Terminal& t = getTieLine().getTerminal(m_side);
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), Boundary::getV(b), Boundary::getAngle(b), m_side).otherSideU(m_parent);
}

const VoltageLevel& AbstractHalfLineBoundary::getNetworkSideVoltageLevel() const {
    return getTieLine().getTerminal(m_side).getVoltageLevel();
}

VoltageLevel& AbstractHalfLineBoundary::getNetworkSideVoltageLevel() {
    return getTieLine().getTerminal(m_side).getVoltageLevel();
}

}  // namespace half_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl
