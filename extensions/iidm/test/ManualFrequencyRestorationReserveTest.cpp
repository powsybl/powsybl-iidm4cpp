/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserve.hpp>
#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserveAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ManualFrequencyRestorationReserveTestSuite)

BOOST_AUTO_TEST_CASE(extensionTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    auto& gen = network.getGenerator("GEN");

    gen.newExtension<ManualFrequencyRestorationReserveAdder>()
        .withParticipate(true)
        .add();
    auto& extension = gen.getExtension<ManualFrequencyRestorationReserve>();

    BOOST_CHECK_EQUAL("manualFrequencyRestorationReserve", extension.getName());
    BOOST_CHECK(extension.isParticipate());
    extension.setParticipate(false);
    BOOST_CHECK(!extension.isParticipate());
}

BOOST_FIXTURE_TEST_CASE(manualFrequencyRestorationReserveXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2022-04-06T13:43:05.020+02:00"));
    Generator& gen = network.getGenerator("GEN");
    gen.newExtension<ManualFrequencyRestorationReserveAdder>()
        .withParticipate(true)
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/manualFrequencyRestorationReserve.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

