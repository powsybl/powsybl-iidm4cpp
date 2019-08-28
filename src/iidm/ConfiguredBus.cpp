/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ConfiguredBus.hpp"

#include <cmath>

#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/logging/MessageFormat.hpp>
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
    m_network(voltageLevel.getNetwork()),
    m_terminals(m_network.get().getVariantManager().getVariantArraySize()),
    m_v(m_network.get().getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_angle(m_network.get().getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_connectedComponentNumber(m_network.get().getVariantManager().getVariantArraySize(), ComponentConstants::INVALID_NUM),
    m_synchronousComponentNumber(m_network.get().getVariantManager().getVariantArraySize(), ComponentConstants::INVALID_NUM) {

}

void ConfiguredBus::addTerminal(BusTerminal& terminal) {
    m_terminals[m_network.get().getVariantIndex()].push_back(std::ref(terminal));
}

void ConfiguredBus::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_terminals[index] = m_terminals[sourceIndex];
        m_v[index] = m_v[sourceIndex];
        m_angle[index] = m_angle[sourceIndex];
        m_connectedComponentNumber[index] = m_connectedComponentNumber[sourceIndex];
        m_synchronousComponentNumber[index] = m_synchronousComponentNumber[sourceIndex];
    }
}

void ConfiguredBus::deleteVariantArrayElement(unsigned long index) {
    m_terminals[index].clear();
}

void ConfiguredBus::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_terminals.resize(m_terminals.size() + number, m_terminals[sourceIndex]);
    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_angle.resize(m_angle.size() + number, m_angle[sourceIndex]);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() + number, m_connectedComponentNumber[sourceIndex]);
    m_synchronousComponentNumber.resize(m_synchronousComponentNumber.size() + number, m_synchronousComponentNumber[sourceIndex]);
}

double ConfiguredBus::getAngle() const {
    return m_angle[m_network.get().getVariantIndex()];
}

stdcxx::CReference<Component> ConfiguredBus::getConnectedComponent() const {
    ConnectedComponentsManager& ccm = m_voltageLevel.get().getNetwork().getConnectedComponentsManager();
    ccm.update();
    return stdcxx::cref<Component>(ccm.getComponent(getConnectedComponentNumber()));
}

stdcxx::Reference<Component> ConfiguredBus::getConnectedComponent() {
    ConnectedComponentsManager& ccm = m_voltageLevel.get().getNetwork().getConnectedComponentsManager();
    ccm.update();
    return ccm.getComponent(getConnectedComponentNumber());
}

long ConfiguredBus::getConnectedComponentNumber() const {
    return m_connectedComponentNumber[m_network.get().getVariantIndex()];
}

unsigned long ConfiguredBus::getConnectedTerminalCount() const {
    unsigned long count = 0;

    const std::list<std::reference_wrapper<BusTerminal> >& terminals = m_terminals[m_network.get().getVariantIndex()];
    for (const auto& terminal : terminals) {
        if (terminal.get().isConnected()) {
            ++count;
        }
    }

    return count;
}

stdcxx::const_range<Terminal> ConfiguredBus::getConnectedTerminals() const {
    const auto& busTerminals = m_terminals[m_network.get().getVariantIndex()];

    const auto& filter = [](const Terminal& terminal) {
        return terminal.isConnected();
    };

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, Terminal>;

    return busTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}

stdcxx::range<Terminal> ConfiguredBus::getConnectedTerminals() {
    const auto& busTerminals = m_terminals[m_network.get().getVariantIndex()];

    const auto& filter = [](const Terminal& terminal) {
        return terminal.isConnected();
    };

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, Terminal>;

    return busTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}

stdcxx::CReference<Component> ConfiguredBus::getSynchronousComponent() const {
    SynchronousComponentsManager& scm = m_voltageLevel.get().getNetwork().getSynchronousComponentsManager();
    scm.update();
    return stdcxx::cref<Component>(scm.getComponent(getSynchronousComponentNumber()));
}

stdcxx::Reference<Component> ConfiguredBus::getSynchronousComponent() {
    SynchronousComponentsManager& scm = m_voltageLevel.get().getNetwork().getSynchronousComponentsManager();
    scm.update();
    return scm.getComponent(getSynchronousComponentNumber());
}

long ConfiguredBus::getSynchronousComponentNumber() const {
    return m_synchronousComponentNumber[m_network.get().getVariantIndex()];
}

unsigned long ConfiguredBus::getTerminalCount() const {
    return m_terminals[m_network.get().getVariantIndex()].size();
}

stdcxx::const_range<BusTerminal> ConfiguredBus::getTerminals() const {
    const auto& terminals = m_terminals[m_network.get().getVariantIndex()];

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, BusTerminal>;

    return terminals | boost::adaptors::transformed(mapper);
}

stdcxx::range<BusTerminal> ConfiguredBus::getTerminals() {
    const auto& terminals = m_terminals[m_network.get().getVariantIndex()];

    const auto& mapper = stdcxx::map<std::reference_wrapper<BusTerminal>, BusTerminal>;

    return terminals | boost::adaptors::transformed(mapper);
}

double ConfiguredBus::getV() const {
    return m_v[m_network.get().getVariantIndex()];
}

VoltageLevel& ConfiguredBus::getVoltageLevel() const {
    return m_voltageLevel;
}

void ConfiguredBus::reduceVariantArraySize(unsigned long number) {
    m_terminals.resize(m_terminals.size() - number);
    m_v.resize(m_v.size() - number);
    m_angle.resize(m_angle.size() - number);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() - number);
    m_synchronousComponentNumber.resize(m_synchronousComponentNumber.size() - number);
}

void ConfiguredBus::removeTerminal(BusTerminal& terminal) {
    auto& terminals = m_terminals[m_network.get().getVariantIndex()];
    const auto& it = std::find_if(terminals.begin(), terminals.end(), [&terminal](std::reference_wrapper<BusTerminal>& item) {
        return stdcxx::areSame(terminal, item.get());
    });

    if (it != terminals.end()) {
        terminals.erase(it);
    } else {
        throw PowsyblException(logging::format("Terminal %1% not found", terminal));
    }
}

Bus& ConfiguredBus::setAngle(double angle) {
    m_angle[m_network.get().getVariantIndex()] = angle;

    return *this;
}

void ConfiguredBus::setConnectedComponentNumber(long connectedComponentNumber) {
    m_connectedComponentNumber[m_network.get().getVariantIndex()] = connectedComponentNumber;
}

void ConfiguredBus::setSynchronousComponentNumber(long componentNumber) {
    m_synchronousComponentNumber[m_network.get().getVariantIndex()] = componentNumber;
}

Bus& ConfiguredBus::setV(double v) {
    checkVoltage(*this, v);

    m_v[m_network.get().getVariantIndex()] = v;

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
