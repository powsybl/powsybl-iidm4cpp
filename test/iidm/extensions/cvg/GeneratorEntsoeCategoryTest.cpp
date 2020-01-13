/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/GeneratorEntsoeCategory.hpp>
#include <powsybl/iidm/extensions/cvg/GeneratorEntsoeCategoryXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(GeneratorEntsoeCategoryTestSuite)

BOOST_AUTO_TEST_CASE(GeneratorEntsoeCategoryConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    GeneratorEntsoeCategory extension(generator, 11);
    BOOST_CHECK_EQUAL("entsoeCategory", extension.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL(11, extension.getCode());

    extension.setCode(12);
    BOOST_CHECK_EQUAL(12, extension.getCode());

    POWSYBL_ASSERT_THROW(extension.setCode(0), PowsyblException, "Bad generator ENTSO-E code 0");
    POWSYBL_ASSERT_THROW(GeneratorEntsoeCategory(generator, 0), PowsyblException, "Bad generator ENTSO-E code 0");
}

BOOST_FIXTURE_TEST_CASE(GeneratorEntsoeCategorySerializerTest, ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.addExtension(Extension::create<GeneratorEntsoeCategory>(generator, 22));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("entsoeCategory", stdcxx::make_unique<GeneratorEntsoeCategoryXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/generatorEntsoeCategory.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

