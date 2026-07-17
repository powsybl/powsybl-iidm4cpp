/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(DcNodeXmlTestSuite)

Network createBaseNetwork() {
    Network network("dcNodeTest", "code");
    network.setCaseDate(stdcxx::DateTime::parse("2025-01-02T03:04:05.000+01:00"));

    auto& dcNode = network.newDcNode()
                .setId("dcNodeWithoutSolvedV")
                .setName("A DC Node without solved V")
                .setNominalV(500.)
                .setFictitious(true)
                .add();
    dcNode.setProperty("prop name", "prop value");
    dcNode.addAlias("someAlias");
    network.newDcNode()
                .setId("dcNodeWithSolvedV")
                .setName("A DC Node with solved V")
                .setNominalV(500.)
                .add()
                .setV(498.);

    return network;
}


BOOST_FIXTURE_TEST_CASE(DcNodeNetworkTest, test::ResourceFixture) {
    Network network = createBaseNetwork();

    test::converter::RoundTrip::roundTripVersionedXmlTest("dcNodeRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    //Backward compatibility check
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("dcNodeRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

}


BOOST_FIXTURE_TEST_CASE(DcNodeNotSupportedTest, test::ResourceFixture) {

    Network network = createBaseNetwork();
    //Fail writing for all versions < 1.15
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_15(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("network.dcNode is not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.15", version.toString(".")).c_str());
    });

    // check that it doesn't fail for versions previous to 1.15 (only test V1.14) when IIDM version incompatibility behavior is to log error
    ExportOptions options = ExportOptions().setVersion(IidmXmlVersion::V1_14().toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("dcNodeNotSupported.xml", IidmXmlVersion::V1_14()));

    //but fail reading that file since V1_14 is not supported:
    const auto& dcNotSupportedNetwork = test::converter::RoundTrip::getVersionedNetworkPath("dcNodeNotSupported.xml",IidmXmlVersion::V1_14());
    POWSYBL_ASSERT_THROW(Network::readXml(dcNotSupportedNetwork), PowsyblException, "network.dcNode is not supported for IIDM-XML version 1.14. IIDM-XML version should be >= 1.15");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

