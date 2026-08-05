/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/BoundaryLineBoundary.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/util/BoundaryLineUtil.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace util {

namespace boundary_line {

Boundary::Boundary(BoundaryLine& parent) :
    m_parent(parent) {
}

double Boundary::getAngle() const {
    if(BoundaryLineUtil::useHypothesis(m_parent)) {
        return BoundaryLineUtil::getBoundaryBusTheta(m_parent) * stdcxx::toDegrees;
    }
    const Terminal& t = m_parent.getTerminal();
    const stdcxx::CReference<Bus>& b = t.getBusView().getBus();
    if(BoundaryLineUtil::zeroImpedance(m_parent)) {
        return iidm::Boundary::getAngle(b);
    } else {
        return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), TwoSides::ONE).otherSideA(m_parent, false);
    }
}

const BoundaryLine& Boundary::getBoundaryLine() const {
    return m_parent;
}

BoundaryLine& Boundary::getBoundaryLine() {
    return m_parent;
}

double Boundary::getP() const {
    if(BoundaryLineUtil::useHypothesis(m_parent)) {
        return -m_parent.getP0();
    }
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    if(BoundaryLineUtil::zeroImpedance(m_parent)) {
        return -t.getP();
    } else {
        return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), TwoSides::ONE).otherSideP(m_parent, false);
    }
}

double Boundary::getQ() const {
    if(BoundaryLineUtil::useHypothesis(m_parent)) {
        return -m_parent.getQ0();
    }
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    if(BoundaryLineUtil::zeroImpedance(m_parent)) {
        return -t.getQ();
    } else {
        return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), TwoSides::ONE).otherSideQ(m_parent, false);
    }
}

double Boundary::getV() const {
    if(BoundaryLineUtil::useHypothesis(m_parent)) {
        return BoundaryLineUtil::getBoundaryBusU(m_parent);
    }
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    if(BoundaryLineUtil::zeroImpedance(m_parent)) {
        return iidm::Boundary::getV(b);
    } else {
        return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), TwoSides::ONE).otherSideU(m_parent, false);
    }
}

double Boundary::getI() const {
    if(BoundaryLineUtil::useHypothesis(m_parent)) {
        return std::hypot(getP(), getQ()) / (std::sqrt(3.0) * getV() / 1000.0);
    }
    const Terminal& t = m_parent.getTerminal();
    const auto& b = t.getBusView().getBus();
    if(BoundaryLineUtil::zeroImpedance(m_parent)) {
        return t.getI();
    } else {
        return SV(t.getP(), t.getQ(), iidm::Boundary::getV(b), iidm::Boundary::getAngle(b), TwoSides::ONE).otherSideI(m_parent, false);
    }
}

const VoltageLevel& Boundary::getNetworkSideVoltageLevel() const {
    return m_parent.getTerminal().getVoltageLevel();
}

VoltageLevel& Boundary::getNetworkSideVoltageLevel() {
    return m_parent.getTerminal().getVoltageLevel();
}

}  // namespace boundary_line

}  // namespace util

}  // namespace iidm

}  // namespace powsybl
