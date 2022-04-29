/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createDanglingLineTestNetwork(bool withGeneration) {
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

    DanglingLineAdder adder = vl1.newDanglingLine();
    adder.setId("DL1")
        .setName("DL1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setB(1.0)
        .setG(2.0)
        .setP0(3.0)
        .setQ0(4.0)
        .setR(5.0)
        .setX(6.0)
        .setUcteXnodeCode("ucteXnodeCodeTest");

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

BOOST_AUTO_TEST_SUITE(DanglingLineTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createDanglingLineTestNetwork(false);

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

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'DanglingLine' with the id 'DL1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(danglingLineCount + 1, network.getDanglingLineCount());
    BOOST_CHECK_EQUAL(danglingLineCount + 1, boost::size(network.getDanglingLines()));
    BOOST_TEST(network.getDanglingLine("DL1#0").getUcteXnodeCode().empty());

    DanglingLine& dl = network.getDanglingLine("DL1");
    BOOST_CHECK(!dl.getGeneration());

    adder.setId("DL2");

    DanglingLineAdder::GenerationAdder gAdder = adder.newGeneration();

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Active power setpoint is not set");
    gAdder.setMinP(100).setMaxP(50);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Invalid active limits [100, 50]");
    gAdder.setMinP(100).setMaxP(200);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Active power setpoint is not set");
    gAdder.setTargetP(120);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Invalid reactive power setpoint (nan) while voltage regulator is off");
    gAdder.setTargetQ(140);
    gAdder.setVoltageRegulationOn(true);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Invalid voltage setpoint value (nan) while voltage regulator is on");
    gAdder.setTargetV(-1);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Invalid voltage setpoint value (-1) while voltage regulator is on");
    gAdder.setTargetV(0);

    POWSYBL_ASSERT_THROW(gAdder.add(), PowsyblException, "Dangling line 'DL2': Invalid voltage setpoint value (0) while voltage regulator is on");
    gAdder.setTargetV(160);

    BOOST_CHECK_NO_THROW(gAdder.add().add());

    DanglingLine& dl2 = network.getDanglingLine("DL2");
    BOOST_CHECK(dl2.getGeneration());

    BOOST_CHECK_CLOSE(100, dl2.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setMinP(101)));
    BOOST_CHECK_CLOSE(101, dl2.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(200, dl2.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setMaxP(201)));
    BOOST_CHECK_CLOSE(201, dl2.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(120, dl2.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setTargetP(121)));
    BOOST_CHECK_CLOSE(121, dl2.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(140, dl2.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setTargetQ(141)));
    BOOST_CHECK_CLOSE(141, dl2.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(160, dl2.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setTargetV(161)));
    BOOST_CHECK_CLOSE(161, dl2.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(dl2.getGeneration().get().isVoltageRegulationOn());
    BOOST_CHECK(stdcxx::areSame(dl2.getGeneration().get(), dl2.getGeneration().get().setVoltageRegulationOn(false)));
    BOOST_CHECK(!dl2.getGeneration().get().isVoltageRegulationOn());

    // Test if new Generation is instantiate at each add
    DanglingLine& dl3 = adder.setId("DL3").add();
    BOOST_CHECK(!stdcxx::areSame(dl2.getGeneration(), dl3.getGeneration()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createDanglingLineTestNetwork(false);

    const DanglingLine& danglingLine = network.getDanglingLine("DL1");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_EQUAL("DL1_NAME", danglingLine.getOptionalName());
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
    BOOST_CHECK_EQUAL(1, boost::size(network.getDanglingLines()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createDanglingLineTestNetwork(false);
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

    danglingLine.setFictitious(true);
    BOOST_CHECK(danglingLine.isFictitious());
    danglingLine.setFictitious(false);
    BOOST_CHECK(!danglingLine.isFictitious());

    danglingLine.remove();
    POWSYBL_ASSERT_THROW(network.getDanglingLine("DL1"), PowsyblException, "Unable to find to the identifiable 'DL1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createDanglingLineTestNetwork(true);

    DanglingLine& danglingLine = network.getDanglingLine("DL1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(1.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());
    danglingLine.setB(100.0).setG(200.0).setP0(300.0).setQ0(400).setR(500.0).setX(600.0);
    danglingLine.getGeneration().get().setMinP(101).setMaxP(201).setTargetP(121).setTargetQ(141).setTargetV(161).setVoltageRegulationOn(false);

    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(100.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(101.0, danglingLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(201.0, danglingLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(121.0, danglingLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(141.0, danglingLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(161.0, danglingLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!danglingLine.getGeneration().get().isVoltageRegulationOn());
    BOOST_CHECK_EQUAL("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(100.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(600.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(101.0, danglingLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(201.0, danglingLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120.0, danglingLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(140.0, danglingLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(160.0, danglingLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(danglingLine.getGeneration().get().isVoltageRegulationOn());
    danglingLine.setB(150.0).setG(250.0).setP0(350.0).setQ0(450).setR(550.0).setX(650.0);
    danglingLine.getGeneration().get().setMinP(102).setMaxP(202).setTargetP(122).setTargetQ(142).setTargetV(162).setVoltageRegulationOn(true);

    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, danglingLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, danglingLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(122.0, danglingLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(142.0, danglingLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(162.0, danglingLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(danglingLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, danglingLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, danglingLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120.0, danglingLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(140.0, danglingLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(160.0, danglingLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(danglingLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_EQUAL("DL1", danglingLine.getId());
    BOOST_CHECK_CLOSE(150.0, danglingLine.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250.0, danglingLine.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350.0, danglingLine.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(450.0, danglingLine.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, danglingLine.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(650.0, danglingLine.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(102.0, danglingLine.getGeneration().get().getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(202.0, danglingLine.getGeneration().get().getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(122.0, danglingLine.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(142.0, danglingLine.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(162.0, danglingLine.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(danglingLine.getGeneration().get().isVoltageRegulationOn());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(currentLimits) {
    Network network = createDanglingLineTestNetwork(false);

    DanglingLine& danglingLine = network.getDanglingLine("DL1");
    const DanglingLine& cDanglingLine = danglingLine;
    BOOST_TEST(stdcxx::areSame(danglingLine, cDanglingLine));

    BOOST_TEST(!cDanglingLine.getCurrentLimits());
    BOOST_TEST(!danglingLine.getCurrentLimits());

    BOOST_CHECK_EQUAL(0, boost::size(cDanglingLine.getOperationalLimits()));
    BOOST_CHECK_EQUAL(0, boost::size(danglingLine.getOperationalLimits()));

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

    BOOST_CHECK_EQUAL(1, boost::size(cDanglingLine.getOperationalLimits()));
    BOOST_CHECK_EQUAL(1, boost::size(danglingLine.getOperationalLimits()));

    BOOST_CHECK(!cDanglingLine.getActivePowerLimits());
    BOOST_CHECK(!danglingLine.getActivePowerLimits());

    danglingLine.newActivePowerLimits()
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

    BOOST_CHECK(cDanglingLine.getActivePowerLimits());
    BOOST_CHECK(danglingLine.getActivePowerLimits());

    BOOST_CHECK_EQUAL(2, boost::size(cDanglingLine.getOperationalLimits()));
    BOOST_CHECK_EQUAL(2, boost::size(danglingLine.getOperationalLimits()));

    danglingLine.newApparentPowerLimits()
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

    BOOST_CHECK(cDanglingLine.getApparentPowerLimits());
    BOOST_CHECK(danglingLine.getApparentPowerLimits());

    BOOST_CHECK_EQUAL(3, boost::size(cDanglingLine.getOperationalLimits()));
    BOOST_CHECK_EQUAL(3, boost::size(danglingLine.getOperationalLimits()));
}

BOOST_AUTO_TEST_CASE(getBoundary) {
    Network network = createDanglingLineTestNetwork(false);
    DanglingLine& danglingLine = network.getDanglingLine("DL1");
    const DanglingLine& cDanglingLine = network.getDanglingLine("DL1");

    danglingLine.setB(0.0);
    danglingLine.setG(2.0);
    danglingLine.setP0(-200.0);
    danglingLine.setQ0(0);
    danglingLine.setR(5.0);
    danglingLine.setX(0.0);

    danglingLine.getTerminal().getBusView().getBus().get().setAngle(2);
    danglingLine.getTerminal().setP(3);
    danglingLine.getTerminal().setQ(0);
    danglingLine.getTerminal().getBusView().getBus().get().setV(5);
    BOOST_CHECK(stdcxx::areSame(cDanglingLine.getBoundary(), danglingLine.getBoundary()));
    const Boundary& cBoundary = danglingLine.getBoundary();
    Boundary& boundary = danglingLine.getBoundary();

    constexpr double ACCEPTABLE_THRESHOLD = 1e-6;
    BOOST_CHECK_CLOSE(2.0, boundary.getAngle(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(200, boundary.getP(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(0, boundary.getQ(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK_CLOSE(13.333333333, boundary.getV(), ACCEPTABLE_THRESHOLD);
    BOOST_CHECK(stdcxx::areSame(cDanglingLine, cBoundary.getConnectable()));
    BOOST_CHECK(stdcxx::areSame(cDanglingLine, boundary.getConnectable()));
    BOOST_CHECK(!boundary.getSide());
    BOOST_CHECK(stdcxx::areSame(cDanglingLine.getTerminal().getVoltageLevel(), cBoundary.getVoltageLevel()));
    BOOST_CHECK(stdcxx::areSame(danglingLine.getTerminal().getVoltageLevel(), boundary.getVoltageLevel()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
