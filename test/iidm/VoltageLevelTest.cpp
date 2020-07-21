/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createLineTestNetwork2() {
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

    Bus& vl1Bus2 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS2")
        .add();

    network.newLine()
        .setId("VL1_VL3")
        .setVoltageLevel1(vl1.getId())
        .setBus1(vl1Bus1.getId())
        .setConnectableBus1(vl1Bus1.getId())
        .setVoltageLevel2(vl1.getId())
        .setBus2(vl1Bus2.getId())
        .setConnectableBus2(vl1Bus2.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(1.0)
        .setB1(0.2)
        .setG2(2.0)
        .setB2(0.4)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(VoltageLevelTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createNetwork();
    unsigned long voltageLevelCount = network.getVoltageLevelCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL("VL1", vl1.getId());
    BOOST_CHECK_EQUAL("VL1_NAME", vl1.getOptionalName());
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, vl1.getTopologyKind());
    BOOST_CHECK_CLOSE(340, vl1.getLowVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(420, vl1.getHighVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(380, vl1.getNominalVoltage(), std::numeric_limits<double>::epsilon());

    Substation& s1 = network.getSubstation("S1");
    VoltageLevelAdder adder = s1.newVoltageLevel().setId("VL1");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Nominal voltage is undefined");

    adder.setNominalVoltage(50).setLowVoltageLimit(-100);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Low voltage limit is < 0");

    adder.setLowVoltageLimit(100).setHighVoltageLimit(-10);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': High voltage limit is < 0");

    adder.setHighVoltageLimit(0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [100, 0]");

    adder.setHighVoltageLimit(200);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Voltage level 'VL1': TopologyKind is not set");

    adder.setTopologyKind(static_cast<TopologyKind>(5)).setLowVoltageLimit(stdcxx::nan()).setHighVoltageLimit(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected TopologyKind value: 5");

    adder.setTopologyKind(TopologyKind::BUS_BREAKER);
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'BusBreakerVoltageLevel' with the id 'VL1'");

    adder.setId("UNIQUE_VOLTAGE_LEVEL_ID").setLowVoltageLimit(0).setHighVoltageLimit(0);
    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(voltageLevelCount + 1, network.getVoltageLevelCount());

    POWSYBL_ASSERT_ENUM_EQ(Container::Type::NETWORK, network.getContainerType());
    POWSYBL_ASSERT_ENUM_EQ(Container::Type::SUBSTATION, s1.getContainerType());
    POWSYBL_ASSERT_ENUM_EQ(Container::Type::VOLTAGE_LEVEL, vl1.getContainerType());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createNetwork();

    POWSYBL_ASSERT_THROW(network.getVoltageLevel("UNKNOWN"), PowsyblException, "Unable to find to the identifiable 'UNKNOWN'");
    POWSYBL_ASSERT_THROW(network.getVoltageLevel("LOAD1"), PowsyblException, "Identifiable 'LOAD1' is not a powsybl::iidm::VoltageLevel");

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL(380, vl1.getNominalVoltage());
    BOOST_CHECK_EQUAL(340, vl1.getLowVoltageLimit());
    BOOST_CHECK_EQUAL(420, vl1.getHighVoltageLimit());

    POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(-10), ValidationException, "Voltage level 'VL1': Nominal voltage is <= 0");
    POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(0), ValidationException, "Voltage level 'VL1': Nominal voltage is <= 0");
    POWSYBL_ASSERT_THROW(vl1.setNominalVoltage(stdcxx::nan()), ValidationException, "Voltage level 'VL1': Nominal voltage is undefined");
    BOOST_TEST(stdcxx::areSame(vl1, vl1.setNominalVoltage(100)));
    BOOST_CHECK_EQUAL(100, vl1.getNominalVoltage());

    POWSYBL_ASSERT_THROW(vl1.setLowVoltageLimit(-10), ValidationException, "Voltage level 'VL1': Low voltage limit is < 0");
    POWSYBL_ASSERT_THROW(vl1.setLowVoltageLimit(440), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [440, 420]");
    BOOST_TEST(stdcxx::areSame(vl1, vl1.setLowVoltageLimit(stdcxx::nan())));
    BOOST_TEST(stdcxx::areSame(vl1, vl1.setLowVoltageLimit(360)));
    BOOST_CHECK_EQUAL(360, vl1.getLowVoltageLimit());

    POWSYBL_ASSERT_THROW(vl1.setHighVoltageLimit(-10), ValidationException, "Voltage level 'VL1': High voltage limit is < 0");
    POWSYBL_ASSERT_THROW(vl1.setHighVoltageLimit(320), ValidationException, "Voltage level 'VL1': Inconsistent voltage limit range [360, 320]");
    BOOST_TEST(stdcxx::areSame(vl1, vl1.setHighVoltageLimit(stdcxx::nan())));
    BOOST_TEST(stdcxx::areSame(vl1, vl1.setHighVoltageLimit(440)));
    BOOST_CHECK_EQUAL(440, vl1.getHighVoltageLimit());

    BOOST_CHECK_EQUAL(1, vl1.getConnectableCount());
    BOOST_CHECK_EQUAL(vl1.getConnectableCount(), vl1.getConnectableCount<Connectable>());

    auto& load1 = network.getLoad("LOAD1");
    POWSYBL_ASSERT_REF_TRUE(vl1.getConnectable<Connectable>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl1.getConnectable<Connectable>("LOAD1").get()));
    POWSYBL_ASSERT_REF_TRUE(vl1.getConnectable<Injection>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl1.getConnectable<Injection>("LOAD1").get()));
    POWSYBL_ASSERT_REF_TRUE(vl1.getConnectable<Load>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl1.getConnectable<Load>("LOAD1").get()));
    POWSYBL_ASSERT_REF_FALSE(vl1.getConnectable<Generator>("LOAD1"));

    auto& load2 = vl1.newLoad()
        .setId("LOAD2")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .setName("LOAD2_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(60.0)
        .setQ0(70.0)
        .add();
    BOOST_CHECK_EQUAL(2, vl1.getConnectableCount());
    BOOST_CHECK_EQUAL(vl1.getConnectableCount(), vl1.getConnectableCount<Connectable>());
    BOOST_CHECK_EQUAL(2, vl1.getConnectableCount<Injection>());
    BOOST_CHECK_EQUAL(2, vl1.getConnectableCount<Load>());
    BOOST_CHECK_EQUAL(0, vl1.getConnectableCount<Generator>());
    load2.getTerminal().disconnect();
    BOOST_CHECK_EQUAL(2, vl1.getConnectableCount());
    load2.remove();
    BOOST_CHECK_EQUAL(1, vl1.getConnectableCount());

    // test const version
    const VoltageLevel& vl = vl1;
    BOOST_TEST(stdcxx::areSame(network, vl.getNetwork()));
    BOOST_TEST(stdcxx::areSame(network.getSubstation("S1"), vl.getSubstation()));

    POWSYBL_ASSERT_REF_TRUE(vl.getConnectable<Connectable>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl.getConnectable<Connectable>("LOAD1").get()));
    POWSYBL_ASSERT_REF_TRUE(vl.getConnectable<Injection>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl.getConnectable<Injection>("LOAD1").get()));
    POWSYBL_ASSERT_REF_TRUE(vl.getConnectable<Load>("LOAD1"));
    BOOST_TEST(stdcxx::areSame(load1, vl.getConnectable<Load>("LOAD1").get()));
    POWSYBL_ASSERT_REF_FALSE(vl.getConnectable<Generator>("LOAD1"));
}

BOOST_AUTO_TEST_CASE(getConnectablesCheckUnique) {
    const Network network = createLineTestNetwork2();
    const VoltageLevel& vl = network.getVoltageLevel("VL1");
    BOOST_CHECK_EQUAL(1UL, boost::size(vl.getConnectables<Line>()));
    BOOST_CHECK_EQUAL(1UL, vl.getConnectableCount<Line>());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
