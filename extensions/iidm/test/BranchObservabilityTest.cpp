/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/iidm/BranchObservability.hpp>
#include <powsybl/iidm/extensions/iidm/BranchObservabilityAdder.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(BranchObservabilityTestSuite)

BOOST_AUTO_TEST_CASE(branchObservability) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Line& line = network.getLine("NHV1_NHV2_1");

    line.newExtension<BranchObservabilityAdder>().withObservable(true).add();
    auto& extension = line.getExtension<BranchObservability>();

    BOOST_CHECK_EQUAL("branchObservability", extension.getName());
    BOOST_CHECK(extension.isObservable());
    BOOST_CHECK(!extension.getQualityP1());
    BOOST_CHECK(!extension.getQualityP2());
    BOOST_CHECK(!extension.getQualityQ1());
    BOOST_CHECK(!extension.getQualityQ2());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setObservable(false)));
    BOOST_CHECK(!extension.isObservable());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityP1(1.1), extension));
    BOOST_CHECK_CLOSE(1.1, extension.getQualityP1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityP1().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityP1(2.2, true), extension));
    BOOST_CHECK_CLOSE(2.2, extension.getQualityP1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityP1().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityP2(3.3), extension));
    BOOST_CHECK_CLOSE(3.3, extension.getQualityP2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityP2().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityP2(4.4, true), extension));
    BOOST_CHECK_CLOSE(4.4, extension.getQualityP2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityP2().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityQ1(5.5), extension));
    BOOST_CHECK_CLOSE(5.5, extension.getQualityQ1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityQ1().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityQ1(6.6, true), extension));
    BOOST_CHECK_CLOSE(6.6, extension.getQualityQ1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityQ1().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityQ2(7.7), extension));
    BOOST_CHECK_CLOSE(7.7, extension.getQualityQ2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityQ2().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.setQualityQ2(8.8, true), extension));
    BOOST_CHECK_CLOSE(8.8, extension.getQualityQ2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityQ2().get().isRedundant());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Line& line = network.getLine("NHV1_NHV2_1");

    line.newExtension<BranchObservabilityAdder>().withObservable(true).add();
    const BranchObservability& emptyBo = line.getExtension<BranchObservability>();
    BOOST_CHECK(!emptyBo.getQualityP1());
    BOOST_CHECK(!emptyBo.getQualityP2());
    BOOST_CHECK(!emptyBo.getQualityQ1());
    BOOST_CHECK(!emptyBo.getQualityQ2());

    line.newExtension<BranchObservabilityAdder>()
        .withObservable(true)
        .withStandardDeviationP1(1.1)
        .withRedundantP1(true)
        .withStandardDeviationP2(2.2)
        .withRedundantP2(false)
        .withStandardDeviationQ1(3.3)
        .withRedundantQ1(true)
        .withStandardDeviationQ2(4.4)
        .withRedundantQ2(false)
        .add();
    const BranchObservability& extension = line.getExtension<BranchObservability>();

    BOOST_CHECK(extension.isObservable());

    BOOST_CHECK_CLOSE(1.1, extension.getQualityP1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityP1().get().isRedundant());

    BOOST_CHECK_CLOSE(2.2, extension.getQualityP2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityP2().get().isRedundant());

    BOOST_CHECK_CLOSE(3.3, extension.getQualityQ1().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityQ1().get().isRedundant());

    BOOST_CHECK_CLOSE(4.4, extension.getQualityQ2().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getQualityQ2().get().isRedundant());
}

BOOST_FIXTURE_TEST_CASE(InjectionObservabilityXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Line& line1 = network.getLine("NHV1_NHV2_1");

    line1.newExtension<BranchObservabilityAdder>()
        .withObservable(true)
        .withStandardDeviationP1(0.03)
        .withRedundantP1(false)
        .withStandardDeviationP2(0.6)
        .withRedundantP2(false)
        .withStandardDeviationQ1(0.1)
        .withRedundantQ1(false)
        .withStandardDeviationQ2(0.04)
        .withRedundantQ2(true)
        .add();

    Line& line2 = network.getLine("NHV1_NHV2_2");
    line2.newExtension<BranchObservabilityAdder>()
        .withObservable(false)
        .withStandardDeviationP1(0.1)
        .withRedundantP1(true)
        .withStandardDeviationP2(0.2)
        .withRedundantP2(true)
        .withStandardDeviationQ1(0.3)
        .withRedundantQ1(true)
        .withStandardDeviationQ2(0.4)
        .withRedundantQ2(false)
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/branchObservabilityRoundTripRef.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_FIXTURE_TEST_CASE(InjectionObservabilityErrorTest, test::ResourceFixture) {
    const auto& networkPath = ResourceFixture::getResourcePath("/branchObservabilityRoundTripRefInvalid.xml");

    POWSYBL_ASSERT_THROW(Network::readXml(networkPath), PowsyblException, "Unexpected element: qualityV");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

