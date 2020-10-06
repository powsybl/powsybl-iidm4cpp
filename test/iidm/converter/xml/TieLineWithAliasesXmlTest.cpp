/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(TieLineWithAliasesTestSuite)

BOOST_FIXTURE_TEST_CASE(TieLineWithAliasesXmlTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("tielineWithAliases.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // Tests for backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("tielineWithAliases.xml", IidmXmlVersion::V1_3());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
