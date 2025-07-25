/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createGeneratorTestNetwork() {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .setFictitious(false)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .setFictitious(false)
        .add();

    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD1")
        .setNode1(0)
        .setNode2(1)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB1")
        .setNode1(1)
        .setNode2(2)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.newGenerator()
        .setId("GEN1")
        .setName("GEN1_NAME")
        .setNode(2)
        .setActivePowerSetpoint(0.02)
        .setEnergySource(EnergySource::WIND)
        .setMaxP(50.0)
        .setMinP(3.0)
        .setRatedS(4.0)
        .setReactivePowerSetpoint(10.0)
        .setTargetP(45.0)
        .setTargetQ(5.0)
        .setTargetV(24.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(6.0)
        .add();

    vl.getNodeBreakerView().newDisconnector()
        .setId("SWD2")
        .setNode1(0)
        .setNode2(3)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.getNodeBreakerView().newBreaker()
        .setId("SWB2")
        .setNode1(3)
        .setNode2(4)
        .setRetained(false)
        .setOpen(false)
        .add();

    vl.newLoad()
        .setId("LOAD1")
        .setNode(4)
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .setFictitious(false)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(GeneratorTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createGeneratorTestNetwork();
    unsigned long generatorCount = network.getGeneratorCount();
    BOOST_CHECK_EQUAL(1UL, generatorCount);
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getGenerators()));

    VoltageLevel& vl1 = network.getVoltageLevel("VL");
    GeneratorAdder adder = vl1.newGenerator().setId("GEN1").setNode(3);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': Minimum active power is not set");
    adder.setMinP(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': Maximum active power is not set");
    adder.setMaxP(40.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': Active power setpoint is not set");
    adder.setTargetP(45.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': voltage regulator status is not set");
    adder.setVoltageRegulatorOn(false);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    adder.setTargetQ(60.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Generator 'GEN1': Invalid active limits [50, 40]");
    adder.setMaxP(80.0);

    Terminal& terminal = network.getLoad("LOAD1").getTerminal();
    adder.setRegulatingTerminal(stdcxx::ref<Terminal>(terminal));
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'Generator' with the id 'GEN1'");

    //Terminal from other network
    adder.setRegulatingTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Generator 'GEN1': Regulating terminal is not part of the network");

    adder.setRegulatingTerminal(stdcxx::ref<Terminal>());
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(generatorCount + 1, network.getGeneratorCount());

    const Network& cNetwork = network;
    BOOST_CHECK_EQUAL(generatorCount + 1, boost::size(cNetwork.getGenerators()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createGeneratorTestNetwork();

    Generator& gen = network.getGenerator("GEN1");
    BOOST_CHECK_EQUAL("GEN1", gen.getId());
    BOOST_CHECK_EQUAL("GEN1_NAME", gen.getOptionalName());
    BOOST_CHECK_EQUAL(IdentifiableType::GENERATOR, gen.getType());
    std::ostringstream oss;
    oss << gen.getType();
    BOOST_CHECK_EQUAL("GENERATOR", oss.str());
    BOOST_CHECK_CLOSE(45, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::WIND, gen.getEnergySource());
    BOOST_CHECK_CLOSE(50.0, gen.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, gen.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, gen.getRatedS(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(gen.getRegulatingTerminal(), gen.getTerminal()));
    BOOST_CHECK_CLOSE(45.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());

    BOOST_TEST(stdcxx::areSame(gen, gen.setActivePowerSetpoint(100)));
    BOOST_CHECK_CLOSE(100, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetP(), gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(gen.setActivePowerSetpoint(stdcxx::nan()), ValidationException, "Generator 'GEN1': Active power setpoint is not set");

    BOOST_TEST(stdcxx::areSame(gen, gen.setEnergySource(EnergySource::SOLAR)));
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::SOLAR, gen.getEnergySource());

    BOOST_TEST(stdcxx::areSame(gen, gen.setMaxP(200)));
    BOOST_CHECK_CLOSE(200, gen.getMaxP(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(gen.setMaxP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Maximum active power is not set");

    POWSYBL_ASSERT_THROW(gen.setMinP(300), ValidationException, "Generator 'GEN1': Invalid active limits [300, 200]");
    BOOST_TEST(stdcxx::areSame(gen, gen.setMinP(150)));
    BOOST_CHECK_CLOSE(150, gen.getMinP(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(gen.setMinP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Minimum active power is not set");

    BOOST_TEST(stdcxx::areSame(gen, gen.setRatedS(400)));
    BOOST_CHECK_CLOSE(400, gen.getRatedS(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(gen.setRatedS(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setRatedS(-1.0), ValidationException, "Generator 'GEN1': Invalid rated S value: -1");

    gen.setVoltageRegulatorOn(true);
    BOOST_TEST(stdcxx::areSame(gen, gen.setReactivePowerSetpoint(500)));
    BOOST_CHECK_CLOSE(500, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetQ(), gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(gen.setReactivePowerSetpoint(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(false), ValidationException, "Generator 'GEN1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    gen.setReactivePowerSetpoint(500);
    BOOST_CHECK_NO_THROW(gen.setVoltageRegulatorOn(false));

    BOOST_TEST(stdcxx::areSame(gen, gen.setTargetP(600)));
    BOOST_CHECK_CLOSE(600, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetP(), gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(gen.setTargetP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Active power setpoint is not set");

    gen.setVoltageRegulatorOn(true);
    BOOST_TEST(stdcxx::areSame(gen, gen.setTargetQ(700)));
    BOOST_CHECK_CLOSE(700, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetQ(), gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(gen.setTargetQ(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(false), ValidationException, "Generator 'GEN1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    gen.setTargetQ(700);
    BOOST_CHECK_NO_THROW(gen.setVoltageRegulatorOn(false));

    gen.setVoltageRegulatorOn(false);
    BOOST_TEST(stdcxx::areSame(gen, gen.setTargetV(800)));
    BOOST_CHECK_CLOSE(800, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetV(), gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(gen.setTargetV(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    BOOST_CHECK_NO_THROW(gen.setTargetV(-20));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (-20) while voltage regulator is on");
    gen.setTargetV(800);
    BOOST_CHECK_NO_THROW(gen.setVoltageRegulatorOn(true));

    gen.setVoltageRegulatorOn(false);
    BOOST_TEST(stdcxx::areSame(gen, gen.setVoltageSetpoint(900)));
    BOOST_CHECK_CLOSE(900, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(gen.getTargetV(), gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_NO_THROW(gen.setVoltageSetpoint(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    BOOST_CHECK_NO_THROW(gen.setVoltageSetpoint(-20));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (-20) while voltage regulator is on");
    gen.setVoltageSetpoint(800);
    BOOST_CHECK_NO_THROW(gen.setVoltageRegulatorOn(true));

    const Generator& cGen = gen;
    const auto& terminal = cGen.getRegulatingTerminal();
    gen.setRegulatingTerminal(stdcxx::ref(terminal));

    Terminal& terminal2 = network.getLoad("LOAD1").getTerminal();
    gen.setRegulatingTerminal(stdcxx::ref<Terminal>(terminal2));

    gen.setFictitious(true);
    BOOST_CHECK(gen.isFictitious());
    gen.setFictitious(false);
    BOOST_CHECK(!gen.isFictitious());

    //Terminal from other network
    POWSYBL_ASSERT_THROW(gen.setRegulatingTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "Generator 'GEN1': Regulating terminal is not part of the network");

    gen.remove();
    POWSYBL_ASSERT_THROW(network.getGenerator("GEN1"), PowsyblException, "Unable to find to the identifiable 'GEN1'");
}

BOOST_AUTO_TEST_CASE(reactiveLimits) {
    Network network = createGeneratorTestNetwork();
    Generator& gen = network.getGenerator("GEN1");

    BOOST_CHECK_NO_THROW(gen.getReactiveLimits<MinMaxReactiveLimits>());
    gen.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(1)
        .setMinQ(15)
        .setMaxQ(25)
        .endPoint()
        .beginPoint()
        .setP(2)
        .setMinQ(10)
        .setMaxQ(20)
        .endPoint()
        .add();
    BOOST_CHECK_NO_THROW(gen.getReactiveLimits<ReactiveCapabilityCurve>());
    POWSYBL_ASSERT_THROW(gen.getReactiveLimits<MinMaxReactiveLimits>(), ValidationException,
        "Generator 'GEN1': Incorrect reactive limits type MinMaxReactiveLimits, expected ReactiveCapabilityCurve");

    gen.newMinMaxReactiveLimits()
        .setMinQ(1)
        .setMaxQ(2)
        .add();
    BOOST_CHECK_NO_THROW(gen.getReactiveLimits<MinMaxReactiveLimits>());
    POWSYBL_ASSERT_THROW(gen.getReactiveLimits<ReactiveCapabilityCurve>(), ValidationException,
        "Generator 'GEN1': Incorrect reactive limits type ReactiveCapabilityCurve, expected MinMaxReactiveLimits");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createGeneratorTestNetwork();

    Generator& gen = network.getGenerator("GEN1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_EQUAL("GEN1", gen.getId());
    BOOST_CHECK_CLOSE(45, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_ENUM_EQ(EnergySource::WIND, gen.getEnergySource());
    BOOST_CHECK_CLOSE(50.0, gen.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, gen.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, gen.getRatedS(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(gen.getRegulatingTerminal(), gen.getTerminal()));
    BOOST_CHECK_CLOSE(45.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());
    gen.setTargetP(100).setTargetQ(200).setTargetV(300).setVoltageRegulatorOn(false);

    BOOST_CHECK_CLOSE(100, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!gen.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(45, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(45.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());
    gen.setTargetP(150).setTargetQ(250).setTargetV(350).setVoltageRegulatorOn(true);

    BOOST_CHECK_CLOSE(150, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(150, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(45, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(45.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(6.0, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_CLOSE(150, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(150, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(250, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(350, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
