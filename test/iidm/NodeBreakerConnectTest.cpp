/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

namespace powsybl {

namespace iidm {

/**
 * <pre>
 *           LD        G
 *           |    B1   |
 *           |---[+]---|
 *       B2 [-]       [+] B3
 *           |    C    |
 *  BBS1 --------[-]-------- BBS2
 * </pre>
 */
Network createNbkNetwork() {
    Network network("test", "test");
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setNode(0)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS2")
        .setNode(1)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("C")
        .setNode1(1)
        .setNode2(0)
        .setOpen(true)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("B2")
        .setNode1(0)
        .setNode2(2)
        .setOpen(true)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("B1")
        .setNode1(2)
        .setNode2(3)
        .setOpen(false)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("B3")
        .setNode1(3)
        .setNode2(1)
        .setOpen(false)
        .add();
    vl.newLoad()
        .setId("LD")
        .setNode(2)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.newGenerator()
        .setId("G")
        .setNode(3)
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(400)
        .setTargetP(1)
        .setTargetQ(0)
        .add();
    return network;
}

/**
 * <pre>
 *     L
 *     |
 *  ---1---
 *  |     |
 * BR1   BR2
 *  |     |
 *  ---0--- BBS1
 * </pre>
 */
Network createDiamondNetwork() {
    Network network("test", "test");
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setNode(0)
        .add();
    vl.newLoad()
        .setId("L")
        .setNode(1)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("BR1")
        .setNode1(1)
        .setNode2(0)
        .setOpen(false)
        .add();
    vl.getNodeBreakerView().newBreaker()
        .setId("BR2")
        .setNode1(1)
        .setNode2(0)
        .setOpen(false)
        .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(NetworkTestSuite)

BOOST_AUTO_TEST_CASE(NodeBreakerConnectConnectedLoad) {
    Network network = createNbkNetwork();
    Load& l = network.getLoad("LD");
    BOOST_CHECK(l.getTerminal().isConnected());
    BOOST_CHECK(network.getSwitch("B2").isOpen());

    l.getTerminal().connect();
    BOOST_CHECK(network.getSwitch("B2").isOpen());
    BOOST_CHECK(l.getTerminal().isConnected());
}

BOOST_AUTO_TEST_CASE(NodeBreakerDisconnectDisconnectedLoad) {
    Network network = createNbkNetwork();
    network.getSwitch("B3").setOpen(true);
    Load& l = network.getLoad("LD");
    BOOST_CHECK(!l.getTerminal().isConnected());

    l.getTerminal().disconnect();
    BOOST_CHECK(!network.getSwitch("B1").isOpen());
    BOOST_CHECK(!l.getTerminal().isConnected());
}

BOOST_AUTO_TEST_CASE(NodeBreakerDisconnectionDiamond) {
    Network network = createDiamondNetwork();
    Load& l = network.getLoad("L");
    BOOST_CHECK(l.getTerminal().isConnected());
    l.getTerminal().disconnect();
    BOOST_CHECK(!l.getTerminal().isConnected());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
