/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

Network createDanglingLineTestNetwork() {
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

BOOST_AUTO_TEST_SUITE(DanglingLineTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createDanglingLineTestNetwork();

    unsigned long danglingLineCount = network.getDanglingLineCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    DanglingLineAdder adder = vl1.newDanglingLine()
        .setId("DL1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': p0 is invalid");
    adder.setP0(7.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': q0 is invalid");
    adder.setQ0(8.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': r is invalid");
    adder.setR(9.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': x is invalid");
    adder.setX(10.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': g is invalid");
    adder.setG(11.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Dangling line 'DL1': b is invalid");
    adder.setB(12.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'DL1' already exists (powsybl::iidm::DanglingLine)");
    adder.setId("UNIQUE_DL_ID");

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(danglingLineCount + 1, network.getDanglingLineCount());
    BOOST_TEST(network.getDanglingLine("UNIQUE_DL_ID").getUcteXnodeCode().empty());
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createDanglingLineTestNetwork();

    const DanglingLine& danglingLine = network.getDanglingLine("DL1");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_EQUAL(ConnectableType::DANGLING_LINE, danglingLine.getType());
    std::ostringstream oss;
    oss << danglingLine.getType();
    BOOST_CHECK_EQUAL("DANGLING_LINE", oss.str());
    BOOST_CHECK_CLOSE(1.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createDanglingLineTestNetwork();
    DanglingLine& danglingLine = network.getDanglingLine("DL1");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setB(100.0)));
    BOOST_CHECK_CLOSE(100.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setB(stdcxx::nan()), ValidationException, "Dangling line 'DL1': b is invalid");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setG(200.0)));
    BOOST_CHECK_CLOSE(200.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setG(stdcxx::nan()), ValidationException, "Dangling line 'DL1': g is invalid");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setP0(300.0)));
    BOOST_CHECK_CLOSE(300.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setP0(stdcxx::nan()), ValidationException, "Dangling line 'DL1': p0 is invalid");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setQ0(400.0)));
    BOOST_CHECK_CLOSE(400.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setQ0(stdcxx::nan()), ValidationException, "Dangling line 'DL1': q0 is invalid");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setR(500.0)));
    BOOST_CHECK_CLOSE(500.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setR(stdcxx::nan()), ValidationException, "Dangling line 'DL1': r is invalid");

    BOOST_TEST(stdcxx::areSame(danglingLine, danglingLine.setX(600.0)));
    BOOST_CHECK_CLOSE(600.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(danglingLine.setX(stdcxx::nan()), ValidationException, "Dangling line 'DL1': x is invalid");

    danglingLine.remove();
    POWSYBL_ASSERT_THROW(network.getDanglingLine("DL1"), PowsyblException, "Unable to find to the identifiable 'DL1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createDanglingLineTestNetwork();

    DanglingLine& danglingLine = network.getDanglingLine("DL1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(1.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());
    danglingLine.setB(100.0).setG(200.0).setP0(300.0).setQ0(400).setR(500.0).setX(600.0);

    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(100.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(100.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    danglingLine.setB(150.0).setG(250.0).setP0(350.0).setQ0(450).setR(550.0).setX(650.0);

    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getName());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1ul, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(currentLimits) {
    Network network = createDanglingLineTestNetwork();

    DanglingLine& danglingLine = network.getDanglingLine("DL1");
    const DanglingLine& cDanglingLine = danglingLine;
    BOOST_TEST(stdcxx::areSame(danglingLine, cDanglingLine));

    BOOST_TEST(!cDanglingLine.getCurrentLimits());
    BOOST_TEST(!danglingLine.getCurrentLimits());

    danglingLine.newCurrentLimits()
        .setPermanentLimit(10.0)
        .beginTemporaryLimit()
        .setName("TL1")
        .setValue(13.0)
        .setAcceptableDuration(1UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL2")
        .setValue(12.0)
        .setAcceptableDuration(2UL)
        .setFictitious(true)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("TL3")
        .setValue(11.0)
        .setAcceptableDuration(3UL)
        .setFictitious(false)
        .endTemporaryLimit()
        .add();

    BOOST_TEST(cDanglingLine.getCurrentLimits());
    BOOST_TEST(danglingLine.getCurrentLimits());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
