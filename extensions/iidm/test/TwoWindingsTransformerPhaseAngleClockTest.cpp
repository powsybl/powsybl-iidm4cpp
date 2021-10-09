/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClock.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TwoWindingsTransformerPhaseAngleClockTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("NHV2_NLOAD");
    POWSYBL_ASSERT_THROW(transformer.newExtension<TwoWindingsTransformerPhaseAngleClockAdder>().add(), PowsyblException, "Undefined value for phaseAngleClock");
    POWSYBL_ASSERT_THROW(transformer.newExtension<TwoWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClock(12UL).add(), PowsyblException, "Unexpected value for phaseAngleClock: 12");

    transformer.newExtension<TwoWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClock(3UL).add();

    auto& twtpac = transformer.getExtension<TwoWindingsTransformerPhaseAngleClock>();
    BOOST_CHECK_EQUAL(3, twtpac.getPhaseAngleClock());
    BOOST_CHECK(stdcxx::areSame(twtpac, twtpac.setPhaseAngleClock(4UL)));
    BOOST_CHECK_EQUAL(4, twtpac.getPhaseAngleClock());
    POWSYBL_ASSERT_THROW(twtpac.setPhaseAngleClock(12), PowsyblException, "Unexpected value for phaseAngleClock: 12");
}

BOOST_FIXTURE_TEST_CASE(TwoWindingsTransformerPhaseAngleClockXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));

    TwoWindingsTransformer& transformer = network.getTwoWindingsTransformer("NHV2_NLOAD");
    transformer.newExtension<TwoWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClock(3).add();

    const std::string& networkStr = ResourceFixture::getResource("twoWindingsTransformerPhaseAngleClock.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
