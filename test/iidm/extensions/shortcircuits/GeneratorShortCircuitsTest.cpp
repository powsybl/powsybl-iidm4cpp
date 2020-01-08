/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/shortcircuits/GeneratorShortCircuits.hpp>
#include <powsybl/iidm/extensions/shortcircuits/GeneratorShortCircuitsXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

BOOST_AUTO_TEST_SUITE(GeneratorShortCircuitsTestSuite)

BOOST_AUTO_TEST_CASE(GeneratorShortCircuitsConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    GeneratorShortCircuits extension(generator, 0.1, 0.2);
    BOOST_CHECK_EQUAL("generatorShortCircuits", extension.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(0.1, extension.getTransientReactance(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, extension.getStepUpTransformerReactance(), std::numeric_limits<double>::epsilon());

    extension.setTransientReactance(0.3);
    extension.setStepUpTransformerReactance(0.4);
    BOOST_CHECK_CLOSE(0.3, extension.getTransientReactance(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, extension.getStepUpTransformerReactance(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setTransientReactance(stdcxx::nan()), PowsyblException, "Reactance is not defined.");
    POWSYBL_ASSERT_THROW(extension.setTransientReactance(-1.0), PowsyblException, "Reactance may not be negative.");
}

BOOST_FIXTURE_TEST_CASE(GeneratorShortCircuitsXmlSerializerTest, ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.addExtension(Extension::create<GeneratorShortCircuits>(generator, 1.1, 2.2));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("generatorShortCircuits", stdcxx::make_unique<GeneratorShortCircuitsXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/shortcircuits/generatorShortCircuits.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

