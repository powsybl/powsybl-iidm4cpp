/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/DroopCurveAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(LineCommutatedConverterXmlTestSuite)

Network createBaseNetwork() {
    Network network("lineCommutatedConverterTest", "code");
    network.setCaseDate(stdcxx::DateTime::parse("2025-01-02T03:04:05.000+01:00"));

    DcNode& dcNode1 = network.newDcNode()
                         .setId("dcNode1")
                         .setNominalV(500.)
                         .add();
    DcNode& dcNode2 = network.newDcNode()
                         .setId("dcNode2")
                         .setNominalV(500.)
                         .add();
    Substation& s = network.newSubstation().setId("S").add();
    VoltageLevel& vlBb400 = s.newVoltageLevel()
                               .setId("vlBB400")
                               .setTopologyKind(TopologyKind::BUS_BREAKER)
                               .setNominalV(400.)
                               .add();
    Bus& bus1 = vlBb400.getBusBreakerView()
                   .newBus()
                   .setId("bus1")
                   .add();
    Bus& bus2 = vlBb400.getBusBreakerView()
                   .newBus()
                   .setId("bus2")
                   .add();
    VoltageLevel& vlNb400 = s.newVoltageLevel()
                               .setId("vlNB400")
                               .setTopologyKind(TopologyKind::NODE_BREAKER)
                               .setNominalV(400.)
                               .add();
    vlNb400.getNodeBreakerView().newBusbarSection().setId("bbs400-1").setNode(0).add();
    vlNb400.getNodeBreakerView().newBusbarSection().setId("bbs400-2").setNode(1).add();
    vlNb400.getNodeBreakerView().newInternalConnection().setNode1(0).setNode2(2).add();
    vlNb400.getNodeBreakerView().newInternalConnection().setNode1(2).setNode2(4).add();
    vlNb400.getNodeBreakerView().newInternalConnection().setNode1(1).setNode2(3).add();
    vlNb400.getNodeBreakerView().newInternalConnection().setNode1(3).setNode2(5).add();
    Line& lineBb = network.newLine()
                      .setId("lineBb")
                      .setR(0.)
                      .setX(3.)
                      .setB1(0.)
                      .setG1(0.)
                      .setB2(0.)
                      .setG2(0.)
                      .setBus1(bus1.getId())
                      .setBus2(bus2.getId())
                      .add();
    Line& lineNb = network.newLine()
                      .setId("lineNb")
                      .setR(0.)
                      .setX(3.)
                      .setB1(0.)
                      .setG1(0.)
                      .setB2(0.)
                      .setG2(0.)
                      .setVoltageLevel1(vlNb400.getId())
                      .setNode1(2)
                      .setVoltageLevel2(vlNb400.getId())
                      .setNode2(3)
                      .add();
    LineCommutatedConverter& lcc1 = vlBb400.newLineCommutatedConverter()
                                       .setId("lccBb1ACWithoutSolvedV")
                                       .setName("LCC in Bus/Breaker with one AC Terminal without solved values")
                                       .setDcNode1(dcNode1.getId())
                                       .setDcConnected1(false)
                                       .setDcNode2(dcNode2.getId())
                                       .setDcConnected2(false)
                                       .setConnectableBus1(bus1.getId())
                                       .setReactiveModel(LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR)
                                       .setPowerFactor(0.92)
                                       .setControlMode(AcDcConverter::ControlMode::V_DC)
                                       .setTargetVdc(502.)
                                       .add();
    lcc1.setProperty("prop name", "prop value");
    lcc1.addAlias("someAlias");

    LineCommutatedConverter& lcc2 = vlBb400.newLineCommutatedConverter()
                                       .setId("lccBb2ACWithSolvedV")
                                       .setName("LCC in Bus/Breaker with two AC Terminals with solved values and droop curve")
                                       .setDcNode1(dcNode1.getId())
                                       .setDcConnected1(true)
                                       .setDcNode2(dcNode2.getId())
                                       .setDcConnected2(true)
                                       .setBus1(bus1.getId())
                                       .setBus2(bus2.getId())
                                       .setReactiveModel(LineCommutatedConverter::ReactiveModel::CALCULATED_POWER_FACTOR)
                                       .setPowerFactor(0.92)
                                       .setControlMode(AcDcConverter::ControlMode::P_PCC_DROOP)
                                       .setTargetVdc(502.)
                                       .setTargetP(301.)
                                       .setPccTerminal(stdcxx::ref(lineBb.getTerminal1()))
                                       .setIdleLoss(2.0)
                                       .setSwitchingLoss(0.2)
                                       .setResistiveLoss(2e-6)
                                       .add();
    lcc2.getDcTerminal1().setP(-100.).setI(-200.);
    lcc2.getDcTerminal2().setP(0.4).setI(200.);
    lcc2.getTerminal1().setP(-100.).setQ(-200.);
    lcc2.getTerminal2().get().setP(-100.1).setQ(-200.2);

    lcc2.newDroopCurve()
        .addSegment(-500., -10.)
        .addSegment(-100., -5.)
        .addSegment(100., -1.)
        .setMaxV(500.)
        .add();

    LineCommutatedConverter& lcc3 = vlNb400.newLineCommutatedConverter()
                                       .setId("lccNb2ACWithPartiallySolvedV")
                                       .setName("LCC in Node/Breaker with two AC Terminals with some solved values missing")
                                       .setDcNode1(dcNode1.getId())
                                       .setDcConnected1(true)
                                       .setDcNode2(dcNode2.getId())
                                       .setDcConnected2(true)
                                       .setNode1(4)
                                       .setNode2(5)
                                       .setReactiveModel(LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR)
                                       .setPowerFactor(0.94)
                                       .setControlMode(AcDcConverter::ControlMode::P_PCC)
                                       .setTargetVdc(503.)
                                       .setTargetP(302.)
                                       .setPccTerminal(stdcxx::ref(lineNb.getTerminal2()))
                                       .setIdleLoss(3.0)
                                       .setSwitchingLoss(0.3)
                                       .setResistiveLoss(3e-6)
                                       .add();

    lcc3.getTerminal1().getBusView().getBus().get().setV(402.).setAngle(12.3);
    lcc3.getDcTerminal1().setP(-102.);              // no I
    lcc3.getDcTerminal2().setI(202.);               // no P
    lcc3.getTerminal1().setP(-105.);                // no Q
    lcc3.getTerminal2().get().setQ(-200.8); // no P
    return network;
}


BOOST_FIXTURE_TEST_CASE(LineCommutatedConverterNetworkTest, test::ResourceFixture) {
    Network network = createBaseNetwork();

    test::converter::RoundTrip::runXml(network, test::converter::RoundTrip::getVersionedNetwork("lineCommutatedConverterRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));

    //Backward compatibility check
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("lineCommutatedConverterRoundTripRef.xml", IidmXmlVersion::V1_15());

}


BOOST_FIXTURE_TEST_CASE(LineCommutatedConverterNotSupportedTest, test::ResourceFixture) {

    //Fail writing for all versions < 1.15
    //DcNodeXmlTest is sufficient, since LineCommutatedConverter cannot exist withouth DcNode

    auto network = createBaseNetwork();
    // check that it doesn't fail for versions previous to 1.15 (only test V1.14) when IIDM version incompatibility behavior is to log error
    ExportOptions options = ExportOptions().setVersion(IidmXmlVersion::V1_14().toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("lineCommutatedConverterNotSupported.xml", IidmXmlVersion::V1_14()));

    //but fail reading that file since V1_14 is not supported:
    const auto& dcNotSupportedNetwork = test::converter::RoundTrip::getVersionedNetworkPath("lineCommutatedConverterNotSupported.xml",IidmXmlVersion::V1_14());
    POWSYBL_ASSERT_THROW(Network::readXml(dcNotSupportedNetwork), PowsyblException, "network.dcNode is not supported for IIDM-XML version 1.14. IIDM-XML version should be >= 1.15");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

