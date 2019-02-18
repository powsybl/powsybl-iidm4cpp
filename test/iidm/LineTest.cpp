/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(225.0)
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
        .setNominalVoltage(380.0)
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
        .setNominalVoltage(225.0)
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

TEST(Line, constructor) {
    const Network& network = createLineTestNetwork();

    ASSERT_EQ(1ul, network.getLineCount());

    Line& line = network.getLine("VL1_VL3");
    ASSERT_EQ("VL1_VL3", line.getId());
    ASSERT_EQ("", line.getName());
    ASSERT_EQ(ConnectableType::LINE, line.getType());
    ASSERT_FALSE(line.isTieLine());
    ASSERT_DOUBLE_EQ(3.0, line.getR());
    ASSERT_DOUBLE_EQ(33.0, line.getX());
    ASSERT_DOUBLE_EQ(1.0, line.getG1());
    ASSERT_DOUBLE_EQ(0.2, line.getB1());
    ASSERT_DOUBLE_EQ(2.0, line.getG2());
    ASSERT_DOUBLE_EQ(0.4, line.getB2());
}

TEST(Line, integrity) {
    const Network& network = createLineTestNetwork();

    Line& line = network.getLine("VL1_VL3");

    ASSERT_TRUE(stdcxx::areSame(line, line.setR(100)));
    ASSERT_DOUBLE_EQ(100, line.getR());
    POWSYBL_ASSERT_THROW(line.setR(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': r is invalid");

    ASSERT_TRUE(stdcxx::areSame(line, line.setX(200)));
    ASSERT_DOUBLE_EQ(200, line.getX());
    POWSYBL_ASSERT_THROW(line.setX(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': x is invalid");

    ASSERT_TRUE(stdcxx::areSame(line, line.setG1(300)));
    ASSERT_DOUBLE_EQ(300, line.getG1());
    POWSYBL_ASSERT_THROW(line.setG1(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': g1 is invalid");

    ASSERT_TRUE(stdcxx::areSame(line, line.setG2(400)));
    ASSERT_DOUBLE_EQ(400, line.getG2());
    POWSYBL_ASSERT_THROW(line.setG2(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': g2 is invalid");

    ASSERT_TRUE(stdcxx::areSame(line, line.setB1(500)));
    ASSERT_DOUBLE_EQ(500, line.getB1());
    POWSYBL_ASSERT_THROW(line.setB1(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': b1 is invalid");

    ASSERT_TRUE(stdcxx::areSame(line, line.setB2(600)));
    ASSERT_DOUBLE_EQ(600, line.getB2());
    POWSYBL_ASSERT_THROW(line.setB2(stdcxx::nan()), ValidationException, "AC line 'VL1_VL3': b2 is invalid");

    line.remove();
    POWSYBL_ASSERT_THROW(network.getLine("VL1_VL3"), PowsyblException, "Unable to find to the identifiable 'VL1_VL3'");
}

TEST(Line, adder) {
    Network network = createLineTestNetwork();

    ASSERT_EQ(1ul, network.getLineCount());

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

    POWSYBL_ASSERT_THROW(lineAdder.add(), PowsyblException, "Invalid id");
    lineAdder.setId("VL1_VL3");
    POWSYBL_ASSERT_THROW(lineAdder.add(), PowsyblException, "Object 'VL1_VL3' already exists (powsybl::iidm::Line)");
    lineAdder.setName("VL2_VL4");
    lineAdder.setId("UNIQUE_LINE_ID");
    ASSERT_NO_THROW(lineAdder.add());

    ASSERT_EQ(2ul, network.getLineCount());
}

}  // namespace iidm

}  // namespace powsybl
