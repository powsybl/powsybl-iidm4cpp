/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_CONNECTEDCOMPONENTS_HPP
#define POWSYBL_IIDM_UTIL_CONNECTEDCOMPONENTS_HPP

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;

namespace ConnectedComponents {

stdcxx::optional<unsigned long> getCcNum(const stdcxx::CReference<Bus>& bus);

stdcxx::optional<unsigned long> getCcNum(const stdcxx::Reference<Bus>& bus);

stdcxx::optional<unsigned long> getCcNum(const Bus& bus);

}  // namespace ConnectedComponents

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_CONNECTEDCOMPONENTS_HPP

