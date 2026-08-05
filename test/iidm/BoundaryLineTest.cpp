/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/BoundaryLineUtil.hpp>
#include <powsybl/network/BoundaryLineNetworkFactory.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createBoundaryLineTestNetwork(bool withGeneration) {
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

    BoundaryLineAdder adder = vl1.newBoundaryLine();
    adder.setId("BL1")
        .setName("BL1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setB(1.0)
        .setG(2.0)
        .setP0(3.0)
        .setQ0(4.0)
        .setR(5.0)
        .setX(6.0)
        .setPairingKey("pairKeyTest");

    if (withGeneration) {
        adder.newGeneration()
            .setMinP(100)
            .setMaxP(200)
            .setTargetP(120)
            .setTargetQ(140)
            .setTargetV(160)
            .setVoltageRegulationOn(true)
            .add();
    }

    adder.add();

    return network;
}

BOOST_AUTO_TEST_SUITE(BoundaryLineTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createBoundaryLineTestNetwork(false);

    unsigned long boundaryLineCount = network.getBoundaryLineCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    BoundaryLineAdder adder = vl1.newBoundaryLine()
        .setId("BL1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': p0 is invalid");
    adder.setP0(7.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': q0 is invalid");
    adder.setQ0(8.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': r is invalid");
    adder.setR(9.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': x is invalid");
    adder.setX(10.0);

    adder.setG(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': g is invalid");
    adder.setG(11.0);

    adder.setB(stdcxx::nan());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Boundary line 'BL1': b is invalid");
    adder.setB(12.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'BoundaryLine' with the id 'BL1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(boundaryLineCount + 1, network.getBoundaryLineCount());
    BOOST_CHECK_EQUAL(boundaryLineCount + 1, boost::size(network.getBoundaryLines()));
    BOOST_TEST(network.getBoundaryLine("BL1#0").getPairingKey().empty());

    BoundaryLine& bl = network.getBoundaryLine("BL1");
    BOOST_CHECK(!bl.getGeneration());

    adder.setId("BL2");

    BoundaryLineAdder::GenerationAdder gAdder = adder.newGeneration();

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (nan) for activePowerSetpoint");
    gAdder.setMinP(100).setMaxP(50);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': Invalid active limits [100, 50]");
    gAdder.setMinP(100).setMaxP(200);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (nan) for activePowerSetpoint");
    gAdder.setTargetP(120);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (nan) for reactivePowerSetpoint (voltage regulator is off)");
    gAdder.setTargetQ(140);
    gAdder.setVoltageRegulationOn(true);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (nan) for voltageSetpoint (voltage regulator is on)");
    gAdder.setTargetV(-1);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (-1) for voltageSetpoint (voltage regulator is on)");
    gAdder.setTargetV(0);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Boundary line 'BL2': invalid value (0) for voltageSetpoint (voltage regulator is on)");
    gAdder.setTargetV(160);

    BOOST_CHECK_NO_THROW(gAdder.add().add());

    BoundaryLine& bl2 = network.getBoundaryLine("BL2");
    BOOST_CHECK(bl2.getGeneration());

    BOOST_CHECK_CLOSE(100, bl2.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setMinP(101)));
    BOOST_CHECK_CLOSE(101, bl2.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(200, bl2.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setMaxP(201)));
    BOOST_CHECK_CLOSE(201, bl2.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(120, bl2.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setTargetP(121)));
    BOOST_CHECK_CLOSE(121, bl2.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(140, bl2.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setTargetQ(141)));
    BOOST_CHECK_CLOSE(141, bl2.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(160, bl2.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setTargetV(161)));
    BOOST_CHECK_CLOSE(161, bl2.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(bl2.getGeneration().get().isVoltageRegulationOn());
    BOOST_CHECK(stdcxx::areSame(bl2.getGeneration().get(), bl2.getGeneration().get().setVoltageRegulationOn(false)));
    BOOST_CHECK(!bl2.getGeneration().get().isVoltageRegulationOn());

    // Test if new Generation is instantiate at each add
    BoundaryLine& bl3 = adder.setId("BL3").add();
    BOOST_CHECK(!stdcxx::areSame(bl2.getGeneration(), bl3.getGeneration()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createBoundaryLineTestNetwork(false);

    const BoundaryLine& boundaryLine = network.getBoundaryLine("BL1");
    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_EQUAL("BL1_NAME", boundaryLine.getOptionalName());
    BOOST_CHECK_EQUAL(IdentifiableType::BOUNDARY_LINE, boundaryLine.getType());
    std::ostringstream oss;
    oss << boundaryLine.getType();
    BOOST_CHECK_EQUAL("BOUNDARY_LINE", oss.str());
    BOOST_CHECK_CLOSE(1.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("pairKeyTest", boundaryLine.getPairingKey());
    BOOST_CHECK_EQUAL(1, boost::size(network.getBoundaryLines()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createBoundaryLineTestNetwork(false);
    BoundaryLine& boundaryLine = network.getBoundaryLine("BL1");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setB(100.0)));
    BOOST_CHECK_CLOSE(100.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setB(stdcxx::nan()), ValidationException, "Boundary line 'BL1': b is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setG(200.0)));
    BOOST_CHECK_CLOSE(200.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setG(stdcxx::nan()), ValidationException, "Boundary line 'BL1': g is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setP0(300.0)));
    BOOST_CHECK_CLOSE(300.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setP0(stdcxx::nan()), ValidationException, "Boundary line 'BL1': p0 is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setQ0(400.0)));
    BOOST_CHECK_CLOSE(400.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setQ0(stdcxx::nan()), ValidationException, "Boundary line 'BL1': q0 is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setR(500.0)));
    BOOST_CHECK_CLOSE(500.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setR(stdcxx::nan()), ValidationException, "Boundary line 'BL1': r is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setX(600.0)));
    BOOST_CHECK_CLOSE(600.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(boundaryLine.setX(stdcxx::nan()), ValidationException, "Boundary line 'BL1': x is invalid");

    BOOST_TEST(stdcxx::areSame(boundaryLine, boundaryLine.setPairingKey("new_code")));
    BOOST_CHECK_EQUAL(boundaryLine.getPairingKey(), "new_code");

    boundaryLine.setFictitious(true);
    BOOST_CHECK(boundaryLine.isFictitious());
    boundaryLine.setFictitious(false);
    BOOST_CHECK(!boundaryLine.isFictitious());

    boundaryLine.remove();
    POWSYBL_ASSERT_THROW(network.getBoundaryLine("BL1"), PowsyblException, "Unable to find to the identifiable 'BL1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createBoundaryLineTestNetwork(true);

    BoundaryLine& boundaryLine = network.getBoundaryLine("BL1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(1.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("pairKeyTest", boundaryLine.getPairingKey());
    boundaryLine.setB(100.0).setG(200.0).setP0(300.0).setQ0(400).setR(500.0).setX(600.0);
    boundaryLine.getGeneration().get().setMinP(101).setMaxP(201).setTargetP(121).setTargetQ(141).setTargetV(161).setVoltageRegulationOn(false);

    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(100.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(101.0, boundaryLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(201.0, boundaryLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(121.0, boundaryLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(141.0, boundaryLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(161.0, boundaryLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!boundaryLine.getGeneration().get().isVoltageRegulationOn());
    BOOST_CHECK_EQUAL("pairKeyTest", boundaryLine.getPairingKey());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(100.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(101.0, boundaryLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(201.0, boundaryLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120.0, boundaryLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(140.0, boundaryLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(160.0, boundaryLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(boundaryLine.getGeneration().get().isVoltageRegulationOn());
    boundaryLine.setB(150.0).setG(250.0).setP0(350.0).setQ0(450).setR(550.0).setX(650.0);
    boundaryLine.getGeneration().get().setMinP(102).setMaxP(202).setTargetP(122).setTargetQ(142).setTargetV(162).setVoltageRegulationOn(true);

    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(150.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, boundaryLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, boundaryLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(122.0, boundaryLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(142.0, boundaryLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(162.0, boundaryLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(boundaryLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(150.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, boundaryLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, boundaryLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120.0, boundaryLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(140.0, boundaryLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(160.0, boundaryLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(boundaryLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("BL1", boundaryLine.getId());
    BOOST_CHECK_CLOSE(150.0, boundaryLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, boundaryLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, boundaryLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, boundaryLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, boundaryLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, boundaryLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, boundaryLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, boundaryLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(122.0, boundaryLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(142.0, boundaryLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(162.0, boundaryLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(boundaryLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(currentLimits) {
    Network network = createBoundaryLineTestNetwork(false);

    BoundaryLine& boundaryLine = network.getBoundaryLine("BL1");
    const BoundaryLine& cBoundaryLine = boundaryLine;
    BOOST_TEST(stdcxx::areSame(boundaryLine, cBoundaryLine));

    BOOST_TEST(!cBoundaryLine.getCurrentLimits());
    BOOST_TEST(!boundaryLine.getCurrentLimits());

    BOOST_CHECK_EQUAL(0, boost::size(cBoundaryLine.getOperationalLimitsGroups()));
    BOOST_CHECK_EQUAL(0, boost::size(boundaryLine.getOperationalLimitsGroups()));

    boundaryLine.getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits()
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

    BOOST_TEST(cBoundaryLine.getCurrentLimits());
    BOOST_TEST(boundaryLine.getCurrentLimits());

    BOOST_CHECK_EQUAL(1, boost::size(cBoundaryLine.getOperationalLimitsGroups()));
    BOOST_CHECK_EQUAL(1, boost::size(boundaryLine.getOperationalLimitsGroups()));

    BOOST_CHECK(!cBoundaryLine.getActivePowerLimits());
    BOOST_CHECK(!boundaryLine.getActivePowerLimits());

    boundaryLine.getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits()
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

    BOOST_CHECK(cBoundaryLine.getActivePowerLimits());
    BOOST_CHECK(boundaryLine.getActivePowerLimits());

    BOOST_CHECK_EQUAL(1, boost::size(cBoundaryLine.getOperationalLimitsGroups()));
    BOOST_CHECK_EQUAL(1, boost::size(boundaryLine.getOperationalLimitsGroups()));

    boundaryLine.getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits()
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

    BOOST_CHECK(cBoundaryLine.getApparentPowerLimits());
    BOOST_CHECK(boundaryLine.getApparentPowerLimits());

    BOOST_CHECK_EQUAL(1, boost::size(cBoundaryLine.getOperationalLimitsGroups()));
    BOOST_CHECK_EQUAL(1, boost::size(boundaryLine.getOperationalLimitsGroups()));
}

BOOST_AUTO_TEST_CASE(getBoundary) {
    Network network = createBoundaryLineTestNetwork(false);
    BoundaryLine& boundaryLine = network.getBoundaryLine("BL1");
    const BoundaryLine& cBoundaryLine = network.getBoundaryLine("BL1");

    BOOST_CHECK(stdcxx::areSame(cBoundaryLine.getBoundary(), boundaryLine.getBoundary()));
    const Boundary& cBoundary = boundaryLine.getBoundary();
    Boundary& boundary = boundaryLine.getBoundary();

    BOOST_CHECK(stdcxx::areSame(cBoundaryLine, cBoundary.getBoundaryLine()));
    BOOST_CHECK(stdcxx::areSame(cBoundaryLine, boundary.getBoundaryLine()));

    BOOST_CHECK(stdcxx::areSame(cBoundaryLine.getTerminal().getVoltageLevel(), cBoundary.getNetworkSideVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(boundaryLine.getTerminal().getVoltageLevel(), boundary.getNetworkSideVoltageLevel()));
}

BOOST_AUTO_TEST_CASE(boundaryWithGeneration) {
    Network network = powsybl::network::BoundaryLineNetworkFactory::createWithGeneration();
    double tol = 1e-3;
    BoundaryLine& boundaryLine = network.getBoundaryLine("BL");
    BOOST_CHECK(!BoundaryLineUtil::zeroImpedance(boundaryLine));
    BOOST_CHECK(!BoundaryLineUtil::useHypothesis(boundaryLine));

    // P Q V and angle are computed from SV Util:
    BOOST_CHECK(std::isnan(boundaryLine.getBoundary().getP())); // there is no good solution here.
    // we run an DC load flow and fill state variable
    boundaryLine.getTerminal().setP(-298.937);
    boundaryLine.getTerminal().setQ(stdcxx::nan());
    boundaryLine.getTerminal().getBusView().getBus().get().setAngle(0.0);
    boundaryLine.getTerminal().getBusView().getBus().get().setV(stdcxx::nan());
    BOOST_CHECK_CLOSE(298.937, boundaryLine.getBoundary().getP(), tol);
    BOOST_CHECK_CLOSE(1.712783, boundaryLine.getBoundary().getAngle(), tol);
    // we run an AC load flow
    boundaryLine.getTerminal().setP(-298.937);
    boundaryLine.getTerminal().setQ(-7.413);
    boundaryLine.getTerminal().getBusView().getBus().get().setAngle(0.0);
    boundaryLine.getTerminal().getBusView().getBus().get().setV(100.0);
    BOOST_CHECK_CLOSE(389.953, boundaryLine.getBoundary().getP(), tol);
    BOOST_CHECK_CLOSE(16.3146, boundaryLine.getBoundary().getQ(), tol);
    BOOST_CHECK_CLOSE(130.087, boundaryLine.getBoundary().getV(), tol);
    BOOST_CHECK_CLOSE(0.999, boundaryLine.getBoundary().getAngle(), tol);

}

BOOST_AUTO_TEST_CASE(boundaryZeroImpedanceWithGeneration) {
    Network network = powsybl::network::BoundaryLineNetworkFactory::createWithGeneration();
    BoundaryLine& boundaryLine = network.getBoundaryLine("BL");
    BOOST_CHECK(!BoundaryLineUtil::zeroImpedance(boundaryLine));
    BOOST_CHECK(!BoundaryLineUtil::useHypothesis(boundaryLine));

    //Set zeroImpedance
    boundaryLine.setR(0.0).setX(0.0);
    BOOST_CHECK(BoundaryLineUtil::zeroImpedance(boundaryLine));

    // P Q from terminal; V and Angle from bus:
    BOOST_CHECK(std::isnan(boundaryLine.getBoundary().getP())); // there is no good solution here.
    // fill state variable
    boundaryLine.getTerminal().setP(-298.937);
    boundaryLine.getTerminal().setQ(-7.413);
    boundaryLine.getTerminal().getBusView().getBus().get().setAngle(0.0);
    boundaryLine.getTerminal().getBusView().getBus().get().setV(100.0);
    BOOST_CHECK_CLOSE(298.937, boundaryLine.getBoundary().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(7.413, boundaryLine.getBoundary().getQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100.0, boundaryLine.getBoundary().getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, boundaryLine.getBoundary().getAngle(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_CASE(boundaryZeroImpedanceWithoutGeneration) {
    Network network = powsybl::network::BoundaryLineNetworkFactory::create();
    // double tol = 1e-3;
    BoundaryLine& boundaryLine = network.getBoundaryLine("BL");
    BOOST_CHECK(!BoundaryLineUtil::zeroImpedance(boundaryLine));
    BOOST_CHECK(BoundaryLineUtil::useHypothesis(boundaryLine));

    //Set zeroImpedance
    boundaryLine.setR(0.0).setX(0.0);
    BOOST_CHECK(BoundaryLineUtil::zeroImpedance(boundaryLine));

    // P Q from P0 Q0; V and Angle from BoundaryLineUtil:
    BOOST_CHECK(std::isnan(boundaryLine.getTerminal().getP())); // not set
    BOOST_CHECK(std::isnan(boundaryLine.getTerminal().getQ())); // not set
    // fill state variable
    boundaryLine.getTerminal().getBusView().getBus().get().setAngle(0.0);
    boundaryLine.getTerminal().getBusView().getBus().get().setV(100.0);
    BOOST_CHECK_CLOSE(-50.0, boundaryLine.getBoundary().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-30.0, boundaryLine.getBoundary().getQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100.0, boundaryLine.getBoundary().getV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, boundaryLine.getBoundary().getAngle(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
