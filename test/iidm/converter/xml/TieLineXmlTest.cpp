/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/TieLine.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(TieLineXmlTestSuite)

BOOST_FIXTURE_TEST_CASE(TieLineWithAliasesXmlTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("tielineWithAliases.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // Tests for backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("tielineWithAliases.xml", IidmXmlVersion::V1_3());
}

BOOST_FIXTURE_TEST_CASE(TieLineAnonymize, test::ResourceFixture) {

    Network network = ::powsybl::network::EurostagFactory::createWithTieLinesAndAreas();

    // Between 1.10 and 1.15, attribute is "danglingLineId..", since 1.16 "boundaryLineId.."
    test::converter::RoundTrip::testForAllVersionsSince(IidmXmlVersion::V1_10(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        //Export with Anonymize option
        ExportOptions exOptions = ExportOptions().setVersion(version.toString(".")).setAnonymized(true);
        std::stringstream buffer;
        Network::writeXml("AnonymizedEurostagWithTieLinesAndAreas.xiidm", buffer, network, exOptions);
        
        //Import (using Anonymizer)
        Network importedNetwork = Network::readXml("AnonymizedEurostagWithTieLinesAndAreas.xiidm", buffer);

        POWSYBL_ASSERT_REF_TRUE(importedNetwork.find("NHV1_NHV2_1"));
        TieLine& tieLine = importedNetwork.getTieLine("NHV1_NHV2_1");
        BOOST_CHECK_EQUAL(tieLine.getBoundaryLine1().getId(), "NHV1_XNODE1");
        BOOST_CHECK_EQUAL(tieLine.getBoundaryLine2().getId(), "XNODE1_NHV2");

    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
