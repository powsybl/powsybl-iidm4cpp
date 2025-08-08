/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservability.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservabilityAdder.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(InjectionObservabilityTestSuite)

BOOST_AUTO_TEST_CASE(injectionObservability) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");
    load.newExtension<InjectionObservabilityAdder>().withObservable(true).add();
    auto& extension = load.getExtension<InjectionObservability>();

    BOOST_CHECK_EQUAL("injectionObservability", extension.getName());
    BOOST_CHECK(extension.isObservable());
    BOOST_CHECK(!extension.getQualityP());
    BOOST_CHECK(!extension.getQualityQ());
    BOOST_CHECK(!extension.getQualityV());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setObservable(false)));
    BOOST_CHECK(!extension.isObservable());

    extension.setQualityP(1.1);
    BOOST_CHECK_CLOSE(1.1, extension.getQualityP().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityP().get().getRedundant().has_value());
    BOOST_CHECK(!extension.getQualityP().get().isRedundant());

    extension.setQualityP(2.2, true);
    BOOST_CHECK_CLOSE(2.2, extension.getQualityP().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityP().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityP().get().isRedundant());

    extension.setQualityQ(3.3);
    BOOST_CHECK_CLOSE(3.3, extension.getQualityQ().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());

    extension.setQualityQ(4.4, true);
    BOOST_CHECK_CLOSE(4.4, extension.getQualityQ().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityQ().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityQ().get().isRedundant());

    extension.setQualityV(5.5);
    BOOST_CHECK_CLOSE(5.5, extension.getQualityV().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());

    extension.setQualityV(6.6, true);
    BOOST_CHECK_CLOSE(6.6, extension.getQualityV().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.getQualityV().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityV().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.getQualityV().get(), extension.getQualityV().get().setRedundant(false)));
    BOOST_CHECK(extension.getQualityV().get().getRedundant().has_value());
    BOOST_CHECK(!extension.getQualityV().get().isRedundant());
    extension.getQualityV().get().setRedundant(true);
    BOOST_CHECK(extension.getQualityV().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityV().get().isRedundant());

    BOOST_CHECK(stdcxx::areSame(extension.getQualityV().get(), extension.getQualityV().get().setStandardDeviation(7.7)));
    BOOST_CHECK_CLOSE(7.7, extension.getQualityV().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    extension.getQualityV().get().setStandardDeviation(8.8);
    BOOST_CHECK_CLOSE(8.8, extension.getQualityV().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");

    load.newExtension<InjectionObservabilityAdder>()
        .withObservable(false)
        .withStandardDeviationP(1.1)
        .withRedundantP(true)
        .withStandardDeviationQ(2.2)
        .withRedundantQ(false)
        .withStandardDeviationV(3.3)
        .withRedundantV(true)
        .add();
    const InjectionObservability& extension = load.getExtension<InjectionObservability>();

    BOOST_CHECK(!extension.isObservable());

    BOOST_CHECK_CLOSE(1.1, extension.getQualityP().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getQualityQ().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.3, extension.getQualityV().get().getStandardDeviation(), std::numeric_limits<double>::epsilon());


    BOOST_CHECK(extension.getQualityQ().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityQ().get().getRedundant().has_value());
    BOOST_CHECK(extension.getQualityV().get().getRedundant().has_value());

    BOOST_CHECK(extension.getQualityP().get().isRedundant());
    BOOST_CHECK(!extension.getQualityQ().get().isRedundant());
    BOOST_CHECK(extension.getQualityV().get().isRedundant());
}

BOOST_FIXTURE_TEST_CASE(InjectionObservabilityXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& bat = network.getBattery("BAT");

    bat.newExtension<InjectionObservabilityAdder>()
        .withObservable(true)
        .withStandardDeviationP(0.03)
        .withRedundantP(false)
        .withStandardDeviationQ(0.6)
        .withRedundantQ(false)
        .add();

    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<InjectionObservabilityAdder>()
        .withObservable(false)
        .withStandardDeviationP(0.02)
        .withRedundantP(true)
        .withStandardDeviationQ(0.5)
        .withRedundantQ(true)
        .withStandardDeviationV(0.0)
        .withRedundantV(true)
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/injectionObservabilityRoundTripRef.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_FIXTURE_TEST_CASE(InjectionObservabilityErrorTest, test::ResourceFixture) {
    const auto& networkPath = ResourceFixture::getResourcePath("/injectionObservabilityRoundTripRefInvalid.xml");

    POWSYBL_ASSERT_THROW(Network::readXml(networkPath), PowsyblException, "Unexpected element: qualityZ");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

