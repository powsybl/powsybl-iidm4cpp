/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createSubstationTransformerCountTestNetwork() {
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

     vl1.getBusBreakerView().newBus()
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

     VoltageLevel& vl3 = substation.newVoltageLevel()
         .setId("VL3")
         .setName("VL3_NAME")
         .setTopologyKind(TopologyKind::BUS_BREAKER)
         .setNominalVoltage(380.0)
         .setLowVoltageLimit(340.0)
         .setHighVoltageLimit(420.0)
         .add();

     vl3.getBusBreakerView().newBus()
         .setId("VL3_BUS1")
         .add();

     Substation& substation2 = network.newSubstation()
         .setId("S2")
         .setName("S2_NAME")
         .setCountry(Country::FR)
         .setTso("TSO")
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

     substation2.newTwoWindingsTransformer()
         .setId("2WT_VL4_VL4")
         .setVoltageLevel1(vl4.getId())
         .setBus1(vl4Bus1.getId())
         .setConnectableBus1(vl4Bus1.getId())
         .setVoltageLevel2(vl4.getId())
         .setBus2(vl4Bus1.getId())
         .setConnectableBus2(vl4Bus1.getId())
         .setR(3.0)
         .setX(33.0)
         .setG(1.0)
         .setB(0.2)
         .setRatedU1(2.0)
         .setRatedU2(0.4)
         .add();

     substation2.newThreeWindingsTransformer()
         .setId("3WT_VL4_VL4_VL4")
         .setName("3WT_VL4_VL4_VL4_NAME")
         .newLeg1()
         .setR(1.3)
         .setX(1.4)
         .setG(1.6)
         .setB(1.7)
         .setRatedU(1.1)
         .setVoltageLevel(vl4.getId())
         .setBus(vl4Bus1.getId())
         .setConnectableBus(vl4Bus1.getId())
         .add()
         .newLeg2()
         .setR(2.3)
         .setX(2.4)
         .setG(0.0)
         .setB(0.0)
         .setRatedU(2.1)
         .setVoltageLevel(vl4.getId())
         .setBus(vl4Bus1.getId())
         .setConnectableBus(vl4Bus1.getId())
         .add()
         .newLeg3()
         .setR(3.3)
         .setX(3.4)
         .setG(0.0)
         .setB(0.0)
         .setRatedU(3.1)
         .setVoltageLevel(vl4.getId())
         .setBus(vl4Bus1.getId())
         .setConnectableBus(vl4Bus1.getId())
         .add()
         .add();

     return network;
}

BOOST_AUTO_TEST_SUITE(SubstationTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getSubstationCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getSubstations()));

    const Substation& substation = network.getSubstation("S1");
    BOOST_CHECK_EQUAL("S1", substation.getId());
    BOOST_CHECK_EQUAL("S1_NAME", substation.getOptionalName());
    POWSYBL_ASSERT_ENUM_EQ(Country::FR, *substation.getCountry());
    BOOST_CHECK_EQUAL("TSO", substation.getTso());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network("id", "test");
    BOOST_CHECK_EQUAL(0UL, network.getSubstationCount());
    BOOST_TEST(boost::empty(network.getSubstations()));

    network.newSubstation()
        .setId("S")
        .setGeographicalTags({"FR", "DE"})
        .addGeographicalTag("IT")
        .add();
    BOOST_CHECK_EQUAL(1UL, network.getSubstationCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getSubstations()));

    Substation& substation = network.getSubstation("S");
    const Substation& cSubstation = network.getSubstation("S");
    BOOST_TEST(stdcxx::areSame(substation, cSubstation));
    BOOST_TEST(stdcxx::areSame(network, substation.getNetwork()));
    BOOST_TEST(stdcxx::areSame(network, cSubstation.getNetwork()));
    BOOST_TEST(!substation.getCountry());
    POWSYBL_ASSERT_ENUM_EQ(Country::BE, *substation.setCountry(Country::BE).setTso("TSO2").getCountry());
    substation.setCountry(stdcxx::optional<Country>());
    BOOST_TEST(!substation.getCountry());
    BOOST_CHECK_EQUAL("TSO2", substation.getTso());
    BOOST_CHECK_EQUAL(3, substation.getGeographicalTags().size());
    substation.addGeographicalTag("LU");
    BOOST_CHECK_EQUAL(4, substation.getGeographicalTags().size());
    substation.addGeographicalTag("IT");
    BOOST_CHECK_EQUAL(4, substation.getGeographicalTags().size());

    substation.setFictitious(true);
    BOOST_CHECK(substation.isFictitious());
    substation.setFictitious(false);
    BOOST_CHECK(!substation.isFictitious());
}

BOOST_AUTO_TEST_CASE(twoWindingsTransformerCount) {
    Network network = createSubstationTransformerCountTestNetwork();
    Substation& substation = network.getSubstation("S1");

    BOOST_CHECK_EQUAL(1UL, network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(0UL, substation.getTwoWindingsTransformerCount());

    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL2")
        .setVoltageLevel1("VL1")
        .setBus1("VL1_BUS1")
        .setConnectableBus1("VL1_BUS1")
        .setVoltageLevel2("VL2")
        .setBus2("VL2_BUS1")
        .setConnectableBus2("VL2_BUS1")
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    BOOST_CHECK_EQUAL(2UL, network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1UL, substation.getTwoWindingsTransformerCount());

    substation.newTwoWindingsTransformer()
        .setId("2WT_VL1_VL1")
        .setVoltageLevel1("VL1")
        .setBus1("VL1_BUS1")
        .setConnectableBus1("VL1_BUS1")
        .setVoltageLevel2("VL1")
        .setBus2("VL1_BUS1")
        .setConnectableBus2("VL1_BUS1")
        .setR(3.0)
        .setX(33.0)
        .setG(1.0)
        .setB(0.2)
        .setRatedU1(2.0)
        .setRatedU2(0.4)
        .add();

    BOOST_CHECK_EQUAL(3UL, network.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(2UL, substation.getTwoWindingsTransformerCount());

    network.remove(substation);
    POWSYBL_ASSERT_THROW(network.getSubstation("S1"), PowsyblException, "Unable to find to the identifiable 'S1'");
}

BOOST_AUTO_TEST_CASE(threeWindingsTransformerCount) {
    Network network = createSubstationTransformerCountTestNetwork();
    Substation& substation = network.getSubstation("S1");

    BOOST_CHECK_EQUAL(1UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(0UL, substation.getThreeWindingsTransformerCount());

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL1_VL2_VL3")
        .setName("3WT_VL1_VL2_VL3_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setVoltageLevel("VL1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .add()
        .newLeg2()
        .setR(2.3)
        .setX(2.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.1)
        .setVoltageLevel("VL2")
        .setBus("VL2_BUS1")
        .setConnectableBus("VL2_BUS1")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .add();

    BOOST_CHECK_EQUAL(2UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1UL, substation.getThreeWindingsTransformerCount());

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL1_VL3_VL3")
        .setName("3WT_VL1_VL3_VL3_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setVoltageLevel("VL1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .add()
        .newLeg2()
        .setR(2.3)
        .setX(2.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .add();

    BOOST_CHECK_EQUAL(3UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(2UL, substation.getThreeWindingsTransformerCount());

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL3_VL1_VL3")
        .setName("3WT_VL3_VL1_VL3_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg2()
        .setR(2.3)
        .setX(2.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.1)
        .setVoltageLevel("VL1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .add();

    BOOST_CHECK_EQUAL(4UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(3UL, substation.getThreeWindingsTransformerCount());

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL3_VL3_VL1")
        .setName("3WT_VL3_VL3_VL1_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg2()
        .setR(2.3)
        .setX(2.4)
        .setG(0)
        .setB(0)
        .setRatedU(2.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.1)
        .setVoltageLevel("VL1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .add()
        .add();

    BOOST_CHECK_EQUAL(5UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(4UL, substation.getThreeWindingsTransformerCount());

    substation.newThreeWindingsTransformer()
        .setId("3WT_VL3_VL3_VL3")
        .setName("3WT_VL3_VL3_VL3_NAME")
        .newLeg1()
        .setR(1.3)
        .setX(1.4)
        .setG(1.6)
        .setB(1.7)
        .setRatedU(1.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg2()
        .setR(2.3)
        .setX(2.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(2.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .newLeg3()
        .setR(3.3)
        .setX(3.4)
        .setG(0.0)
        .setB(0.0)
        .setRatedU(3.1)
        .setVoltageLevel("VL3")
        .setBus("VL3_BUS1")
        .setConnectableBus("VL3_BUS1")
        .add()
        .add();

    BOOST_CHECK_EQUAL(6UL, network.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(5UL, substation.getThreeWindingsTransformerCount());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
