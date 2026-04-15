/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network createRatioTapChangerNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2024-01-08T09:17:53.764Z"));

    Substation& substation = network.newSubstation()
                                .setId("SUBSTATION")
                                .setCountry(Country::FR)
                                .add();
    VoltageLevel& vl1 = substation.newVoltageLevel()
                           .setId("VL_1")
                           .setNominalV(132.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl1.getBusBreakerView().newBus().setId("BUS_1").add();
    VoltageLevel& vl2 = substation.newVoltageLevel()
                           .setId("VL_2")
                           .setNominalV(33.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl2.getBusBreakerView().newBus().setId("BUS_2").add();

    TwoWindingsTransformer& t2wt = network.getSubstation("SUBSTATION").newTwoWindingsTransformer()
                                                                            .setId("T2wT")
                                                                            .setRatedU1(132.0)
                                                                            .setRatedU2(33.0)
                                                                            .setR(17.0)
                                                                            .setX(10.0)
                                                                            .setG(0.00573921028466483)
                                                                            .setB(0.000573921028466483)
                                                                            .setBus1("BUS_1")
                                                                            .setBus2("BUS_2")
                                                                            .add();

    t2wt.newRatioTapChanger()
        .beginStep()
        .setRho(0.9)
        .setR(0.1089)
        .setX(0.01089)
        .setG(0.8264462809917356)
        .setB(0.08264462809917356)
        .endStep()
        .setTapPosition(0)
        .setLoadTapChangingCapabilities(true)
        .setRegulating(true)
        .setRegulationMode(RatioTapChanger::RegulationMode::REACTIVE_POWER)
        .setRegulationValue(-10)
        .setRegulationTerminal(stdcxx::ref(t2wt.getTerminal2()))
        .setTargetDeadband(0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(RatioTapChangerXmlTestSuite)

BOOST_FIXTURE_TEST_CASE(RatioTapChangerXmlTest, test::ResourceFixture) {

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("ratioTapChangerReactivePowerControlRef.xml", IidmXmlVersion::V1_12());

    Network network = createRatioTapChangerNetwork();
    const std::string& networkStr = test::converter::RoundTrip::getVersionedNetwork("ratioTapChangerReactivePowerControlRef.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::runXml(network, networkStr);

}

BOOST_FIXTURE_TEST_CASE(RatioTapChangerWithSolvedTapPositionXmlTest, test::ResourceFixture) {

    Network network = createRatioTapChangerNetwork();
    network.getTwoWindingsTransformer("T2wT").getRatioTapChanger()
        .setSolvedTapPosition(0);

    test::converter::RoundTrip::runXml(network, test::converter::RoundTrip::getVersionedNetwork("ratioTapChangerWithSolvedTapPositionRef.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("ratioTapChangerWithSolvedTapPositionRef.xml", IidmXmlVersion::V1_14());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

