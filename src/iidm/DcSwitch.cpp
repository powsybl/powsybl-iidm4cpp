/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcSwitch.hpp>

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

const Network& DcSwitch::getNetwork() const {
    if(m_dcNode1) {
        return m_dcNode1.get().getNetwork();
    } else if (m_dcNode2) {
        return m_dcNode2.get().getNetwork();
    }
    throw PowsyblException(stdcxx::format("%1% %2% has no network", getTypeDescription(), getId()));
}

Network& DcSwitch::getNetwork() {
    return const_cast<Network&>(static_cast<const DcSwitch*>(this)->getNetwork());
}

const Network& DcSwitch::getParentNetwork() const {
    if(m_dcNode1) {
        return m_dcNode1.get().getParentNetwork();
    } else if (m_dcNode2) {
        return m_dcNode2.get().getParentNetwork();
    }
    return getNetwork();
}

Network& DcSwitch::getParentNetwork() {
    return const_cast<Network&>(static_cast<const DcSwitch*>(this)->getParentNetwork());
}

const IdentifiableType& DcSwitch::getType() const {
    static IdentifiableType s_type = IdentifiableType::DC_SWITCH;
    return s_type;
}

const std::string& DcSwitch::getTypeDescription() const {
    static std::string s_typeDescription = "DC Switch";
    return s_typeDescription;
}

void DcSwitch::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_open[index] = m_open[sourceIndex];
    }
}

void DcSwitch::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_open.resize(m_open.size() + number, m_open[sourceIndex]);
}

void DcSwitch::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_open.resize(m_open.size() - number);
}

DcSwitchKind DcSwitch::getKind() const {
    return m_kind;
}

const DcNode& DcSwitch::getDcNode1() const {
    return m_dcNode1.get();
}
DcNode& DcSwitch::getDcNode1() {
    return m_dcNode1.get();
}

const DcNode& DcSwitch::getDcNode2() const {
    return m_dcNode2.get();
}
DcNode& DcSwitch::getDcNode2() {
    return m_dcNode2.get();
}

bool DcSwitch::isOpen() const {
    return m_open[getNetwork().getVariantIndex()];
}

DcSwitch& DcSwitch::setOpen(bool open) {
    unsigned long index = getNetwork().getVariantIndex();
    bool oldValue = m_open[index];
    if (oldValue != open) {
        m_open[index] = open;
    }
    return *this;
}

void DcSwitch::remove() {
    getNetwork().remove(*this);
}

DcSwitch::DcSwitch(const std::string& id, const std::string& name, bool fictitious, 
                    DcNode& node1, DcNode& node2, const DcSwitchKind& kind, bool open) :
    Identifiable(id, name, fictitious),
    m_dcNode1(node1),
    m_dcNode2(node2),
    m_kind(kind),
    m_open(node1.getNetwork().getVariantManager().getVariantArraySize(), open) {
}

}  // namespace iidm

}  // namespace powsybl
