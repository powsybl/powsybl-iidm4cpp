/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Connectable.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/util/ConnectDisconnectUtil.hpp>

namespace powsybl {

namespace iidm {

void Connectable::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto& terminal : m_terminals) {
        terminal->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

Terminal& Connectable::addTerminal(std::unique_ptr<Terminal>&& terminal) {
    m_terminals.emplace_back(std::move(terminal));
    m_terminals.back()->setConnectable(stdcxx::ref(*this));

    return *m_terminals.back();
}

void Connectable::replaceTerminal(Terminal& oldTerminal, std::unique_ptr<Terminal>&& newTerminal, TopologyModel& topologyModelToAttach) {
    if (!newTerminal) {
        throw PowsyblException(getId() + " cannot replace terminal by null");
    }

    auto it = std::find_if(m_terminals.begin(), m_terminals.end(), [&oldTerminal](const std::unique_ptr<powsybl::iidm::Terminal>& ptrTerminal) {
        return ptrTerminal.get() == std::addressof(oldTerminal);
    });
    if(it!=m_terminals.end()) { //oldTerminal owned by this connectable
        //attach and validates new Terminal
        newTerminal->setConnectable(stdcxx::ref(*this));
        topologyModelToAttach.attach(*newTerminal, false);

        //detach oldTerminal
        oldTerminal.getVoltageLevel().getTopologyModel().detach(oldTerminal);

        //notify Referrers
        oldTerminal.notifyReplacement(*newTerminal);

        //replace owned terminal and deletes old one
        *it = std::move(newTerminal);
    }
}

void Connectable::deleteVariantArrayElement(unsigned long index) {
    Identifiable::deleteVariantArrayElement(index);

    for (auto& terminal : m_terminals) {
        terminal->deleteVariantArrayElement(index);
    }
}

void Connectable::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    for (auto& terminal : m_terminals) {
        terminal->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

const Network& Connectable::getNetwork() const {
    if (m_terminals.empty()) {
        throw PowsyblException(getId() + " is not attached to a network");
    }

    return m_terminals.at(0)->getVoltageLevel().getNetwork();
}

Network& Connectable::getNetwork() {
    return const_cast<Network&>(static_cast<const Connectable*>(this)->getNetwork());
}

const Network& Connectable::getParentNetwork() const {
    // the parent network is the network that contains all terminals of the connectable.
    std::set<std::string> subnetworkIds;
    for (const auto& terminal : m_terminals) {
        subnetworkIds.emplace(terminal->getVoltageLevel().getParentNetwork().getId());
    }
    if(subnetworkIds.size() == 1) {
        return m_terminals.at(0)->getVoltageLevel().getParentNetwork();
    }
    return getNetwork();
}
Network& Connectable::getParentNetwork() {
    return const_cast<Network&>(static_cast<const Connectable*>(this)->getParentNetwork());
}

const Terminal& Connectable::getTerminal(unsigned long index) const {
    return *m_terminals.at(index);
}

Terminal& Connectable::getTerminal(unsigned long index) {
    return *m_terminals.at(index);
}

std::vector<std::reference_wrapper<Terminal> > Connectable::getTerminals() const {
    return getTerminals(stdcxx::optional<ThreeSides>());
}
std::vector<std::reference_wrapper<Terminal> > Connectable::getTerminals(const stdcxx::optional<ThreeSides>& side) const {
    std::vector<std::reference_wrapper<Terminal> > terminals;

    terminals.reserve(m_terminals.size());
    for (const auto& terminal : m_terminals) {
        if (!side.has_value() || terminal->getSide() == side.get()) {
            terminals.push_back(std::ref(*terminal));
        }
    }
    terminals.shrink_to_fit();
    return terminals;
}

void Connectable::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    for (auto& terminal : m_terminals) {
        terminal->reduceVariantArraySize(number);
    }
}

void Connectable::remove() {
    Network& network = getNetwork();

    for (auto& terminal : m_terminals) {
        terminal->notifyRemoval();
        VoltageLevel& voltageLevel = terminal->getVoltageLevel();
        voltageLevel.getTopologyModel().detach(*terminal);
    }

    network.remove(*this);
}

bool Connectable::connect() {
    return connect(SwitchPredicate::IS_NONFICTIONAL_BREAKER());
}

bool Connectable::connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    return connect(isTypeSwitchToOperate, stdcxx::optional<ThreeSides>());
}

bool Connectable::connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<ThreeSides>& side) {
    return ConnectDisconnectUtil::connectAllTerminals(getTerminals(side), isTypeSwitchToOperate);
}

bool Connectable::disconnect() {
    return disconnect(SwitchPredicate::IS_CLOSED_BREAKER());
}
bool Connectable::disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable) {
    return disconnect(isSwitchOpenable, stdcxx::optional<ThreeSides>());
}
bool Connectable::disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<ThreeSides>& side) {
    return ConnectDisconnectUtil::disconnectAllTerminals(getTerminals(side), isSwitchOpenable);
}

}  // namespace iidm

}  // namespace powsybl
