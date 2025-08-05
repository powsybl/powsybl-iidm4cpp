/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

#include <powsybl/iidm/extensions/iidm/GeneratorStartup.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorStartupAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(GeneratorStartupTestSuite)

BOOST_AUTO_TEST_CASE(GeneratorStartupConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& generator = network.getGenerator("GEN");

    generator.newExtension<GeneratorStartupAdder>()
        .withPredefinedActivePowerSetpoint(1.0)
        .withStartupCost(5.0)
        .withMarginalCost(2.0)
        .withPlannedOutageRate(3.0)
        .withForcedOutageRate(4.0)
        .add();

    auto& extension = generator.getExtension<GeneratorStartup>();
    BOOST_CHECK_EQUAL("startup", extension.getName());
    BOOST_CHECK(stdcxx::areSame(generator, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(1.0, extension.getPredefinedActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, extension.getStartupCost(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, extension.getMarginalCost(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, extension.getPlannedOutageRate(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, extension.getForcedOutageRate(), std::numeric_limits<double>::epsilon());

    extension.setPredefinedActivePowerSetpoint(11.0);
    BOOST_CHECK_CLOSE(11.0, extension.getPredefinedActivePowerSetpoint(), std::numeric_limits<double>::epsilon());

    extension.setMarginalCost(22.0);
    BOOST_CHECK_CLOSE(22.0, extension.getMarginalCost(), std::numeric_limits<double>::epsilon());

    extension.setPlannedOutageRate(33.0);
    BOOST_CHECK_CLOSE(33.0, extension.getPlannedOutageRate(), std::numeric_limits<double>::epsilon());

    extension.setForcedOutageRate(44.0);
    BOOST_CHECK_CLOSE(44.0, extension.getForcedOutageRate(), std::numeric_limits<double>::epsilon());

    extension.setStartupCost(55.0);
    BOOST_CHECK_CLOSE(55.0, extension.getStartupCost(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(GeneratorStartupSerializerTest, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-01-18T18:45:00.000+01:00"));
    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<GeneratorStartupAdder>()
        .withPredefinedActivePowerSetpoint(1.1)
        .withMarginalCost(2.2)
        .withPlannedOutageRate(3.3)
        .withForcedOutageRate(4.4)
        .withStartupCost(5.5)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/generatorStartup.xml");
    test::converter::RoundTrip::runXml(network, networkStr);

    std::istringstream stream(networkStr);
    Network networkRef = Network::readXml("/generatorStartup.xml", stream);

    test::converter::RoundTrip::runXml(networkRef, networkStr);

}


BOOST_FIXTURE_TEST_CASE(GeneratorStartupSerializerBackwardCompatibility, test::ResourceFixture) {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58+02:00"));
    Substation& s = network.newSubstation()
                .setId("S")
                .setCountry(Country::FR)
                .add();
    VoltageLevel& vl = s.newVoltageLevel()
                .setId("VL")
                .setNominalV(400)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();
    vl.getBusBreakerView().newBus()
                .setId("B")
                .add();
    vl.newGenerator()
                .setId("G")
                .setBus("B")
                .setConnectableBus("B")
                .setTargetP(100)
                .setTargetV(380)
                .setVoltageRegulatorOn(true)
                .setMaxP(100)
                .setMinP(0)
                .add();

    //extends generator:
    Generator& generator = network.getGenerator("G");

    generator.newExtension<GeneratorStartupAdder>()
        .withPredefinedActivePowerSetpoint(90)
        .withStartupCost(5)
        .withMarginalCost(10)
        .withPlannedOutageRate(0.8)
        .withForcedOutageRate(0.7)
        .add();

    // backward compatibility
    std::string filename = "generatorStartupRef-1.0.xml";
    std::string filenameTesla = "generatorStartupRef-1.0-itesla.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION();

    const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        options.addExtensionVersion("startup", "1.0");
        Network::writeXml(filename, stream, n, options);
    };
    const auto& writerTesla = [&version, &filenameTesla](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        options.addExtensionVersion("startup", "1.0-itesla");
        Network::writeXml(filenameTesla, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };
    const auto& readerTesla = [&filenameTesla](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filenameTesla, stream);
    };

    const std::string& expected = ResourceFixture::getResource(filename);
    test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expected);

    const std::string& expectedTesla = ResourceFixture::getResource(filenameTesla);
    test::converter::RoundTrip::run(network, writerTesla, readerTesla, test::converter::RoundTrip::compareXml, expectedTesla);

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

