/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Switch.hpp>

#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

Switch::Switch(VoltageLevel& voltageLevel, const std::string& id, const std::string& name, SwitchKind kind, bool open,
               bool retained, bool fictitious) :
    Identifiable(id, name),
    m_voltageLevel(voltageLevel),
    m_kind(kind),
    m_fictitious(fictitious),
    m_open(voltageLevel.getNetwork().getStateManager().getStateArraySize(), open),
    m_retained(voltageLevel.getNetwork().getStateManager().getStateArraySize(), retained) {
}

void Switch::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_open[index] = m_open[sourceIndex];
        m_retained[index] = m_retained[sourceIndex];
    }
}

void Switch::deleteStateArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

void Switch::extendStateArraySize(unsigned long /*initStateArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_open.resize(m_open.size() + number, m_open[sourceIndex]);
    m_retained.resize(m_retained.size() + number, m_retained[sourceIndex]);
}

SwitchKind Switch::getKind() const {
    return m_kind;
}

const std::string& Switch::getTypeDescription() const {
    static std::string s_typeDescription = "Switch";

    return s_typeDescription;
}

VoltageLevel& Switch::getVoltageLevel() const {
    return m_voltageLevel.get();
}

bool Switch::isFictitious() const {
    return m_fictitious;
}

bool Switch::isOpen() const {
    return m_open[m_voltageLevel.get().getNetwork().getStateIndex()];
}

bool Switch::isRetained() const {
    return m_retained[m_voltageLevel.get().getNetwork().getStateIndex()];
}

void Switch::reduceStateArraySize(unsigned long number) {
    m_open.resize(m_open.size() - number);
    m_retained.resize(m_retained.size() - number);
}

Switch& Switch::setFictitious(bool fictitious) {
    bool oldValue = m_fictitious;
    if (oldValue != fictitious) {
        m_fictitious = fictitious;
        m_voltageLevel.get().invalidateCache();
    }

    return *this;
}

Switch& Switch::setOpen(bool open) {
    unsigned long index = m_voltageLevel.get().getNetwork().getStateIndex();
    bool oldValue = m_open[index];
    if (oldValue != open) {
        m_open[index] = open;
        m_voltageLevel.get().invalidateCache();
    }

    return *this;
}

Switch& Switch::setRetained(bool retained) {
    if (m_voltageLevel.get().getTopologyKind() != TopologyKind::NODE_BREAKER) {
        throw ValidationException(m_voltageLevel.get(), logging::format("retain status is not modifiable in a non node/breaker voltage level"));
    }

    unsigned long index = m_voltageLevel.get().getNetwork().getStateIndex();
    bool oldValue = m_retained[index];
    if (oldValue != retained) {
        m_retained[index] = retained;
        m_voltageLevel.get().invalidateCache();
    }

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
