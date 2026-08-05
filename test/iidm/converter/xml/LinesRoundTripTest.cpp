/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network createBlGenerationLinear() {
    Network network("boundary-line", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2020-07-16T10:08:48.321+02:00"));

    Substation& substation = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setId("VL")
        .setNominalV(100.0)
        .setLowVoltageLimit(80.0)
        .setHighVoltageLimit(120.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS")
        .add();
    voltageLevel.newGenerator()
        .setId("G")
        .setMinP(0.0)
        .setMaxP(100.0)
        .setVoltageRegulatorOn(true)
        .setTargetV(100.0)
        .setTargetP(50.0)
        .setTargetQ(30.0)
        .setBus("BUS")
        .add();

    BoundaryLine& boundaryLine = network.getVoltageLevel("VL").newBoundaryLine()
        .setId("BL")
        .setBus("BUS")
        .setR(10.0)
        .setX(1.0)
        .setB(10e-6)
        .setG(10e-5)
        .setP0(50.0)
        .setQ0(30.0)
        .newGeneration()
        .setTargetP(440)
        .setMaxP(900)
        .setMinP(0)
        .setTargetV(400)
        .setVoltageRegulationOn(true)
        .add()
        .add();
    boundaryLine.getGeneration().get().newReactiveCapabilityCurve()
        .beginPoint()
        .setP(0.0)
        .setMinQ(-59.3)
        .setMaxQ(60.0)
        .endPoint()
        .beginPoint()
        .setP(70.0)
        .setMinQ(-54.55)
        .setMaxQ(46.25)
        .endPoint()
        .add();
    boundaryLine.getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits()
        .setPermanentLimit(100.0)
        .beginTemporaryLimit()
        .setName("20'")
        .setValue(120.0)
        .setAcceptableDuration(20 * 60)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("10'")
        .setValue(140.0)
        .setAcceptableDuration(10 * 60)
        .endTemporaryLimit()
        .add();
    boundaryLine.setProperty("test", "test");
    return network;
}

BOOST_AUTO_TEST_SUITE(XmlRoundTrip)

BOOST_FIXTURE_TEST_CASE(BoundaryLineTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("boundaryLine.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(BoundaryLineWithGenerationTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("boundaryLineWithGeneration.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.3
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("boundaryLineWithGeneration.xml", IidmXmlVersion::V1_3());

    Network network = createBlGenerationLinear();

    // check it fails for all versions < 1.3
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_3(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("danglingLine.generation is not null and not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.3", version.toString(".")).c_str());
    });

    // check it doesn't fail for all versions < 1.3 if IidmVersionIncompatibilityBehavior is to log error
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_3(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("boundaryLineWithGeneration.xml", version));
    });
}

BOOST_FIXTURE_TEST_CASE(testRejectDanglingLine, test::ResourceFixture) {
    test::converter::RoundTrip::testForAllVersionsSince(IidmXmlVersion::V1_16(), [](const iidm::converter::xml::IidmXmlVersion& version) {
        POWSYBL_ASSERT_THROW(Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("danglingLine.xml", version)), PowsyblException,
            stdcxx::format("danglingLine is not supported for IIDM-XML version %1%. IIDM-XML version should be <= 1.15", version.toString(".")).c_str());
    });
}

BOOST_FIXTURE_TEST_CASE(TieLineTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("tieline.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(TieLineFictitiousTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("tielineFictitious.xml", IidmXmlVersion::all());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

