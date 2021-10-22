/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockAdder.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ThreeWindingsTransformerPhaseAngleClockTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT");
    POWSYBL_ASSERT_THROW(transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().add(), PowsyblException, "Undefined value for phaseAngleClockLeg2");
    POWSYBL_ASSERT_THROW(transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(12UL).add(), PowsyblException, "Undefined value for phaseAngleClockLeg3");
    POWSYBL_ASSERT_THROW(transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(12UL).withPhaseAngleClockLeg3(0UL).add(), PowsyblException, "Unexpected value for phaseAngleClock: 12");
    POWSYBL_ASSERT_THROW(transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(0UL).withPhaseAngleClockLeg3(12UL).add(), PowsyblException, "Unexpected value for phaseAngleClock: 12");

    transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(2UL).withPhaseAngleClockLeg3(3UL).add();

    auto& threewtpac = transformer.getExtension<ThreeWindingsTransformerPhaseAngleClock>();

    BOOST_CHECK_EQUAL(2, threewtpac.getPhaseAngleClockLeg2());
    BOOST_CHECK(stdcxx::areSame(threewtpac, threewtpac.setPhaseAngleClockLeg2(4UL)));
    BOOST_CHECK_EQUAL(4, threewtpac.getPhaseAngleClockLeg2());
    POWSYBL_ASSERT_THROW(threewtpac.setPhaseAngleClockLeg2(12), PowsyblException, "Unexpected value for phaseAngleClock: 12");

    BOOST_CHECK_EQUAL(3, threewtpac.getPhaseAngleClockLeg3());
    BOOST_CHECK(stdcxx::areSame(threewtpac, threewtpac.setPhaseAngleClockLeg3(6UL)));
    BOOST_CHECK_EQUAL(6, threewtpac.getPhaseAngleClockLeg3());
    POWSYBL_ASSERT_THROW(threewtpac.setPhaseAngleClockLeg3(12), PowsyblException, "Unexpected value for phaseAngleClock: 12");
}

BOOST_FIXTURE_TEST_CASE(ThreeWindingsTransformerPhaseAngleClockXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2018-03-05T13:30:30.486+01:00"));

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT");
    transformer.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(3).withPhaseAngleClockLeg3(1).add();

    const std::string& networkStr = ResourceFixture::getResource("threeWindingsTransformerPhaseAngleClock.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl