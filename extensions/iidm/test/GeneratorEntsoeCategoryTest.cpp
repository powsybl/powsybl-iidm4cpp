/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategory.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategoryAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(GeneratorEntsoeCategoryTestSuite)

BOOST_AUTO_TEST_CASE(GeneratorEntsoeCategoryConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    GeneratorEntsoeCategoryAdder gecAdder = generator.newExtension<GeneratorEntsoeCategoryAdder>();
    POWSYBL_ASSERT_THROW(gecAdder.add(), PowsyblException, "Bad generator ENTSO-E code 0");

    gecAdder.withCode(11)
        .add();
    auto& extension = generator.getExtension<GeneratorEntsoeCategory>();
    BOOST_CHECK_EQUAL("entsoeCategory", extension.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL(11, extension.getCode());

    extension.setCode(12);
    BOOST_CHECK_EQUAL(12, extension.getCode());

    POWSYBL_ASSERT_THROW(extension.setCode(0), PowsyblException, "Bad generator ENTSO-E code 0");
    POWSYBL_ASSERT_THROW(GeneratorEntsoeCategory(generator, 0), PowsyblException, "Bad generator ENTSO-E code 0");
    extension.setCode(-1);

}

BOOST_FIXTURE_TEST_CASE(GeneratorEntsoeCategorySerializerTest, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<GeneratorEntsoeCategoryAdder>()
        .withCode(22)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/generatorEntsoeCategoryRef.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
        
    std::istringstream stream(networkStr);
    Network networkRef = Network::readXml("/generatorEntsoeCategoryRef.xml", stream);

    test::converter::RoundTrip::runXml(networkRef, networkStr);

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

