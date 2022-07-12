/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/exception.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkExtension.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

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
        .setNominalV(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

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

    Network network("id", "sourceFormat");
    BOOST_CHECK_EQUAL("id", network.getId());
    BOOST_CHECK_EQUAL("id", network.getOptionalName());
    BOOST_CHECK_EQUAL("sourceFormat", network.getSourceFormat());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());

    stdcxx::DateTime caseDate = stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00");
    network.setCaseDate(caseDate);
    BOOST_CHECK_EQUAL(caseDate, network.getCaseDate());

    const auto& cNetwork = network;
    BOOST_TEST(stdcxx::areSame(network, network.getNetwork()));
    BOOST_TEST(stdcxx::areSame(cNetwork, cNetwork.getNetwork()));
}

BOOST_AUTO_TEST_CASE(move) {
    Network n1("id", "test");
    Substation& s1 = n1.newSubstation()
        .setId("S1")
        .add();

    n1.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "variant2");
    n1.getVariantManager().setWorkingVariant("variant2");

    Network n2 = std::move(n1);

    BOOST_TEST(stdcxx::areSame(n2, s1.getNetwork()));
    BOOST_TEST(1, n2.getSubstationCount());
    BOOST_TEST(2, n2.getVariantManager().getVariantArraySize());
    BOOST_TEST("variant2", n2.getVariantManager().getWorkingVariantId());
}

BOOST_AUTO_TEST_CASE(forecastDistance) {
    Network network("id", "test");

    POWSYBL_ASSERT_THROW(network.setForecastDistance(-1), ValidationException, "Network 'id': Forecast distance is < 0");
    BOOST_TEST(stdcxx::areSame(network, network.setForecastDistance(1)));
    BOOST_CHECK_EQUAL(1, network.getForecastDistance());
}

BOOST_AUTO_TEST_CASE(fictitious) {
    Network network("id", "test");

    BOOST_CHECK(!network.isFictitious());
    network.setFictitious(true);
    BOOST_CHECK(network.isFictitious());
    network.setFictitious(false);
    BOOST_CHECK(!network.isFictitious());
}

BOOST_AUTO_TEST_CASE(country) {
    Network network = createSwitchBBKNetwork();

    BOOST_CHECK_EQUAL(4, network.getSubstationCount());
    BOOST_CHECK_EQUAL(2, network.getCountryCount());

    const std::set<Country>& countries = { Country::ES, Country::FR };
    BOOST_CHECK(countries == network.getCountries());

    network.getSubstation("S2").setCountry(stdcxx::optional<Country>());
    BOOST_CHECK_EQUAL(1, network.getCountryCount());
}

BOOST_AUTO_TEST_CASE(getConnectablesTest) {
    Network network = createSwitchBBKNetwork();
    const Network& cNetwork = network;

    BOOST_CHECK_EQUAL(2, boost::size(network.getConnectables<Load>()));
    BOOST_CHECK_EQUAL(2, boost::size(cNetwork.getConnectables<Load>()));
    BOOST_CHECK_EQUAL(2UL, network.getConnectableCount<Load>());

    BOOST_CHECK_EQUAL(2, boost::size(network.getConnectables<Line>()));
    BOOST_CHECK_EQUAL(2, boost::size(cNetwork.getConnectables<Line>()));
    BOOST_CHECK_EQUAL(2UL, network.getConnectableCount<Line>());

    BOOST_CHECK_EQUAL(1, boost::size(network.getConnectables<TieLine>()));
    BOOST_CHECK_EQUAL(1, boost::size(cNetwork.getConnectables<TieLine>()));
    BOOST_CHECK_EQUAL(1UL, network.getConnectableCount<TieLine>());

    BOOST_CHECK_EQUAL(1, boost::size(network.getConnectables<DanglingLine>()));
    BOOST_CHECK_EQUAL(1, boost::size(cNetwork.getConnectables<DanglingLine>()));
    BOOST_CHECK_EQUAL(1UL, network.getConnectableCount<DanglingLine>());

    BOOST_CHECK_EQUAL(1, boost::size(network.getConnectables<TwoWindingsTransformer>()));
    BOOST_CHECK_EQUAL(1, boost::size(cNetwork.getConnectables<TwoWindingsTransformer>()));
    BOOST_CHECK_EQUAL(1UL, network.getConnectableCount<TwoWindingsTransformer>());

    BOOST_CHECK_EQUAL(3, boost::size(network.getConnectables<Injection>()));
    BOOST_CHECK_EQUAL(3, boost::size(cNetwork.getConnectables<Injection>()));
    BOOST_CHECK_EQUAL(3UL, network.getConnectableCount<Injection>());

    BOOST_CHECK_EQUAL(6UL, boost::size(network.getConnectables()));
    BOOST_CHECK_EQUAL(6UL, boost::size(cNetwork.getConnectables()));
    BOOST_CHECK_EQUAL(6UL, network.getConnectableCount());

    BOOST_CHECK_EQUAL(3UL, boost::size(network.getConnectables<Branch>()));
    BOOST_CHECK_EQUAL(3UL, boost::size(cNetwork.getConnectables<Branch>()));
    BOOST_CHECK_EQUAL(3UL, network.getConnectableCount<Branch>());

    BOOST_CHECK_EQUAL(0, boost::size(network.getConnectables<ThreeWindingsTransformer>()));
    BOOST_CHECK_EQUAL(0, boost::size(cNetwork.getConnectables<ThreeWindingsTransformer>()));
    BOOST_CHECK_EQUAL(0UL, network.getConnectableCount<ThreeWindingsTransformer>());
}

BOOST_AUTO_TEST_CASE(branch) {
    Network network = createSwitchBBKNetwork();
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
    Network network1 = createSwitchBBKNetwork();
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
    POWSYBL_ASSERT_REF_TRUE(network2.getBusView().getBus("VL5_0"));
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
    POWSYBL_ASSERT_REF_TRUE(cNetwork2.getBusBreakerView().getBus("VL5_0"));
    POWSYBL_ASSERT_REF_FALSE(cNetwork2.getBusBreakerView().getBus("UNKNOWN"));
    const auto& cBuses3 = cNetwork2.getBusBreakerView().getBuses();
    BOOST_CHECK_EQUAL(4, boost::size(cBuses3));

    const auto& cBuses4 = cNetwork2.getBusView().getBuses();
    BOOST_CHECK_EQUAL(0, boost::size(cBuses4));
    POWSYBL_ASSERT_REF_FALSE(cNetwork2.getBusView().getBus("UNKNOWN"));
}
 
BOOST_AUTO_TEST_CASE(Properties) {
    Network n("test", "test");

    BOOST_TEST(!n.hasProperty());
    BOOST_TEST(!n.hasProperty("unknown"));
    BOOST_CHECK_EQUAL(0UL, boost::size(n.getPropertyNames()));

    n.setProperty("key1", "value1");
    n.setProperty("key2", "value2");
    BOOST_TEST(n.hasProperty());
    BOOST_CHECK_EQUAL(2UL, boost::size(n.getPropertyNames()));

    BOOST_CHECK_EQUAL("value1", n.getProperty("key1"));

    POWSYBL_ASSERT_THROW(n.getProperty("key3"), stdcxx::PropertyNotFoundException, "Property key3 does not exist");
    BOOST_CHECK_EQUAL("value3", n.getProperty("key3", "value3"));

    BOOST_CHECK_EQUAL(2, boost::size(n.getPropertyNames()));
    BOOST_CHECK(n.removeProperty("key2"));
    BOOST_CHECK_EQUAL(1, boost::size(n.getPropertyNames()));

    BOOST_CHECK(!n.hasProperty("key2"));

    BOOST_CHECK(!n.removeProperty("badKey"));
}

BOOST_AUTO_TEST_CASE(MultiVariant) {
    Network n("test", "test");
    n.addExtension(stdcxx::make_unique<NetworkExtension>(n, true));

    auto& ext = n.getExtension<NetworkExtension>();
    n.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v1", "v2"});

    n.getVariantManager().setWorkingVariant("v1");
    ext.setValue(false);
    BOOST_CHECK_EQUAL(false, ext.getValue());

    n.getVariantManager().setWorkingVariant("v2");
    BOOST_CHECK_EQUAL(true, ext.getValue());
    n.getVariantManager().removeVariant("v2");

    n.getVariantManager().cloneVariant("v1", "v2");
    n.getVariantManager().setWorkingVariant("v2");
    BOOST_CHECK_EQUAL(false, ext.getValue());
}

BOOST_AUTO_TEST_CASE(NullSubstationTestNBK) {
    Network network("test", "test");
    VoltageLevel& voltageLevel = network.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(340.0)
        .add();
    BOOST_CHECK(!voltageLevel.getSubstation());
}

BOOST_AUTO_TEST_CASE(NullSubstationTestBBK) {
    Network network("test", "test");
    VoltageLevel& voltageLevel = network.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(340.0)
        .add();
    BOOST_CHECK(!voltageLevel.getSubstation());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
