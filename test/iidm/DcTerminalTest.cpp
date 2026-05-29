/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcLineAdder.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcTerminalTestSuite)

void checkDcTerminalInMultiVariant(Network& network, DcTerminal& dcTerminal) {
    dcTerminal.setConnected(true) .setP(10.) .setI(5.);

    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "s1";
    std::string variant2 = "s2";
    std::string variant3 = "s3";
    std::string variant4 = "s4";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2, variant3, variant4});

    variantManager.setWorkingVariant("s4");
    // check values cloned by extend
    BOOST_CHECK(dcTerminal.isConnected());
    BOOST_CHECK_CLOSE(10.0, dcTerminal.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, dcTerminal.getI(), std::numeric_limits<double>::epsilon());

    // change values in s4
    dcTerminal.setConnected(false).setP(-20.).setI(-10.);

    // remove s2
    variantManager.removeVariant("s2");

    variantManager.cloneVariant("s4", "s2b");
    variantManager.setWorkingVariant("s2b");
    // check values cloned by allocate
    BOOST_CHECK(!dcTerminal.isConnected());
    BOOST_CHECK_CLOSE(-20.0, dcTerminal.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-10.0, dcTerminal.getI(), std::numeric_limits<double>::epsilon());

    // recheck initial variant value
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(dcTerminal.isConnected());
    BOOST_CHECK_CLOSE(10.0, dcTerminal.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, dcTerminal.getI(), std::numeric_limits<double>::epsilon());

    // remove working variant s4
    variantManager.setWorkingVariant("s4");
    variantManager.removeVariant("s4");
    POWSYBL_ASSERT_THROW(dcTerminal.isConnected(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(dcTerminal.getP(), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(dcTerminal.getI(), PowsyblException, "Variant index not set");

    // check we delete a single variant's values
    variantManager.setWorkingVariant("s3");
    BOOST_CHECK(dcTerminal.isConnected());
    BOOST_CHECK_CLOSE(10.0, dcTerminal.getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, dcTerminal.getI(), std::numeric_limits<double>::epsilon());
}


BOOST_AUTO_TEST_CASE(testDcTerminalOfDcLine) {
    Network network("test", "test");
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1Id").setNominalV(500.).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2Id").setNominalV(500.).add();

    DcLine& dcLine = network.newDcLine()
                .setId("dcLine")
                .setDcNode1(dcNode1.getId())
                .setConnected1(true)
                .setDcNode2(dcNode2.getId())
                .setConnected2(true)
                .setR(1.1)
                .add();

    DcTerminal& dcLineTerminal = dcLine.getDcTerminal1();
    BOOST_CHECK(std::isnan(dcLineTerminal.getP()));
    BOOST_CHECK(std::isnan(dcLineTerminal.getI()));
    BOOST_CHECK(dcLineTerminal.isConnected());

    checkDcTerminalInMultiVariant(network, dcLineTerminal);
}


BOOST_AUTO_TEST_CASE(testDcTerminalOfAcDcConverter) {
    Network network("test", "test");
    Substation& sa = network.newSubstation().setId("S").add();
    VoltageLevel& vl = sa.newVoltageLevel().setId("VL").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();
    Bus& b1 = vl.getBusBreakerView().newBus().setId("B1").add();
    DcNode& dcNode1 = network.newDcNode().setId("dcNode1").setNominalV(500.).add();
    DcNode& dcNode2 = network.newDcNode().setId("dcNode2").setNominalV(500.).add();
    LineCommutatedConverter& converter = vl.newLineCommutatedConverter()
                .setId("dcConverter")
                .setBus1(b1.getId())
                .setDcNode1(dcNode1.getId())
                .setDcNode2(dcNode2.getId())
                .setControlMode(AcDcConverter::ControlMode::P_PCC)
                .setTargetP(100.)
                .setTargetVdc(500.)
                .add();

    DcTerminal& dcConverterTerminal = converter.getDcTerminal(TerminalNumber::ONE);

    BOOST_CHECK(std::isnan(dcConverterTerminal.getP()));
    BOOST_CHECK(std::isnan(dcConverterTerminal.getI()));
    BOOST_CHECK(dcConverterTerminal.isConnected());

    checkDcTerminalInMultiVariant(network, dcConverterTerminal);
}

BOOST_AUTO_TEST_CASE(testTerminalBuilder) {
    Network network("test", "test");
    Substation& sa = network.newSubstation().setId("S").add();
    VoltageLevel& vl = sa.newVoltageLevel().setId("VL").setTopologyKind(TopologyKind::BUS_BREAKER).setNominalV(175).add();

    POWSYBL_ASSERT_THROW(TerminalBuilder(vl, vl, ThreeSides::ONE, TerminalNumber::ONE), ValidationException, "Voltage level 'VL': cannot create a terminal that have both side and number");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
