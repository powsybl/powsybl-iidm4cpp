/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
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
    gecAdder.add();

    auto& extension0 = generator.getExtension<GeneratorEntsoeCategory>();
    BOOST_CHECK_EQUAL("entsoeCategory", extension0.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension0.getExtendable().get()));
    BOOST_CHECK_EQUAL(0UL, extension0.getCode());

    gecAdder.withCode(11)
        .add();
    auto& extension11 = generator.getExtension<GeneratorEntsoeCategory>();
    BOOST_CHECK_EQUAL("entsoeCategory", extension11.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension11.getExtendable().get()));
    BOOST_CHECK_EQUAL(11, extension11.getCode());

    extension11.setCode(12);
    BOOST_CHECK_EQUAL(12, extension11.getCode());
    extension11.setCode(0);
    BOOST_CHECK_EQUAL(0UL, extension11.getCode());
    
    GeneratorEntsoeCategory newGenerator(generator, 0);
    BOOST_CHECK_EQUAL(0UL, newGenerator.getCode());
    //negative code values are not possible by implementation on an unsigned long: 
    newGenerator.setCode(-1);
    BOOST_CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), newGenerator.getCode());

}

BOOST_FIXTURE_TEST_CASE(GeneratorEntsoeCategorySerializerTest, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<GeneratorEntsoeCategoryAdder>()
        .withCode(22)
        .add();

    Generator& generator2 = network.getVoltageLevel("VLGEN").newGenerator()
        .setId("GEN2")
        .setBus("NGEN")
        .setConnectableBus("NGEN")
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();
    generator2.newMinMaxReactiveLimits()
        .setMinQ(-9999.99)
        .setMaxQ(9999.99)
        .add();
    generator2.newExtension<GeneratorEntsoeCategoryAdder>()
        .withCode(0UL)
        .add();

    //Check with latest version
    const std::string& networkStr = ResourceFixture::getResource("/generatorEntsoeCategoryRef.xml");
    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    POWSYBL_ASSERT_REF_TRUE(network2.getGenerator("GEN").findExtension<GeneratorEntsoeCategory>());
    BOOST_CHECK_EQUAL(22UL, network2.getGenerator("GEN").getExtension<GeneratorEntsoeCategory>().getCode());
    POWSYBL_ASSERT_REF_TRUE(network2.getGenerator("GEN2").findExtension<GeneratorEntsoeCategory>());
    BOOST_CHECK_EQUAL(0UL, network2.getGenerator("GEN2").getExtension<GeneratorEntsoeCategory>().getCode());

    //Check backward compatibility
    // code 0 are not supported in extension version 1.0
    std::string filename1_0 = "generatorEntsoeCategoryRef_1.0.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_15();

    const auto& readerCompatibility = [&filename1_0](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename1_0, stream);
    };
    const auto& writerCompatibility = [&version, &filename1_0](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        options.addExtensionVersion("entsoeCategory", "1.0");
        Network::writeXml(filename1_0, stream, n, options);
    };

    const std::string& expected1_0 = ResourceFixture::getResource(filename1_0);
    Network networkExtension1_0 = test::converter::RoundTrip::run(network, writerCompatibility, readerCompatibility, test::converter::RoundTrip::compareXml, expected1_0);

    POWSYBL_ASSERT_REF_TRUE(networkExtension1_0.getGenerator("GEN").findExtension<GeneratorEntsoeCategory>());
    BOOST_CHECK_EQUAL(22UL, networkExtension1_0.getGenerator("GEN").getExtension<GeneratorEntsoeCategory>().getCode());
    POWSYBL_ASSERT_REF_FALSE(networkExtension1_0.getGenerator("GEN2").findExtension<GeneratorEntsoeCategory>());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

