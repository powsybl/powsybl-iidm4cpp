/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

Network createStaticVarCompensatorTestNetwork() {
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

    vl1.newStaticVarCompensator()
        .setId("SVC1")
        .setName("SVC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setBmin(-0.01)
        .setBmax(0.02)
        .setVoltageSetpoint(380.0)
        .setReactivePowerSetpoint(90.0)
        .setRegulationMode(StaticVarCompensator::RegulationMode::REACTIVE_POWER)
        .add();

    return network;
}

Network createSvcNetwork() {
    Network network("network", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-29T14:54:03.427+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(380)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    vl1.newGenerator()
        .setId("G1")
        .setConnectableBus("B1")
        .setBus("B1")
        .setVoltageRegulatorOn(true)
        .setTargetP(100.0)
        .setTargetV(400.0)
        .setMinP(50.0)
        .setMaxP(150.0)
        .add();
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("VL2")
        .setNominalVoltage(380)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getBusBreakerView().newBus()
        .setId("B2")
        .add();
    vl2.newLoad()
        .setId("L2")
        .setConnectableBus("B2")
        .setBus("B2")
        .setP0(100.0)
        .setQ0(50.0)
        .add();
    vl2.newStaticVarCompensator()
        .setId("SVC2")
        .setConnectableBus("B2")
        .setBus("B2")
        .setBmin(0.0002)
        .setBmax(0.0008)
        .setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE)
        .setVoltageSetpoint(390)
        .add();
    network.newLine()
        .setId("L1")
        .setVoltageLevel1("VL1")
        .setConnectableBus1("B1")
        .setBus1("B1")
        .setVoltageLevel2("VL2")
        .setConnectableBus2("B2")
        .setBus2("B2")
        .setR(4.0)
        .setX(200.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();
    return network;
}

StaticVarCompensator& createSvc(Network& network, const std::string& id, const stdcxx::Reference<Terminal>& regulatingTerminal) {
    VoltageLevel& vl2 = network.getVoltageLevel("VL2");
    return vl2.newStaticVarCompensator()
        .setId(id)
        .setConnectableBus("B2")
        .setBus("B2")
        .setBmin(0.0002)
        .setBmax(0.0008)
        .setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE)
        .setVoltageSetpoint(390.0)
        .setReactivePowerSetpoint(1.0)
        .setRegulatingTerminal(regulatingTerminal)
        .add();
}

BOOST_AUTO_TEST_SUITE(StaticVarCompensatorTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createStaticVarCompensatorTestNetwork();
    unsigned long staticVarCompensatorCount = network.getStaticVarCompensatorCount();

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    StaticVarCompensatorAdder adder = vl1.newStaticVarCompensator()
        .setId("SVC1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "staticVarCompensator 'SVC1': bMin is invalid");
    adder.setBmin(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "staticVarCompensator 'SVC1': bMax is invalid");
    adder.setBmax(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "staticVarCompensator 'SVC1': Regulation mode is invalid");
    adder.setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "staticVarCompensator 'SVC1': invalid value (nan) for voltage setpoint");
    adder.setVoltageSetpoint(30.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'StaticVarCompensator' with the id 'SVC1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(staticVarCompensatorCount + 1, network.getStaticVarCompensatorCount());
    BOOST_CHECK_EQUAL(staticVarCompensatorCount + 1, boost::size(network.getStaticVarCompensators()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createStaticVarCompensatorTestNetwork();
    BOOST_CHECK_EQUAL(1, boost::size(network.getStaticVarCompensators()));

    const StaticVarCompensator& svc = network.getStaticVarCompensator("SVC1");
    BOOST_CHECK_EQUAL("SVC1", svc.getId());
    BOOST_CHECK_EQUAL("SVC1_NAME", *svc.getOptionalName());
    BOOST_CHECK_EQUAL(ConnectableType::STATIC_VAR_COMPENSATOR, svc.getType());
    std::ostringstream oss;
    oss << svc.getType();
    BOOST_CHECK_EQUAL("STATIC_VAR_COMPENSATOR", oss.str());
    BOOST_CHECK_CLOSE(-0.01, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.02, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(90.0, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(380.0, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createStaticVarCompensatorTestNetwork();

    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC1");

    BOOST_TEST(stdcxx::areSame(svc, svc.setBmax(100)));
    BOOST_CHECK_CLOSE(100, svc.getBmax(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(svc.setBmax(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC1': bMax is invalid");

    BOOST_TEST(stdcxx::areSame(svc, svc.setBmin(200)));
    BOOST_CHECK_CLOSE(200, svc.getBmin(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(svc.setBmin(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC1': bMin is invalid");

    BOOST_TEST(stdcxx::areSame(svc, svc.setRegulationMode(StaticVarCompensator::RegulationMode::OFF)));
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    POWSYBL_ASSERT_THROW(svc.setRegulationMode(static_cast<StaticVarCompensator::RegulationMode>(5)), AssertionError, "Unexpected RegulationMode value: 5");

    BOOST_TEST(stdcxx::areSame(svc, svc.setVoltageSetpoint(400)));
    BOOST_CHECK_CLOSE(400, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(svc.setVoltageSetpoint(stdcxx::nan()));

    BOOST_TEST(stdcxx::areSame(svc, svc.setReactivePowerSetpoint(500)));
    BOOST_CHECK_CLOSE(500, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(svc.setReactivePowerSetpoint(stdcxx::nan()));

    POWSYBL_ASSERT_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::REACTIVE_POWER), ValidationException, "staticVarCompensator 'SVC1': invalid value (nan) for reactive power setpoint");
    POWSYBL_ASSERT_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE), ValidationException, "staticVarCompensator 'SVC1': invalid value (nan) for voltage setpoint");

    svc.setVoltageSetpoint(600);
    BOOST_CHECK_NO_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE));

    svc.setReactivePowerSetpoint(700);
    BOOST_CHECK_NO_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::REACTIVE_POWER));

    svc.remove();
    POWSYBL_ASSERT_THROW(network.getStaticVarCompensator("SVC1"), PowsyblException, "Unable to find to the identifiable 'SVC1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createStaticVarCompensatorTestNetwork();

    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC1");

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {}), PowsyblException, "Empty target variant id list");
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_CLOSE(-0.01, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.02, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(90.0, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(380.0, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    svc.setBmin(100).setBmax(200).setReactivePowerSetpoint(300).setVoltageSetpoint(400).setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE);

    BOOST_CHECK_CLOSE(100, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::VOLTAGE, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(400, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(100, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(90.0, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(380.0, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    svc.setBmin(150).setBmax(250).setReactivePowerSetpoint(350).setVoltageSetpoint(450).setRegulationMode(StaticVarCompensator::RegulationMode::OFF);

    BOOST_CHECK_CLOSE(150, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(450, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(150, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(90.0, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(380.0, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant("s2", "s2"), PowsyblException, "Target variant 's2' already exists");
    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant("s4", "s3"), PowsyblException, "Variant 's4' not found");
    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_CLOSE(150, svc.getBmin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, svc.getBmax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, svc.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    BOOST_CHECK_CLOSE(450.0, svc.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());

    const std::string workingVariantId = network.getVariantManager().getWorkingVariantId();
    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());
    POWSYBL_ASSERT_THROW(network.getVariantManager().removeVariant(workingVariantId), PowsyblException, "Variant 's3' not found");

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());

    const Network& cNetwork = network;
    BOOST_CHECK_EQUAL(cNetwork.getVariantManager().getVariantIds().size(), network.getVariantManager().getVariantArraySize());
    POWSYBL_ASSERT_THROW(cNetwork.getVariantManager().getWorkingVariantId(), PowsyblException, "Variant index not set");
    const std::string& initialVariantId = VariantManager::getInitialVariantId();
    BOOST_CHECK_EQUAL("InitialVariant", initialVariantId);
    POWSYBL_ASSERT_THROW(network.getVariantManager().removeVariant(initialVariantId), PowsyblException, "Removing initial variant is forbidden");
}

BOOST_AUTO_TEST_CASE(regulatingTerminalTest) {
    Network network = createSvcNetwork();
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");
    BOOST_CHECK(stdcxx::areSame(svc.getTerminal(), svc.getRegulatingTerminal()));

    Terminal& loadTerminal = network.getLoad("L2").getTerminal();
    svc.setRegulatingTerminal(stdcxx::ref<Terminal>(loadTerminal));
    BOOST_CHECK(stdcxx::areSame(loadTerminal, svc.getRegulatingTerminal()));

    svc.setRegulatingTerminal(stdcxx::ref<Terminal>());
    BOOST_CHECK(stdcxx::areSame(svc.getTerminal(), svc.getRegulatingTerminal()));

    StaticVarCompensator& svc3 = createSvc(network, "SVC3", stdcxx::ref<Terminal>());
    BOOST_CHECK(stdcxx::areSame(svc3.getTerminal(), svc3.getRegulatingTerminal()));
    svc3.remove();

    StaticVarCompensator& svc4 = createSvc(network, "SVC4", stdcxx::ref<Terminal>(loadTerminal));
    BOOST_CHECK(stdcxx::areSame(loadTerminal, svc4.getRegulatingTerminal()));

    Network network2 = createSvcNetwork();
    StaticVarCompensator& svc2 = network2.getStaticVarCompensator("SVC2");
    POWSYBL_ASSERT_THROW(svc2.setRegulatingTerminal(stdcxx::ref<Terminal>(loadTerminal)), ValidationException, "staticVarCompensator 'SVC2': Regulating terminal is not part of the network");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
