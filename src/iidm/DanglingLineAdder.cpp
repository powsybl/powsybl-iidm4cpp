/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

DanglingLineAdder::DanglingLineAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_r(stdcxx::nan()),
    m_x(stdcxx::nan()),
    m_g(stdcxx::nan()),
    m_b(stdcxx::nan()),
    m_p0(stdcxx::nan()),
    m_q0(stdcxx::nan()),
    m_ucteXnodeCode() {
}

DanglingLine& DanglingLineAdder::add() {
    checkP0(*this, m_p0);
    checkQ0(*this, m_q0);
    checkR(*this, m_r);
    checkX(*this, m_x);
    checkG(*this, m_g);
    checkB(*this, m_b);

    std::unique_ptr<DanglingLine> ptrDanglingLine = stdcxx::make_unique<DanglingLine>(getNetwork(), getId(), getName(), m_p0, m_q0, m_r, m_x, m_g, m_b, m_ucteXnodeCode);
    auto& danglingLine = getNetwork().checkAndAdd<DanglingLine>(std::move(ptrDanglingLine));

    Terminal& terminal = danglingLine.addTerminal(getTerminal());
    m_voltageLevel.attach(terminal, false);

    return danglingLine;
}

Network& DanglingLineAdder::getNetwork() {
    return m_voltageLevel.getNetwork();
}

const std::string& DanglingLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Dangling line";

    return s_typeDescription;
}

DanglingLineAdder& DanglingLineAdder::setB(double b) {
    m_b = b;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setG(double g) {
    m_g = g;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setP0(double p0) {
    m_p0 = p0;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setQ0(double q0) {
    m_q0 = q0;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setR(double r) {
    m_r = r;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setUcteXnodeCode(const std::string& ucteXnodeCode) {
    m_ucteXnodeCode = ucteXnodeCode;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setX(double x) {
    m_x = x;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
