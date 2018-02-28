/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CalculatedBus.hpp"

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

CalculatedBus::CalculatedBus(const std::string& id, NodeBreakerVoltageLevel& voltageLevel, std::vector<std::reference_wrapper<NodeTerminal> >&& terminals) :
    Bus(id),
    m_voltageLevel(voltageLevel),
    m_terminals(std::move(terminals)),
    m_valid(true) {
}

void CalculatedBus::checkValidity() const {
    if (!m_valid) {
        throw PowsyblException("Bus has been invalidated");
    }
}

double CalculatedBus::getAngle() const {
    checkValidity();

    return m_terminals.empty() ? stdcxx::nan() : m_terminals.front().get().getAngle();
}

unsigned long CalculatedBus::getConnectedTerminalCount() const {
    return m_terminals.size();
}

double CalculatedBus::getV() const {
    checkValidity();

    return m_terminals.empty() ? stdcxx::nan() : m_terminals.front().get().getV();
}

VoltageLevel& CalculatedBus::getVoltageLevel() const {
    return m_voltageLevel;
}

void CalculatedBus::invalidate() {
    m_valid = false;
    m_voltageLevel.reset();
    m_terminals.clear();
}

Bus& CalculatedBus::setAngle(double angle) {
    checkValidity();
    for (auto& it : m_terminals) {
        it.get().setAngle(angle);
    }

    return *this;
}

Bus& CalculatedBus::setV(double v) {
    checkValidity();
    for (auto& it : m_terminals) {
        it.get().setV(v);
    }

    return *this;
}

}

}
