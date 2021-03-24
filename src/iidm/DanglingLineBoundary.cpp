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
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideA(m_parent);
}

double Boundary::getAngle(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getAngle() : stdcxx::nan();
}

double Boundary::getP() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideP(m_parent);
}

double Boundary::getQ() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideQ(m_parent);
}

double Boundary::getV() const {
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideU(m_parent);
}

double Boundary::getV(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getV() : stdcxx::nan();
}

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl
