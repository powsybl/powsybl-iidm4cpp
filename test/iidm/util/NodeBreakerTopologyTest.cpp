/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/NodeBreakerTopology.hpp>

#include "../NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(NodeBreakerTopologyTestSuite)

BOOST_AUTO_TEST_CASE(removeIsolatedSwitches) {
    Network network = createNetworkTest1();

    VoltageLevel& vl = network.getVoltageLevel("voltageLevel1");
    VoltageLevel::NodeBreakerView& topo = vl.getNodeBreakerView();

    BOOST_CHECK(topo.getSwitch("load1Disconnector1"));
    BOOST_CHECK(topo.getSwitch("load1Breaker1"));
    BOOST_CHECK_EQUAL(5, topo.getSwitchCount());

    // remove the load
    vl.getConnectable<Load>("load1").get().remove();
    BOOST_CHECK(!vl.getConnectable<Load>("load1"));

    // remove the switch connected to the bus bar
    topo.removeSwitch("load1Breaker1");
    BOOST_CHECK(!topo.getSwitch("load1Breaker1"));

    // The connecting switch of the load is now isolated: remove it
    NodeBreakerTopology::removeIsolatedSwitches(topo);

    BOOST_CHECK(!topo.getSwitch("load1Disconnector1"));
    BOOST_CHECK(!topo.getSwitch("load1Breaker1"));
    // 2 switches have been removed
    BOOST_CHECK_EQUAL(3, topo.getSwitchCount());
}

BOOST_AUTO_TEST_CASE(newStandardConnection) {
    Network network = createNetworkTest1();

    VoltageLevel& vl = network.getVoltageLevel("voltageLevel1");
    VoltageLevel::NodeBreakerView& topo = vl.getNodeBreakerView();

    unsigned long initialSwitchCount = topo.getSwitchCount();

    BusbarSection& bb = topo.getBusbarSection("voltageLevel1BusbarSection1");
    unsigned long connectionNode = NodeBreakerTopology::newStandardConnection(bb);

    const Load& load = vl.newLoad()
        .setId("load2")
        .setP0(10)
        .setQ0(0)
        .setNode(connectionNode)
        .add();

    // Check the new load is correctly connected to the bus corresponding to the bus bar.
    BOOST_CHECK(stdcxx::areSame(bb.getTerminal().getBusView().getBus().get(), load.getTerminal().getBusView().getBus().get()));
    BOOST_CHECK_EQUAL(initialSwitchCount + 2, topo.getSwitchCount());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
