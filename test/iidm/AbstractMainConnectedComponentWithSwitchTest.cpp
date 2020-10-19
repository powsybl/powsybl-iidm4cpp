/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(AbstractMainConnectedComponentWithSwitchTest)

BOOST_AUTO_TEST_CASE(test) {
    Network network("test_mcc", "test");

    Substation& s1 = network.newSubstation()
        .setId("A")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("B")
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(0.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl1.getNodeBreakerView().newBusbarSection()
        .setId("C")
        .setNode(0)
        .add();
    vl1.getNodeBreakerView().newSwitch()
        .setId("D")
        .setKind(SwitchKind::DISCONNECTOR)
        .setRetained(false)
        .setOpen(false)
        .setNode1(0)
        .setNode2(1)
        .add();
    vl1.getNodeBreakerView().newSwitch()
        .setId("E")
        .setKind(SwitchKind::BREAKER)
        .setRetained(false)
        .setOpen(false)
        .setNode1(1)
        .setNode2(2)
        .add();

    Substation& s2 = network.newSubstation()
        .setId("F")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("G")
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(0.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl2.getNodeBreakerView().newBusbarSection()
        .setId("H")
        .setNode(0)
        .add();
    vl2.getNodeBreakerView().newBusbarSection()
        .setId("I")
        .setNode(1)
        .add();
    vl2.getNodeBreakerView().newSwitch()
        .setId("J")
        .setKind(SwitchKind::DISCONNECTOR)
        .setRetained(true)
        .setOpen(false)
        .setNode1(0)
        .setNode2(2)
        .add();
    vl2.getNodeBreakerView().newSwitch()
        .setId("K")
        .setKind(SwitchKind::DISCONNECTOR)
        .setRetained(true)
        .setOpen(false)
        .setNode1(1)
        .setNode2(3)
        .add();
    vl2.getNodeBreakerView().newSwitch()
        .setId("L")
        .setKind(SwitchKind::BREAKER)
        .setRetained(true)
        .setOpen(false)
        .setNode1(2)
        .setNode2(3)
        .add();
    vl2.getNodeBreakerView().newSwitch()
        .setId("M")
        .setKind(SwitchKind::BREAKER)
        .setRetained(false)
        .setOpen(false)
        .setNode1(0)
        .setNode2(4)
        .add();

    network.newLine()
        .setId("N")
        .setR(0.001)
        .setX(0.1)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .setVoltageLevel1("B")
        .setNode1(2)
        .setVoltageLevel2("G")
        .setNode2(4)
        .add();

    for (Bus& b : network.getBusView().getBuses()) {
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            b.setV(230.0).setAngle(0.5);
        } else {
            b.setV(220.0).setAngle(0.7);
        }
    }

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getBuses()));
    for (Bus& b : network.getBusView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
    }

    BOOST_CHECK_EQUAL(5, boost::size(network.getBusBreakerView().getBuses()));
    for (Bus& b : network.getBusBreakerView().getBuses()) {
        BOOST_CHECK(b.isInMainConnectedComponent());
        if (stdcxx::areSame(b.getVoltageLevel(), vl1)) {
            BOOST_CHECK_CLOSE(230.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.5, b.getAngle(), std::numeric_limits<double>::epsilon());
        } else {
            BOOST_CHECK_CLOSE(220.0, b.getV(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(0.7, b.getAngle(), std::numeric_limits<double>::epsilon());
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
