/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/DroopCurveAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>
#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(AcDcConverterTestSuite)

Network createAcDcNetwork() {
    Network network("test", "test");
    Substation& sa = network.newSubstation().setId("SA").add();
    VoltageLevel& vla = sa.newVoltageLevel().setId("VLA").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1a = vla.getBusBreakerView().newBus().setId("B1A").add();
    Bus& b2a = vla.getBusBreakerView().newBus().setId("B2A").add();
    network.newDcNode().setId("dcNode1a").setNominalV(1.).add();
    network.newDcNode().setId("dcNode2a").setNominalV(500.).add();
    Substation& sb = network.newSubstation().setId("SB").add();
    VoltageLevel& vlb = sb.newVoltageLevel().setId("VLB").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1b = vlb.getBusBreakerView().newBus().setId("B1B").add();
    Bus& b2b = vlb.getBusBreakerView().newBus().setId("B2B").add();
    network.newDcNode().setId("dcNode1b").setNominalV(1.).add();
    network.newDcNode().setId("dcNode2b").setNominalV(500.).add();

    VoltageLevel& vlax = sa.newVoltageLevel().setId("VLA400").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(400).add();
    Bus& b1ax = vlax.getBusBreakerView().newBus().setId("B1AX").add();
    Bus& b2ax = vlax.getBusBreakerView().newBus().setId("B2AX").add();
    vlax.newLoad()
                .setId("LAX")
                .setBus(b1ax.getId())
                .setP0(0.0).setQ0(0.0)
                .add();
    network.newLine()
                .setId("LINEAX")
                .setVoltageLevel1(vlax.getId())
                .setBus1(b1ax.getId())
                .setConnectableBus1(b1ax.getId())
                .setVoltageLevel2(vlax.getId())
                .setBus2(b2ax.getId())
                .setConnectableBus2(b2ax.getId())
                .setR(0.3)
                .setX(3.0)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .add();

    sa.newTwoWindingsTransformer()
                .setId("TRA1")
                .setVoltageLevel1(vlax.getId())
                .setBus1(b1ax.getId())
                .setConnectableBus1(b1ax.getId())
                .setRatedU1(400.0)
                .setVoltageLevel2(vla.getId())
                .setBus2(b1a.getId())
                .setConnectableBus2(b1a.getId())
                .setRatedU2(150)
                .setR(0.1)
                .setX(5.0)
                .setG(0.0)
                .setB(0.0)
                .add();
    sa.newTwoWindingsTransformer()
                .setId("TRA2")
                .setVoltageLevel1(vlax.getId())
                .setBus1(b1ax.getId())
                .setConnectableBus1(b1ax.getId())
                .setRatedU1(400.0)
                .setVoltageLevel2(vla.getId())
                .setBus2(b2a.getId())
                .setConnectableBus2(b2a.getId())
                .setRatedU2(150)
                .setR(0.1)
                .setX(5.0)
                .setG(0.0)
                .setB(0.0)
                .add();

    VoltageLevel& vlbx = sb.newVoltageLevel().setId("VLB400").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(400).add();
    Bus& b1bx = vlbx.getBusBreakerView().newBus().setId("B1BX").add();
    Bus& b2bx = vlbx.getBusBreakerView().newBus().setId("B2BX").add();
    network.newLine()
                .setId("LINEBX")
                .setVoltageLevel1(vlbx.getId())
                .setBus1(b1bx.getId())
                .setConnectableBus1(b1bx.getId())
                .setVoltageLevel2(vlbx.getId())
                .setBus2(b2bx.getId())
                .setConnectableBus2(b2bx.getId())
                .setR(0.3)
                .setX(3.0)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .add();

    sb.newTwoWindingsTransformer()
                .setId("TRB1")
                .setVoltageLevel1(vlbx.getId())
                .setBus1(b1bx.getId())
                .setConnectableBus1(b1bx.getId())
                .setRatedU1(400.0)
                .setVoltageLevel2(vlb.getId())
                .setBus2(b1b.getId())
                .setConnectableBus2(b1b.getId())
                .setRatedU2(150)
                .setR(0.1)
                .setX(5.0)
                .setG(0.0)
                .setB(0.0)
                .add();
    sb.newTwoWindingsTransformer()
                .setId("TRB2")
                .setVoltageLevel1(vlbx.getId())
                .setBus1(b1bx.getId())
                .setConnectableBus1(b1bx.getId())
                .setRatedU1(400.0)
                .setVoltageLevel2(vlb.getId())
                .setBus2(b2b.getId())
                .setConnectableBus2(b2b.getId())
                .setRatedU2(150)
                .setR(0.1)
                .setX(5.0)
                .setG(0.0)
                .setB(0.0)
                .add();

    return network;
}

LineCommutatedConverterAdder createLccAdder(VoltageLevel& vl) {
    return vl.newLineCommutatedConverter()
                .setIdleLoss(0.01)
                .setSwitchingLoss(0.02)
                .setResistiveLoss(0.03)
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.);
}

LineCommutatedConverter& createLccA(Network& network) {
    VoltageLevel& vla = network.getVoltageLevel("VLA");
    Line& lineax = network.getLine("LINEAX");
    return createLccAdder(vla)
                .setId("converterA")
                .setBus1("B1A")
                .setConnectableBus1("B1A")
                .setBus2("B2A")
                .setConnectableBus2("B2A")
                .setDcNode1("dcNode1a")
                .setDcNode2("dcNode2a")
                .setDcConnected1(true)
                .setDcConnected2(true)
                .setPccTerminal(stdcxx::ref(lineax.getTerminal1()))
                .add();
}

LineCommutatedConverter& createLccB(Network& network) {
    VoltageLevel& vlb = network.getVoltageLevel("VLB");
    Line& linebx = network.getLine("LINEBX");
    return createLccAdder(vlb)
                .setId("converterB")
                .setBus1("B1B")
                .setBus2("B2B")
                .setDcNode1("dcNode1b")
                .setDcNode2("dcNode2b")
                .setDcConnected1(false)
                .setDcConnected2(false)
                .setPccTerminal(stdcxx::ref(linebx.getTerminal1()))
                .setReactiveModel(LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR)
                .setPowerFactor(0.6)
                .add();
}

VoltageSourceConverterAdder createVscAdder(VoltageLevel& vl) {
    return vl.newVoltageSourceConverter()
                .setIdleLoss(0.01)
                .setSwitchingLoss(0.02)
                .setResistiveLoss(0.03)
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.);
}

VoltageSourceConverter& createVscA(Network& network) {
    VoltageLevel& vla = network.getVoltageLevel("VLA");
    Line& lineax = network.getLine("LINEAX");
    return createVscAdder(vla)
                .setId("converterA")
                .setBus1("B1A")
                .setBus2("B2A")
                .setDcNode1("dcNode1a")
                .setDcNode2("dcNode2a")
                .setDcConnected1(true)
                .setDcConnected2(true)
                .setPccTerminal(stdcxx::ref(lineax.getTerminal1()))
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();
}

VoltageSourceConverter& createVscB(Network& network) {
    VoltageLevel& vlb = network.getVoltageLevel("VLB");
    Line& linebx = network.getLine("LINEBX");
    return createVscAdder(vlb)
                .setId("converterB")
                .setBus1("B1B")
                .setBus2("B2B")
                .setDcNode1("dcNode1b")
                .setDcNode2("dcNode2b")
                .setDcConnected1(false)
                .setDcConnected2(false)
                .setPccTerminal(stdcxx::ref(linebx.getTerminal1()))
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();
}

void checkAcDcConvertersAB(Network& network) {

    AcDcConverter& acDcConverterA = network.get<AcDcConverter>("converterA");
    AcDcConverter& acDcConverterB = network.get<AcDcConverter>("converterB");

    BOOST_CHECK_EQUAL(TwoSides::UNDEFINED, acDcConverterA.getDcTerminal1().getSide());
    BOOST_CHECK_EQUAL(TwoSides::UNDEFINED, acDcConverterA.getDcTerminal2().getSide());
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, acDcConverterA.getDcTerminal1().getTerminalNumber());
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, acDcConverterA.getDcTerminal2().getTerminalNumber());
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, acDcConverterA.getTerminalNumber(acDcConverterA.getDcTerminal1()));
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, acDcConverterA.getTerminalNumber(acDcConverterA.getDcTerminal2()));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getDcTerminal1(), acDcConverterA.getDcTerminal(TerminalNumber::ONE)));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getDcTerminal2(), acDcConverterA.getDcTerminal(TerminalNumber::TWO)));

    BOOST_CHECK_EQUAL("B1A", acDcConverterA.getTerminal1().getBusBreakerView().getBus().get().getId());
    POWSYBL_ASSERT_REF_TRUE(acDcConverterA.getTerminal2());
    BOOST_CHECK_EQUAL("B2A", acDcConverterA.getTerminal2().get().getBusBreakerView().getBus().get().getId());
    
    BOOST_CHECK_EQUAL(ThreeSides::UNDEFINED, acDcConverterA.getTerminal1().getSide());
    BOOST_CHECK_EQUAL(ThreeSides::UNDEFINED, acDcConverterA.getTerminal2().get().getSide());
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, acDcConverterA.getTerminal1().getTerminalNumber());
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, acDcConverterA.getTerminal2().get().getTerminalNumber());
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, acDcConverterA.getTerminalNumber(acDcConverterA.getTerminal1()));
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, acDcConverterA.getTerminalNumber(acDcConverterA.getTerminal2().get()));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal1(), acDcConverterA.getTerminal(TerminalNumber::ONE)));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal2().get(), acDcConverterA.getTerminal(TerminalNumber::TWO)));
    BOOST_CHECK(!Terminal::getConnectableSide(acDcConverterA.getTerminal1()).has_value());
    BOOST_CHECK(!Terminal::getConnectableSide(acDcConverterA.getTerminal2().get()).has_value());
    Line& lineax = network.getLine("LINEAX");
    BOOST_CHECK(!Terminal::getConnectableTerminalNumber(lineax.getTerminal1()).has_value());
    BOOST_CHECK(!Terminal::getConnectableTerminalNumber(lineax.getTerminal2()).has_value());
    BOOST_CHECK_EQUAL(TerminalNumber::ONE, Terminal::getConnectableTerminalNumber(acDcConverterA.getTerminal1()).get());
    BOOST_CHECK_EQUAL(TerminalNumber::TWO, Terminal::getConnectableTerminalNumber(acDcConverterA.getTerminal2().get()).get());
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal1(), Terminal::getTerminal(acDcConverterA, TerminalNumber::ONE)));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal2().get(), Terminal::getTerminal(acDcConverterA, TerminalNumber::TWO)));

    BOOST_CHECK(acDcConverterA.getDcTerminal1().isConnected());
    BOOST_CHECK(acDcConverterA.getDcTerminal2().isConnected());
    BOOST_CHECK_EQUAL(2, acDcConverterA.getDcTerminals().size());
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getDcTerminals().at(0).get(), acDcConverterA.getDcTerminal1()));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getDcTerminals().at(1).get(), acDcConverterA.getDcTerminal2()));
    BOOST_CHECK_EQUAL("dcNode1a", acDcConverterA.getDcTerminal1().getDcNode().getId());
    BOOST_CHECK_EQUAL("dcNode2a", acDcConverterA.getDcTerminal2().getDcNode().getId());
    BOOST_CHECK(stdcxx::areSame(acDcConverterA, acDcConverterA.getDcTerminal1().getDcConnectable().get()));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA, acDcConverterA.getDcTerminal2().getDcConnectable().get()));
    BOOST_CHECK_CLOSE(0.01, acDcConverterA.getIdleLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.02, acDcConverterA.getSwitchingLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.03, acDcConverterA.getResistiveLoss(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(!acDcConverterB.getDcTerminal1().isConnected());
    BOOST_CHECK(!acDcConverterB.getDcTerminal2().isConnected());
    BOOST_CHECK_EQUAL("dcNode1b", acDcConverterB.getDcTerminal1().getDcNode().getId());
    BOOST_CHECK_EQUAL("dcNode2b", acDcConverterB.getDcTerminal2().getDcNode().getId());

}

BOOST_AUTO_TEST_CASE(testBaseLcc) {

    Network network = createAcDcNetwork();
    auto& acDcConverterA = createLccA(network);
    BOOST_CHECK_EQUAL(1, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(IdentifiableType::LINE_COMMUTATED_CONVERTER, acDcConverterA.getType());
    auto& acDcConverterB = createLccB(network);
    BOOST_CHECK_EQUAL(2, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(IdentifiableType::LINE_COMMUTATED_CONVERTER, acDcConverterB.getType());

    checkAcDcConvertersAB(network);

    // default values
    BOOST_CHECK_EQUAL(LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR, acDcConverterA.getReactiveModel());
    BOOST_CHECK_CLOSE(cos(atan(0.5)), acDcConverterA.getPowerFactor(), std::numeric_limits<double>::epsilon());
    // explicitly set values
    BOOST_CHECK_EQUAL(LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR, acDcConverterB.getReactiveModel());
    BOOST_CHECK_CLOSE(0.6, acDcConverterB.getPowerFactor(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_EQUAL(2, boost::size(network.getLineCommutatedConverters()));
    BOOST_CHECK_EQUAL(2, boost::size(network.getConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(network.getDcConnectables<LineCommutatedConverter>()));

    VoltageLevel& vla = network.getVoltageLevel("VLA");
    BOOST_CHECK_EQUAL(1, vla.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(1, boost::size(vla.getLineCommutatedConverters()));
    VoltageLevel& vlb = network.getVoltageLevel("VLB");
    BOOST_CHECK_EQUAL(1, vlb.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(1, boost::size(vlb.getLineCommutatedConverters()));

}

BOOST_AUTO_TEST_CASE(testBaseVsc) {

    Network network = createAcDcNetwork();
    auto& acDcConverterA = createVscA(network);
    BOOST_CHECK_EQUAL(1, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(IdentifiableType::VOLTAGE_SOURCE_CONVERTER, acDcConverterA.getType());
    auto& acDcConverterB = createVscB(network);
    BOOST_CHECK_EQUAL(2, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(IdentifiableType::VOLTAGE_SOURCE_CONVERTER, acDcConverterB.getType());

    checkAcDcConvertersAB(network);

    BOOST_CHECK_EQUAL(2, boost::size(network.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(2, boost::size(network.getDcConnectables<VoltageSourceConverter>()));

    VoltageLevel& vla = network.getVoltageLevel("VLA");
    BOOST_CHECK_EQUAL(1, vla.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(1, boost::size(vla.getVoltageSourceConverters()));
    VoltageLevel& vlb = network.getVoltageLevel("VLB");
    BOOST_CHECK_EQUAL(1, vlb.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(1, boost::size(vlb.getVoltageSourceConverters()));

}

BOOST_AUTO_TEST_CASE(testLossParametersGetterSetter) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = createLccA(network);
    BOOST_CHECK_CLOSE(0.01, acDcConverterA.getIdleLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.02, acDcConverterA.getSwitchingLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.03, acDcConverterA.getResistiveLoss(), std::numeric_limits<double>::epsilon());

    acDcConverterA
                .setIdleLoss(0.04)
                .setSwitchingLoss(0.05)
                .setResistiveLoss(0.06);
    BOOST_CHECK_CLOSE(0.04, acDcConverterA.getIdleLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.05, acDcConverterA.getSwitchingLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.06, acDcConverterA.getResistiveLoss(), std::numeric_limits<double>::epsilon());
    acDcConverterA
                .setIdleLoss(0.)
                .setSwitchingLoss(0.)
                .setResistiveLoss(0.);
    BOOST_CHECK_CLOSE(0., acDcConverterA.getIdleLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0., acDcConverterA.getSwitchingLoss(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0., acDcConverterA.getResistiveLoss(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(acDcConverterA.setIdleLoss(stdcxx::nan()), ValidationException, "AC/DC Line Commutated Converter 'converterA': idleLoss is invalid");
    POWSYBL_ASSERT_THROW(acDcConverterA.setIdleLoss(-1.0), ValidationException, "AC/DC Line Commutated Converter 'converterA': idleLoss is invalid");
    POWSYBL_ASSERT_THROW(acDcConverterA.setSwitchingLoss(stdcxx::nan()), ValidationException, "AC/DC Line Commutated Converter 'converterA': switchingLoss is invalid");
    POWSYBL_ASSERT_THROW(acDcConverterA.setSwitchingLoss(-1.0), ValidationException, "AC/DC Line Commutated Converter 'converterA': switchingLoss is invalid");
    POWSYBL_ASSERT_THROW(acDcConverterA.setResistiveLoss(stdcxx::nan()), ValidationException, "AC/DC Line Commutated Converter 'converterA': resistiveLoss is invalid");
    POWSYBL_ASSERT_THROW(acDcConverterA.setResistiveLoss(-1.0), ValidationException, "AC/DC Line Commutated Converter 'converterA': resistiveLoss is invalid");
}

BOOST_AUTO_TEST_CASE(testLccGetterSetter) {
    Network network = createAcDcNetwork();
    auto& lccA = createLccA(network);

    //Default values
    BOOST_CHECK_EQUAL(LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR, lccA.getReactiveModel());
    BOOST_CHECK_CLOSE(cos(atan(0.5)), lccA.getPowerFactor(), std::numeric_limits<double>::epsilon());

    lccA.setPowerFactor(0.85).setReactiveModel(LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR);
    BOOST_CHECK_EQUAL(LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR, lccA.getReactiveModel());
    BOOST_CHECK_CLOSE(0.85, lccA.getPowerFactor(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(lccA.setPowerFactor(stdcxx::nan()), ValidationException, "AC/DC Line Commutated Converter 'converterA': power factor is invalid");
    POWSYBL_ASSERT_THROW(lccA.setPowerFactor(-0.1), ValidationException, "AC/DC Line Commutated Converter 'converterA': power factor is invalid, it must be between 0 and 1");
    POWSYBL_ASSERT_THROW(lccA.setPowerFactor(1.1), ValidationException, "AC/DC Line Commutated Converter 'converterA': power factor is invalid, it must be between 0 and 1");
}


BOOST_AUTO_TEST_CASE(testRemoveLcc) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = createLccA(network);
    auto& acDcConverterB = createLccB(network);
    VoltageLevel& vla = network.getVoltageLevel("VLA");
    VoltageLevel& vlb = network.getVoltageLevel("VLB");

    BOOST_CHECK_EQUAL(2, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(1, vla.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(1, vlb.getLineCommutatedConverterCount());
    acDcConverterA.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("converterA"));
    BOOST_CHECK_EQUAL(1, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(0, vla.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(0, boost::size(vla.getLineCommutatedConverters()));
    acDcConverterB.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("converterB"));
    BOOST_CHECK_EQUAL(0, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(0, vlb.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(0, boost::size(vlb.getLineCommutatedConverters()));
}

BOOST_AUTO_TEST_CASE(testRemoveVsc) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = createVscA(network);
    auto& acDcConverterB = createVscB(network);
    VoltageLevel& vla = network.getVoltageLevel("VLA");
    VoltageLevel& vlb = network.getVoltageLevel("VLB");

    BOOST_CHECK_EQUAL(2, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(1, vla.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(1, vlb.getVoltageSourceConverterCount());
    acDcConverterA.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("converterA"));
    BOOST_CHECK_EQUAL(1, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(0, vla.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(0, boost::size(vla.getVoltageSourceConverters()));
    acDcConverterB.remove();
    POWSYBL_ASSERT_REF_FALSE(network.find("converterB"));
    BOOST_CHECK_EQUAL(0, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(0, vlb.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(0, boost::size(vlb.getVoltageSourceConverters()));
}

BOOST_AUTO_TEST_CASE(testSubnetworksLcc) {

    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    Network& subnetwork2 = netWithSubnet.newSubnetwork("subnetwork2", "format2");
    Substation& sSubnet1 = subnetwork1.newSubstation().setId("SSubnetwork1").add();
    VoltageLevel& vlSubnet1 = sSubnet1.newVoltageLevel().setId("VLSubnet1").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1Subnet1 = vlSubnet1.getBusBreakerView().newBus().setId("B1Subnet1").add();
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnet1").setNominalV(1.).add();
    DcNode& dcNode2Subnet1 = subnetwork1.newDcNode().setId("dcNode2Subnet1").setNominalV(500.).add();
    Substation& sSubnet2 = subnetwork2.newSubstation().setId("SSubnetwork2").add();
    VoltageLevel& vlSubnet2 = sSubnet2.newVoltageLevel().setId("VLSubnet2").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1Subnet2 = vlSubnet2.getBusBreakerView().newBus().setId("B1Subnet2").add();
    DcNode& dcNode1Subnet2 = subnetwork2.newDcNode().setId("dcNode1Subnet2").setNominalV(1.).add();
    DcNode& dcNode2Subnet2 = subnetwork2.newDcNode().setId("dcNode2Subnet2").setNominalV(500.).add();

    LineCommutatedConverter& converterSubnet1 = vlSubnet1
                .newLineCommutatedConverter()
                .setId("converterSubnet1")
                .setBus1(b1Subnet1.getId())
                .setDcNode1(dcNode1Subnet1.getId())
                .setDcNode2(dcNode2Subnet1.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .add();
    LineCommutatedConverter& converterSubnet2 = vlSubnet2
                .newLineCommutatedConverter()
                .setId("converterSubnet2")
                .setBus1(b1Subnet2.getId())
                .setDcNode1(dcNode1Subnet2.getId())
                .setDcNode2(dcNode2Subnet2.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .add();

    // network content
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getLineCommutatedConverters()));
    BOOST_CHECK_EQUAL(2, netWithSubnet.getLineCommutatedConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, netWithSubnet.getIdentifiable(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, netWithSubnet.getLineCommutatedConverter(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, netWithSubnet.getIdentifiable(converterSubnet2.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, netWithSubnet.getLineCommutatedConverter(converterSubnet2.getId())));

    // subnetwork1 content
    BOOST_CHECK_EQUAL(1, boost::size(subnetwork1.getLineCommutatedConverters()));
    BOOST_CHECK_EQUAL(1, subnetwork1.getLineCommutatedConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, subnetwork1.getIdentifiable(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, subnetwork1.getLineCommutatedConverter(converterSubnet1.getId())));
    POWSYBL_ASSERT_THROW(subnetwork1.getIdentifiable(converterSubnet2.getId()), PowsyblException, "Identifiable 'converterSubnet2' does not belong to the subnetwork 'subnetwork1'");
    POWSYBL_ASSERT_THROW(subnetwork1.getLineCommutatedConverter(converterSubnet2.getId()), PowsyblException, "LineCommutatedConverter 'converterSubnet2' does not belong to the subnetwork 'subnetwork1'");

    // subnetwork2 content
    BOOST_CHECK_EQUAL(1, boost::size(subnetwork2.getLineCommutatedConverters()));
    BOOST_CHECK_EQUAL(1, subnetwork2.getLineCommutatedConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, subnetwork2.getIdentifiable(converterSubnet2.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, subnetwork2.getLineCommutatedConverter(converterSubnet2.getId())));
    POWSYBL_ASSERT_THROW(subnetwork2.getIdentifiable(converterSubnet1.getId()), PowsyblException, "Identifiable 'converterSubnet1' does not belong to the subnetwork 'subnetwork2'");
    POWSYBL_ASSERT_THROW(subnetwork2.getLineCommutatedConverter(converterSubnet1.getId()), PowsyblException, "LineCommutatedConverter 'converterSubnet1' does not belong to the subnetwork 'subnetwork2'");
    

    // Connectables are retrieved from the root network even when called from a subnetwork
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getDcConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork1.getConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork1.getDcConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork2.getConnectables<LineCommutatedConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork2.getDcConnectables<LineCommutatedConverter>()));

}


BOOST_AUTO_TEST_CASE(testSubnetworksVsc) {

    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    Network& subnetwork2 = netWithSubnet.newSubnetwork("subnetwork2", "format2");
    Substation& sSubnet1 = subnetwork1.newSubstation().setId("SSubnetwork1").add();
    VoltageLevel& vlSubnet1 = sSubnet1.newVoltageLevel().setId("VLSubnet1").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1Subnet1 = vlSubnet1.getBusBreakerView().newBus().setId("B1Subnet1").add();
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnet1").setNominalV(1.).add();
    DcNode& dcNode2Subnet1 = subnetwork1.newDcNode().setId("dcNode2Subnet1").setNominalV(500.).add();
    Substation& sSubnet2 = subnetwork2.newSubstation().setId("SSubnetwork2").add();
    VoltageLevel& vlSubnet2 = sSubnet2.newVoltageLevel().setId("VLSubnet2").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1Subnet2 = vlSubnet2.getBusBreakerView().newBus().setId("B1Subnet2").add();
    DcNode& dcNode1Subnet2 = subnetwork2.newDcNode().setId("dcNode1Subnet2").setNominalV(1.).add();
    DcNode& dcNode2Subnet2 = subnetwork2.newDcNode().setId("dcNode2Subnet2").setNominalV(500.).add();

    VoltageSourceConverter& converterSubnet1 = vlSubnet1
                .newVoltageSourceConverter()
                .setId("converterSubnet1")
                .setBus1(b1Subnet1.getId())
                .setDcNode1(dcNode1Subnet1.getId())
                .setDcNode2(dcNode2Subnet1.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();
    VoltageSourceConverter& converterSubnet2 = vlSubnet2
                .newVoltageSourceConverter()
                .setId("converterSubnet2")
                .setBus1(b1Subnet2.getId())
                .setDcNode1(dcNode1Subnet2.getId())
                .setDcNode2(dcNode2Subnet2.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();

    // network content
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(2, netWithSubnet.getVoltageSourceConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, netWithSubnet.getIdentifiable(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, netWithSubnet.getVoltageSourceConverter(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, netWithSubnet.getIdentifiable(converterSubnet2.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, netWithSubnet.getVoltageSourceConverter(converterSubnet2.getId())));

    // subnetwork1 content
    BOOST_CHECK_EQUAL(1, boost::size(subnetwork1.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(1, subnetwork1.getVoltageSourceConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, subnetwork1.getIdentifiable(converterSubnet1.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet1, subnetwork1.getVoltageSourceConverter(converterSubnet1.getId())));
    POWSYBL_ASSERT_THROW(subnetwork1.getIdentifiable(converterSubnet2.getId()), PowsyblException, "Identifiable 'converterSubnet2' does not belong to the subnetwork 'subnetwork1'");
    POWSYBL_ASSERT_THROW(subnetwork1.getVoltageSourceConverter(converterSubnet2.getId()), PowsyblException, "VoltageSourceConverter 'converterSubnet2' does not belong to the subnetwork 'subnetwork1'");

    // subnetwork2 content
    BOOST_CHECK_EQUAL(1, boost::size(subnetwork2.getVoltageSourceConverters()));
    BOOST_CHECK_EQUAL(1, subnetwork2.getVoltageSourceConverterCount());
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, subnetwork2.getIdentifiable(converterSubnet2.getId())));
    BOOST_CHECK(stdcxx::areSame(converterSubnet2, subnetwork2.getVoltageSourceConverter(converterSubnet2.getId())));
    POWSYBL_ASSERT_THROW(subnetwork2.getIdentifiable(converterSubnet1.getId()), PowsyblException, "Identifiable 'converterSubnet1' does not belong to the subnetwork 'subnetwork2'");
    POWSYBL_ASSERT_THROW(subnetwork2.getVoltageSourceConverter(converterSubnet1.getId()), PowsyblException, "VoltageSourceConverter 'converterSubnet1' does not belong to the subnetwork 'subnetwork2'");
    

    // Connectables are retrieved from the root network even when called from a subnetwork
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getConnectables<VoltageSourceConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(netWithSubnet.getDcConnectables<VoltageSourceConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork1.getConnectables<VoltageSourceConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork1.getDcConnectables<VoltageSourceConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork2.getConnectables<VoltageSourceConverter>()));
    BOOST_CHECK_EQUAL(2, boost::size(subnetwork2.getDcConnectables<VoltageSourceConverter>()));

}


BOOST_AUTO_TEST_CASE(testNotAcrossNetworkSubnetworks) {
    Network netWithSubnet("test", "test");
    Network& subnetwork1 = netWithSubnet.newSubnetwork("subnetwork1", "format1");
    Network& subnetwork2 = netWithSubnet.newSubnetwork("subnetwork2", "format2");
    Substation& sSubnet1 = subnetwork1.newSubstation().setId("SSubnetwork1").add();
    VoltageLevel& vlSubnet1 = sSubnet1.newVoltageLevel().setId("VLSubnet1").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1Subnet1 = vlSubnet1.getBusBreakerView().newBus().setId("B1Subnet1").add();
    DcNode& dcNode1Root = netWithSubnet.newDcNode().setId("dcNode1Root").setNominalV(1.).add();
    DcNode& dcNode1Subnet1 = subnetwork1.newDcNode().setId("dcNode1Subnet1").setNominalV(1.).add();
    DcNode& dcNode1Subnet2 = subnetwork2.newDcNode().setId("dcNode1Subnet2").setNominalV(1.).add();
    DcNode& dcNode2Subnet2 = subnetwork2.newDcNode().setId("dcNode2Subnet2").setNominalV(500.).add();

    VoltageSourceConverterAdder adder = vlSubnet1
                .newVoltageSourceConverter()
                .setId("converterAcrossSubnets")
                .setBus1(b1Subnet1.getId())
                .setDcNode1(dcNode1Subnet1.getId())
                .setDcNode2(dcNode1Subnet2.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Voltage Source Converter 'converterAcrossSubnets': DC Nodes 'dcNode1Subnet1' and 'dcNode1Subnet2' are in different networks 'subnetwork1' and 'subnetwork2'");

    adder.setDcNode1(dcNode1Subnet2.getId()).setDcNode2(dcNode2Subnet2.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Voltage Source Converter 'converterAcrossSubnets': DC Nodes 'dcNode1Subnet2' and 'dcNode2Subnet2' are in network 'subnetwork2' but DC Equipment is in 'subnetwork1'");

    adder.setDcNode1(dcNode1Subnet1.getId()).setDcNode2(dcNode1Root.getId());
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Voltage Source Converter 'converterAcrossSubnets': DC Nodes 'dcNode1Subnet1' and 'dcNode1Root' are in different networks 'subnetwork1' and 'test'");

}

BOOST_AUTO_TEST_CASE(testVscReactiveLimits) {
    Network network = createAcDcNetwork();
    auto& vsc = createVscA(network);

    POWSYBL_ASSERT_ENUM_EQ(ReactiveLimitsKind::MIN_MAX, vsc.getReactiveLimits<ReactiveLimits>().getKind());
    BOOST_CHECK(stdcxx::areSame(vsc.getReactiveLimits<ReactiveLimits>(), vsc.getReactiveLimits<MinMaxReactiveLimits>()));
    BOOST_CHECK_CLOSE(-std::numeric_limits<double>::max(), vsc.getReactiveLimits<ReactiveLimits>().getMinQ(0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(std::numeric_limits<double>::max(), vsc.getReactiveLimits<ReactiveLimits>().getMaxQ(0), std::numeric_limits<double>::epsilon());
    

    vsc.newMinMaxReactiveLimits().setMinQ(-100.).setMaxQ(150.).add();
    BOOST_CHECK_CLOSE(-100.0, vsc.getReactiveLimits<ReactiveLimits>().getMinQ(0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(150.0, vsc.getReactiveLimits<ReactiveLimits>().getMaxQ(0), std::numeric_limits<double>::epsilon());

    vsc.newReactiveCapabilityCurve()
                .beginPoint().setP(-100.).setMinQ(-80).setMaxQ(70.).endPoint()
                .beginPoint().setP(0.).setMinQ(-100).setMaxQ(90.).endPoint()
                .beginPoint().setP(100.).setMinQ(-70).setMaxQ(60.).endPoint()
                .add();
    POWSYBL_ASSERT_ENUM_EQ(ReactiveLimitsKind::CURVE, vsc.getReactiveLimits<ReactiveLimits>().getKind());
    BOOST_CHECK(stdcxx::areSame(vsc.getReactiveLimits<ReactiveLimits>(), vsc.getReactiveLimits<ReactiveCapabilityCurve>()));
    BOOST_CHECK_CLOSE(-90., vsc.getReactiveLimits<ReactiveLimits>().getMinQ(-50), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(80, vsc.getReactiveLimits<ReactiveLimits>().getMaxQ(-50), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(testCreationError) {
    Network network = createAcDcNetwork();
    LineCommutatedConverterAdder adder = network.getVoltageLevel("VLA").newLineCommutatedConverter()
                .setId("converterA")
                .setBus1("B1A")
                .setBus2("B2A");
    
    auto& acDcConverterB = createLccB(network);

    // VDC by default
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': targetVdc is invalid");

    adder.setControlMode(AcDcConverter::ControlMode::P_PCC);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': targetP is invalid");
    adder.setTargetP(200.);

    adder.setPccTerminal(stdcxx::ref(network.getLoad("LAX").getTerminal()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': pccTerminal is not a line or transformer or converter terminal");

    adder.setPccTerminal(stdcxx::ref(acDcConverterB.getTerminal1()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': pccTerminal cannot be the terminal of another converter");

    adder.setPccTerminal(stdcxx::ref(network.getLine("LINEAX").getTerminal1()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': dcNode1 is not set");

    adder.setDcNode1("dcNode1a");
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': dcNode2 is not set");
    adder.setDcNode2("dcNode2a");

    Network& subnet = network.newSubnetwork("subNet", "code");
    VoltageLevel& subNetVl = subnet.newVoltageLevel().setId("subNetVl").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(400.).add();
    Bus& subnetB1 = subNetVl.getBusBreakerView().newBus().setId("subNetB1").add();
    Bus& subnetB2 = subNetVl.getBusBreakerView().newBus().setId("subNetB2").add();
    Line& subNetLine = subnet.newLine().setId("subNetLine")
                .setBus1(subnetB1.getId())
                .setBus2(subnetB2.getId())
                .setR(0.).setX(1.0).setB1(0.).setB2(0.).setG1(0.).setG2(0.)
                .add();

    adder.setPccTerminal(stdcxx::ref(subNetLine.getTerminal1()));
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "AC/DC Line Commutated Converter 'converterA': pccTerminal is not in the same parent network as the voltage level");

}

BOOST_AUTO_TEST_CASE(testSingleAcTerminal) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = network.getVoltageLevel("VLA").newVoltageSourceConverter()
                .setId("converterA")
                .setBus1("B1A")
                .setDcNode1("dcNode1a")
                .setDcNode2("dcNode2a")
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();

    POWSYBL_ASSERT_REF_FALSE(acDcConverterA.getTerminal2());
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getPccTerminal(), acDcConverterA.getTerminal1())); //Pcc terminal by default set to terminal1

    POWSYBL_ASSERT_THROW(acDcConverterA.getTerminal(TerminalNumber::TWO), PowsyblException, "AC/DC Converter 'converterA' does not have a second AC Terminal");

}

BOOST_AUTO_TEST_CASE(testTwoAcTerminals) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = network.getVoltageLevel("VLA").newVoltageSourceConverter()
                .setId("converterA")
                .setBus1("B1A")
                .setBus2("B2A")
                .setDcNode1("dcNode1a")
                .setDcNode2("dcNode2a")
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .add();

    POWSYBL_ASSERT_REF_TRUE(acDcConverterA.getTerminal2());
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getPccTerminal(), acDcConverterA.getTerminal1())); //Pcc terminal by default set to terminal1

    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal(TerminalNumber::TWO), acDcConverterA.getTerminal2().get()));

    // change PCC Terminal to line terminal
    acDcConverterA.setPccTerminal(stdcxx::ref(network.getLine("LINEAX").getTerminal1()));
    BOOST_CHECK(stdcxx::areSame(network.getLine("LINEAX").getTerminal1(), acDcConverterA.getPccTerminal()));

    // check we can set again to converter's AC terminal
    acDcConverterA.setPccTerminal(stdcxx::ref(acDcConverterA.getTerminal1()));
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getTerminal1(), acDcConverterA.getPccTerminal()));

    // change again PCC Terminal to line terminal
    acDcConverterA.setPccTerminal(stdcxx::ref(network.getLine("LINEAX").getTerminal1()));
    BOOST_CHECK(stdcxx::areSame(network.getLine("LINEAX").getTerminal1(), acDcConverterA.getPccTerminal()));

    //remove line, Pcc Terminal removed, return default converter terminal 1
    network.getLine("LINEAX").remove();
    BOOST_CHECK(stdcxx::areSame(acDcConverterA.getPccTerminal(), acDcConverterA.getTerminal1()));

}

BOOST_AUTO_TEST_CASE(testSetterGetterInMultiVariants) {
    Network network = createAcDcNetwork();
    auto& vscA = network.getVoltageLevel("VLA").newVoltageSourceConverter()
                .setId("converterA")
                .setBus1("B1A")
                .setDcNode1("dcNode1a")
                .setDcNode2("dcNode2a")
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(10.0)
                .setVoltageSetpoint(400.0)
                .add();

    BOOST_CHECK_EQUAL(AcDcConverter::ControlMode::P_PCC, vscA.getControlMode());
    BOOST_CHECK_CLOSE(100.0, vscA.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, vscA.getTargetVdc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, vscA.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, vscA.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!vscA.isVoltageRegulatorOn());

    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "s1";
    std::string variant2 = "s2";
    std::string variant3 = "s3";
    std::string variant4 = "s4";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2, variant3, variant4});

    variantManager.setWorkingVariant("s4");
    // check values cloned by extend
    BOOST_CHECK_EQUAL(AcDcConverter::ControlMode::P_PCC, vscA.getControlMode());
    BOOST_CHECK_CLOSE(100.0, vscA.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, vscA.getTargetVdc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, vscA.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, vscA.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!vscA.isVoltageRegulatorOn());
    // change values in s4
    vscA.setControlMode(AcDcConverter::ControlMode::V_DC)
            .setTargetP(-50.)
            .setTargetVdc(495.)
            .setReactivePowerSetpoint(20.0)
            .setVoltageSetpoint(405.0)
            .setVoltageRegulatorOn(true);

    // remove s2
    variantManager.removeVariant("s2");

    variantManager.cloneVariant("s4", "s2b");
    variantManager.setWorkingVariant("s2b");
    // check values cloned by allocate
    BOOST_CHECK_EQUAL(AcDcConverter::ControlMode::V_DC, vscA.getControlMode());
    BOOST_CHECK_CLOSE(-50.0, vscA.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(495.0, vscA.getTargetVdc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, vscA.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(405.0, vscA.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(vscA.isVoltageRegulatorOn());

    // recheck initial variant value unchanged
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL(AcDcConverter::ControlMode::P_PCC, vscA.getControlMode());
    BOOST_CHECK_CLOSE(100.0, vscA.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, vscA.getTargetVdc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, vscA.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, vscA.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!vscA.isVoltageRegulatorOn());

    // remove working variant s4
    variantManager.setWorkingVariant("s4");
    variantManager.removeVariant("s4");
    POWSYBL_ASSERT_THROW(vscA.getControlMode(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(vscA.getTargetP(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(vscA.getTargetVdc(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(vscA.getReactivePowerSetpoint(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(vscA.getVoltageSetpoint(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(vscA.isVoltageRegulatorOn(), PowsyblException, "Variant index not set");

}

BOOST_AUTO_TEST_CASE(testDroopCurve) {
    Network network = createAcDcNetwork();
    auto& acDcConverterA = createVscA(network);
    
    acDcConverterA.setControlMode(AcDcConverter::ControlMode::DROOP);
    BOOST_CHECK_EQUAL(AcDcConverter::ControlMode::DROOP, acDcConverterA.getControlMode());

    acDcConverterA.newDroopCurve()
                    .addSegment(-500,-10)
                    .addSegment(-100,-5)
                    .addSegment(100, -1)
                    .setMaxV(500)
                    .add();
    POWSYBL_ASSERT_REF_TRUE(acDcConverterA.getDroopCurve());
    const auto& curve = acDcConverterA.getDroopCurve().get();
    BOOST_CHECK_CLOSE(-10.0, curve.getK(-250.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-5.0, curve.getK(-100.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-1.0, curve.getK(400.0), std::numeric_limits<double>::epsilon());

    acDcConverterA.newDroopCurve().add();
    BOOST_CHECK_CLOSE(0.0, acDcConverterA.getDroopCurve().get().getK(400.0), std::numeric_limits<double>::epsilon());

    acDcConverterA.removeDroopCurve();
    POWSYBL_ASSERT_REF_FALSE(acDcConverterA.getDroopCurve());
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
