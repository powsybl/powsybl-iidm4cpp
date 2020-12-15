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

    TieLine::HalfLine half1 = m_halfLineAdder1->build();
    TieLine::HalfLine half2 = m_halfLineAdder2->build();

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

void TieLineAdder::setHalfLineAdder1(const HalfLineAdder& adder) {
    m_halfLineAdder1.emplace(adder);
}

void TieLineAdder::setHalfLineAdder2(const HalfLineAdder& adder) {
    m_halfLineAdder2.emplace(adder);
}

TieLineAdder& TieLineAdder::setUcteXnodeCode(const std::string& ucteXnodeCode) {
    m_ucteXnodeCode = ucteXnodeCode;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
