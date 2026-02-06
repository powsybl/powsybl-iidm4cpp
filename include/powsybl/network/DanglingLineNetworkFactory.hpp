/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_DANGLINGLINENETWORKFACTORY_HPP
#define POWSYBL_NETWORK_DANGLINGLINENETWORKFACTORY_HPP

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace network {

class DanglingLineNetworkFactory {
public:
    static iidm::Network create();

    static iidm::Network createWithGeneration();

private:
    static iidm::Network createBase();

    static void createDanglingLineCurrentLimits(iidm::DanglingLine& danglingLine);

};


}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_DANGLINGLINENETWORKFACTORY_HPP
