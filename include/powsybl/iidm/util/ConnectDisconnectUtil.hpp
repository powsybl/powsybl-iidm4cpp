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

bool connectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);

bool disconnectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchOpenable);

}  // namespace ConnectDisconnectUtil

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_CONNECTDISCONNECTUTIL_HPP
