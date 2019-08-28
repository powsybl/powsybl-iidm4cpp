/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TieLineAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

TieLineAdder::TieLineAdder(Network& network) :
    m_network(network) {
}

TieLine& TieLineAdder::add() {
    VoltageLevel& voltageLevel1 = checkAndGetVoltageLevel1();
    VoltageLevel& voltageLevel2 = checkAndGetVoltageLevel2();
    std::unique_ptr<Terminal> ptrTerminal1 = checkAndGetTerminal1();
    std::unique_ptr<Terminal> ptrTerminal2 = checkAndGetTerminal2();

    checkNotEmpty(*this, m_ucteXnodeCode, "ucteXnodeCode is not set");
    checkHalf(*this, m_half1, 1);
    checkHalf(*this, m_half2, 2);

    // check that the line is attachable on both side
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);

    std::unique_ptr<TieLine> ptrTieLine = stdcxx::make_unique<TieLine>(getId(), getName(), m_ucteXnodeCode, m_half1, m_half2);
    auto& tieLine = m_network.checkAndAdd<TieLine>(std::move(ptrTieLine));

    Terminal& terminal1 = tieLine.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = tieLine.addTerminal(std::move(ptrTerminal2));
    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);

    return tieLine;
}

TieLine::HalfLine& TieLineAdder::getActiveHalf() const {
    if (!m_activeHalf) {
        throw ValidationException(*this, "No active half of the line");
    }
    return m_activeHalf.get();
}

Network& TieLineAdder::getNetwork() {
    return m_network;
}

const std::string& TieLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "AC tie line";

    return s_typeDescription;
}

TieLineAdder& TieLineAdder::line1() {
    m_activeHalf = stdcxx::ref<TieLine::HalfLine>(m_half1);
    return *this;
}

TieLineAdder& TieLineAdder::line2() {
    m_activeHalf = stdcxx::ref<TieLine::HalfLine>(m_half2);
    return *this;
}

TieLineAdder& TieLineAdder::setB1(double b1) {
    getActiveHalf().setB1(b1);
    return *this;
}

TieLineAdder& TieLineAdder::setB2(double b2) {
    getActiveHalf().setB2(b2);
    return *this;
}

TieLineAdder& TieLineAdder::setG1(double g1) {
    getActiveHalf().setG1(g1);
    return *this;
}

TieLineAdder& TieLineAdder::setG2(double g2) {
    getActiveHalf().setG2(g2);
    return *this;
}

TieLineAdder& TieLineAdder::setId(const std::string& id) {
    if (!m_activeHalf) {
        BranchAdder::setId(id);
    } else {
        getActiveHalf().setId(id);
    }
    return *this;
}

TieLineAdder& TieLineAdder::setName(const std::string& name) {
    if (!m_activeHalf) {
        BranchAdder::setName(name);
    } else {
        getActiveHalf().setName(name);
    }
    return *this;
}

TieLineAdder& TieLineAdder::TieLineAdder::setR(double r) {
    getActiveHalf().setR(r);
    return *this;
}

TieLineAdder& TieLineAdder::setUcteXnodeCode(const std::string& ucteXnodeCode) {
    m_ucteXnodeCode = ucteXnodeCode;
    return *this;
}

TieLineAdder& TieLineAdder::setX(double x) {
    getActiveHalf().setX(x);
    return *this;
}

TieLineAdder& TieLineAdder::setXnodeP(double xnodeP) {
    getActiveHalf().setXnodeP(xnodeP);
    return *this;
}

TieLineAdder& TieLineAdder::setXnodeQ(double xnodeQ) {
    getActiveHalf().setXnodeQ(xnodeQ);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
