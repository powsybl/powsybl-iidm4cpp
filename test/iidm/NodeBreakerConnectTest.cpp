/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
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

/**
 * <pre>
 *     L
 *     |
 *   2 |-------
 *     |      |
 *  -------   |
 *  |     |   D1
 * BR1   D2   |                     LA
 *  |     |   |                      |
 *  ---1---   |                      4
 *     |      |                      |
 *    BR2     |                     BR4
 *     |      |                      |
 *  ---0-------   -----BR3-----   ---3---
 *    BBS1                         BBS2</pre>
 */
Network createNbk2Network() {
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
    vl.getNodeBreakerView().newBusbarSection().setId("BBS1").setNode(0).add();
    vl.getNodeBreakerView().newBusbarSection().setId("BBS2").setNode(3).add();
    vl.getNodeBreakerView().newBreaker().setId("BR3").setNode1(0).setNode2(3).setOpen(false).add();
    vl.getNodeBreakerView().newBreaker().setId("BR4").setNode1(3).setNode2(4).setOpen(false).add();
    vl.newLoad()
        .setId("LA")
        .setNode(4)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.newLoad()
        .setId("L")
        .setNode(2)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.getNodeBreakerView().newBreaker().setId("BR1").setNode1(1).setNode2(2).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D2").setNode1(1).setNode2(2).setOpen(false).add();
    vl.getNodeBreakerView().newBreaker().setId("BR2").setNode1(1).setNode2(0).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D1").setNode1(0).setNode2(2).setOpen(false).add();
    return network;
}

/**
 * <pre>
 *    L1                   L2
 *     |                    |
 *     1                    3
 *     |                    |
 *    D1                   D2
 *     |                    |
 *  ---0---  ----BR----  ---2---
 *    BBS1                BBS2</pre>
 */
Network createNbk3Network() {
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
    vl.getNodeBreakerView().newBusbarSection().setId("BBS1").setNode(0).add();
    vl.getNodeBreakerView().newBusbarSection().setId("BBS2").setNode(2).add();
    vl.getNodeBreakerView().newBreaker().setId("BR").setNode1(0).setNode2(2).setOpen(false).add();
    vl.newLoad()
        .setId("L1")
        .setNode(1)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.newLoad()
        .setId("L2")
        .setNode(3)
        .setP0(1)
        .setQ0(1)
        .add();
    vl.getNodeBreakerView().newDisconnector().setId("D1").setNode1(0).setNode2(1).setOpen(false).add();
    vl.getNodeBreakerView().newDisconnector().setId("D2").setNode1(2).setNode2(3).setOpen(false).add();
    return network;
}

/**
 * <pre>
 *     Load        Line    2WT
 *       |          |       |
 *   ----2---       3       4
 *   |      |       |       |
 *  BR2    BR3     BR4     BR5
 *   |      |       |       |
 *   -----------1------------
 *              |
 *             BR1
 *              |
 *   -----------0------------
 *            BBS1</pre>
 */
Network createNbk4Network() {
    Network network("test", "test");
    Substation& s = network.newSubstation()
                       .setId("S")
                       .setCountry(Country::FR)
                       .add();
    VoltageLevel& vl1 = s.newVoltageLevel()
                           .setId("VL1")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();
    vl1.getNodeBreakerView().newBusbarSection().setId("BBS1").setNode(0).add();
    vl1.getNodeBreakerView().newBreaker().setId("BR1").setNode1(0).setNode2(1).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("BR2").setNode1(1).setNode2(2).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("BR3").setNode1(1).setNode2(2).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("BR4").setNode1(1).setNode2(3).setOpen(false).add();
    vl1.getNodeBreakerView().newBreaker().setId("BR5").setNode1(1).setNode2(4).setOpen(false).add();
    vl1.newLoad()
        .setId("Load")
        .setNode(2)
        .setP0(1)
        .setQ0(1)
        .add();

    VoltageLevel& vl2 = s.newVoltageLevel()
                           .setId("VL2")
                           .setNominalV(400.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();
    vl2.getNodeBreakerView().newBusbarSection().setId("BBS2").setNode(0).add();
    vl2.getNodeBreakerView().newBreaker().setId("BR6").setNode1(0).setNode2(1).setOpen(false).add();
    vl2.getNodeBreakerView().newBreaker().setId("BR7").setNode1(0).setNode2(2).setOpen(false).add();

    s.newTwoWindingsTransformer()
        .setId("2WT")
        .setVoltageLevel1(vl1.getId())
        .setNode1(4)
        .setVoltageLevel2(vl2.getId())
        .setNode2(2)
        .setR(250)
        .setX(100)
        .setG(52)
        .setB(12)
        .setRatedU1(405)
        .setRatedU2(405)
        .add();
    network.newLine()
        .setId("Line")
        .setVoltageLevel1(vl1.getId())
        .setNode1(3)
        .setVoltageLevel2(vl2.getId())
        .setNode2(1)
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
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

BOOST_AUTO_TEST_CASE(NodeBreakerConnectViaVoltageLevelConnectedLoad) {
    Network network = createNbkNetwork();
    Load& l = network.getLoad("LD");
    BOOST_CHECK(l.getTerminal().isConnected());
    BOOST_CHECK(network.getSwitch("B2").isOpen());

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL");
    voltageLevel.getTopologyModel().connect(l.getTerminal());

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
BOOST_AUTO_TEST_CASE(NodeBreakerDisconnectionViaVoltageLevelDiamond) {
    Network network = createDiamondNetwork();
    Load& l = network.getLoad("L");
    BOOST_CHECK(l.getTerminal().isConnected());

    VoltageLevel& voltageLevel = network.getVoltageLevel("VL");
    voltageLevel.getTopologyModel().disconnect(l.getTerminal());

    BOOST_CHECK(!l.getTerminal().isConnected());
}

/**
 * <pre>
 *     L
 *     |
 *  ---1---
 *  |     |
 * BR1   BR2
 *  |     |
 *  ---0--- BBS1</pre>
 */
BOOST_AUTO_TEST_CASE(DisconnectDiamondTest) {
    Network network = createDiamondNetwork();
    Load& l = network.getLoad("L");
    BOOST_CHECK(l.getTerminal().isConnected());
    BOOST_CHECK(l.getTerminal().disconnect());
    BOOST_CHECK(!l.getTerminal().isConnected());
}

/**
 * <pre>
 *     L
 *     |
 *   2 |-------
 *     |      |
 *  -------   |
 *  |     |   D1
 * BR1   D2   |                     LA
 *  |     |   |                      |
 *  ---1---   |                      4
 *     |      |                      |
 *    BR2     |                     BR4
 *     |      |                      |
 *  ---0-------   -----BR3-----   ---3---
 *    BBS1                         BBS2</pre>
 */
BOOST_AUTO_TEST_CASE(DisconnectNbk2Test) {
    Network network = createNbk2Network();
    Load& l = network.getLoad("L");
    BOOST_CHECK(l.getTerminal().isConnected());
    BOOST_CHECK(!l.getTerminal().disconnect());
    BOOST_CHECK(l.getTerminal().isConnected()); // because of D1 which is not openable when using the default predicate
}

/**
 * <pre>
 *    L1                   L2
 *     |                    |
 *     1                    3
 *     |                    |
 *    D1                   D2
 *     |                    |
 *  ---0---  ----BR----  ---2---
 *    BBS1                BBS2</pre>
 */
BOOST_AUTO_TEST_CASE(DisconnectNbk3Test) {
    Network network = createNbk3Network();
    Switch& s = network.getSwitch("BR");
    BOOST_CHECK(!s.isOpen());
    Load& l1 = network.getLoad("L1");
    BOOST_CHECK(l1.getTerminal().isConnected());
    BOOST_CHECK(!l1.getTerminal().disconnect());
    BOOST_CHECK(!s.isOpen());
    BOOST_CHECK(l1.getTerminal().isConnected()); // because of D1 which is not openable when using the default predicate
}

/**
 * <pre>
 *     Load        Line    2WT
 *       |          |       |
 *   ----2---       3       4
 *   |      |       |       |
 *  BR2    BR3     BR4     BR5
 *   |      |       |       |
 *   -----------1------------
 *              |
 *             BR1
 *              |
 *   -----------0------------
 *            BBS1</pre>
 */
BOOST_AUTO_TEST_CASE(DisconnectNbk4Test) {
    Network network = createNbk4Network();
    Switch& s1 = network.getSwitch("BR1");
    Switch& s2 = network.getSwitch("BR2");
    Switch& s3 = network.getSwitch("BR3");
    Switch& s4 = network.getSwitch("BR4");
    Switch& s5 = network.getSwitch("BR5");

    BOOST_CHECK(!s1.isOpen());
    BOOST_CHECK(!s2.isOpen());
    BOOST_CHECK(!s3.isOpen());
    BOOST_CHECK(!s4.isOpen());
    BOOST_CHECK(!s5.isOpen());

    Load& l1 = network.getLoad("Load");
    BOOST_CHECK(l1.getTerminal().isConnected());
    BOOST_CHECK(l1.getTerminal().disconnect());
    BOOST_CHECK(!l1.getTerminal().isConnected());

    BOOST_CHECK(!s1.isOpen());
    BOOST_CHECK(s2.isOpen());
    BOOST_CHECK(s3.isOpen());
    BOOST_CHECK(!s4.isOpen());
    BOOST_CHECK(!s5.isOpen());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
