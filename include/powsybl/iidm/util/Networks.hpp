/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_NETWORKS_HPP
#define POWSYBL_IIDM_UTIL_NETWORKS_HPP

#include <map>
#include <set>
#include <string>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;
class Terminal;

namespace Networks {

/**
 * Return a terminal for the specified node.
 * If a terminal is attached to the node, return this terminal. Otherwise, this method traverses the topology and return
 * the closest and equivalent terminal.
 *
 * @param voltageLevel The voltage level to traverse
 * @param node The starting node
 *
 * @return A terminal for the specified node or an invalid {@link stdcxx::reference_wrapper}.
 */
stdcxx::CReference<Terminal> getEquivalentTerminal(const VoltageLevel& voltageLevel, unsigned long node);

/**
 * Return the list of nodes (N/B topology) for each bus of a the Bus view
 * If a node is not associated to a bus, it is not included in any list.
 *
 * @param voltageLevel The voltage level to traverse
 *
 * @return the list of nodes (N/B topology) for each bus of a Bus view
 */
std::map<std::string, std::set<unsigned long>> getNodesByBus(const VoltageLevel& voltageLevel);

}  // namespace Networks

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_NETWORKS_HPP
