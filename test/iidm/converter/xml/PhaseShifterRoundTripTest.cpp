/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
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

BOOST_AUTO_TEST_SUITE(PhaseShifterRoundTrip)

BOOST_FIXTURE_TEST_CASE(PhaseShifterRoundTripTest, test::ResourceFixture) {

    const std::string& filename = "phaseShifterRoundTripRef.xml";

    test::converter::RoundTrip::roundTripVersionedXmlTest(filename, IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest(filename, IidmXmlVersion::V1_14());

    // Since 1.14, FIXED_TAP not supported, though "old" versions can still be read
    //Round trip would fail since export will write them as non-regulating CURRENT_LIMITER
    auto versionfilter = [](const iidm::converter::xml::IidmXmlVersion& version) {
        return version < IidmXmlVersion::V1_14();
    };
    for (const auto& version : iidm::converter::xml::IidmXmlVersion::all() | boost::adaptors::filtered(versionfilter)) {

        const std::string& previousVersionNetwork = test::converter::RoundTrip::getVersionedNetwork(filename, version);
        std::istringstream stream(previousVersionNetwork);
        iidm::Network network = iidm::Network::readXml(filename, stream);

        const std::string& currentVersionNetworkExpected = test::converter::RoundTrip::getVersionedNetwork(filename, IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
        test::converter::RoundTrip::runXml(network, currentVersionNetworkExpected);
    }

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

