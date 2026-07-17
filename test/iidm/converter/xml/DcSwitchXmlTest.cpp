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
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcSwitchAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(DcSwitchXmlTestSuite)

Network createBaseNetwork() {
    Network network("dcSwitchTest", "code");
    network.setCaseDate(stdcxx::DateTime::parse("2025-01-02T03:04:05.000+01:00"));

    DcNode& dcNode1 = network.newDcNode()
                         .setId("dcNode1")
                         .setNominalV(500.)
                         .add();
    DcNode& dcNode2 = network.newDcNode()
                         .setId("dcNode2")
                         .setNominalV(500.)
                         .add();
    DcSwitch& dcSwitch1 = network.newDcSwitch()
                             .setId("dcSwitchOpened")
                             .setName("An opened DC Switch")
                             .setFictitious(true)
                             .setDcNode1(dcNode1.getId())
                             .setDcNode2(dcNode2.getId())
                             .setKind(DcSwitchKind::DISCONNECTOR)
                             .setOpen(true)
                             .add();
    dcSwitch1.setProperty("prop name", "prop value");
    dcSwitch1.addAlias("someAlias");
    network.newDcSwitch()
        .setId("dcSwitchClosed")
        .setName("A closed DC Switch")
        .setDcNode1(dcNode1.getId())
        .setDcNode2(dcNode2.getId())
        .setKind(DcSwitchKind::BREAKER)
        .setOpen(false)
        .add();
    return network;
}


BOOST_FIXTURE_TEST_CASE(DcSwitchNetworkTest, test::ResourceFixture) {
    Network network = createBaseNetwork();

    test::converter::RoundTrip::roundTripVersionedXmlTest("dcSwitchRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    //Backward compatibility check
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("dcSwitchRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

}


BOOST_FIXTURE_TEST_CASE(DcSwitchNotSupportedTest, test::ResourceFixture) {

    //Fail writing for all versions < 1.15
    //DcNodeXmlTest is sufficient, since DcSwitch cannot exist withouth DcNode

    auto network = createBaseNetwork();
    // check that it doesn't fail for versions previous to 1.15 (only test V1.14) when IIDM version incompatibility behavior is to log error
    ExportOptions options = ExportOptions().setVersion(IidmXmlVersion::V1_14().toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("dcSwitchNotSupported.xml", IidmXmlVersion::V1_14()));

    //but fail reading that file since V1_14 is not supported:
    const auto& dcNotSupportedNetwork = test::converter::RoundTrip::getVersionedNetworkPath("dcSwitchNotSupported.xml",IidmXmlVersion::V1_14());
    POWSYBL_ASSERT_THROW(Network::readXml(dcNotSupportedNetwork), PowsyblException, "network.dcNode is not supported for IIDM-XML version 1.14. IIDM-XML version should be >= 1.15");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

