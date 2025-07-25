/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusTerminal.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "BusBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

BusTerminal::BusTerminal(VoltageLevel& voltageLevel, const std::string& connectableBusId, bool connected) :
    Terminal(voltageLevel),
    m_connected(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), connected),
    m_connectableBusId(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), checkNotEmpty(connectableBusId, "ConnectableBusId is required")),
    m_busBreakerView(*this),
    m_busView(*this) {
}

void BusTerminal::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Terminal::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_connected[index] = m_connected[sourceIndex];
        m_connectableBusId[index] = m_connectableBusId[sourceIndex];
    }
}

void BusTerminal::deleteVariantArrayElement(unsigned long index) {
    Terminal::deleteVariantArrayElement(index);

    m_connectableBusId[index] = "";
}

void BusTerminal::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Terminal::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_connected.resize(m_connected.size() + number, m_connected[sourceIndex]);
    m_connectableBusId.resize(m_connectableBusId.size() + number, m_connectableBusId[sourceIndex]);
}

const std::string& BusTerminal::getConnectableBusId() const {
    return m_connectableBusId[getNetwork().getVariantIndex()];
}

double BusTerminal::getAngle() const {
    const auto& bus = m_busBreakerView.getBus();

    return static_cast<bool>(bus) ? bus.get().getAngle() : stdcxx::nan();
}

const BusTerminal::BusBreakerView& BusTerminal::getBusBreakerView() const {
    return m_busBreakerView;
}

BusTerminal::BusBreakerView& BusTerminal::getBusBreakerView() {
    return m_busBreakerView;
}

const BusTerminal::BusView& BusTerminal::getBusView() const {
    return m_busView;
}

BusTerminal::BusView& BusTerminal::getBusView() {
    return m_busView;
}

const BusTerminal::NodeBreakerView& BusTerminal::getNodeBreakerView() const {
    throw AssertionError("Not implemented");
}

BusTerminal::NodeBreakerView& BusTerminal::getNodeBreakerView() {
    throw AssertionError("Not implemented");
}

double BusTerminal::getV() const {
    const auto& bus = m_busBreakerView.getBus();

    return static_cast<bool>(bus) ? bus.get().getV() : stdcxx::nan();
}

bool BusTerminal::isConnected() const {
    return m_connected[getNetwork().getVariantIndex()];
}

void BusTerminal::reduceVariantArraySize(unsigned long number) {
    Terminal::reduceVariantArraySize(number);

    m_connected.resize(m_connected.size() - number);
    m_connectableBusId.resize(m_connectableBusId.size() - number);
}

BusTerminal& BusTerminal::setConnectableBusId(const std::string& connectableBusId) {
    m_connectableBusId[getNetwork().getVariantIndex()] = connectableBusId;

    return *this;
}

BusTerminal& BusTerminal::setConnected(bool connected) {
    m_connected[getNetwork().getVariantIndex()] = connected;

    return *this;
}

bool BusTerminal::traverse(TopologyTraverser& traverser) {
    auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(getVoltageLevel());
    return voltageLevel.traverse(*this, traverser);
}

bool BusTerminal::traverse(TopologyTraverser& traverser, TerminalSet& traversedTerminals) {
    auto& voltageLevel = dynamic_cast<BusBreakerVoltageLevel&>(getVoltageLevel());
    return voltageLevel.traverse(*this, traverser, traversedTerminals);
}

std::ostream& operator<<(std::ostream& stream, const BusTerminal& busTerminal) {
    stream << stdcxx::simpleClassName(busTerminal) << "[" << busTerminal.getConnectableBusId() << "]";

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
