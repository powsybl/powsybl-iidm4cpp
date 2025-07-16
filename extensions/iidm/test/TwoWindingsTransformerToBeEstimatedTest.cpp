/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimated.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimatedAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TwoWindingsTransformerToBeEstimatedTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NHV2_NLOAD");
    twt.newExtension<TwoWindingsTransformerToBeEstimatedAdder>()
        .withPhaseTapChangerStatus(true)
        .withRatioTapChangerStatus(false)
        .add();

    auto& extension = twt.getExtension<TwoWindingsTransformerToBeEstimated>();

    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger(false)));
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger());

    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger(true)));
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger());
}

BOOST_FIXTURE_TEST_CASE(TwoWindingsTransformerToBeEstimatedXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));

    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NHV2_NLOAD");
    twt.newExtension<TwoWindingsTransformerToBeEstimatedAdder>()
        .withRatioTapChangerStatus(true)
        .withPhaseTapChangerStatus(false)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("twoWindingsTransformerToBeEstimated.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
