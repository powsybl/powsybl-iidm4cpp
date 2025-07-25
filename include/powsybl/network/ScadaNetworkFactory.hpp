/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_SCADAFACTORY_HPP
#define POWSYBL_NETWORK_SCADAFACTORY_HPP

#include <string>

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace network {

class ScadaNetworkFactory {
public:
    static iidm::Network create();

public:
    ScadaNetworkFactory() = delete;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_SCADAFACTORY_HPP
