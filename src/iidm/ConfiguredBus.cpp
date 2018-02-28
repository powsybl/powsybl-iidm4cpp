/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ConfiguredBus.hpp"

#include <cmath>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ConfiguredBus::ConfiguredBus(const std::string& id, BusBreakerVoltageLevel& voltageLevel) :
    Bus(id),
    m_voltageLevel(voltageLevel),
    m_network(voltageLevel.getNetwork()),
    m_terminals(m_network.get().getStateManager().getStateArraySize()),
    m_v(m_network.get().getStateManager().getStateArraySize(), stdcxx::nan()),
    m_angle(m_network.get().getStateManager().getStateArraySize(), stdcxx::nan()) {

}

void ConfiguredBus::addTerminal(BusTerminal& terminal) {
    m_terminals[m_network.get().getStateIndex()].push_back(std::ref(terminal));
}

void ConfiguredBus::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_terminals[index] = m_terminals[sourceIndex];
        m_v[index] = m_v[sourceIndex];
        m_angle[index] = m_angle[sourceIndex];
    }
}

void ConfiguredBus::deleteStateArrayElement(unsigned long index) {
    m_terminals[index].clear();
}

void ConfiguredBus::extendStateArraySize(unsigned long /*initStateArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_terminals.resize(m_terminals.size() + number, m_terminals[sourceIndex]);
    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_angle.resize(m_angle.size() + number, m_angle[sourceIndex]);
}

double ConfiguredBus::getAngle() const {
    return m_angle[m_network.get().getStateIndex()];
}

unsigned long ConfiguredBus::getConnectedTerminalCount() const {
    unsigned long count = 0;

    const std::list<std::reference_wrapper<BusTerminal> >& terminals = m_terminals[m_network.get().getStateIndex()];
    for (const auto& it : terminals) {
        if (it.get().isConnected()) {
            count++;
        }
    }

    return count;
}

unsigned long ConfiguredBus::getTerminalCount() const {
    return m_terminals[m_network.get().getStateIndex()].size();
}

double ConfiguredBus::getV() const {
    return m_v[m_network.get().getStateIndex()];
}

VoltageLevel& ConfiguredBus::getVoltageLevel() const {
    return m_voltageLevel;
}

void ConfiguredBus::reduceStateArraySize(unsigned long number) {
    m_terminals.resize(m_terminals.size() - number);
    m_v.resize(m_v.size() - number);
    m_angle.resize(m_angle.size() - number);
}

void ConfiguredBus::removeTerminal(BusTerminal& terminal) {
    auto& terminals = m_terminals[m_network.get().getStateIndex()];
    const auto& it = std::find_if(terminals.begin(), terminals.end(), [&](std::reference_wrapper<BusTerminal>& item)
    {
        return std::addressof(terminal) == std::addressof(item.get());
    });

    if (it != terminals.end()) {
        terminals.erase(it);
    } else {
        throw PowsyblException(logging::format("Terminal %1% not found", terminal));
    }
}

Bus& ConfiguredBus::setAngle(double angle) {
    m_angle[m_network.get().getStateIndex()] = angle;

    return *this;
}

Bus& ConfiguredBus::setV(double v) {
    checkVoltage(*this, v);

    m_v[m_network.get().getStateIndex()] = v;

    return *this;
}

}

}
