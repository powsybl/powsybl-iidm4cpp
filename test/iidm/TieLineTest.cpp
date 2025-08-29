/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/util/SV.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
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


    double r = 10.0;
    double r2 = 1.0;
    double x = 20.0;
    double x2 = 2.0;
    double hl1g1 = 0.03;
    double hl1g2 = 0.035;
    double hl1b1 = 0.04;
    double hl1b2 = 0.045;
    double hl2g1 = 0.013;
    double hl2g2 = 0.0135;
    double hl2b1 = 0.014;
    double hl2b2 = 0.0145;

    DanglingLine& dl1 = network.getVoltageLevel(vl1.getId()).newDanglingLine()
        .setId("H1_TL_VL1_VL3")
        .setR(r)
        .setX(x)
        .setG(hl1g1 + hl1g2)
        .setB(hl1b1 + hl1b2)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setUcteXnodeCode("UcteXnodeCode")
        .add();
    DanglingLine& dl2 = network.getVoltageLevel(vl3.getId()).newDanglingLine()
        .setId("H2_TL_VL1_VL3")
        .setR(r2)
        .setX(x2)
        .setG(hl2g1 + hl2g2)
        .setB(hl2b1 + hl2b2)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus(vl3Bus1.getId())
        .setConnectableBus(vl3Bus1.getId())
        .setUcteXnodeCode("UcteXnodeCode")
        .add();
    network.newTieLine()
        .setId("TL_VL1_VL3")
        .setDanglingLine1(dl1.getId())
        .setDanglingLine2(dl2.getId())
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(TieLineTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createTieLineTestNetwork();
    constexpr double ACCEPTABLE_THRESHOLD = 1e-6;

    BOOST_CHECK_EQUAL(0UL, network.getLineCount());
    BOOST_CHECK_EQUAL(1UL, network.getTieLineCount());

    TieLine& modifiableTieLine = network.getTieLine("TL_VL1_VL3");
    const auto& tieLine = dynamic_cast<const TieLine&>(modifiableTieLine);
    BOOST_CHECK_EQUAL("TL_VL1_VL3", tieLine.getId());
    BOOST_CHECK(tieLine.getOptionalName().empty());
    BOOST_CHECK_EQUAL(IdentifiableType::TIE_LINE, tieLine.getType());
    std::ostringstream oss;
    oss << tieLine.getType();
    BOOST_CHECK_EQUAL("TIE_LINE", oss.str());
    BOOST_CHECK_CLOSE(11.0, tieLine.getR(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(22.0, tieLine.getX(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(0.065, tieLine.getG1(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(0.085, tieLine.getB1(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(0.0265, tieLine.getG2(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(0.0285, tieLine.getB2(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_EQUAL("UcteXnodeCode", tieLine.getUcteXnodeCode());

    const DanglingLine& dl1 = tieLine.getDanglingLine1();
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", dl1.getId());
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", dl1.getNameOrId());
    BOOST_CHECK_CLOSE(10.0, dl1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, dl1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.03+0.035, dl1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.04+0.045, dl1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, dl1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, dl1.getQ0(), std::numeric_limits<double>::epsilon());

    DanglingLine& modifiabledl1 = modifiableTieLine.getDanglingLine1();
    POWSYBL_ASSERT_THROW(modifiabledl1.setR(stdcxx::nan()), ValidationException, "Dangling line 'H1_TL_VL1_VL3': r is invalid");
    BOOST_CHECK(stdcxx::areSame(dl1, modifiabledl1.setR(1.0)));
    BOOST_CHECK_CLOSE(1.0, dl1.getR(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiabledl1.setX(stdcxx::nan()), ValidationException, "Dangling line 'H1_TL_VL1_VL3': x is invalid");
    BOOST_CHECK(stdcxx::areSame(dl1, modifiabledl1.setX(2.0)));
    BOOST_CHECK_CLOSE(2.0, dl1.getX(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiabledl1.setG(stdcxx::nan()), ValidationException, "Dangling line 'H1_TL_VL1_VL3': g is invalid");
    BOOST_CHECK(stdcxx::areSame(dl1, modifiabledl1.setG(3.0)));
    BOOST_CHECK_CLOSE(3.0, dl1.getG(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(modifiabledl1.setB(stdcxx::nan()), ValidationException, "Dangling line 'H1_TL_VL1_VL3': b is invalid");
    BOOST_CHECK(stdcxx::areSame(dl1, modifiabledl1.setB(4.0)));
    BOOST_CHECK_CLOSE(4.0, dl1.getB(), std::numeric_limits<double>::epsilon());

    const DanglingLine& dl2 = tieLine.getDanglingLine2();
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", dl2.getId());
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", dl2.getNameOrId());
    BOOST_CHECK_CLOSE(1.0, dl2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, dl2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.013+0.0135, dl2.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.014+0.0145, dl2.getB(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(dl1, tieLine.getDanglingLine(Branch::Side::ONE)));
    BOOST_CHECK(stdcxx::areSame(dl2, tieLine.getDanglingLine(Branch::Side::TWO)));
    BOOST_CHECK(stdcxx::areSame(dl1, modifiableTieLine.getDanglingLine(Branch::Side::ONE)));
    BOOST_CHECK(stdcxx::areSame(dl2, modifiableTieLine.getDanglingLine(Branch::Side::TWO)));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createTieLineTestNetwork();

    POWSYBL_ASSERT_THROW(network.getLine("TL_VL1_VL3"), PowsyblException, "Identifiable 'TL_VL1_VL3' is not a powsybl::iidm::Line");

    TieLine& tieLine = network.getTieLine("TL_VL1_VL3");
    BOOST_CHECK_EQUAL("H1_TL_VL1_VL3", tieLine.getDanglingLine1().getId());
    BOOST_CHECK_EQUAL("H2_TL_VL1_VL3", tieLine.getDanglingLine2().getId());

    tieLine.setFictitious(true);
    BOOST_CHECK(tieLine.isFictitious());
    tieLine.setFictitious(false);
    BOOST_CHECK(!tieLine.isFictitious());

    tieLine.remove();
    POWSYBL_ASSERT_THROW(network.getTieLine("TL_VL1_VL3"), PowsyblException, "Unable to find to the identifiable 'TL_VL1_VL3'");
}

BOOST_AUTO_TEST_CASE(adderFail) {
    Network network = createTieLineTestNetwork();

    TieLineAdder tieLineAdder = network.newTieLine();
    
    DanglingLine& dl1 = network.getVoltageLevel("VL2").newDanglingLine()
        .setId("H1_TL_VL2_VL4")
        .setR(6.0)
        .setX(66.0)
        .setG(0.2)
        .setB(0.4)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus("VL2_BUS1")
        .setConnectableBus("VL2_BUS1")
        .setUcteXnodeCode("UcteXnodeCode")
        .add();
    DanglingLine& dl2 = network.getVoltageLevel("VL4").newDanglingLine()
        .setId("H2_TL_VL2_VL4")
        .setR(7.0)
        .setX(77.0)
        .setG(0.8)
        .setB(1.1)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus("VL4_BUS1")
        .setConnectableBus("VL4_BUS1")
        .setUcteXnodeCode("UcteXnodeCode")
        .add();
    
    tieLineAdder.setDanglingLine1(dl1.getId())
                .setDanglingLine2(dl2.getId());
    
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), PowsyblException, "AC tie line id is not set");

}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createTieLineTestNetwork();

    BOOST_CHECK_EQUAL(0UL, network.getLineCount());
    BOOST_CHECK_EQUAL(1UL, network.getTieLineCount());

    TieLineAdder tieLineAdder = network.newTieLine();
    tieLineAdder.setName("TL_VL2_VL4");
    tieLineAdder.setId("UNIQUE_TIE_LINE_ID");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': undefined dangling line");

    //DL1
    auto danglingLineLineAdder1 = network.getVoltageLevel("VL2").newDanglingLine();
    danglingLineLineAdder1.setName("H1_TL_VL2_VL4");
    danglingLineLineAdder1.setId("H1_TL_VL2_VL4");

    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), ValidationException, "Dangling line 'H1_TL_VL2_VL4': p0 is invalid");
    danglingLineLineAdder1.setP0(0.0).setQ0(0.0);

    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), ValidationException, "Dangling line 'H1_TL_VL2_VL4': r is invalid");
    danglingLineLineAdder1.setR(60.0);

    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), ValidationException, "Dangling line 'H1_TL_VL2_VL4': x is invalid");
    danglingLineLineAdder1.setX(660.0);

    danglingLineLineAdder1.setG(stdcxx::nan());
    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), ValidationException, "Dangling line 'H1_TL_VL2_VL4': g is invalid");
    danglingLineLineAdder1.setG(2.0);

    danglingLineLineAdder1.setB(stdcxx::nan());
    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), ValidationException, "Dangling line 'H1_TL_VL2_VL4': b is invalid");
    danglingLineLineAdder1.setB(4.0);

    danglingLineLineAdder1.setId("");
    POWSYBL_ASSERT_THROW(danglingLineLineAdder1.add(), PowsyblException, "Dangling line id is not set");
    danglingLineLineAdder1.setId("H1_TL_VL2_VL4");

    danglingLineLineAdder1.setBus("VL2_BUS1");
    danglingLineLineAdder1.setConnectableBus("");
    danglingLineLineAdder1.setUcteXnodeCode("UcteXnodeCodeTest");
    danglingLineLineAdder1.add();

    tieLineAdder.setDanglingLine1("H1_TL_VL2_VL4");
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': undefined dangling line");

    //Dl2
    auto danglingLineLineAdder2 = network.getVoltageLevel("VL4").newDanglingLine();
    danglingLineLineAdder2.setName("H2_TL_VL2_VL4")
        .setId("H2_TL_VL2_VL4")
        .setP0(0.0)
        .setQ0(0.0)
        .setR(1.0)
        .setB(2.0)
        .setG(3.0)
        .setX(4.0)
        .setBus("VL4_BUS1")
        .setConnectableBus("")
        .setUcteXnodeCode("UcteXnodeCodeTest")
        .setFictitious(true)
        .add();
    tieLineAdder.setDanglingLine2("H2_TL_VL2_VL4");

    BOOST_CHECK_NO_THROW(tieLineAdder.add());
    POWSYBL_ASSERT_THROW(tieLineAdder.add(), ValidationException, "AC tie line 'UNIQUE_TIE_LINE_ID': danglingLine1 (H1_TL_VL2_VL4) already has a tie line");
    BOOST_CHECK_EQUAL(2UL, network.getTieLineCount());

    TieLine& line = network.getTieLine("UNIQUE_TIE_LINE_ID");
    BOOST_CHECK(!line.getDanglingLine(Branch::Side::ONE).isFictitious());
    BOOST_CHECK(line.getDanglingLine(Branch::Side::TWO).isFictitious());
    BOOST_CHECK(!line.isFictitious());
}

BOOST_AUTO_TEST_CASE(fictitious) {
    Network network = createTieLineTestNetwork();

    TieLineAdder tieLineAdder = network.newTieLine()
        .setName("TL_VL2_VL4")
        .setId("UNIQUE_TIE_LINE_ID")
        .setFictitious(true); // test that generated TieLine can be fictitious

    //DL1
    network.getVoltageLevel("VL2").newDanglingLine()
        .setName("H1_TL_VL2_VL4")
        .setId("H1_TL_VL2_VL4")
        .setB(4.0)
        .setG(2.0)
        .setR(60.0)
        .setX(660.0)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus("")
        .setConnectableBus("VL2_BUS1")
        .setUcteXnodeCode("UcteXnodeCodeTest")
        .setFictitious(true)
        .add();

    //Half2
    network.getVoltageLevel("VL4").newDanglingLine()
        .setName("H2_TL_VL2_VL4")
        .setId("H2_TL_VL2_VL4")
        .setB(7.0)
        .setG(6.0)
        .setR(70.0)
        .setX(700.0)
        .setP0(0.0)
        .setQ0(0.0)
        .setBus("")
        .setConnectableBus("VL4_BUS1")
        .setUcteXnodeCode("UcteXnodeCodeTest")
        .setFictitious(false)
        .add();

    tieLineAdder.setDanglingLine1("H1_TL_VL2_VL4").setDanglingLine2("H2_TL_VL2_VL4");

    TieLine& line = tieLineAdder.add();
    BOOST_CHECK(line.getDanglingLine(Branch::Side::ONE).isFictitious());
    BOOST_CHECK(!line.getDanglingLine(Branch::Side::TWO).isFictitious());
    BOOST_CHECK(line.isFictitious());
}

BOOST_AUTO_TEST_CASE(getBoundary) {
    Network network = createTieLineTestNetwork();
    TieLine& tieLine = network.getTieLine("TL_VL1_VL3");
    const TieLine& cTieLine = network.getTieLine("TL_VL1_VL3");

    double p1 = -605.0;
    double q1 = -302.5;
    double p2 = 600.0;
    double q2 = 300.0;
    double v1 = 420.0;
    double v2 = 380.0;
    double angle1 = -1e-4;
    double angle2 = -1.7e-3;

    tieLine.getDanglingLine1().getTerminal().getBusView().getBus().get().setAngle(angle1);
    tieLine.getDanglingLine1().getTerminal().setP(p1);
    tieLine.getDanglingLine1().getTerminal().setQ(q1);
    tieLine.getDanglingLine1().getTerminal().getBusView().getBus().get().setV(v1);
    tieLine.getDanglingLine2().getTerminal().setP(p2).setQ(q2).getBusView().getBus().get().setAngle(angle2).setV(v2);
    BOOST_CHECK(stdcxx::areSame(cTieLine.getDanglingLine1().getBoundary(), tieLine.getDanglingLine1().getBoundary()));
    const Boundary& cBoundary = tieLine.getDanglingLine1().getBoundary();
    Boundary& boundary = tieLine.getDanglingLine1().getBoundary();

    constexpr double ACCEPTABLE_THRESHOLD = 1e-6;
    SV expectedSV1(p1,q1,v1,angle1,Branch::Side::ONE);
    SV expectedSV2(p2,q2,v2,angle2,Branch::Side::TWO);

    BOOST_CHECK_CLOSE(expectedSV1.otherSideP(tieLine.getDanglingLine1(), true), tieLine.getDanglingLine1().getBoundary().getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV1.otherSideQ(tieLine.getDanglingLine1(), true), tieLine.getDanglingLine1().getBoundary().getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV2.otherSideP(tieLine.getDanglingLine2(), true), tieLine.getDanglingLine2().getBoundary().getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV2.otherSideQ(tieLine.getDanglingLine2(), true), tieLine.getDanglingLine2().getBoundary().getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV1.otherSideU(tieLine.getDanglingLine1(), true), tieLine.getDanglingLine1().getBoundary().getV(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV1.otherSideA(tieLine.getDanglingLine1(), true), tieLine.getDanglingLine1().getBoundary().getAngle(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV2.otherSideU(tieLine.getDanglingLine2(), true), tieLine.getDanglingLine2().getBoundary().getV(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(expectedSV2.otherSideA(tieLine.getDanglingLine2(), true), tieLine.getDanglingLine2().getBoundary().getAngle(), ACCEPTABLE_THRESHOLD);

    BOOST_CHECK(stdcxx::areSame(cTieLine, cBoundary.getDanglingLine().getTieLine().get()));
    BOOST_CHECK(stdcxx::areSame(cTieLine, boundary.getDanglingLine().getTieLine().get()));
    BOOST_CHECK(stdcxx::areSame(cTieLine.getDanglingLine(Branch::Side::ONE).getTerminal().getVoltageLevel(), cBoundary.getNetworkSideVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(cTieLine.getDanglingLine(Branch::Side::ONE).getTerminal().getVoltageLevel(), boundary.getNetworkSideVoltageLevel()));
}

BOOST_AUTO_TEST_CASE(defaultValuesTieLine) {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("sub")
        .setCountry(Country::FR)
        .setTso("RTE")
        .add();
    VoltageLevel& voltageLevelA = substation.newVoltageLevel()
        .setId("vl1")
        .setName("vl1")
        .setNominalV(440.0)
        .setHighVoltageLimit(400.0)
        .setLowVoltageLimit(200.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    voltageLevelA.getBusBreakerView().newBus()
        .setId("busA")
        .setName("busA")
        .add();
    VoltageLevel& voltageLevelB = substation.newVoltageLevel()
        .setId("vl2").setName("vl2")
        .setNominalV(200.0)
        .setHighVoltageLimit(400.0)
        .setLowVoltageLimit(200.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    voltageLevelB.getBusBreakerView().newBus()
        .setId("busB")
        .setName("busB")
        .add();

    Substation& s1 = network.newSubstation()
                        .setId("S1")
                        .add();
    VoltageLevel& s1vl1 = s1.newVoltageLevel()
                             .setId("S1VL1")
                             .setNominalV(1.0)
                             .setLowVoltageLimit(0.95)
                             .setHighVoltageLimit(1.05)
                             .setTopologyKind(TopologyKind::BUS_BREAKER)
                             .add();

    s1vl1.getBusBreakerView()
            .newBus()
            .setName("S1VL1-BUS")
            .setId("S1VL1-BUS")
            .add();

    Substation& s2 = network.newSubstation()
                        .setId("S2")
                        .add();
    VoltageLevel& s2vl1 = s2.newVoltageLevel()
                             .setId("S2VL1")
                             .setNominalV(1.0)
                             .setLowVoltageLimit(0.95)
                             .setHighVoltageLimit(1.05)
                             .setTopologyKind(TopologyKind::BUS_BREAKER)
                             .add();

    s2vl1.getBusBreakerView()
            .newBus()
            .setName("S2VL1-BUS")
            .setId("S2VL1-BUS")
            .add();

    std::string boundarySide1 = "Branch::Side::ONE";
    std::string boundarySide2 = "Branch::Side::TWO";

    s1vl1.newDanglingLine()
            .setId(boundarySide1)
            .setName(boundarySide1)
            .setR(1.0)
            .setX(2.0)
            .setP0(0.0)
            .setQ0(0.0)
            .setBus("S1VL1-BUS")
            .setUcteXnodeCode("UcteNode")
            .add();
    s2vl1.newDanglingLine()
            .setId(boundarySide2)
            .setName(boundarySide2)
            .setR(1.0)
            .setX(2.0)
            .setP0(0.0)
            .setQ0(0.0)
            .setBus("S2VL1-BUS")
            .setUcteXnodeCode("UcteNode")
            .add();

    TieLineAdder adder = network.newTieLine()
                             .setId(boundarySide1 + " + " + boundarySide2)
                             .setName(boundarySide1 + " + " + boundarySide2)
                             .setDanglingLine1(boundarySide1)
                             .setDanglingLine2(boundarySide2);

    TieLine& tieLine = adder.add();

    BOOST_CHECK_CLOSE(0.0, tieLine.getDanglingLine1().getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getDanglingLine2().getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getDanglingLine1().getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, tieLine.getDanglingLine2().getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(s1vl1, tieLine.getDanglingLine1().getTerminal().getVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(s2vl1, tieLine.getDanglingLine2().getTerminal().getVoltageLevel()));

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
