/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createShuntCompensatorTestNetwork() {
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

    vl1.newShuntCompensator()
        .setId("SHUNT1")
        .setName("SHUNT1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setbPerSection(12.0)
        .setCurrentSectionCount(2UL)
        .setMaximumSectionCount(3UL)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(ShuntCompensatorTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createShuntCompensatorTestNetwork();

    unsigned long shuntCompensatorCount = network.getShuntCompensatorCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    ShuntCompensatorAdder adder = vl1.newShuntCompensator()
        .setId("SHUNT1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is invalid");
    adder.setbPerSection(0.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is equal to zero");
    adder.setbPerSection(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");
    adder.setMaximumSectionCount(0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");
    adder.setMaximumSectionCount(10);
    adder.setCurrentSectionCount(20);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Shunt compensator 'SHUNT1': the current number (20) of section should be lesser than the maximum number of section (10)");
    adder.setCurrentSectionCount(5);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'SHUNT1' already exists (powsybl::iidm::ShuntCompensator)");
    adder.setId("UNIQUE_SHUNT_ID");

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(shuntCompensatorCount + 1, network.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(shuntCompensatorCount + 1, boost::size(network.getShuntCompensators()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createShuntCompensatorTestNetwork();
    BOOST_CHECK_EQUAL(1, boost::size(network.getShuntCompensators()));

    const ShuntCompensator& shunt = network.getShuntCompensator("SHUNT1");
    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_EQUAL(ConnectableType::SHUNT_COMPENSATOR, shunt.getType());
    std::ostringstream oss;
    oss << shunt.getType();
    BOOST_CHECK_EQUAL("SHUNT_COMPENSATOR", oss.str());
    BOOST_CHECK_CLOSE(12.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(3UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(24.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(36.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createShuntCompensatorTestNetwork();

    ShuntCompensator& shunt = network.getShuntCompensator("SHUNT1");

    BOOST_TEST(stdcxx::areSame(shunt, shunt.setbPerSection(100.0)));
    BOOST_CHECK_CLOSE(100.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(shunt.setbPerSection(stdcxx::nan()), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is invalid");
    POWSYBL_ASSERT_THROW(shunt.setbPerSection(0.0), ValidationException, "Shunt compensator 'SHUNT1': susceptance per section is equal to zero");

    BOOST_TEST(stdcxx::areSame(shunt, shunt.setMaximumSectionCount(400UL)));
    BOOST_CHECK_EQUAL(400UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(200.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40000.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(shunt.setMaximumSectionCount(0UL), ValidationException, "Shunt compensator 'SHUNT1': the maximum number of section (0) should be greater than 0");

    BOOST_TEST(stdcxx::areSame(shunt, shunt.setCurrentSectionCount(350UL)));
    BOOST_CHECK_EQUAL(350UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_CLOSE(35000.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40000.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(shunt.setCurrentSectionCount(500UL), ValidationException, "Shunt compensator 'SHUNT1': the current number (500) of section should be lesser than the maximum number of section (400)");
    POWSYBL_ASSERT_THROW(shunt.setMaximumSectionCount(250UL), ValidationException, "Shunt compensator 'SHUNT1': the current number (350) of section should be lesser than the maximum number of section (250)");

    POWSYBL_ASSERT_THROW(shunt.getTerminal().setP(1.0), ValidationException, "Shunt compensator 'SHUNT1': cannot set active power on a shunt compensator");

    shunt.remove();
    POWSYBL_ASSERT_THROW(network.getShuntCompensator("SHUNT1"), PowsyblException, "Unable to find to the identifiable 'SHUNT1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createShuntCompensatorTestNetwork();

    ShuntCompensator& shunt = network.getShuntCompensator("SHUNT1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(12.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(3UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(24.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(36.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
    shunt.setbPerSection(100.0).setMaximumSectionCount(300UL).setCurrentSectionCount(200UL);

    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(100.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(200UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(300UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(20000.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30000.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(100.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(300UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(200.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30000.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());
    shunt.setbPerSection(150.0).setMaximumSectionCount(350UL).setCurrentSectionCount(250UL);

    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(150.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(250UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(350UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(37500.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(52500.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(150.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(350UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(300.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(52500.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("SHUNT1", shunt.getId());
    BOOST_CHECK_EQUAL("SHUNT1_NAME", shunt.getName());
    BOOST_CHECK_CLOSE(150.0, shunt.getbPerSection(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(250UL, shunt.getCurrentSectionCount());
    BOOST_CHECK_EQUAL(350UL, shunt.getMaximumSectionCount());
    BOOST_CHECK_CLOSE(37500.0, shunt.getCurrentB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(52500.0, shunt.getMaximumB(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
