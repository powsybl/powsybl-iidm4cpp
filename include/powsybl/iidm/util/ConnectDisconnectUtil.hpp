/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_CONNECTDISCONNECTUTIL_HPP
#define POWSYBL_IIDM_UTIL_CONNECTDISCONNECTUTIL_HPP

#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class Identifiable;
class Switch;
class Terminal;

namespace ConnectDisconnectUtil {

    /**
     * Try to connect all the given terminals. If at least one terminal cannot be connected, none are connected
     * 
     * @param terminals The list of all the terminals to connect
     * @param isTypeSwitchToOperate predicate that must verify the switches to be operated
     * @return true if all the given terminals have been connected, false if at least one terminal cannot be connected, or if they are all already connected.
     */
bool connectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);

    /**
     * Try to disconnect all the given terminals. If at least one terminal cannot be disconnected, none are disconnected
     * 
     * @param terminals The list of all the terminals to disconnect
     * @param isTypeSwitchOpenable predicate that must verify the switches to be operated
     * @return true if all the given terminals have been disconnected, false if at least one terminal cannot be disconnected, or if they are all already disconnected.
     */
bool disconnectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchOpenable);

}  // namespace ConnectDisconnectUtil

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_CONNECTDISCONNECTUTIL_HPP
