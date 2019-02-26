/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

Network createGeneratorTestNetwork() {
    Network network("test", "test");

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(5);

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
        .add();

    return network;
}

TEST(Generator, constructor) {
    const Network& network = createGeneratorTestNetwork();
    unsigned long generatorCount = network.getGeneratorCount();
    ASSERT_EQ(1ul, generatorCount);

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
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'GEN1' already exists (powsybl::iidm::Generator)");

    //Terminal from other network
    adder.setRegulatingTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2()));
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Generator 'GEN1': Regulating terminal is not part of the network");

    adder.setRegulatingTerminal(stdcxx::ref<Terminal>());
    adder.setId("UNIQUE_GEN_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(generatorCount + 1, network.getGeneratorCount());
}

TEST(Generator, integrity) {
    Network network = createGeneratorTestNetwork();

    Generator& gen = network.getGenerator("GEN1");
    ASSERT_EQ("GEN1", gen.getId());
    ASSERT_EQ("GEN1_NAME", gen.getName());
    ASSERT_EQ(ConnectableType::GENERATOR, gen.getType());
    ASSERT_DOUBLE_EQ(45, gen.getActivePowerSetpoint());
    ASSERT_EQ(EnergySource::WIND, gen.getEnergySource());
    ASSERT_DOUBLE_EQ(50.0, gen.getMaxP());
    ASSERT_DOUBLE_EQ(3.0, gen.getMinP());
    ASSERT_DOUBLE_EQ(4.0, gen.getRatedS());
    ASSERT_DOUBLE_EQ(5.0, gen.getReactivePowerSetpoint());
    ASSERT_FALSE(gen.getRegulatingTerminal());
    ASSERT_DOUBLE_EQ(45.0, gen.getTargetP());
    ASSERT_DOUBLE_EQ(5.0, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(6.0, gen.getTargetV());
    ASSERT_DOUBLE_EQ(6.0, gen.getVoltageSetpoint());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());

    ASSERT_TRUE(stdcxx::areSame(gen, gen.setActivePowerSetpoint(100)));
    ASSERT_DOUBLE_EQ(100, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(gen.getTargetP(), gen.getActivePowerSetpoint());
    POWSYBL_ASSERT_THROW(gen.setActivePowerSetpoint(stdcxx::nan()), ValidationException, "Generator 'GEN1': Active power setpoint is not set");

    ASSERT_TRUE(stdcxx::areSame(gen, gen.setEnergySource(EnergySource::SOLAR)));
    ASSERT_EQ(EnergySource::SOLAR, gen.getEnergySource());

    ASSERT_TRUE(stdcxx::areSame(gen, gen.setMaxP(200)));
    ASSERT_DOUBLE_EQ(200, gen.getMaxP());
    POWSYBL_ASSERT_THROW(gen.setMaxP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Maximum active power is not set");

    POWSYBL_ASSERT_THROW(gen.setMinP(300), ValidationException, "Generator 'GEN1': Invalid active limits [300, 200]");
    ASSERT_TRUE(stdcxx::areSame(gen, gen.setMinP(150)));
    ASSERT_DOUBLE_EQ(150, gen.getMinP());
    POWSYBL_ASSERT_THROW(gen.setMinP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Minimum active power is not set");

    ASSERT_TRUE(stdcxx::areSame(gen, gen.setRatedS(400)));
    ASSERT_DOUBLE_EQ(400, gen.getRatedS());
    ASSERT_NO_THROW(gen.setRatedS(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setRatedS(-1.0), ValidationException, "Generator 'GEN1': Invalid rated S value: -1");

    gen.setVoltageRegulatorOn(true);
    ASSERT_TRUE(stdcxx::areSame(gen, gen.setReactivePowerSetpoint(500)));
    ASSERT_DOUBLE_EQ(500, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(gen.getTargetQ(), gen.getReactivePowerSetpoint());
    ASSERT_NO_THROW(gen.setReactivePowerSetpoint(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(false), ValidationException, "Generator 'GEN1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    gen.setReactivePowerSetpoint(500);
    ASSERT_NO_THROW(gen.setVoltageRegulatorOn(false));

    ASSERT_TRUE(stdcxx::areSame(gen, gen.setTargetP(600)));
    ASSERT_DOUBLE_EQ(600, gen.getTargetP());
    ASSERT_DOUBLE_EQ(gen.getTargetP(), gen.getActivePowerSetpoint());
    POWSYBL_ASSERT_THROW(gen.setTargetP(stdcxx::nan()), ValidationException, "Generator 'GEN1': Active power setpoint is not set");

    gen.setVoltageRegulatorOn(true);
    ASSERT_TRUE(stdcxx::areSame(gen, gen.setTargetQ(700)));
    ASSERT_DOUBLE_EQ(700, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(gen.getTargetQ(), gen.getReactivePowerSetpoint());
    ASSERT_NO_THROW(gen.setTargetQ(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(false), ValidationException, "Generator 'GEN1': Invalid reactive power setpoint (nan) while voltage regulator is off");
    gen.setTargetQ(700);
    ASSERT_NO_THROW(gen.setVoltageRegulatorOn(false));

    gen.setVoltageRegulatorOn(false);
    ASSERT_TRUE(stdcxx::areSame(gen, gen.setTargetV(800)));
    ASSERT_DOUBLE_EQ(800, gen.getTargetV());
    ASSERT_DOUBLE_EQ(gen.getTargetV(), gen.getVoltageSetpoint());
    ASSERT_NO_THROW(gen.setTargetV(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    ASSERT_NO_THROW(gen.setTargetV(-20));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (-20) while voltage regulator is on");
    gen.setTargetV(800);
    ASSERT_NO_THROW(gen.setVoltageRegulatorOn(true));

    gen.setVoltageRegulatorOn(false);
    ASSERT_TRUE(stdcxx::areSame(gen, gen.setVoltageSetpoint(900)));
    ASSERT_DOUBLE_EQ(900, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(gen.getTargetV(), gen.getVoltageSetpoint());
    ASSERT_NO_THROW(gen.setVoltageSetpoint(stdcxx::nan()));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (nan) while voltage regulator is on");
    ASSERT_NO_THROW(gen.setVoltageSetpoint(-20));
    POWSYBL_ASSERT_THROW(gen.setVoltageRegulatorOn(true), ValidationException, "Generator 'GEN1': Invalid voltage setpoint value (-20) while voltage regulator is on");
    gen.setVoltageSetpoint(800);
    ASSERT_NO_THROW(gen.setVoltageRegulatorOn(true));

    const Generator& cGen = gen;
    const auto terminal = cGen.getRegulatingTerminal();
    ASSERT_FALSE(terminal);
    gen.setRegulatingTerminal(terminal);

    Terminal& terminal2 = network.getLoad("LOAD1").getTerminal();
    gen.setRegulatingTerminal(stdcxx::ref<Terminal>(terminal2));

    //Terminal from other network
    POWSYBL_ASSERT_THROW(gen.setRegulatingTerminal(stdcxx::ref<Terminal>(getTerminalFromNetwork2())), ValidationException, "Generator 'GEN1': Regulating terminal is not part of the network");

    gen.remove();
    POWSYBL_ASSERT_THROW(network.getGenerator("GEN1"), PowsyblException, "Unable to find to the identifiable 'GEN1'");
}

TEST(Generator, multivariant) {
    Network network = createGeneratorTestNetwork();

    Generator& gen = network.getGenerator("GEN1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_EQ("GEN1", gen.getId());
    ASSERT_EQ("GEN1_NAME", gen.getName());
    ASSERT_DOUBLE_EQ(45, gen.getActivePowerSetpoint());
    ASSERT_EQ(EnergySource::WIND, gen.getEnergySource());
    ASSERT_DOUBLE_EQ(50.0, gen.getMaxP());
    ASSERT_DOUBLE_EQ(3.0, gen.getMinP());
    ASSERT_DOUBLE_EQ(4.0, gen.getRatedS());
    ASSERT_DOUBLE_EQ(5.0, gen.getReactivePowerSetpoint());
    ASSERT_FALSE(gen.getRegulatingTerminal());
    ASSERT_DOUBLE_EQ(45.0, gen.getTargetP());
    ASSERT_DOUBLE_EQ(5.0, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(6.0, gen.getTargetV());
    ASSERT_DOUBLE_EQ(6.0, gen.getVoltageSetpoint());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());
    gen.setTargetP(100).setTargetQ(200).setTargetV(300).setVoltageRegulatorOn(false);

    ASSERT_DOUBLE_EQ(100, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(200, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(300, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(100, gen.getTargetP());
    ASSERT_DOUBLE_EQ(200, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(300, gen.getTargetV());
    ASSERT_FALSE(gen.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_DOUBLE_EQ(45, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(5.0, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(6.0, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(45.0, gen.getTargetP());
    ASSERT_DOUBLE_EQ(5.0, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(6.0, gen.getTargetV());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());
    gen.setTargetP(150).setTargetQ(250).setTargetV(350).setVoltageRegulatorOn(true);

    ASSERT_DOUBLE_EQ(150, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(250, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(350, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(150, gen.getTargetP());
    ASSERT_DOUBLE_EQ(250, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(350, gen.getTargetV());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_DOUBLE_EQ(45, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(5.0, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(6.0, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(45.0, gen.getTargetP());
    ASSERT_DOUBLE_EQ(5.0, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(6.0, gen.getTargetV());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_DOUBLE_EQ(150, gen.getActivePowerSetpoint());
    ASSERT_DOUBLE_EQ(250, gen.getReactivePowerSetpoint());
    ASSERT_DOUBLE_EQ(350, gen.getVoltageSetpoint());
    ASSERT_DOUBLE_EQ(150, gen.getTargetP());
    ASSERT_DOUBLE_EQ(250, gen.getTargetQ());
    ASSERT_DOUBLE_EQ(350, gen.getTargetV());
    ASSERT_TRUE(gen.isVoltageRegulatorOn());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}

}  // namespace iidm

}  // namespace powsybl
