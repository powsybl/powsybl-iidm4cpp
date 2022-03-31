/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/NodeBreakerTopology.hpp>

#include <functional>
#include <set>

#include <boost/range/adaptor/filtered.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace NodeBreakerTopology {

unsigned long newStandardConnection(BusbarSection& bb) {
    unsigned long n = bb.getTerminal().getNodeBreakerView().getNode();
    VoltageLevel::NodeBreakerView& topo = bb.getTerminal().getVoltageLevel().getNodeBreakerView();

    const long& oldCount = topo.getMaximumNodeIndex() + 1L;
    topo.newDisconnector()
        .setId(stdcxx::format("disconnector %1%-%2%", bb.getId(), oldCount))
        .setNode1(n)
        .setNode2(oldCount)
        .add();
    topo.newBreaker()
        .setId(stdcxx::format("breaker %1%-%2%", bb.getId(), oldCount + 1))
        .setNode1(oldCount)
        .setNode2(oldCount + 1)
        .add();

    return oldCount + 1;
}

void removeIsolatedSwitches(voltage_level::NodeBreakerView& topo) {
    const auto& filter = [&topo](unsigned int node) {
        return static_cast<bool>(topo.getTerminal(node));
    };
    stdcxx::const_range<unsigned long> nodesWithTerminal = topo.getNodes() | boost::adaptors::filtered(filter);

    std::set<std::string> encounteredSwitchIds;
    voltage_level::NodeBreakerView::TopologyTraverser traverser = [&encounteredSwitchIds](unsigned long /*node1*/, const stdcxx::Reference<Switch>& sw, unsigned long /*node2*/) {
        encounteredSwitchIds.emplace(sw.get().getId()); // the traversing started from a terminal, thus the switch is not isolated
        return math::TraverseResult::CONTINUE; // if n2 has a terminal, we could also choose to stop as it will be or has already been traversed
    };
    topo.traverse(nodesWithTerminal, traverser);

    const auto& filterNotEncounteredSwitches = [&encounteredSwitchIds](const Switch& sw) {
        return encounteredSwitchIds.find(sw.getId()) == encounteredSwitchIds.end();
    };
    for (const Switch& sw : topo.getSwitches() | boost::adaptors::filtered(filterNotEncounteredSwitches)) {
        topo.removeSwitch(sw.getId());
    }
}

} // namespace NodeBreakerTopology

}  // namespace iidm

}  // namespace powsybl
