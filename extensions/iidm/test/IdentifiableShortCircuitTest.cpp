/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuitAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(IdentifiableShortCircuitTestSuite)

BOOST_AUTO_TEST_CASE(identifiableShortCircuit) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    generator.newExtension<IdentifiableShortCircuitAdder>().withIpMin(1.1).withIpMax(2.2).add();
    auto& extension = generator.getExtension<IdentifiableShortCircuit>();

    BOOST_CHECK_CLOSE(1.1, extension.getIpMin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getIpMax(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setIpMin(11.1)));
    BOOST_CHECK_CLOSE(11.1, extension.getIpMin(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setIpMax(stdcxx::nan()), PowsyblException, "Undefined ipMax");
    BOOST_CHECK(stdcxx::areSame(extension, extension.setIpMax(22.2)));
    BOOST_CHECK_CLOSE(22.2, extension.getIpMax(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Identifiable& Identifiable = network.getIdentifiable("GEN");

    auto adder = Identifiable.newExtension<IdentifiableShortCircuitAdder>();
    adder.withIpMin(stdcxx::nan())
        .withIpMax(stdcxx::nan());

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Undefined ipMax");
    adder.withIpMax(1.0).add();

    auto& extension = Identifiable.getExtension<IdentifiableShortCircuit>();
    BOOST_CHECK(std::isnan(extension.getIpMin()));
    BOOST_CHECK_CLOSE(1.0, extension.getIpMax(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(IdentifiableShortCircuitXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    VoltageLevel& vlhv1 = network.getVoltageLevel("VLHV1");
    vlhv1.newExtension<IdentifiableShortCircuitAdder>()
        .withIpMin(500)
        .withIpMax(1500)
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/voltageLevelShortCircuitRef.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
