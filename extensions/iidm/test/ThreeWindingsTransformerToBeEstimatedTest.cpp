/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimated.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimatedAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ThreeWindingsTransformerToBeEstimatedTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();

    ThreeWindingsTransformer& transformer = network.getThreeWindingsTransformer("3WT");
    transformer.newExtension<ThreeWindingsTransformerToBeEstimatedAdder>()
        .withPhaseTapChanger1Status(true)
        .withPhaseTapChanger2Status(false)
        .withPhaseTapChanger3Status(true)
        .withRatioTapChanger1Status(false)
        .withRatioTapChanger2Status(true)
        .withRatioTapChanger3Status(false)
        .add();

    auto& extension = transformer.getExtension<ThreeWindingsTransformerToBeEstimated>();

    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger1());
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger(ThreeSides::ONE));
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger2());
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger(ThreeSides::TWO));
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger3());
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger(ThreeSides::THREE));
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger1());
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger(ThreeSides::ONE));
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger2());
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger(ThreeSides::TWO));
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger3());
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger(ThreeSides::THREE));

    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger1(false)));
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger1());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger2(true)));
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger2());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger3(false)));
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger3());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger1(true)));
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger1());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger2(false)));
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger2());
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger3(true)));
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger3());

    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger(true, ThreeSides::ONE)));
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger(ThreeSides::ONE));
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger(false, ThreeSides::TWO)));
    BOOST_CHECK(!extension.shouldEstimatePhaseTapChanger(ThreeSides::TWO));
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimatePhaseTapChanger(true, ThreeSides::THREE)));
    BOOST_CHECK(extension.shouldEstimatePhaseTapChanger(ThreeSides::THREE));
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger(false, ThreeSides::ONE)));
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger(ThreeSides::ONE));
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger(true, ThreeSides::TWO)));
    BOOST_CHECK(extension.shouldEstimateRatioTapChanger(ThreeSides::TWO));
    BOOST_CHECK(stdcxx::areSame(extension, extension.shouldEstimateRatioTapChanger(false, ThreeSides::THREE)));
    BOOST_CHECK(!extension.shouldEstimateRatioTapChanger(ThreeSides::THREE));
}

BOOST_FIXTURE_TEST_CASE(ThreeWindingsTransformerToBeEstimatedXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));

    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT");
    twt.newExtension<ThreeWindingsTransformerToBeEstimatedAdder>()
        .withRatioTapChanger2Status(true)
        .withRatioTapChanger3Status(true)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("threeWindingsTransformerToBeEstimated.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
