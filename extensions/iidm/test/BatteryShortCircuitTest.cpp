/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/BatteryShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/BatteryShortCircuitAdder.hpp>

#include <powsybl/network/BatteryNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(BatteryShortCircuitTestSuite)

BOOST_AUTO_TEST_CASE(batteryShortCircuit) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& bat = network.getBattery("BAT");

    bat.newExtension<BatteryShortCircuitAdder>().withDirectSubtransX(1.0).withDirectTransX(2.0).withStepUpTransformerX(3.0).add();
    auto& extension = bat.getExtension<BatteryShortCircuit>();

    BOOST_CHECK_EQUAL("batteryShortCircuit", extension.getName());
    BOOST_CHECK_CLOSE(1.0, extension.getDirectSubtransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setDirectSubtransX(11.1)));
    BOOST_CHECK_CLOSE(11.1, extension.getDirectSubtransX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setDirectTransX(22.2)));
    BOOST_CHECK_CLOSE(22.2, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setStepUpTransformerX(33.3)));
    BOOST_CHECK_CLOSE(33.3, extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());


    extension.setDirectSubtransX(stdcxx::nan());
    BOOST_CHECK(std::isnan(extension.getDirectSubtransX()));
    POWSYBL_ASSERT_THROW(extension.setDirectTransX(stdcxx::nan()), PowsyblException, "Undefined directTransX");

}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& bat = network.getBattery("BAT");

    auto adder = bat.newExtension<BatteryShortCircuitAdder>();
    auto addernan = bat.newExtension<BatteryShortCircuitAdder>().withDirectTransX(stdcxx::nan());

    POWSYBL_ASSERT_THROW(addernan.add(), PowsyblException, "Undefined directTransX");
    adder.add();

    auto& extension = bat.getExtension<BatteryShortCircuit>();
    BOOST_CHECK(std::isnan(extension.getDirectSubtransX()));
    BOOST_CHECK(std::isnan(extension.getStepUpTransformerX()));
    BOOST_CHECK_CLOSE(0.0, extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(extension.setDirectTransX(stdcxx::nan()), PowsyblException, "Undefined directTransX");
}

BOOST_FIXTURE_TEST_CASE(BatteryShortCircuitXmlRoundTripTest, test::ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    Battery& bat = network.getBattery("BAT");
    bat.newExtension<BatteryShortCircuitAdder>().withDirectSubtransX(2.0).withDirectTransX(1.0).withStepUpTransformerX(3.0).add();
    auto& extension = bat.getExtension<BatteryShortCircuit>();

    const std::string& networkStrRef = ResourceFixture::getResource("/batteryShortCircuitRef_1.0.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStrRef);
    
    auto& extension2 = network2.getBattery("BAT").getExtension<BatteryShortCircuit>();
    BOOST_CHECK_CLOSE(extension2.getDirectTransX(), extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extension2.getDirectSubtransX(), extension.getDirectSubtransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extension2.getStepUpTransformerX(), extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(BatteryShortCircuitXmlSerializerLegacyTest, test::ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    Battery& bat = network.getBattery("BAT");
    bat.newExtension<BatteryShortCircuitAdder>().withDirectSubtransX(2.0).withDirectTransX(1.0).withStepUpTransformerX(3.0).add();
    auto& extension = bat.getExtension<BatteryShortCircuit>();

    std::string filenameLegacy= "batteryShortCircuitRef_1.0-legacy.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_13();

    const auto& writer = [&version, &filenameLegacy, &extension](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        // Use an extension version which both serialization name and extension version are not the default
        options.addExtensionVersion(extension.getName(), "1.0-legacy"); //Implies using associated alternative serialization name
        options.setVersion(version.toString("."));
        Network::writeXml(filenameLegacy, stream, n, options);
    };
    const auto& reader = [&filenameLegacy](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filenameLegacy, stream);
    };

    const std::string& expectedLegacy = ResourceFixture::getResource(filenameLegacy);
    Network networkLegacy = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expectedLegacy);
    
    auto& extensionLegacy = networkLegacy.getBattery("BAT").getExtension<BatteryShortCircuit>();
    BOOST_CHECK_CLOSE(extensionLegacy.getDirectTransX(), extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extensionLegacy.getStepUpTransformerX(), extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(extensionLegacy.getDirectSubtransX())); // This attribute is not exported in legacy
}

BOOST_FIXTURE_TEST_CASE(BatteryShortCircuitXmlSerializerLegacy2Test, test::ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    Battery& bat = network.getBattery("BAT");
    bat.newExtension<BatteryShortCircuitAdder>().withDirectSubtransX(2.0).withDirectTransX(1.0).withStepUpTransformerX(3.0).add();
    auto& extension = bat.getExtension<BatteryShortCircuit>();

    std::string filenameLegacy= "batteryShortCircuitRef_1.0-legacy-2.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_13();

    const auto& writer = [&version, &filenameLegacy, &extension](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        // Use an extension version which both serialization name and extension version are not the default
        options.addExtensionVersion(extension.getName(), "1.0-legacy-2"); //Implies using associated alternative serialization name
        options.setVersion(version.toString("."));
        Network::writeXml(filenameLegacy, stream, n, options);
    };
    const auto& reader = [&filenameLegacy](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filenameLegacy, stream);
    };

    const std::string& expectedLegacy = ResourceFixture::getResource(filenameLegacy);
    Network networkLegacy = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expectedLegacy);
    
    auto& extensionLegacy = networkLegacy.getBattery("BAT").getExtension<BatteryShortCircuit>();
    BOOST_CHECK_CLOSE(extensionLegacy.getDirectTransX(), extension.getDirectTransX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extensionLegacy.getStepUpTransformerX(), extension.getStepUpTransformerX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(extensionLegacy.getDirectSubtransX())); // This attribute is not exported in legacy
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
