/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ConfiguredBus.hpp"

#include <cmath>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/stdcxx/MessageFormat.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "BusBreakerVoltageLevel.hpp"
#include "BusTerminal.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ConfiguredBus::ConfiguredBus(const std::string& id, const std::string& name, BusBreakerVoltageLevel& voltageLevel) :
    Bus(id, name),
    m_voltageLevel(voltageLevel),
    m_terminals(voltageLevel.getNetwork().getVariantManager().getVariantArraySize()),
    m_v(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_angle(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), stdcxx::nan()) {

}

void ConfiguredBus::addTerminal(BusTerminal& terminal) {
    m_terminals[getNetwork().getVariantIndex()].push_back(std::ref(terminal));
}

void ConfiguredBus::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_terminals[index] = m_terminals[sourceIndex];
        m_v[index] = m_v[sourceIndex];
        m_angle[index] = m_angle[sourceIndex];
    }
}

void ConfiguredBus::deleteVariantArrayElement(unsigned long index) {
    m_terminals[index].clear();
}

void ConfiguredBus::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_terminals.resize(m_terminals.size() + number, m_terminals[sourceIndex]);
    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_angle.resize(m_angle.size() + number, m_angle[sourceIndex]);
}

double ConfiguredBus::getAngle() const {
    return m_angle[getNetwork().getVariantIndex()];
}

unsigned long ConfiguredBus::getConnectedTerminalCount() const {
    unsigned long count = 0;

    const std::list<std::reference_wrapper<BusTerminal> >& terminals = m_terminals[getNetwork().getVariantIndex()];
    for (const auto& terminal : terminals) {
        if (terminal.get().isConnected()) {
            ++count;
        }
    }

    return count;
}

stdcxx::const_range<Terminal> ConfiguredBus::getConnectedTerminals() const {
    const auto& busTerminals = m_terminals[getNetwork().getVariantIndex()];

    const auto& filter = [](const Terminal& terminal) {
        return terminal.isConnected();
    };

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, Terminal>;

    return busTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}

stdcxx::range<Terminal> ConfiguredBus::getConnectedTerminals() {
    const auto& busTerminals = m_terminals[getNetwork().getVariantIndex()];

    const auto& filter = [](const Terminal& terminal) {
        return terminal.isConnected();
    };

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, Terminal>;

    return busTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}

unsigned long ConfiguredBus::getTerminalCount() const {
    return m_terminals[getNetwork().getVariantIndex()].size();
}

stdcxx::const_range<BusTerminal> ConfiguredBus::getTerminals() const {
    const auto& terminals = m_terminals[getNetwork().getVariantIndex()];

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, BusTerminal>;

    return terminals | boost::adaptors::transformed(mapper);
}

stdcxx::range<BusTerminal> ConfiguredBus::getTerminals() {
    const auto& terminals = m_terminals[getNetwork().getVariantIndex()];

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, BusTerminal>;

    return terminals | boost::adaptors::transformed(mapper);
}

double ConfiguredBus::getV() const {
    return m_v[getNetwork().getVariantIndex()];
}

const VoltageLevel& ConfiguredBus::getVoltageLevel() const {
    return m_voltageLevel;
}

VoltageLevel& ConfiguredBus::getVoltageLevel() {
    return m_voltageLevel;
}

void ConfiguredBus::reduceVariantArraySize(unsigned long number) {
    m_terminals.resize(m_terminals.size() - number);
    m_v.resize(m_v.size() - number);
    m_angle.resize(m_angle.size() - number);
}

void ConfiguredBus::removeTerminal(BusTerminal& terminal) {
    auto& terminals = m_terminals[getNetwork().getVariantIndex()];
    const auto& it = std::find_if(terminals.begin(), terminals.end(), [&terminal](const std::reference_wrapper<BusTerminal>& item) {
        return stdcxx::areSame(terminal, item.get());
    });

    if (it != terminals.end()) {
        terminals.erase(it);
    } else {
        throw PowsyblException(stdcxx::format("Terminal %1% not found", terminal));
    }
}

Bus& ConfiguredBus::setAngle(double angle) {
    m_angle[getNetwork().getVariantIndex()] = angle;

    return *this;
}

Bus& ConfiguredBus::setV(double v) {
    checkVoltage(*this, v);

    m_v[getNetwork().getVariantIndex()] = v;

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
