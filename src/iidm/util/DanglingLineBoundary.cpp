/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/DanglingLineBoundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace util {

namespace dangling_line {

Boundary::Boundary(DanglingLine& parent) :
    m_parent(parent) {
}

double Boundary::getAngle() const {
    const Terminal& t = m_parent.getTerminal();
    const stdcxx::CReference<Bus>& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), Branch::Side::ONE).otherSideA(m_parent, true);
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
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), Branch::Side::ONE).otherSideP(m_parent, true);
}

double Boundary::getQ() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), Branch::Side::ONE).otherSideQ(m_parent, true);
}

stdcxx::optional<Branch::Side> Boundary::getSide() const {
    return {};
}

double Boundary::getV() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), Branch::Side::ONE).otherSideU(m_parent, true);
}

const VoltageLevel& Boundary::getVoltageLevel() const {
    return m_parent.getTerminal().getVoltageLevel();
}

VoltageLevel& Boundary::getVoltageLevel() {
    return m_parent.getTerminal().getVoltageLevel();
}

}  // namespace dangling_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl
