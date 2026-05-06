/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTerminal.hpp>

#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

namespace powsybl {

namespace iidm {

DcTerminal::DcTerminal(DcNode& dcNode, const TwoSides& side, bool connected) :
    m_dcNode(dcNode),
    m_side(side),
    m_connected(dcNode.getNetwork().getVariantManager().getVariantArraySize(), connected),
    m_p(dcNode.getNetwork().getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_i(dcNode.getNetwork().getVariantManager().getVariantArraySize(), stdcxx::nan()) {
}

void DcTerminal::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_connected[index] = m_connected[sourceIndex];
        m_p[index] = m_p[sourceIndex];
        m_i[index] = m_i[sourceIndex];
    }
}

void DcTerminal::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

void DcTerminal::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_connected.resize(m_connected.size() + number, m_connected[sourceIndex]);
    m_p.resize(m_p.size() + number, m_p[sourceIndex]);
    m_i.resize(m_i.size() + number, m_i[sourceIndex]);
}

void DcTerminal::reduceVariantArraySize(unsigned long number) {
    m_connected.resize(m_connected.size() - number);
    m_p.resize(m_p.size() - number);
    m_i.resize(m_i.size() - number);
}

stdcxx::CReference<DcConnectable> DcTerminal::getDcConnectable() const {
    return stdcxx::cref(m_dcConnectable);
}
stdcxx::Reference<DcConnectable> DcTerminal::getDcConnectable() {
    return m_dcConnectable;
}

TwoSides DcTerminal::getSide() const {
    return m_side;
}

const DcNode& DcTerminal::getDcNode() const {
    return m_dcNode;
}
DcNode& DcTerminal::getDcNode() {
    return m_dcNode;
}

bool DcTerminal::isConnected() const {
    return m_connected.at(getNetwork().getVariantIndex());
}
DcTerminal& DcTerminal::setConnected(bool connected) {
    m_connected[getNetwork().getVariantIndex()] = connected;
    return *this;
}

double DcTerminal::getP() const {
    return m_p.at(getNetwork().getVariantIndex());
}
DcTerminal& DcTerminal::setP(double p) {
    m_p[getNetwork().getVariantIndex()] = p;
    return *this;
}

double DcTerminal::getI() const {
    return m_i.at(getNetwork().getVariantIndex());
}
DcTerminal& DcTerminal::setI(double i) {
    m_i[getNetwork().getVariantIndex()] = i;
    return *this;
}

const Network& DcTerminal::getNetwork() const {
    return m_dcNode.getNetwork();
}
DcTerminal& DcTerminal::setDcConnectable(const stdcxx::Reference<DcConnectable>& dcConnectable) {
    m_dcConnectable = dcConnectable;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
