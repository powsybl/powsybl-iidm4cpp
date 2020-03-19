/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_BATTERYNETWORKFACTORY_HPP
#define POWSYBL_NETWORK_BATTERYNETWORKFACTORY_HPP

#include <powsybl/iidm/Network.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace network {

class IIDM_DECLSPEC BatteryNetworkFactory {
public:
    static iidm::Network create();
};


}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_BATTERYNETWORKFACTORY_HPP
