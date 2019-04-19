/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

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

TEST(StaticVarCompensator, constructor) {
    const Network& network = createStaticVarCompensatorTestNetwork();
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

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'SVC1' already exists (powsybl::iidm::StaticVarCompensator)");
    adder.setId("UNIQUE_SVC_ID");

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(staticVarCompensatorCount + 1, network.getStaticVarCompensatorCount());
}

TEST(StaticVarCompensator, integrity) {
    const Network& network = createStaticVarCompensatorTestNetwork();

    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC1");
    ASSERT_EQ("SVC1", svc.getId());
    ASSERT_EQ("SVC1_NAME", svc.getName());
    ASSERT_EQ(ConnectableType::STATIC_VAR_COMPENSATOR, svc.getType());
    ASSERT_DOUBLE_EQ(-0.01, svc.getBmin());
    ASSERT_DOUBLE_EQ(0.02, svc.getBmax());
    ASSERT_DOUBLE_EQ(90.0, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(380.0, svc.getVoltageSetpoint());

    ASSERT_TRUE(stdcxx::areSame(svc, svc.setBmax(100)));
    ASSERT_DOUBLE_EQ(100, svc.getBmax());
    POWSYBL_ASSERT_THROW(svc.setBmax(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC1': bMax is invalid");

    ASSERT_TRUE(stdcxx::areSame(svc, svc.setBmin(200)));
    ASSERT_DOUBLE_EQ(200, svc.getBmin());
    POWSYBL_ASSERT_THROW(svc.setBmin(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC1': bMin is invalid");

    ASSERT_TRUE(stdcxx::areSame(svc, svc.setRegulationMode(StaticVarCompensator::RegulationMode::OFF)));
    ASSERT_EQ(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    POWSYBL_ASSERT_THROW(svc.setRegulationMode(static_cast<StaticVarCompensator::RegulationMode>(5)), AssertionError, "Unexpected regulation mode value: 5");

    ASSERT_TRUE(stdcxx::areSame(svc, svc.setVoltageSetpoint(400)));
    ASSERT_DOUBLE_EQ(400, svc.getVoltageSetpoint());
    ASSERT_NO_THROW(svc.setVoltageSetpoint(stdcxx::nan()));

    ASSERT_TRUE(stdcxx::areSame(svc, svc.setReactivePowerSetpoint(500)));
    ASSERT_DOUBLE_EQ(500, svc.getReactivePowerSetpoint());
    ASSERT_NO_THROW(svc.setReactivePowerSetpoint(stdcxx::nan()));

    POWSYBL_ASSERT_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::REACTIVE_POWER), ValidationException, "staticVarCompensator 'SVC1': invalid value (nan) for reactive power setpoint");
    POWSYBL_ASSERT_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE), ValidationException, "staticVarCompensator 'SVC1': invalid value (nan) for voltage setpoint");

    svc.setVoltageSetpoint(600);
    ASSERT_NO_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE));

    svc.setReactivePowerSetpoint(700);
    ASSERT_NO_THROW(svc.setRegulationMode(StaticVarCompensator::RegulationMode::REACTIVE_POWER));

    svc.remove();
    POWSYBL_ASSERT_THROW(network.getStaticVarCompensator("SVC1"), PowsyblException, "Unable to find to the identifiable 'SVC1'");
}

TEST(StaticVarCompensator, multivariant) {
    Network network = createStaticVarCompensatorTestNetwork();

    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC1");

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {}), PowsyblException, "Empty target variant id list");
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_DOUBLE_EQ(-0.01, svc.getBmin());
    ASSERT_DOUBLE_EQ(0.02, svc.getBmax());
    ASSERT_DOUBLE_EQ(90.0, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(380.0, svc.getVoltageSetpoint());
    svc.setBmin(100).setBmax(200).setReactivePowerSetpoint(300).setVoltageSetpoint(400).setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE);

    ASSERT_DOUBLE_EQ(100, svc.getBmin());
    ASSERT_DOUBLE_EQ(200, svc.getBmax());
    ASSERT_DOUBLE_EQ(300, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::VOLTAGE, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(400, svc.getVoltageSetpoint());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_DOUBLE_EQ(100, svc.getBmin());
    ASSERT_DOUBLE_EQ(200, svc.getBmax());
    ASSERT_DOUBLE_EQ(90.0, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(380.0, svc.getVoltageSetpoint());
    svc.setBmin(150).setBmax(250).setReactivePowerSetpoint(350).setVoltageSetpoint(450).setRegulationMode(StaticVarCompensator::RegulationMode::OFF);

    ASSERT_DOUBLE_EQ(150, svc.getBmin());
    ASSERT_DOUBLE_EQ(250, svc.getBmax());
    ASSERT_DOUBLE_EQ(350, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(450, svc.getVoltageSetpoint());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_DOUBLE_EQ(150, svc.getBmin());
    ASSERT_DOUBLE_EQ(250, svc.getBmax());
    ASSERT_DOUBLE_EQ(90.0, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::REACTIVE_POWER, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(380.0, svc.getVoltageSetpoint());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant("s2", "s2"), PowsyblException, "Target variant 's2' already exists");
    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant("s4", "s3"), PowsyblException, "Variant 's4' not found");
    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_DOUBLE_EQ(150, svc.getBmin());
    ASSERT_DOUBLE_EQ(250, svc.getBmax());
    ASSERT_DOUBLE_EQ(350, svc.getReactivePowerSetpoint());
    ASSERT_EQ(StaticVarCompensator::RegulationMode::OFF, svc.getRegulationMode());
    ASSERT_DOUBLE_EQ(450.0, svc.getVoltageSetpoint());

    const std::string workingVariantId = network.getVariantManager().getWorkingVariantId();
    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());
    POWSYBL_ASSERT_THROW(network.getVariantManager().removeVariant(workingVariantId), PowsyblException, "Variant 's3' not found");

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());

    const Network& cNetwork = network;
    ASSERT_EQ(cNetwork.getVariantManager().getVariantIds().size(), network.getVariantManager().getVariantArraySize());
    POWSYBL_ASSERT_THROW(cNetwork.getVariantManager().getWorkingVariantId(), PowsyblException, "Variant index not set");
    const std::string& initialVariantId = VariantManager::getInitialVariantId();
    ASSERT_EQ("InitialVariant", initialVariantId);
    POWSYBL_ASSERT_THROW(network.getVariantManager().removeVariant(initialVariantId), PowsyblException, "Removing initial variant is forbidden");
}

}  // namespace iidm

}  // namespace powsybl
