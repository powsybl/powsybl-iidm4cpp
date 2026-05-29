/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcConnectable.hpp>

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

const Network& DcConnectable::getNetwork() const {
    if (m_dcTerminals.empty()) {
        throw PowsyblException(getId() + " is not attached to a network");
    }

    return m_dcTerminals.at(0)->getNetwork();
}

Network& DcConnectable::getNetwork()  {
    return const_cast<Network&>(static_cast<const DcConnectable*>(this)->getNetwork());
}

const Network& DcConnectable::getParentNetwork() const {
    // the parent network is the network that contains all dc terminals of the connectable.
    std::set<std::string> subnetworkIds;
    for (const auto& terminal : m_dcTerminals) {
        subnetworkIds.emplace(terminal->getDcNode().getParentNetwork().getId());
    }
    if(subnetworkIds.size() == 1) {
        return m_dcTerminals.at(0)->getDcNode().getParentNetwork();
    }
    return getNetwork();
}

Network& DcConnectable::getParentNetwork() {
    return const_cast<Network&>(static_cast<const DcConnectable*>(this)->getParentNetwork());
}

void DcConnectable::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);
    for (auto& terminal : m_dcTerminals) {
        terminal->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void DcConnectable::deleteVariantArrayElement(unsigned long index) {
    Identifiable::deleteVariantArrayElement(index);
    for (auto& terminal : m_dcTerminals) {
        terminal->deleteVariantArrayElement(index);
    }
}

void DcConnectable::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    for (auto& terminal : m_dcTerminals) {
        terminal->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

void DcConnectable::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);
    for (auto& terminal : m_dcTerminals) {
        terminal->reduceVariantArraySize(number);
    }
}

DcTerminal& DcConnectable::addDcTerminal(std::unique_ptr<DcTerminal>&& dcTerminal) {
    m_dcTerminals.emplace_back(std::move(dcTerminal));
    m_dcTerminals.back()->setDcConnectable(stdcxx::ref(*this));

    return *m_dcTerminals.back();
}

std::vector<std::reference_wrapper<DcTerminal> > DcConnectable::getDcTerminals() const {
    return getDcTerminals(stdcxx::optional<TwoSides>());
}
std::vector<std::reference_wrapper<DcTerminal> > DcConnectable::getDcTerminals(const stdcxx::optional<TwoSides>& side) const {
    std::vector<std::reference_wrapper<DcTerminal> > dcterminals;

    dcterminals.reserve(m_dcTerminals.size());
    for (const auto& terminal : m_dcTerminals) {
        if (!side.has_value() || terminal->getSide() == side.get()) {
            dcterminals.push_back(std::ref(*terminal));
        }
    }
    dcterminals.shrink_to_fit();
    return dcterminals;
}

std::vector<std::reference_wrapper<DcTerminal> > DcConnectable::getDcTerminals(const stdcxx::optional<TerminalNumber>& terminalNumber) const {
    std::vector<std::reference_wrapper<DcTerminal> > dcterminals;

    dcterminals.reserve(m_dcTerminals.size());
    for (const auto& terminal : m_dcTerminals) {
        if (!terminalNumber.has_value() || terminal->getTerminalNumber() == terminalNumber.get()) {
            dcterminals.push_back(std::ref(*terminal));
        }
    }
    dcterminals.shrink_to_fit();
    return dcterminals;
}

void DcConnectable::remove() {
    Network& network = getNetwork();
    network.remove(*this);
}

const DcTerminal& DcConnectable::getDcTerminal(unsigned long index) const {
    return *m_dcTerminals.at(index);
}
DcTerminal& DcConnectable::getDcTerminal(unsigned long index) {
    return *m_dcTerminals.at(index);
}
unsigned long DcConnectable::getDcTerminalsCount() const {
    return m_dcTerminals.size();
}

}  // namespace iidm

}  // namespace powsybl
