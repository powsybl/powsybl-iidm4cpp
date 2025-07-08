/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKFACTORY_HPP
#define POWSYBL_IIDM_NETWORKFACTORY_HPP

namespace powsybl {

namespace iidm {

class Network;
class Terminal;

Network createComponentsTestNetworkBB();

Network createComponentsTestNetworkNB();

Network createHvdcConverterStationTestNetwork();

Network createMixedNodeBreakerBusBreakerNetwork();

Network createNetwork();

Network createNetworkTest1();

Network createNodeBreakerNetwork();

Network createSwitchBBKNetwork();

Terminal& getTerminalFromNetwork2();

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKFACTORY_HPP
