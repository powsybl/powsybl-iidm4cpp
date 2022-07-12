/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuitAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(GeneratorShortCircuitTestSuite)

BOOST_AUTO_TEST_CASE(generatorShortCircuit) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    generator.newExtension<GeneratorShortCircuitAdder>().withDirectSubtransX(1.1).withDirectTransX(2.2).withStepUpTransformerX(3.3).add();
    auto& extension = generator.getExtension<GeneratorShortCircuit>();

    BOOST_CHECK_CLOSE(1.1, extension.getDirectSubtransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.3, extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setDirectSubtransX(11.1)));
    BOOST_CHECK_CLOSE(11.1, extension.getDirectSubtransX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setDirectTransX(22.2)));
    BOOST_CHECK_CLOSE(22.2, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setStepUpTransformerX(33.3)));
    BOOST_CHECK_CLOSE(33.3, extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    auto adder = generator.newExtension<GeneratorShortCircuitAdder>();
    adder.withDirectSubtransX(stdcxx::nan())
        .withDirectTransX(stdcxx::nan())
        .withStepUpTransformerX(stdcxx::nan());

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Undefined directTransX");
    adder.withDirectTransX(0.0).add();

    auto& extension = generator.getExtension<GeneratorShortCircuit>();
    BOOST_CHECK(std::isnan(extension.getDirectSubtransX()));
    BOOST_CHECK_CLOSE(0.0, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(extension.getStepUpTransformerX()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
