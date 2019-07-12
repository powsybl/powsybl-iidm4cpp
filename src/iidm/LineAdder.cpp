/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

LineAdder::LineAdder(Network& network) :
    m_network(network),
    m_b1(stdcxx::nan()),
    m_b2(stdcxx::nan()),
    m_g1(stdcxx::nan()),
    m_g2(stdcxx::nan()),
    m_r(stdcxx::nan()),
    m_x(stdcxx::nan()) {
}

Line& LineAdder::add() {
    VoltageLevel& voltageLevel1 = checkAndGetVoltageLevel1();
    VoltageLevel& voltageLevel2 = checkAndGetVoltageLevel2();
    std::unique_ptr<Terminal> ptrTerminal1 = checkAndGetTerminal1();
    std::unique_ptr<Terminal> ptrTerminal2 = checkAndGetTerminal2();

    checkB1(*this, m_b1);
    checkB2(*this, m_b2);
    checkG1(*this, m_g1);
    checkG2(*this, m_g2);
    checkR(*this, m_r);
    checkX(*this, m_x);

    // check that the line is attachable on both side
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);

    std::unique_ptr<Line> ptrLine = stdcxx::make_unique<Line>(getId(), getName(), m_r, m_x, m_g1, m_b1, m_g2, m_b2);
    auto& line = m_network.checkAndAdd<Line>(std::move(ptrLine));

    Terminal& terminal1 = line.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = line.addTerminal(std::move(ptrTerminal2));
    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);

    return line;
}

Network& LineAdder::getNetwork() {
    return m_network;
}

const std::string& LineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "AC line";

    return s_typeDescription;
}

LineAdder& LineAdder::setB1(double b1) {
    m_b1 = b1;
    return *this;
}

LineAdder& LineAdder::setB2(double b2) {
    m_b2 = b2;
    return *this;
}

LineAdder& LineAdder::setG1(double g1) {
    m_g1 = g1;
    return *this;
}

LineAdder& LineAdder::setG2(double g2) {
    m_g2 = g2;
    return *this;
}

LineAdder& LineAdder::setR(double r) {
    m_r = r;
    return *this;
}

LineAdder& LineAdder::setX(double x) {
    m_x = x;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
