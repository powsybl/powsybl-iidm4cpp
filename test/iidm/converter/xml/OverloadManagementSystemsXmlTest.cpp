/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>



namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {



BOOST_AUTO_TEST_SUITE(OverloadManagementSystemsXmlTestSuite)

BOOST_FIXTURE_TEST_CASE(roundTrip, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("overloadManagementSystemRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks
    test::converter::RoundTrip::roundTripAllPreviousVersionedXmlTest("overloadManagementSystemRoundTripRef.xml");
}

BOOST_FIXTURE_TEST_CASE(importexportDisable, test::ResourceFixture) {
    Network networkWithOms = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("overloadManagementSystemRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));
    BOOST_CHECK_EQUAL(2, networkWithOms.getSubstationCount());
    BOOST_CHECK_EQUAL(2, networkWithOms.getLineCount());
    BOOST_CHECK_EQUAL(1, networkWithOms.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1, networkWithOms.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(2, networkWithOms.getOverloadManagementSystemCount());

    stdcxx::Properties propertiesImport;
    propertiesImport.set(ImportOptions::WITH_AUTOMATION_SYSTEMS, "false");
    Network networkWithoutOms = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("overloadManagementSystemRoundTripRef.xml", 
        IidmXmlVersion::CURRENT_IIDM_XML_VERSION()), ImportOptions(propertiesImport));
    BOOST_CHECK_EQUAL(2, networkWithoutOms.getSubstationCount());
    BOOST_CHECK_EQUAL(2, networkWithoutOms.getLineCount());
    BOOST_CHECK_EQUAL(1, networkWithoutOms.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1, networkWithoutOms.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(0, networkWithoutOms.getOverloadManagementSystemCount());

    stdcxx::Properties propertiesExport;
    std::stringstream ostream;
    propertiesExport.set(ExportOptions::WITH_AUTOMATION_SYSTEMS, "false");
    Network::writeXml("overloadManagementSystemRoundTripRef.xml", ostream, networkWithOms, ExportOptions(propertiesExport));
    BOOST_CHECK(ostream.str().find("overloadManagementSystem")==std::string::npos);

    Network networkWithoutOms2 = Network::readXml("overloadManagementSystemRoundTripRef.xml", ostream);
    BOOST_CHECK_EQUAL(2, networkWithoutOms2.getSubstationCount());
    BOOST_CHECK_EQUAL(2, networkWithoutOms2.getLineCount());
    BOOST_CHECK_EQUAL(1, networkWithoutOms2.getTwoWindingsTransformerCount());
    BOOST_CHECK_EQUAL(1, networkWithoutOms2.getThreeWindingsTransformerCount());
    BOOST_CHECK_EQUAL(0, networkWithoutOms2.getOverloadManagementSystemCount());
}

BOOST_FIXTURE_TEST_CASE(exportInvalidOmsTest, test::ResourceFixture) {
    Network networkWithOms = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("overloadManagementSystemRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));

    Line& line1 = networkWithOms.getLine("LINE_1");
    line1.remove();
    std::stringstream ostream;
    Network::writeXml("overloadManagementSystemRoundTripRef.xml", ostream, networkWithOms);
    Network networkWithoutLine1 = Network::readXml("overloadManagementSystemRoundTripRef.xml", ostream);
    BOOST_CHECK_EQUAL(1, networkWithoutLine1.getOverloadManagementSystemCount());
    POWSYBL_ASSERT_REF_TRUE(networkWithoutLine1.find<OverloadManagementSystem>("OMS1"));
    POWSYBL_ASSERT_REF_FALSE(networkWithoutLine1.find<OverloadManagementSystem>("OMS2"));

    ThreeWindingsTransformer& twt = networkWithOms.getThreeWindingsTransformer("3WT");
    twt.remove();
    std::stringstream ostream2;
    Network::writeXml("overloadManagementSystemRoundTripRef.xml", ostream2, networkWithOms);
    Network networkWithoutTwt = Network::readXml("overloadManagementSystemRoundTripRef.xml", ostream2);
    BOOST_CHECK_EQUAL(0, networkWithoutTwt.getOverloadManagementSystemCount());
    POWSYBL_ASSERT_REF_FALSE(networkWithoutTwt.find<OverloadManagementSystem>("OMS1"));
    POWSYBL_ASSERT_REF_FALSE(networkWithoutTwt.find<OverloadManagementSystem>("OMS2"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
