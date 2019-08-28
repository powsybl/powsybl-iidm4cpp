/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createTestNetwork() {
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

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL2")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl2.getId())
        .setBus2(vl2Bus1.getId())
        .setConnectableBus2(vl2Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::ES)
        .setTso("TSO2")
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

    vl3.newLoad()
        .setId("LOAD1")
        .setBus(vl3Bus1.getId())
        .setConnectableBus(vl3Bus1.getId())
        .setP0(600.0)
        .setQ0(200.0)
        .add();

    Bus& vl3Bus2 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS2")
        .add();

    vl3.newLoad()
        .setId("LOAD2")
        .setBus(vl3Bus2.getId())
        .setConnectableBus(vl3Bus2.getId())
        .setP0(500.0)
        .setQ0(100.0)
        .add();

    Bus& vl3Bus3 = vl3.getBusBreakerView().newBus()
        .setId("VL3_BUS3")
        .add();

    vl3.getBusBreakerView().newSwitch()
        .setId("SW1")
        .setName("SW1_NAME")
        .setBus1(vl3Bus1.getId())
        .setBus2(vl3Bus2.getId())
        .add();

    vl3.getBusBreakerView().newSwitch()
        .setId("SW2")
        .setName("SW2_NAME")
        .setBus1(vl3Bus2.getId())
        .setBus2(vl3Bus3.getId())
        .add();

    VoltageLevel& vl4 = substation2.newVoltageLevel()
        .setId("VL4")
        .setName("VL4_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    Bus& vl4Bus1 = vl4.getBusBreakerView().newBus()
        .setId("VL4_BUS1")
        .add();

    network.newLine()
        .setId("VL2_VL4")
        .setVoltageLevel1(vl2.getId())
        .setBus1(vl2Bus1.getId())
        .setConnectableBus1(vl2Bus1.getId())
        .setVoltageLevel2(vl4.getId())
        .setBus2(vl4Bus1.getId())
        .setConnectableBus2(vl4Bus1.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
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
        .line1()
        .setId("H1_TL_VL1_VL3")
        .setXnodeP(1)
        .setXnodeQ(2)
        .setR(6.0)
        .setX(66.0)
        .setG1(0.2)
        .setB1(0.4)
        .setG2(0.3)
        .setB2(0.5)
        .line2()
        .setId("H2_TL_VL1_VL3")
        .setXnodeP(3)
        .setXnodeQ(4)
        .setR(7.0)
        .setX(77.0)
        .setG1(0.6)
        .setB1(0.7)
        .setG2(0.9)
        .setB2(1.2)
        .add();

    network.newSubstation()
        .setId("S3")
        .setName("S3_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    network.newSubstation()
        .setId("S4")
        .add();

    vl1.newDanglingLine()
        .setId("DL1")
        .setName("DL1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setB(1.0)
        .setG(2.0)
        .setP0(3.0)
        .setQ0(4.0)
        .setR(5.0)
        .setX(6.0)
        .setUcteXnodeCode("ucteXnodeCodeTest")
        .add();

    return network;
}

Network createTestNodeBreakerNetwork() {
    Network network2("test2", "test2");
    Substation& substation = network2.newSubstation()
        .setId("S4")
        .setName("S4_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setId("VL5")
        .setName("VL5_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    voltageLevel.getNodeBreakerView().setNodeCount(5);
    voltageLevel.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setName("BBS1_NAME")
        .setNode(0)
        .add();

    voltageLevel.getNodeBreakerView().newBreaker()
        .setId("BK")
        .setName("BK_NAME")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    voltageLevel.newLoad()
        .setId("LOAD3")
        .setNode(1)
        .setP0(600.0)
        .setQ0(200.0)
        .add();

    voltageLevel.getNodeBreakerView().newInternalConnection()
        .setId("IC_1")
        .setNode1(1)
        .setNode2(2)
        .add();

    voltageLevel.getNodeBreakerView().newDisconnector()
        .setId("DIS1")
        .setName("DIS1_NAME")
        .setNode1(2)
        .setNode2(3)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    voltageLevel.newLoad()
        .setId("LOAD4")
        .setNode(3)
        .setP0(700.0)
        .setQ0(300.0)
        .add();

    voltageLevel.getNodeBreakerView().newSwitch()
        .setId("LBS1")
        .setName("LBS1_NAME")
        .setKind(SwitchKind::LOAD_BREAK_SWITCH)
        .setNode1(3)
        .setNode2(4)
        .setOpen(false)
        .setRetained(true)
        .setFictitious(false)
        .add();

    voltageLevel.newLoad()
        .setId("LOAD5")
        .setNode(4)
        .setP0(800.0)
        .setQ0(400.0)
        .add();

    return network2;
}

BOOST_AUTO_TEST_SUITE(NetworkTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    POWSYBL_ASSERT_THROW(Network("", ""), PowsyblException, "Invalid id");
    POWSYBL_ASSERT_THROW(Network("id", ""), ValidationException, "Network 'id': Source format is empty");

    Network n("id", "sourceFormat");
    Network network(std::move(n));
    BOOST_CHECK_EQUAL("id", network.getId());
    BOOST_CHECK_EQUAL("id", network.getName());
    BOOST_CHECK_EQUAL("sourceFormat", network.getSourceFormat());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());

    stdcxx::DateTime caseDate = stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00");
    network.setCaseDate(caseDate);
    BOOST_CHECK_EQUAL(caseDate, network.getCaseDate());
}

BOOST_AUTO_TEST_CASE(forecastDistance) {
    Network network("id", "test");

    POWSYBL_ASSERT_THROW(network.setForecastDistance(-1), ValidationException, "Network 'id': Forecast distance is < 0");
    BOOST_TEST(stdcxx::areSame(network, network.setForecastDistance(1)));
    BOOST_CHECK_EQUAL(1, network.getForecastDistance());
}

BOOST_AUTO_TEST_CASE(country) {
    Network network = createTestNetwork();

    BOOST_CHECK_EQUAL(4, network.getSubstationCount());
    BOOST_CHECK_EQUAL(2, network.getCountryCount());

    network.getSubstation("S2").setCountry(stdcxx::optional<Country>());
    BOOST_CHECK_EQUAL(1, network.getCountryCount());
}

BOOST_AUTO_TEST_CASE(branch) {
    Network network = createTestNetwork();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(3, cNetwork.getBranchCount());
    POWSYBL_ASSERT_THROW(cNetwork.getBranch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network.getBranch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(cNetwork.getBranch("DL1"), PowsyblException, "Identifiable 'DL1' is not a powsybl::iidm::Branch");
    POWSYBL_ASSERT_THROW(network.getBranch("DL1"), PowsyblException, "Identifiable 'DL1' is not a powsybl::iidm::Branch");
    BOOST_CHECK_NO_THROW(cNetwork.getBranch("TL_VL1_VL3"));
    BOOST_CHECK_NO_THROW(network.getBranch("TL_VL1_VL3"));

    BOOST_CHECK_EQUAL(boost::size(network.getBranches()), boost::size(cNetwork.getBranches()));
    BOOST_CHECK_EQUAL(boost::size(network.getLines()), boost::size(cNetwork.getLines()));
    BOOST_CHECK_EQUAL(boost::size(network.getTwoWindingsTransformers()), boost::size(cNetwork.getTwoWindingsTransformers()));

    unsigned long lineCount = cNetwork.getLineCount();
    unsigned long branchCount = cNetwork.getBranchCount();
    BOOST_CHECK_EQUAL(lineCount + cNetwork.getTwoWindingsTransformerCount(), branchCount);

    unsigned long lineLoopCount = boost::size(network.getLines());
    unsigned long branchLoopCount = boost::size(network.getBranches());
    BOOST_CHECK_EQUAL(lineLoopCount, lineCount);
    BOOST_CHECK_EQUAL(branchLoopCount, branchCount);
}

BOOST_AUTO_TEST_CASE(views) {
    //BusBreaker
    Network network1 = createTestNetwork();
    BOOST_CHECK_EQUAL(2, network1.getSwitchCount());
    BOOST_CHECK_EQUAL(2, network1.getBusBreakerView().getSwitchCount());
    auto switches = network1.getBusBreakerView().getSwitches();
    BOOST_CHECK_EQUAL(network1.getBusBreakerView().getSwitchCount(), boost::size(switches));
    POWSYBL_ASSERT_REF_TRUE(network1.getBusBreakerView().getBus("VL1_BUS1"));
    POWSYBL_ASSERT_REF_FALSE(network1.getBusBreakerView().getBus("UNKNOWN"));
    auto buses = network1.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(6, boost::size(buses));

    buses = network1.getBusView().getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(buses));
    POWSYBL_ASSERT_REF_TRUE(network1.getBusView().getBus("VL1_0"));
    POWSYBL_ASSERT_REF_FALSE(network1.getBusView().getBus("UNKNOWN"));
    POWSYBL_ASSERT_THROW(network1.getSwitch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network1.getSwitch("DL1"), PowsyblException, "Identifiable 'DL1' is not a powsybl::iidm::Switch");
    BOOST_CHECK_NO_THROW(network1.getSwitch("SW1"));

    network1.getSwitch("SW1").setOpen(true);
    network1.getSwitch("SW2").setOpen(true);
    const Network& cNetwork1 = network1;
    BOOST_CHECK_EQUAL(2, cNetwork1.getSwitchCount());
    BOOST_CHECK_EQUAL(2, cNetwork1.getBusBreakerView().getSwitchCount());
    const auto& cSwitches = cNetwork1.getBusBreakerView().getSwitches();
    BOOST_CHECK_EQUAL(cNetwork1.getBusBreakerView().getSwitchCount(), boost::size(cSwitches));
    POWSYBL_ASSERT_REF_TRUE(cNetwork1.getBusBreakerView().getBus("VL1_BUS1"));
    POWSYBL_ASSERT_REF_FALSE(cNetwork1.getBusBreakerView().getBus("UNKNOWN"));
    const auto& cBuses = cNetwork1.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(6, boost::size(cBuses));

    const auto& cBuses2 = cNetwork1.getBusView().getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(cBuses2));
    POWSYBL_ASSERT_REF_TRUE(cNetwork1.getBusView().getBus("VL1_0"));
    POWSYBL_ASSERT_REF_FALSE(cNetwork1.getBusView().getBus("UNKNOWN"));

    //NodeBreaker
    Network network2 = createTestNodeBreakerNetwork();
    BOOST_CHECK_EQUAL(3, network2.getSwitchCount());
    BOOST_CHECK_EQUAL(3, network2.getBusBreakerView().getSwitchCount());
    switches = network2.getBusBreakerView().getSwitches();
    BOOST_CHECK_EQUAL(network2.getBusBreakerView().getSwitchCount(), boost::size(switches));
    POWSYBL_ASSERT_REF_TRUE(network2.getBusBreakerView().getBus("VL5_0"));
    POWSYBL_ASSERT_REF_FALSE(network2.getBusBreakerView().getBus("UNKNOWN"));
    buses = network2.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(buses));

    buses = network2.getBusView().getBuses();
    BOOST_CHECK_EQUAL(1, boost::size(buses));
    POWSYBL_ASSERT_REF_TRUE(network2.getBusView().getBus("VL5_4"));
    POWSYBL_ASSERT_REF_FALSE(network2.getBusView().getBus("UNKNOWN"));
    POWSYBL_ASSERT_THROW(network2.getSwitch("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network2.getSwitch("BBS1"), PowsyblException, "Identifiable 'BBS1' is not a powsybl::iidm::Switch");
    BOOST_CHECK_NO_THROW(network2.getSwitch("BK"));

    network2.getSwitch("BK").setOpen(true);
    network2.getSwitch("DIS1").setOpen(true);
    network2.getSwitch("LBS1").setOpen(true);
    const Network& cNetwork2 = network2;
    BOOST_CHECK_EQUAL(3, cNetwork2.getSwitchCount());
    BOOST_CHECK_EQUAL(3, cNetwork2.getBusBreakerView().getSwitchCount());
    const auto& cSwitches2 = cNetwork2.getBusBreakerView().getSwitches();
    BOOST_CHECK_EQUAL(cNetwork2.getBusBreakerView().getSwitchCount(), boost::size(cSwitches2));
    POWSYBL_ASSERT_REF_TRUE(cNetwork2.getBusBreakerView().getBus("VL5_5"));
    POWSYBL_ASSERT_REF_FALSE(cNetwork2.getBusBreakerView().getBus("UNKNOWN"));
    const auto& cBuses3 = cNetwork2.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(cBuses3));

    const auto& cBuses4 = cNetwork2.getBusView().getBuses();
    BOOST_CHECK_EQUAL(0, boost::size(cBuses4));
    POWSYBL_ASSERT_REF_FALSE(cNetwork2.getBusView().getBus("UNKNOWN"));
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createTestNetwork();
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().setWorkingVariant("s1");

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().setWorkingVariant("s2");

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");

    BOOST_CHECK_EQUAL(1UL, boost::size(network.getBusView().getConnectedComponents()));

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
