/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/Networks.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

namespace Networks {

stdcxx::CReference<Terminal> getEquivalentTerminal(const VoltageLevel& voltageLevel, unsigned long node) {
    if (voltageLevel.getTopologyKind() != TopologyKind::NODE_BREAKER) {
        throw PowsyblException(stdcxx::format("The voltage level %1% is not described in Node/Breaker topology", voltageLevel.getId()));
    }

    stdcxx::CReference<Terminal> equivalentTerminal;

    VoltageLevel::NodeBreakerView::Traverser traverser = [&equivalentTerminal, &voltageLevel](unsigned long /*node1*/, const stdcxx::Reference<Switch>& sw, unsigned long node2) {
        if (sw && sw.get().isOpen()) {
            return math::TraverseResult::TERMINATE_PATH;
        }
        const auto& terminal = voltageLevel.getNodeBreakerView().getTerminal(node2);
        if (terminal) {
            equivalentTerminal = terminal;
        }
        return terminal ? math::TraverseResult::TERMINATE_TRAVERSER : math::TraverseResult::CONTINUE;
    };

    voltageLevel.getNodeBreakerView().traverse(node, traverser);

    return equivalentTerminal;
}

std::map<std::string, std::set<unsigned long>> getNodesByBus(const VoltageLevel& voltageLevel) {
    if (voltageLevel.getTopologyKind() != TopologyKind::NODE_BREAKER) {
        throw PowsyblException(stdcxx::format("The voltage level %1% is not described in Node/Breaker topology", voltageLevel.getId()));
    }

    std::map<std::string, std::set<unsigned long>> nodesByBus;
    for (unsigned long node : voltageLevel.getNodeBreakerView().getNodes()) {
        const auto& terminal = voltageLevel.getNodeBreakerView().getTerminal(node);
        if (terminal) {
            const auto& bus = terminal.get().getBusView().getBus();
            if (bus) {
                nodesByBus[bus.get().getId()].insert(node);
            }
        } else {
            // If there is no terminal for the current node, we try to find one traversing the topology
            const auto& equivalentTerminal = getEquivalentTerminal(voltageLevel, node);
            if (equivalentTerminal) {
                const auto& bus = equivalentTerminal.get().getBusView().getBus();
                if (bus) {
                    nodesByBus[bus.get().getId()].insert(node);
                }
            }
        }
    }

    return nodesByBus;
}

}  // namespace Networks

}  // namespace iidm

}  // namespace powsybl
