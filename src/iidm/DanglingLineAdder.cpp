/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineAdder.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

DanglingLineAdder::DanglingLineAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

DanglingLine& DanglingLineAdder::add() {
    checkP0(*this, m_p0);
    checkQ0(*this, m_q0);
    checkR(*this, m_r);
    checkX(*this, m_x);
    checkG(*this, m_g);
    checkB(*this, m_b);

    std::unique_ptr<DanglingLine::Generation> ptrGeneration = m_generationAdder.is_initialized() ? m_generationAdder->build() : nullptr;

    std::unique_ptr<DanglingLine> ptrDanglingLine = stdcxx::make_unique<DanglingLine>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(),
                                                                                      m_p0, m_q0, m_r, m_x, m_g, m_b, m_ucteXnodeCode, std::move(ptrGeneration));
    auto& danglingLine = getNetwork().checkAndAdd<DanglingLine>(std::move(ptrDanglingLine));

    Terminal& terminal = danglingLine.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return danglingLine;
}

const std::string& DanglingLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Dangling line";

    return s_typeDescription;
}

DanglingLineAdder::GenerationAdder DanglingLineAdder::newGeneration() {
    return GenerationAdder(*this);
}

DanglingLineAdder& DanglingLineAdder::setB(double b) {
    m_b = b;
    return *this;
}

DanglingLineAdder& DanglingLineAdder::setG(double g) {
    m_g = g;
    return *this;
}

void DanglingLineAdder::setGenerationAdder(const GenerationAdder& generationAdder) {
    m_generationAdder.emplace(generationAdder);
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
