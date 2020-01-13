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
#include <powsybl/iidm/extensions/cvg/GeneratorStartup.hpp>
#include <powsybl/iidm/extensions/cvg/GeneratorStartupXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(GeneratorStartupTestSuite)

BOOST_AUTO_TEST_CASE(GeneratorStartupConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    GeneratorStartup extension(generator, 1.0, 2.0, 3.0, 4.0);
    BOOST_CHECK_EQUAL("startup", extension.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL(1.0, extension.getPredefinedActivePowerSetpoint());
    BOOST_CHECK_EQUAL(2.0, extension.getMarginalCost());
    BOOST_CHECK_EQUAL(3.0, extension.getPlannedOutageRate());
    BOOST_CHECK_EQUAL(4.0, extension.getForcedOutageRate());

    extension.setPredefinedActivePowerSetpoint(11.0);
    BOOST_CHECK_CLOSE(11.0, extension.getPredefinedActivePowerSetpoint(), std::numeric_limits<double>::epsilon());

    extension.setMarginalCost(22.0);
    BOOST_CHECK_CLOSE(22.0, extension.getMarginalCost(), std::numeric_limits<double>::epsilon());

    extension.setPlannedOutageRate(33.0);
    BOOST_CHECK_CLOSE(33.0, extension.getPlannedOutageRate(), std::numeric_limits<double>::epsilon());

    extension.setForcedOutageRate(44.0);
    BOOST_CHECK_CLOSE(44.0, extension.getForcedOutageRate(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(GeneratorStartupSerializerTest, ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-01-18T18:45:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.addExtension(Extension::create<GeneratorStartup>(generator, 1.1, 2.2, 3.3, 4.4));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("startup", stdcxx::make_unique<GeneratorStartupXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/generatorStartup.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

