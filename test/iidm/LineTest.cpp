/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createLineTestNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    VoltageLevel& vl2 = substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl3 = substation2.newVoltageLevel()
        .setId("VL3")
        .setName("VL3_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl3Bus1 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS1")
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    network.newLine()
        .setId("VL1_VL3")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl3.getId())
        .setBus2(vl3Bus1.getId())
        .setConnectableBus2(vl3Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(LineTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createLineTestNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getLineCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getLines()));

    const Line& line = network.getLine("VL1_VL3");
    BOOST_CHECK_EQUAL("VL1_VL3", line.getId());
    BOOST_CHECK(line.getOptionalName().empty());
    BOOST_CHECK_EQUAL("VL1_VL3", line.getNameOrId());
    BOOST_CHECK_EQUAL(ConnectableType::LINE, line.getType());
    std::ostringstream oss;
    oss << line.getType();
    BOOST_CHECK_EQUAL("LINE", oss.str());
    BOOST_TEST(!line.isTieLine());
    BOOST_CHECK_CLOSE(3.0, line.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, line.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, line.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, line.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, line.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, line.getB2(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createLineTestNetwork();

    Line& line = network.getLine("VL1_VL3");

    BOOST_TEST(stdcxx::areSame(line, line.setR(100)));
    BOOST_CHECK_CLOSE(100, line.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setR(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': r is invalid");

    BOOST_TEST(stdcxx::areSame(line, line.setX(200)));
    BOOST_CHECK_CLOSE(200, line.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setX(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': x is invalid");

    BOOST_TEST(stdcxx::areSame(line, line.setG1(300)));
    BOOST_CHECK_CLOSE(300, line.getG1(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setG1(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': g1 is invalid");

    BOOST_TEST(stdcxx::areSame(line, line.setG2(400)));
    BOOST_CHECK_CLOSE(400, line.getG2(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setG2(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': g2 is invalid");

    BOOST_TEST(stdcxx::areSame(line, line.setB1(500)));
    BOOST_CHECK_CLOSE(500, line.getB1(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setB1(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': b1 is invalid");

    BOOST_TEST(stdcxx::areSame(line, line.setB2(600)));
    BOOST_CHECK_CLOSE(600, line.getB2(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(line.setB2(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': b2 is invalid");

    line.setFictitious(true);
    BOOST_CHECK(line.isFictitious());
    line.setFictitious(false);
    BOOST_CHECK(!line.isFictitious());

    line.remove();
    POWSYBL_ASSERT_THROW(network.getLine("VL1_VL3"), PowsyblException, "Unable to find to the identifiable 'VL1_VL3'");
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createLineTestNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getLineCount());

    LineAdder lineAdder = network.newLine();

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': First voltage level is not set");
    lineAdder.setVoltageLevel1("INVALID");
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': First voltage level 'INVALID' not found");
    lineAdder.setVoltageLevel1("VL2");

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': Second voltage level is not set");
    lineAdder.setVoltageLevel2("INVALID");
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': Second voltage level 'INVALID' not found");
    lineAdder.setVoltageLevel2("VL4");

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': connectable bus is not set");
    lineAdder.setBus1("VL2_BUS1");
    lineAdder.setConnectableBus1("INVALID");
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': connection bus is different from connectable bus");
    lineAdder.setConnectableBus1("");

    LineAdder lineAdder2 = lineAdder;
    lineAdder2.setNode1(0);
    POWSYBL_ASSERT_THROW(lineAdder2.add(), ValidationException, "AC line '': connection node and connection bus are exclusives");

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': connectable bus is not set");
    lineAdder.setBus1("");
    lineAdder.setConnectableBus1("VL2_BUS1");

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': connectable bus is not set");
    lineAdder.setBus2("VL4_BUS1");
    lineAdder.setConnectableBus2("INVALID");
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': connection bus is different from connectable bus");
    lineAdder.setConnectableBus2("");

    LineAdder lineAdder3 = lineAdder;
    lineAdder3.setNode2(0);
    POWSYBL_ASSERT_THROW(lineAdder3.add(), ValidationException, "AC line '': connection node and connection bus are exclusives");
    lineAdder.setBus2("");
    lineAdder.setConnectableBus2("VL4_BUS1");

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': b1 is invalid");
    lineAdder.setB1(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': b1 is invalid");
    lineAdder.setB1(0.02);

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': b2 is invalid");
    lineAdder.setB2(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': b2 is invalid");
    lineAdder.setB2(0.04);

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': g1 is invalid");
    lineAdder.setG1(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': g1 is invalid");
    lineAdder.setG1(4.0);

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': g2 is invalid");
    lineAdder.setG2(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': g2 is invalid");
    lineAdder.setG2(5.0);

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': r is invalid");
    lineAdder.setR(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': r is invalid");
    lineAdder.setR(6.0);

    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': x is invalid");
    lineAdder.setX(stdcxx::nan());
    POWSYBL_ASSERT_THROW(lineAdder.add(), ValidationException, "AC line '': x is invalid");
    lineAdder.setX(66.0);

    POWSYBL_ASSERT_THROW(lineAdder.add(), PowsyblException, "AC line id is not set");
    lineAdder.setId("VL1_VL3");
    POWSYBL_ASSERT_THROW(lineAdder.add(), PowsyblException, "The network test already contains an object 'Line' with the id 'VL1_VL3'");
    lineAdder.setName("VL2_VL4");
    lineAdder.setEnsureIdUnicity(true);
    BOOST_CHECK_NO_THROW(lineAdder.add());

    BOOST_CHECK_EQUAL(2UL, network.getLineCount());
}

BOOST_AUTO_TEST_CASE(terminal) {
    Network network = createLineTestNetwork();
    Line& line = network.getLine("VL1_VL3");
    const Line& cLine = line;

    Terminal& t1 = line.getTerminal("VL1");
    BOOST_CHECK_EQUAL(Branch::Side::ONE, line.getSide(t1));
    BOOST_TEST(stdcxx::areSame(t1, line.getTerminal1()));
    BOOST_TEST(stdcxx::areSame(t1, line.getTerminal(Branch::Side::ONE)));

    const Terminal& cT1 = cLine.getTerminal("VL1");
    BOOST_CHECK_EQUAL(Branch::Side::ONE, cLine.getSide(cT1));
    BOOST_TEST(stdcxx::areSame(cT1, cLine.getTerminal1()));
    BOOST_TEST(stdcxx::areSame(cT1, cLine.getTerminal(Branch::Side::ONE)));

    Terminal& t2 = line.getTerminal("VL3");
    BOOST_CHECK_EQUAL(Branch::Side::TWO, line.getSide(t2));
    BOOST_TEST(stdcxx::areSame(t2, line.getTerminal2()));
    BOOST_TEST(stdcxx::areSame(t2, line.getTerminal(Branch::Side::TWO)));

    const Terminal& cT2 = cLine.getTerminal("VL3");
    BOOST_CHECK_EQUAL(Branch::Side::TWO, cLine.getSide(cT2));
    BOOST_TEST(stdcxx::areSame(cT2, cLine.getTerminal2()));
    BOOST_TEST(stdcxx::areSame(cT2, cLine.getTerminal(Branch::Side::TWO)));

    POWSYBL_ASSERT_THROW(line.getSide(getTerminalFromNetwork2()), AssertionError, "The terminal is not connected to this branch");
    POWSYBL_ASSERT_THROW(cLine.getSide(getTerminalFromNetwork2()), AssertionError, "The terminal is not connected to this branch");

    POWSYBL_ASSERT_THROW(line.getTerminal("VL2"), PowsyblException, "No terminal connected to voltage level VL2");
    POWSYBL_ASSERT_THROW(cLine.getTerminal("VL2"), PowsyblException, "No terminal connected to voltage level VL2");

    Line& line2 = network.newLine()
        .setId("VL2_VL2")
        .setVoltageLevel1("VL2")
        .setBus1("VL2_BUS1")
        .setConnectableBus1("VL2_BUS1")
        .setVoltageLevel2("VL2")
        .setBus2("VL2_BUS1")
        .setConnectableBus2("VL2_BUS1")
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();
    const Line& cLine2 = line2;

    POWSYBL_ASSERT_THROW(line2.getTerminal("VL2"), PowsyblException, "Both terminals are connected to voltage level VL2");
    POWSYBL_ASSERT_THROW(cLine2.getTerminal("VL2"), PowsyblException, "Both terminals are connected to voltage level VL2");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
