/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlRoundTrip)

BOOST_FIXTURE_TEST_CASE(TerminalRef, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("terminalRef.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(TerminalRefNotFoundTest, test::ResourceFixture) {
    const auto& filePath = test::converter::RoundTrip::getVersionedNetworkPath("terminalRefNotFound.xml", IidmXmlVersion::V1_5());
    POWSYBL_ASSERT_THROW(Network::readXml(filePath), PowsyblException, "Terminal reference identifiable not found: 'UNKNOWN'");
}

BOOST_AUTO_TEST_CASE(badSwitchTerminalResolve) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    POWSYBL_ASSERT_THROW(TerminalRefXml::resolve("S1VL1_LD1_BREAKER", ThreeSides::ONE, network)
    , PowsyblException, "Unexpected terminal reference identifiable instance: powsybl::iidm::Switch");
}

BOOST_AUTO_TEST_CASE(badBranchSideResolve) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    POWSYBL_ASSERT_THROW(TerminalRefXml::resolve("LINE_S2S3", ThreeSides::THREE, network)
    , PowsyblException, "Unexpected Branch side: THREE");
}

BOOST_AUTO_TEST_CASE(badIdentifiableSideResolve) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();

    POWSYBL_ASSERT_THROW(TerminalRefXml::resolve("LIN_S2S3", ThreeSides::ONE, network)
    , PowsyblException, "Terminal reference identifiable not found: 'LIN_S2S3'");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

