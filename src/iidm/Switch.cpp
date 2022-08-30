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

Switch::Switch(VoltageLevel& voltageLevel, const std::string& id, const std::string& name, bool fictitious, SwitchKind kind, bool open,
               bool retained) :
    Identifiable(id, name, fictitious),
    m_voltageLevel(voltageLevel),
    m_kind(kind),
    m_open(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), open),
    m_retained(voltageLevel.getNetwork().getVariantManager().getVariantArraySize(), retained) {
}

void Switch::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_open[index] = m_open[sourceIndex];
        m_retained[index] = m_retained[sourceIndex];
    }
}

void Switch::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_open.resize(m_open.size() + number, m_open[sourceIndex]);
    m_retained.resize(m_retained.size() + number, m_retained[sourceIndex]);
}

SwitchKind Switch::getKind() const {
    return m_kind;
}

const Network& Switch::getNetwork() const {
    return getVoltageLevel().getNetwork();
}

Network& Switch::getNetwork() {
    return getVoltageLevel().getNetwork();
}

const IdentifiableType& Switch::getType() const {
    static IdentifiableType s_type = IdentifiableType::SWITCH;
    return s_type;
}

const std::string& Switch::getTypeDescription() const {
    static std::string s_typeDescription = "Switch";

    return s_typeDescription;
}

const VoltageLevel& Switch::getVoltageLevel() const {
    return m_voltageLevel.get();
}

VoltageLevel& Switch::getVoltageLevel() {
    return m_voltageLevel.get();
}

bool Switch::isOpen() const {
    return m_open[m_voltageLevel.get().getNetwork().getVariantIndex()];
}

bool Switch::isRetained() const {
    return m_retained[m_voltageLevel.get().getNetwork().getVariantIndex()];
}

void Switch::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_open.resize(m_open.size() - number);
    m_retained.resize(m_retained.size() - number);
}

void Switch::setFictitious(bool fictitious) {
    bool oldValue = isFictitious();
    if (oldValue != fictitious) {
        Identifiable::setFictitious(fictitious);
        m_voltageLevel.get().invalidateCache();
    }
}

Switch& Switch::setOpen(bool open) {
    unsigned long index = m_voltageLevel.get().getNetwork().getVariantIndex();
    bool oldValue = m_open[index];
    if (oldValue != open) {
        m_open[index] = open;
        m_voltageLevel.get().invalidateCache();
    }

    return *this;
}

Switch& Switch::setRetained(bool retained) {
    if (m_voltageLevel.get().getTopologyKind() != TopologyKind::NODE_BREAKER) {
        throw ValidationException(*this, stdcxx::format("retain status is not modifiable in a non node/breaker voltage level"));
    }

    unsigned long index = m_voltageLevel.get().getNetwork().getVariantIndex();
    bool oldValue = m_retained[index];
    if (oldValue != retained) {
        m_retained[index] = retained;
        m_voltageLevel.get().invalidateCache();
    }

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
