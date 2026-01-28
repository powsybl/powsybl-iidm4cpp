/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/VoltageRegulation.hpp>
#include <powsybl/iidm/extensions/iidm/VoltageRegulationAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(VoltageRegulationTestSuite)

Network createBatteryNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalV(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newBattery()
        .setId("BAT1")
        .setName("BAT1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setTargetP(100.0)
        .setTargetQ(200.0)
        .setMinP(-200.0)
        .setMaxP(300.0)
        .add();

    return network;
}

Network createBatteryWithVoltageRegulationNetwork() {
    Network network = powsybl::network::BatteryNetworkFactory::create();

    Generator& gen = network.getGenerator("GEN");
    Battery& bat = network.getBattery("BAT");
    bat.newExtension<VoltageRegulationAdder>()
            .withVoltageRegulatorOn(true)
            .withTargetV(100.0)
            .add();

    Battery& bat2 = network.getBattery("BAT2");
    bat2.newExtension<VoltageRegulationAdder>()
            .withRegulatingTerminal(stdcxx::ref(gen.getTerminal()))
            .withVoltageRegulatorOn(true)
            .withTargetV(100)
            .add();

    return network;
}

void assertVoltageRegulationExtension(const Network& network) {
    const VoltageRegulation& voltageRegulationXml = network.getBattery("BAT").getExtension<VoltageRegulation>();
    BOOST_CHECK(voltageRegulationXml.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(100.0, voltageRegulationXml.getTargetV(), std::numeric_limits<double>::epsilon());

    const VoltageRegulation& voltageRegulationXml2 = network.getBattery("BAT2").getExtension<VoltageRegulation>();
    BOOST_CHECK(voltageRegulationXml2.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(100.0, voltageRegulationXml2.getTargetV(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_REF_TRUE(voltageRegulationXml2.getRegulatingTerminal());
    BOOST_CHECK(stdcxx::areSame(network.getGenerator("GEN").getTerminal(),voltageRegulationXml2.getRegulatingTerminal().get()));
}

BOOST_AUTO_TEST_CASE(VoltageRegulationConstructor) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    Network network2 = ::powsybl::network::EurostagFactory::createTutorial1Network();

    battery.newExtension<VoltageRegulationAdder>().withVoltageRegulatorOn(true).withTargetV(5.0).add();
    VoltageRegulation& extension = battery.getExtension<VoltageRegulation>();
    BOOST_CHECK_EQUAL("voltageRegulation", extension.getName());
    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));

    BOOST_CHECK(extension.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(5.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    extension.setVoltageRegulatorOn(false);
    BOOST_CHECK(!extension.isVoltageRegulatorOn());

    BOOST_CHECK(extension.getRegulatingTerminal());
    extension.setRegulatingTerminal(stdcxx::ref<Terminal>());
    BOOST_CHECK(stdcxx::areSame(battery.getTerminal(), extension.getRegulatingTerminal().get()));
    extension.setRegulatingTerminal(stdcxx::ref(battery.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(battery.getTerminal(), extension.getRegulatingTerminal().get()));

    extension.setTargetV(6.0);
    BOOST_CHECK_CLOSE(6.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "newState1");
    network.getVariantManager().setWorkingVariant("newState1");

    BOOST_CHECK(!extension.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(6.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    extension.setVoltageRegulatorOn(true);
    extension.setTargetV(5.0);
    BOOST_CHECK(extension.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(5.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!extension.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(6.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    // error cases
    POWSYBL_ASSERT_THROW(extension.setRegulatingTerminal(stdcxx::ref(network2.getLoad("LOAD").getTerminal())), PowsyblException, "regulating terminal is not part of the same network");
}

BOOST_AUTO_TEST_CASE(ThrowVoltageRegulationConstructor) {

    Network network = createBatteryNetwork();
    Battery& bat = network.getBattery("BAT1");

    VoltageRegulationAdder adder = bat.newExtension<VoltageRegulationAdder>()
        .withRegulatingTerminal(stdcxx::ref(bat.getTerminal()))
        .withTargetV(5.0);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Voltage regulator status is not defined");

    Network network2 = createBatteryNetwork();
    VoltageRegulationAdder adder2 = bat.newExtension<VoltageRegulationAdder>()
        .withRegulatingTerminal(stdcxx::ref(network2.getBattery("BAT1").getTerminal()))
        .withTargetV(5.0)
        .withVoltageRegulatorOn(true);

    POWSYBL_ASSERT_THROW(adder2.add(), PowsyblException, "regulating terminal is not part of the same network");
}

BOOST_FIXTURE_TEST_CASE(VoltageRegulationXmlSerializerTest, test::ResourceFixture) {

    Network network = createBatteryWithVoltageRegulationNetwork();

    const std::string& networkStr = ResourceFixture::getResource("voltageRegulationRoundTripRef.xml");
    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    assertVoltageRegulationExtension(network2);

}

BOOST_FIXTURE_TEST_CASE(VoltageRegulationXmlOlderVersionCompatibilityTest, test::ResourceFixture) {

    Network network = createBatteryWithVoltageRegulationNetwork();

    // backward compatibility
    std::string filenameDefault = "voltageRegulationCompatibility-1.1.xml";
    std::string filenameCompatibility = "voltageRegulationCompatibility-1.12.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_12();

    const auto& writer = [&version, &filenameDefault](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        // Default extension version : 1.1
        Network::writeXml(filenameDefault, stream, n, options);
    };
    const auto& writerCompatibility = [&version, &filenameCompatibility](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        // Compatitiblity with extension version 1.12 :
        options.addExtensionVersion("voltageRegulation", "1.12");
        Network::writeXml(filenameCompatibility, stream, n, options);
    };
    const auto& reader = [&filenameDefault](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filenameDefault, stream);
    };
    const auto& readerCompatibility = [&filenameCompatibility](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filenameCompatibility, stream);
    };

    const std::string& expectedDefault = ResourceFixture::getResource(filenameDefault);
    Network networkDefault = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expectedDefault);
    assertVoltageRegulationExtension(networkDefault);

    const std::string& expectedCompatibility = ResourceFixture::getResource(filenameCompatibility);
    Network networkCompatibility = test::converter::RoundTrip::run(network, writerCompatibility, readerCompatibility, test::converter::RoundTrip::compareXml, expectedCompatibility);
    assertVoltageRegulationExtension(networkCompatibility);

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

