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
    std::vector<std::reference_wrapper<Terminal> > terminals;

    terminals.reserve(m_terminals.size());
    for (const auto& terminal : m_terminals) {
        terminals.push_back(std::ref(*terminal));
    }

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
        VoltageLevel& voltageLevel = terminal->getVoltageLevel();
        voltageLevel.detach(*terminal);
    }

    network.remove(*this);
}

bool Connectable::connect() {
    return connect(SwitchPredicate::IS_NONFICTIONAL_BREAKER());
}

bool Connectable::connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    bool isAlreadyConnected = true;
    bool isNowConnected = true;

    //Check connected state of terminals
    for (auto& terminal : m_terminals) {
        if (!terminal->isConnected()) {
            isAlreadyConnected = false;
        }
    }
    // Exit if the connectable is already fully connected
    if(isAlreadyConnected) {
        return false;
    }

    //Try connecting all disconnected terminals
    for (auto& terminal : m_terminals) {
        if (terminal->isConnected()) {
            continue;
        }
        isNowConnected = isNowConnected && terminal->connect(isTypeSwitchToOperate);
        // Exit if the terminal cannot be connected
        if (!isNowConnected) {
            return false;
        }
    }
    return isNowConnected;
}

bool Connectable::disconnect() {
    return disconnect(SwitchPredicate::IS_CLOSED_BREAKER());
}
bool Connectable::disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable) {
    bool isAlreadyDisconnected = true;
    bool isNowDisconnected = true;

    //Check connected state of terminals
    for (auto& terminal : m_terminals) {
        if (terminal->isConnected()) {
            isAlreadyDisconnected = false;
        }
    }
    // Exit if the connectable is already fully disconnected
    if(isAlreadyDisconnected) {
        return false;
    }

    //We try to disconnect each connected terminal
    for (auto& terminal : m_terminals) {
        if (!terminal->isConnected()) {
            continue;
        }
        isNowDisconnected = isNowDisconnected && terminal->disconnect(isSwitchOpenable);
        // Exit if the terminal cannot be disconnected
        if (!isNowDisconnected) {
            return false;
        }
    }
    return isNowDisconnected;
}

}  // namespace iidm

}  // namespace powsybl
