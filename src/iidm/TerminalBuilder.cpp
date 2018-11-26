/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TerminalBuilder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

TerminalBuilder::TerminalBuilder(Network& network, Validable& validable) :
    m_network(network),
    m_validable(validable) {

}

const std::string& TerminalBuilder::getConnectionBus() const {
    if (!m_bus.empty()) {
        if ((!m_connectableBus.empty()) && (m_bus != m_connectableBus)) {
            throw ValidationException(m_validable, "connection bus is different from connectable bus");
        }

        return m_bus;
    } else {
        return m_connectableBus;
    }
}

std::unique_ptr<Terminal> TerminalBuilder::build() {
    const std::string& connectionBus = getConnectionBus();
    if (checkOptional(m_node) && !connectionBus.empty()) {
        throw ValidationException(m_validable, "connection node and connection bus are exclusives");
    }

    std::unique_ptr<Terminal> ptrTerminal;
    if (!checkOptional(m_node)) {
        if (connectionBus.empty()) {
            throw ValidationException(m_validable, "connectable bus is not set");
        }

        ptrTerminal = createBusTerminal(m_network, connectionBus, !m_bus.empty());
    } else {
        ptrTerminal = createNodeTerminal(m_network, *m_node);
    }

    return ptrTerminal;
}


TerminalBuilder& TerminalBuilder::setBus(const std::string& bus) {
    m_bus = bus;
    return *this;
}

TerminalBuilder& TerminalBuilder::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return *this;
}

TerminalBuilder& TerminalBuilder::setNode(const stdcxx::optional<unsigned long>& node) {
    m_node = node;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
