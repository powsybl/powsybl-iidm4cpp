/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HalfLineBoundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HalfLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/util/SV.hpp>

namespace powsybl {

namespace iidm {

namespace half_line {

Boundary::Boundary(tie_line::HalfLine& halfLine, const std::function<const Terminal&()>& terminalSupplier) :
    m_halfLine(halfLine), m_terminalSupplier(terminalSupplier) {
}

double Boundary::getAngle() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideA(m_halfLine);
}

double Boundary::getAngle(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getAngle() : stdcxx::nan();
}

double Boundary::getP() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideP(m_halfLine);
}

double Boundary::getQ() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideQ(m_halfLine);
}

double Boundary::getV() const {
    const Terminal& t = m_terminalSupplier();
    const auto& b = t.getBusView().getBus();
    return SV(t.getP(), t.getQ(), getV(b), getAngle(b)).otherSideU(m_halfLine);
}

double Boundary::getV(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getV() : stdcxx::nan();
}

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl
