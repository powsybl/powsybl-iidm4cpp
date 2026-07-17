/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcGroundAdder.hpp>
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

BOOST_AUTO_TEST_SUITE(DcGroundXmlTestSuite)

Network createBaseNetwork() {
    Network network("dcGroundTest", "code");
    network.setCaseDate(stdcxx::DateTime::parse("2025-01-02T03:04:05.000+01:00"));

    auto& dcNode = network.newDcNode()
                .setId("dcNode")
                .setNominalV(500.)
                .add();
    auto& dcGround1 = network.newDcGround()
                .setId("dcGroundWithoutSolvedV")
                .setName("A DC Ground without solved values")
                .setFictitious(true)
                .setDcNode(dcNode.getId())
                .setConnected(false)
                .setR(0.1)
                .add();
    dcGround1.setProperty("prop name", "prop value");
    dcGround1.addAlias("someAlias");
    network.newDcGround()
                .setId("dcGroundWithSolvedValues")
                .setName("A DC Ground with solved values")
                .setDcNode(dcNode.getId())
                .setR(2.0)
                .add().getDcTerminal().setP(1.1).setI(2.2);

    return network;
}


BOOST_FIXTURE_TEST_CASE(DcGroundNetworkTest, test::ResourceFixture) {
    Network network = createBaseNetwork();

    test::converter::RoundTrip::roundTripVersionedXmlTest("dcGroundRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    //Backward compatibility check
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("dcGroundRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

}


BOOST_FIXTURE_TEST_CASE(DcGroundNotSupportedTest, test::ResourceFixture) {

    //Fail writing for all versions < 1.15
    //DcNodeXmlTest is sufficient, since DcGround cannot exist withouth DcNode

    auto network = createBaseNetwork();
    // check that it doesn't fail for versions previous to 1.15 (only test V1.14) when IIDM version incompatibility behavior is to log error
    ExportOptions options = ExportOptions().setVersion(IidmXmlVersion::V1_14().toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("dcGroundNotSupported.xml", IidmXmlVersion::V1_14()));

    //but fail reading that file since V1_14 is not supported:
    const auto& dcNotSupportedNetwork = test::converter::RoundTrip::getVersionedNetworkPath("dcGroundNotSupported.xml",IidmXmlVersion::V1_14());
    POWSYBL_ASSERT_THROW(Network::readXml(dcNotSupportedNetwork), PowsyblException, "network.dcNode is not supported for IIDM-XML version 1.14. IIDM-XML version should be >= 1.15");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

