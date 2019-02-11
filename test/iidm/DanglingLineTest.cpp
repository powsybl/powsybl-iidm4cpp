/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

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


TEST(DanglingLine, constructor) {
    const Network& network = createDanglingLineTestNetwork();

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

    ASSERT_NO_THROW(adder.add());
    ASSERT_EQ(danglingLineCount + 1, network.getDanglingLineCount());
    ASSERT_TRUE(network.getDanglingLine("UNIQUE_DL_ID").getUcteXnodeCode().empty());
}

TEST(DanglingLine, integrity) {
    const Network& network = createDanglingLineTestNetwork();

    DanglingLine& danglingLine = network.getDanglingLine("DL1");
    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(1.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(2.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(3.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(4.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(5.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(6.0, danglingLine.getX());
    ASSERT_EQ("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setB(100.0)));
    ASSERT_DOUBLE_EQ(100.0, danglingLine.getB());
    POWSYBL_ASSERT_THROW(danglingLine.setB(stdcxx::nan()), ValidationException, "Dangling line 'DL1': b is invalid");

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setG(200.0)));
    ASSERT_DOUBLE_EQ(200.0, danglingLine.getG());
    POWSYBL_ASSERT_THROW(danglingLine.setG(stdcxx::nan()), ValidationException, "Dangling line 'DL1': g is invalid");

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setP0(300.0)));
    ASSERT_DOUBLE_EQ(300.0, danglingLine.getP0());
    POWSYBL_ASSERT_THROW(danglingLine.setP0(stdcxx::nan()), ValidationException, "Dangling line 'DL1': p0 is invalid");

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setQ0(400.0)));
    ASSERT_DOUBLE_EQ(400.0, danglingLine.getQ0());
    POWSYBL_ASSERT_THROW(danglingLine.setQ0(stdcxx::nan()), ValidationException, "Dangling line 'DL1': q0 is invalid");

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setR(500.0)));
    ASSERT_DOUBLE_EQ(500.0, danglingLine.getR());
    POWSYBL_ASSERT_THROW(danglingLine.setR(stdcxx::nan()), ValidationException, "Dangling line 'DL1': r is invalid");

    ASSERT_TRUE(stdcxx::areSame(danglingLine, danglingLine.setX(600.0)));
    ASSERT_DOUBLE_EQ(600.0, danglingLine.getX());
    POWSYBL_ASSERT_THROW(danglingLine.setX(stdcxx::nan()), ValidationException, "Dangling line 'DL1': x is invalid");

    danglingLine.remove();
    POWSYBL_ASSERT_THROW(network.getDanglingLine("DL1"), PowsyblException, "Unable to find to the identifiable 'DL1'");
}

TEST(DanglingLine, multivariant) {
    Network network = createDanglingLineTestNetwork();

    DanglingLine& danglingLine = network.getDanglingLine("DL1");

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(1.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(2.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(3.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(4.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(5.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(6.0, danglingLine.getX());
    ASSERT_EQ("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());
    danglingLine.setB(100.0).setG(200.0).setP0(300.0).setQ0(400).setR(500.0).setX(600.0);

    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(100.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(200.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(300.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(400.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(500.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(600.0, danglingLine.getX());
    ASSERT_EQ("ucteXnodeCodeTest", danglingLine.getUcteXnodeCode());

    network.getVariantManager().setWorkingVariant("s2");
    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(100.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(200.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(3.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(4.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(500.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(600.0, danglingLine.getX());
    danglingLine.setB(150.0).setG(250.0).setP0(350.0).setQ0(450).setR(550.0).setX(650.0);

    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(150.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(250.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(350.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(450.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(550.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(650.0, danglingLine.getX());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(150.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(250.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(3.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(4.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(550.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(650.0, danglingLine.getX());

    network.getVariantManager().removeVariant("s1");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    ASSERT_EQ("DL1", danglingLine.getId());
    ASSERT_EQ("DL1_NAME", danglingLine.getName());
    ASSERT_DOUBLE_EQ(150.0, danglingLine.getB());
    ASSERT_DOUBLE_EQ(250.0, danglingLine.getG());
    ASSERT_DOUBLE_EQ(350.0, danglingLine.getP0());
    ASSERT_DOUBLE_EQ(450.0, danglingLine.getQ0());
    ASSERT_DOUBLE_EQ(550.0, danglingLine.getR());
    ASSERT_DOUBLE_EQ(650.0, danglingLine.getX());

    network.getVariantManager().removeVariant("s3");
    ASSERT_EQ(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    ASSERT_EQ(1ul, network.getVariantManager().getVariantArraySize());
}


}  // namespace iidm

}  // namespace powsybl
