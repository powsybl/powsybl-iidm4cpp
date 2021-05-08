/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createTieLineTestNetwork() {
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

    network.newTieLine()
        .setId("TL_VL1_VL3")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl3.getId())
        .setBus2(vl3Bus1.getId())
        .setConnectableBus2(vl3Bus1.getId())
        .setUcteXnodeCode("UcteXnodeCode")
        .newHalfLine1()
        .setId("H1_TL_VL1_VL3")
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL1_VL3")
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add()
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(TieLineTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createTieLineTestNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getLineCount());

    TieLine& modifiableTieLine = dynamic_cast<TieLine&>(network.getLine("TL_VL1_VL3"));
    const auto& tieLine = dynamic_cast<const TieLine&>(modifiableTieLine);
    BOOST_CHECK_EQUAL("TL_VL1_VL3", tieLine.getId());
    BOOST_CHECK(tieLine.getOptionalName().empty());
    BOOST_CHECK_EQUAL(ConnectableType::LINE, tieLine.getType());
    std::ostringstream oss;
    oss << tieLine.getType();
    BOOST_CHECK_EQUAL("LINE", oss.str());
    BOOST_TEST(tieLine.isTieLine());
    BOOST_CHECK_CLOSE(13.0, tieLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(143.0, tieLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.8, tieLine.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.1, tieLine.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.2, tieLine.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.7, tieLine.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("UcteXnodeCode", tieLine.getUcteXnodeCode());

    const TieLine::HalfLine& half1 = tieLine.getHalf1();
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", half1.getId());
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", half1.getName());
    BOOST_CHECK_CLOSE(6.0, half1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(66.0, half1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, half1.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, half1.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, half1.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, half1.getB2(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableTieLine.setR(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(modifiableTieLine.setX(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(modifiableTieLine.setG1(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(modifiableTieLine.setB1(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(modifiableTieLine.setG2(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(modifiableTieLine.setB2(0), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");

    TieLine::HalfLine& modifiableHalfLine = modifiableTieLine.getHalf1();
    POWSYBL_ASSERT_THROW(modifiableHalfLine.setR(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': r is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setR(1.0)));
    BOOST_CHECK_CLOSE(1.0, half1.getR(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableHalfLine.setX(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': x is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setX(2.0)));
    BOOST_CHECK_CLOSE(2.0, half1.getX(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableHalfLine.setG1(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': g1 is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setG1(3.0)));
    BOOST_CHECK_CLOSE(3.0, half1.getG1(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableHalfLine.setB1(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': b1 is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setB1(4.0)));
    BOOST_CHECK_CLOSE(4.0, half1.getB1(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableHalfLine.setG2(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': g2 is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setG2(5.0)));
    BOOST_CHECK_CLOSE(5.0, half1.getG2(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiableHalfLine.setB2(stdcxx::nan()), ValidationException, "Half line 'H1_TL_VL1_VL3': b2 is invalid");
    BOOST_CHECK(stdcxx::areSame(half1, modifiableHalfLine.setB2(6.0)));
    BOOST_CHECK_CLOSE(6.0, half1.getB2(), std::numeric_limits<double>::epsilon());

    const TieLine::HalfLine& half2 = tieLine.getHalf2();
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", half2.getId());
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", half2.getName());
    BOOST_CHECK_CLOSE(7.0, half2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(77.0, half2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.6, half2.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.7, half2.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.9, half2.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.2, half2.getB2(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(half1, tieLine.getHalf(Branch::Side::ONE)));
    BOOST_CHECK(stdcxx::areSame(half2, tieLine.getHalf(Branch::Side::TWO)));
    BOOST_CHECK(stdcxx::areSame(half1, modifiableTieLine.getHalf(Branch::Side::ONE)));
    BOOST_CHECK(stdcxx::areSame(half2, modifiableTieLine.getHalf(Branch::Side::TWO)));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createTieLineTestNetwork();

    TieLine& tieLine = dynamic_cast<TieLine&>(network.getLine("TL_VL1_VL3"));
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", tieLine.getHalf1().getId());
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", tieLine.getHalf2().getId());

    POWSYBL_ASSERT_THROW(tieLine.setR(100), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(tieLine.setX(200), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(tieLine.setG1(300), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(tieLine.setG2(400), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(tieLine.setB1(500), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");
    POWSYBL_ASSERT_THROW(tieLine.setB2(600), ValidationException, "AC tie line 'TL_VL1_VL3': direct modification of characteristics not supported for tie lines");

    tieLine.setFictitious(true);
    BOOST_CHECK(tieLine.isFictitious());
    tieLine.setFictitious(false);
    BOOST_CHECK(!tieLine.isFictitious());

    tieLine.remove();
    POWSYBL_ASSERT_THROW(network.getLine("TL_VL1_VL3"), PowsyblException, "Unable to find to the identifiable 'TL_VL1_VL3'");
}

BOOST_AUTO_TEST_CASE(adderFail) {
    Network network = createTieLineTestNetwork();

    TieLineAdder tieLineAdder = network.newTieLine();
    tieLineAdder.setVoltageLevel1("VL2")
        .setBus1("VL2_BUS1")
        .setConnectableBus1("VL2_BUS1")
        .setVoltageLevel2("VL4")
        .setBus2("VL4_BUS1")
        .setConnectableBus2("VL4_BUS1")
        .setUcteXnodeCode("UcteXnodeCodeTest")
        .newHalfLine1()
        .setId("H1_TL_VL2_VL4")
        .setR(60.0)
        .setX(660.0)
        .setG1(2.0)
        .setB1(4.0)
        .setG2(3.0)
        .setB2(5.0)
        .add()
        .newHalfLine2()
        .setId("H2_TL_VL2_VL4")
        .setR(70.0)
        .setX(770.0)
        .setG1(6.0)
        .setB1(7.0)
        .setG2(9.0)
        .setB2(12.0)
        .add();

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), PowsyblException, "AC tie line id is not set");

}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createTieLineTestNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getLineCount());

    TieLineAdder tieLineAdder = network.newTieLine();

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': First voltage level is not set");
    tieLineAdder.setVoltageLevel1("INVALID");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': First voltage level 'INVALID' not found");
    tieLineAdder.setVoltageLevel1("VL2");

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': Second voltage level is not set");
    tieLineAdder.setVoltageLevel2("INVALID");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': Second voltage level 'INVALID' not found");
    tieLineAdder.setVoltageLevel2("VL4");

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': connectable bus is not set");
    tieLineAdder.setBus1("VL2_BUS1");
    tieLineAdder.setConnectableBus1("INVALID");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': connection bus is different from connectable bus");
    tieLineAdder.setConnectableBus1("");

    TieLineAdder tieLineAdder2 = tieLineAdder;
    tieLineAdder2.setNode1(0);
    POWSYBL_ASSERT_THROW(tieLineAdder2.add(), ValidationException, "AC tie line '': connection node and connection bus are exclusives");

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': connectable bus is not set");
    tieLineAdder.setBus1("");
    tieLineAdder.setConnectableBus1("VL2_BUS1");

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': connectable bus is not set");
    tieLineAdder.setBus2("VL4_BUS1");
    tieLineAdder.setConnectableBus2("INVALID");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': connection bus is different from connectable bus");
    tieLineAdder.setConnectableBus2("");

    TieLineAdder tieLineAdder3 = tieLineAdder;
    tieLineAdder3.setNode2(0);
    POWSYBL_ASSERT_THROW(tieLineAdder3.add(), ValidationException, "AC tie line '': connection node and connection bus are exclusives");
    tieLineAdder.setBus2("");
    tieLineAdder.setConnectableBus2("VL4_BUS1");

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line '': ucteXnodeCode is not set");
    tieLineAdder.setUcteXnodeCode("UcteXnodeCodeTest");
    tieLineAdder.setName("TL_VL2_VL4");
    tieLineAdder.setId("UNIQUE_TIE_LINE_ID");

    //Half1
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': half line 1 is not set");
    auto halfLineAdder1 = tieLineAdder.newHalfLine1();
    halfLineAdder1.setName("H1_TL_VL2_VL4");
    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': id is not set for half line 1");
    halfLineAdder1.setId("H1_TL_VL2_VL4");

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': r is not set for half line 1");
    halfLineAdder1.setR(60.0);

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': x is not set for half line 1");
    halfLineAdder1.setX(660.0);

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': g1 is not set for half line 1");
    halfLineAdder1.setG1(2.0);

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': b1 is not set for half line 1");
    halfLineAdder1.setB1(4.0);

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': g2 is not set for half line 1");
    halfLineAdder1.setG2(3.0);

    POWSYBL_ASSERT_THROW(halfLineAdder1.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': b2 is not set for half line 1");
    halfLineAdder1.setB2(5.0);

    halfLineAdder1.add();

    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': half line 2 is not set");

    //Half2
    auto halfLineAdder2 = tieLineAdder.newHalfLine2();
    halfLineAdder2.setName("H2_TL_VL2_VL4");
    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': id is not set for half line 2");
    halfLineAdder2.setId("H2_TL_VL2_VL4");

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': r is not set for half line 2");
    halfLineAdder2.setR(70.0);

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': x is not set for half line 2");
    halfLineAdder2.setX(700.0);

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': g1 is not set for half line 2");
    halfLineAdder2.setG1(6.0);

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': b1 is not set for half line 2");
    halfLineAdder2.setB1(7.0);

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': g2 is not set for half line 2");
    halfLineAdder2.setG2(9.0);

    POWSYBL_ASSERT_THROW(halfLineAdder2.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': b2 is not set for half line 2");
    halfLineAdder2.setB2(12.0);
    halfLineAdder2.add();

    halfLineAdder2.setFictitious(true);
    halfLineAdder2.add();

    BOOST_CHECK_NO_THROW(tieLineAdder.add());
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), PowsyblException, "The network test already contains an object 'TieLine' with the id 'UNIQUE_TIE_LINE_ID'");
    BOOST_CHECK_EQUAL(2UL, network.getLineCount());

    TieLine& line = static_cast<TieLine&>(network.getLine("UNIQUE_TIE_LINE_ID"));
    BOOST_CHECK(!line.getHalf(Branch::Side::ONE).isFictitious());
    BOOST_CHECK(line.getHalf(Branch::Side::TWO).isFictitious());
    BOOST_CHECK(!line.isFictitious());
}

BOOST_AUTO_TEST_CASE(fictitious) {
    Network network = createTieLineTestNetwork();

    TieLineAdder tieLineAdder = network.newTieLine()
        .setVoltageLevel1("VL2")
        .setVoltageLevel2("VL4")
        .setBus1("")
        .setConnectableBus1("VL2_BUS1")
        .setBus2("")
        .setConnectableBus2("VL4_BUS1")
        .setUcteXnodeCode("UcteXnodeCodeTest")
        .setName("TL_VL2_VL4")
        .setId("UNIQUE_TIE_LINE_ID")
        .setFictitious(true); // test that generated TieLine can be fictitious

    //Half1
    tieLineAdder.newHalfLine1()
        .setName("H1_TL_VL2_VL4")
        .setId("H1_TL_VL2_VL4")
        .setB1(4.0)
        .setB2(5.0)
        .setG1(2.0)
        .setG2(3.0)
        .setR(60.0)
        .setX(660.0)
        .setFictitious(true)
        .add();

    //Half2
    tieLineAdder.newHalfLine2()
        .setName("H2_TL_VL2_VL4")
        .setId("H2_TL_VL2_VL4")
        .setB1(7.0)
        .setB2(12.0)
        .setG1(6.0)
        .setG2(9.0)
        .setR(70.0)
        .setX(700.0)
        .setFictitious(false)
        .add();

    TieLine& line = tieLineAdder.add();
    BOOST_CHECK(line.getHalf(Branch::Side::ONE).isFictitious());
    BOOST_CHECK(!line.getHalf(Branch::Side::TWO).isFictitious());
    BOOST_CHECK(line.isFictitious());
}

BOOST_AUTO_TEST_CASE(getBoundary) {
    Network network = createTieLineTestNetwork();
    TieLine& tieLine = dynamic_cast<TieLine&>(network.getLine("TL_VL1_VL3"));
    const TieLine& cTieLine = dynamic_cast<const TieLine&>(network.getLine("TL_VL1_VL3"));

    tieLine.getTerminal1().getBusView().getBus().get().setAngle(2);
    tieLine.getTerminal1().setP(3);
    tieLine.getTerminal1().setQ(4);
    tieLine.getTerminal1().getBusView().getBus().get().setV(5);
    BOOST_CHECK(stdcxx::areSame(cTieLine.getHalf1().getBoundary(), tieLine.getHalf1().getBoundary()));
    const Boundary& cBoundary = tieLine.getHalf1().getBoundary();
    Boundary& boundary = tieLine.getHalf1().getBoundary();
    BOOST_CHECK_CLOSE(168.31385922271897, boundary.getAngle(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10051.099999999999, boundary.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-16154.5, boundary.getQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(182.584227139148, boundary.getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(cTieLine, cBoundary.getConnectable()));
    BOOST_CHECK(stdcxx::areSame(cTieLine, boundary.getConnectable()));
    BOOST_CHECK_EQUAL(Branch::Side::ONE, *boundary.getSide());
    BOOST_CHECK(stdcxx::areSame(cTieLine.getTerminal(Branch::Side::ONE).getVoltageLevel(), cBoundary.getVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(cTieLine.getTerminal(Branch::Side::ONE).getVoltageLevel(), boundary.getVoltageLevel()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
