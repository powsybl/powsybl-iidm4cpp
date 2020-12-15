/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

TieLineAdder::TieLineAdder(Network& network) :
    m_network(network) {
}

TieLine& TieLineAdder::add() {
    VoltageLevel& voltageLevel1 = checkAndGetVoltageLevel1();
    VoltageLevel& voltageLevel2 = checkAndGetVoltageLevel2();
    std::unique_ptr<Terminal> ptrTerminal1 = checkAndGetTerminal1(voltageLevel1);
    std::unique_ptr<Terminal> ptrTerminal2 = checkAndGetTerminal2(voltageLevel2);

    checkNotEmpty(*this, m_ucteXnodeCode, "ucteXnodeCode is not set");

    if (!m_halfLineAdder1.is_initialized()) {
        throw ValidationException(*this, "half line 1 is not set");
    }
    if (!m_halfLineAdder2.is_initialized()) {
        throw ValidationException(*this, "half line 2 is not set");
    }

    TieLine::HalfLine half1;
    half1.setId(m_halfLineAdder1->m_id);
    half1.setName(m_halfLineAdder1->m_name);
    half1.setFictitious(m_halfLineAdder1->m_fictitious);
    half1.setXnodeP(m_halfLineAdder1->m_xnodeP);
    half1.setXnodeQ(m_halfLineAdder1->m_xnodeQ);
    half1.setR(m_halfLineAdder1->m_r);
    half1.setX(m_halfLineAdder1->m_x);
    half1.setG1(m_halfLineAdder1->m_g1);
    half1.setG2(m_halfLineAdder1->m_g2);
    half1.setB1(m_halfLineAdder1->m_b1);
    half1.setB2(m_halfLineAdder1->m_b2);

    TieLine::HalfLine half2;
    half2.setId(m_halfLineAdder2->m_id);
    half2.setName(m_halfLineAdder2->m_name);
    half2.setFictitious(m_halfLineAdder2->m_fictitious);
    half2.setXnodeP(m_halfLineAdder2->m_xnodeP);
    half2.setXnodeQ(m_halfLineAdder2->m_xnodeQ);
    half2.setR(m_halfLineAdder2->m_r);
    half2.setX(m_halfLineAdder2->m_x);
    half2.setG1(m_halfLineAdder2->m_g1);
    half2.setG2(m_halfLineAdder2->m_g2);
    half2.setB1(m_halfLineAdder2->m_b1);
    half2.setB2(m_halfLineAdder2->m_b2);

    checkHalf(*this, half1, 1);
    checkHalf(*this, half2, 2);

    // check that the line is attachable on both side
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);

    std::unique_ptr<TieLine> ptrTieLine = stdcxx::make_unique<TieLine>(checkAndGetUniqueId(), getName(), isFictitious(), m_ucteXnodeCode, std::move(half1), std::move(half2));
    auto& tieLine = m_network.checkAndAdd<TieLine>(std::move(ptrTieLine));

    Terminal& terminal1 = tieLine.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = tieLine.addTerminal(std::move(ptrTerminal2));
    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);

    return tieLine;
}

const Network& TieLineAdder::getNetwork() const {
    return m_network;
}

Network& TieLineAdder::getNetwork() {
    return m_network;
}

const std::string& TieLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "AC tie line";

    return s_typeDescription;
}

TieLineAdder::HalfLineAdder TieLineAdder::newHalfLine1() {
    return HalfLineAdder(*this, 1);
}

TieLineAdder::HalfLineAdder TieLineAdder::newHalfLine2() {
    return HalfLineAdder(*this, 2);
}

void TieLineAdder::setHalfLineAdder1(HalfLineAdder& adder) {
    m_halfLineAdder1.emplace(adder);
}

void TieLineAdder::setHalfLineAdder2(HalfLineAdder& adder) {
    m_halfLineAdder2.emplace(adder);
}

TieLineAdder& TieLineAdder::setUcteXnodeCode(const std::string& ucteXnodeCode) {
    m_ucteXnodeCode = ucteXnodeCode;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
