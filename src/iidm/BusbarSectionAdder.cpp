/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusbarSectionAdder.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

BusbarSectionAdder::BusbarSectionAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

BusbarSection& BusbarSectionAdder::add() {
    checkOptional(*this, m_node, "Node is not set");

    std::unique_ptr<BusbarSection> ptrBusbarSection = stdcxx::make_unique<BusbarSection>(getNetwork(), getId(), getName());
    BusbarSection& busbarSection = getNetwork().checkAndAdd(std::move(ptrBusbarSection));

    Terminal& terminal = busbarSection.addTerminal(stdcxx::make_unique<NodeTerminal>(getNetwork(), *m_node));
    m_voltageLevel.attach(terminal, false);

    return busbarSection;
}

Network& BusbarSectionAdder::getNetwork() {
    return m_voltageLevel.getNetwork();
}

const std::string& BusbarSectionAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Busbar section";

    return s_typeDescription;
}

BusbarSectionAdder& BusbarSectionAdder::setNode(int node) {
    m_node = node;

    return *this;
}

}

}
