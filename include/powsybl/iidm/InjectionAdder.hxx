/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INJECTIONADDER_HXX
#define POWSYBL_IIDM_INJECTIONADDER_HXX

#include <powsybl/iidm/InjectionAdder.hpp>

#include <powsybl/iidm/BusTerminal.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NodeTerminal.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

template <typename Adder>
const std::string& InjectionAdder<Adder>::getConnectionBus() const {
    if (!m_bus.empty()) {
        if ((!m_connectableBus.empty()) && (m_bus != m_connectableBus)) {
            throw ValidationException(*this, "connection bus is different from connectable bus");
        }

        return m_bus;
    } else {
        return m_connectableBus;
    }
}

template <typename Adder>
std::unique_ptr<Terminal> InjectionAdder<Adder>::getTerminal() {
    const std::string& connectionBus = getConnectionBus();
    if (m_node.is_initialized() && !connectionBus.empty()) {
        throw ValidationException(*this, "connection node and connection bus are exclusives");
    }

    std::unique_ptr<Terminal> ptrTerminal;
    if (!m_node.is_initialized()) {
        if (connectionBus.empty()) {
            throw ValidationException(*this, "connectable bus is not set");
        }

        ptrTerminal = stdcxx::make_unique<BusTerminal>(this->getNetwork(), connectionBus, !m_bus.empty());
    } else {
        ptrTerminal = stdcxx::make_unique<NodeTerminal>(this->getNetwork(), *m_node);
    }

    return ptrTerminal;
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setBus(const std::string& bus) {
    m_bus = bus;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return static_cast<Adder&>(*this);
}

template <typename Adder>
Adder& InjectionAdder<Adder>::setNode(int node) {
    m_node = node;
    return static_cast<Adder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INJECTIONADDER_HXX
