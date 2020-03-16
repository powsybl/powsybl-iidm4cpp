/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ThreeWindingsTransformerPhaseAngleClockTestSuite)

BOOST_FIXTURE_TEST_CASE(ThreeWindingsTransformerPhaseAngleClockXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2018-03-05T13:30:30.486+01:00"));

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT");
    transformer.addExtension(Extension::create<ThreeWindingsTransformerPhaseAngleClock>(transformer, 3, 1));

    const std::string& networkStr = ResourceFixture::getResource("threeWindingsTransformerPhaseAngleClock.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl