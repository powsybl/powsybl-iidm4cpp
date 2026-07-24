/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(NBKConsistencyTestSuite)

Network createNetwork() {

    Network network("testSwitchConsistency", "test");
    Substation& s = network.newSubstation()
                       .setId("S1")
                       .setCountry(Country::FR)
                       .add();
    VoltageLevel& vl = s.newVoltageLevel()
                          .setId("VL1")
                          .setNominalV(10.0)
                          .setTopologyKind(TopologyKind::NODE_BREAKER)
                          .add();

    // Create busbar sections
    vl.getNodeBreakerView().newBusbarSection().setId("BBS1").setNode(0).add();
    vl.getNodeBreakerView().newBusbarSection().setId("BBS2").setNode(2).add();

    // Create 2 switches
    vl.getNodeBreakerView().newBreaker().setId("BREAKER1").setNode1(0).setNode2(1).add();
    vl.getNodeBreakerView().newDisconnector().setId("DISCONNECTOR1").setNode1(1).setNode2(2).add();

    return network;
}


BOOST_AUTO_TEST_CASE(testSwitchConsistencyWithoutInternalConnections) {
    Network network = createNetwork();
    
    VoltageLevel& vl = network.getVoltageLevel("VL1");

    vl.getNodeBreakerView().newBreaker().setId("BREAKER2").setNode1(2).setNode2(3).add();

    auto& topo = vl.getNodeBreakerView();

    //Verify getSwitches return as many as getSwitchCount
    unsigned long switchCount = topo.getSwitchCount();
    unsigned long switchesSize = boost::size(topo.getSwitches());

    BOOST_CHECK_EQUAL(3, switchCount);
    BOOST_CHECK_EQUAL(3, switchesSize);
}

BOOST_AUTO_TEST_CASE(testSwitchConsistencyWithInternalConnections) {
    Network network = createNetwork();
    
    VoltageLevel& vl = network.getVoltageLevel("VL1");

    // Add 3 internal connections
    vl.getNodeBreakerView().newInternalConnection().setNode1(3).setNode2(4).add();
    vl.getNodeBreakerView().newInternalConnection().setNode1(5).setNode2(6).add();
    vl.getNodeBreakerView().newInternalConnection().setNode1(7).setNode2(8).add();

    auto& topo = vl.getNodeBreakerView();

    // Verify that we have 2 switches and 3 internal connections

    unsigned long switchCount = topo.getSwitchCount();
    unsigned long switchesSize = boost::size(topo.getSwitches());
    unsigned long internalConnectionsCount = topo.getInternalConnectionCount();
    unsigned long internalConnectionsSize = boost::size(topo.getInternalConnections());

    BOOST_CHECK_EQUAL(2, switchCount);
    BOOST_CHECK_EQUAL(2, switchesSize);
    BOOST_CHECK_EQUAL(3, internalConnectionsCount);
    BOOST_CHECK_EQUAL(3, internalConnectionsSize);

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
