/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeTerminal.hpp"

#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "NodeBreakerVoltageLevel.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

NodeTerminal::NodeTerminal(VariantManagerHolder& network, unsigned long node) :
    Terminal(network),
    m_node(node),
    m_v(network.getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_angle(network.getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_connectedComponentNumber(network.getVariantManager().getVariantArraySize(), ComponentConstants::INVALID_NUM),
    m_synchronousComponentNumber(network.getVariantManager().getVariantArraySize(), ComponentConstants::INVALID_NUM),
    m_nodeBreakerView(*this),
    m_busBreakerView(*this),
    m_busView(*this) {

}

void NodeTerminal::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Terminal::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_v[index] = m_v[sourceIndex];
        m_angle[index] = m_angle[sourceIndex];
        m_connectedComponentNumber[index] = m_connectedComponentNumber[sourceIndex];
        m_synchronousComponentNumber[index] = m_synchronousComponentNumber[sourceIndex];
    }
}

void NodeTerminal::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Terminal::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_angle.resize(m_angle.size() + number, m_angle[sourceIndex]);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() + number, m_connectedComponentNumber[sourceIndex]);
    m_synchronousComponentNumber.resize(m_synchronousComponentNumber.size() + number, m_synchronousComponentNumber[sourceIndex]);
}

bool NodeTerminal::isConnected() const {
    return dynamic_cast<const NodeBreakerVoltageLevel&>(getVoltageLevel()).isConnected(*this);
}

double NodeTerminal::getAngle() const {
    return m_angle[getNetwork().getVariantIndex()];
}

const NodeTerminal::BusBreakerView& NodeTerminal::getBusBreakerView() const {
    return m_busBreakerView;
}

NodeTerminal::BusBreakerView& NodeTerminal::getBusBreakerView() {
    return m_busBreakerView;
}

const NodeTerminal::BusView& NodeTerminal::getBusView() const {
    return m_busView;
}

NodeTerminal::BusView& NodeTerminal::getBusView() {
    return m_busView;
}

long NodeTerminal::getConnectedComponentNumber() const {
    return m_connectedComponentNumber[getNetwork().getVariantIndex()];
}

unsigned long NodeTerminal::getNode() const {
    return m_node;
}

const NodeTerminal::NodeBreakerView& NodeTerminal::getNodeBreakerView() const {
    return m_nodeBreakerView;
}

NodeTerminal::NodeBreakerView& NodeTerminal::getNodeBreakerView() {
    return m_nodeBreakerView;
}

long NodeTerminal::getSynchronousComponentNumber() const {
    return m_synchronousComponentNumber[getNetwork().getVariantIndex()];
}

double NodeTerminal::getV() const {
    return m_v[getNetwork().getVariantIndex()];
}

void NodeTerminal::reduceVariantArraySize(unsigned long number) {
    Terminal::reduceVariantArraySize(number);

    m_v.resize(m_v.size() - number);
    m_angle.resize(m_angle.size() - number);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() - number);
    m_synchronousComponentNumber.resize(m_synchronousComponentNumber.size() - number);
}

NodeTerminal& NodeTerminal::setAngle(double angle) {
    m_angle[getNetwork().getVariantIndex()] = angle;

    return *this;
}

void NodeTerminal::setConnectedComponentNumber(long connectedComponentNumber) {
    m_connectedComponentNumber[getNetwork().getVariantIndex()] = connectedComponentNumber;
}

void NodeTerminal::setSynchronousComponentNumber(long componentNumber) {
    m_synchronousComponentNumber[getNetwork().getVariantIndex()] = componentNumber;
}

NodeTerminal& NodeTerminal::setV(double v) {
    checkVoltage(getConnectable(), v);

    m_v[getNetwork().getVariantIndex()] = v;

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
