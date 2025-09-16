/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(VoltageAngleLimitXmlTestSuite)

BOOST_FIXTURE_TEST_CASE(VoltageAngleLimitXmlTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("voltageAngleLimit.xml",IidmXmlVersion::V1_11());

    Network network =  ::powsybl::network::EurostagFactory::createWithVoltageAngleLimit();
    const std::string& networkStrRef = test::converter::RoundTrip::getVersionedNetwork("voltageAngleLimit.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    
    Network networkRslt = test::converter::RoundTrip::runXml(network, networkStrRef);

    BOOST_CHECK_EQUAL(3, networkRslt.getVoltageAngleLimitsCount());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
