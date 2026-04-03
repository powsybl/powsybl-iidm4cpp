/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlAdder.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ActivePowerControlTestSuite)

Network createNetwork() {
    Network network = powsybl::network::BatteryNetworkFactory::create();

    Battery& battery = network.getBattery("BAT");
    battery.newExtension<ActivePowerControlAdder>().withParticipate(true).withDroop(4.0).withParticipationFactor(1.2).add();

    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<ActivePowerControlAdder>().withParticipate(false).withDroop(3.0).withParticipationFactor(1).add();

    return network;
}

void checkValues(ActivePowerControl& apc, bool participate, double droop, double participationFactor, double minTargetP, double maxTargetP) {

    if(participate) {
        BOOST_CHECK(apc.isParticipate());
    } else {
        BOOST_CHECK(!apc.isParticipate());
    }

    BOOST_CHECK_CLOSE(droop, apc.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(participationFactor, apc.getParticipationFactor(), std::numeric_limits<double>::epsilon());

    if(!std::isnan(minTargetP)) {
        BOOST_CHECK_CLOSE(minTargetP, apc.getMinTargetP(), std::numeric_limits<double>::epsilon());
    } else {
        BOOST_CHECK(std::isnan(apc.getMinTargetP()));
    }
    if(!std::isnan(maxTargetP)) {
        BOOST_CHECK_CLOSE(maxTargetP, apc.getMaxTargetP(), std::numeric_limits<double>::epsilon());
    } else {
        BOOST_CHECK(std::isnan(apc.getMaxTargetP()));
    }

}

BOOST_AUTO_TEST_CASE(ActivePowerControlLimitsCheckTest) {

    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& battery = network.getBattery("BAT");
    double minP = battery.getMinP();
    double maxP = battery.getMaxP();

    ActivePowerControlAdder apcAdder = battery.newExtension<ActivePowerControlAdder>().withParticipate(true).withDroop(4.0).withParticipationFactor(1.2);

    apcAdder.withMaxTargetP(maxP+1);
    POWSYBL_ASSERT_THROW(apcAdder.add(), PowsyblException, "maxTargetP value (10001) is not between minP and maxP for component BAT");
    apcAdder.withMaxTargetP(minP-1);
    POWSYBL_ASSERT_THROW(apcAdder.add(), PowsyblException, "maxTargetP value (-10001) is not between minP and maxP for component BAT");
    apcAdder.withMinTargetP(minP-1);
    POWSYBL_ASSERT_THROW(apcAdder.add(), PowsyblException, "minTargetP value (-10001) is not between minP and maxP for component BAT");
    apcAdder.withMinTargetP(maxP+1);
    POWSYBL_ASSERT_THROW(apcAdder.add(), PowsyblException, "minTargetP value (10001) is not between minP and maxP for component BAT");

    apcAdder.withMinTargetP(3000).withMaxTargetP(200.);
    POWSYBL_ASSERT_THROW(apcAdder.add(), PowsyblException, "invalid targetP limits [3000, 200]");

    apcAdder.withMinTargetP(200).withMaxTargetP(3000.);
    apcAdder.add();
    ActivePowerControl& apc = battery.getExtension<ActivePowerControl>();

    POWSYBL_ASSERT_THROW(apc.setMaxTargetP(maxP+1), PowsyblException, "maxTargetP value (10001) is not between minP and maxP for component BAT");
    POWSYBL_ASSERT_THROW(apc.setMinTargetP(minP-1), PowsyblException, "minTargetP value (-10001) is not between minP and maxP for component BAT");
    BOOST_CHECK_CLOSE(200, apc.getMinTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3000, apc.getMaxTargetP(), std::numeric_limits<double>::epsilon());

    //try to fool the extension
    battery.setMinP(500).setMaxP(1000);
    BOOST_CHECK_CLOSE(500, apc.getMinTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1000, apc.getMaxTargetP(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_CASE(ActivePowerControlTest) {
    Network network = createNetwork();

    Generator& generator = network.getGenerator("GEN");
    ActivePowerControl& apcGen = generator.getExtension<ActivePowerControl>();
    BOOST_CHECK_EQUAL("activePowerControl", apcGen.getName());
    BOOST_CHECK_CLOSE(3, apcGen.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1, apcGen.getParticipationFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!apcGen.isParticipate());
    BOOST_CHECK(stdcxx::areSame(generator, apcGen.getExtendable<Generator>().get()));

    Battery& battery = network.getBattery("BAT");
    ActivePowerControl& apcBat = battery.getExtension<ActivePowerControl>();
    BOOST_CHECK_EQUAL("activePowerControl", apcBat.getName());
    BOOST_CHECK_CLOSE(4, apcBat.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.2, apcBat.getParticipationFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(apcBat.isParticipate());
    BOOST_CHECK(stdcxx::areSame(battery, apcBat.getExtendable<Battery>().get()));

    apcGen.setMaxTargetP(800.0);
    apcBat.setMinTargetP(10.0);

    BOOST_CHECK_CLOSE(800.0, apcGen.getMaxTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(apcGen.getMinTargetP()));
    BOOST_CHECK_CLOSE(10.0, apcBat.getMinTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(apcBat.getMaxTargetP()));

}

BOOST_AUTO_TEST_CASE(variantTest) {
    Network network = createNetwork();
    Battery& bat = network.getBattery("BAT");
    ActivePowerControl& activePowerControl = bat.getExtension<ActivePowerControl>();

    // Testing variant cloning
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v1"});
    network.getVariantManager().cloneVariant("v1", "v2");
    network.getVariantManager().setWorkingVariant("v1");
    checkValues(activePowerControl, true, 4.0, 1.2, stdcxx::nan(), stdcxx::nan());

    // Testing setting different values in the cloned variant and going back to the initial one
    activePowerControl.setDroop(6.0);
    activePowerControl.setParticipate(false);
    activePowerControl.setParticipationFactor(3.0);
    checkValues(activePowerControl, false, 6.0, 3.0, stdcxx::nan(), stdcxx::nan());
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    checkValues(activePowerControl, true, 4.0, 1.2, stdcxx::nan(), stdcxx::nan());

    // Removes a variant then adds another variant to test variant recycling (hence calling allocateVariantArrayElement)
    network.getVariantManager().removeVariant("v1");
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v1","v3"});
    network.getVariantManager().setWorkingVariant("v1");
    checkValues(activePowerControl, true, 4.0, 1.2, stdcxx::nan(), stdcxx::nan());
    network.getVariantManager().setWorkingVariant("v2");
    checkValues(activePowerControl, true, 4.0, 1.2, stdcxx::nan(), stdcxx::nan());
    network.getVariantManager().setWorkingVariant("v3");
    checkValues(activePowerControl, true, 4.0, 1.2, stdcxx::nan(), stdcxx::nan());


    // test limitControl
    double minP = bat.getMinP();
    double maxP = bat.getMaxP();
    POWSYBL_ASSERT_THROW(activePowerControl.setMaxTargetP(maxP+1), PowsyblException, "maxTargetP value (10001) is not between minP and maxP for component BAT");
    POWSYBL_ASSERT_THROW(activePowerControl.setMaxTargetP(minP-1), PowsyblException, "maxTargetP value (-10001) is not between minP and maxP for component BAT");
    POWSYBL_ASSERT_THROW(activePowerControl.setMinTargetP(maxP+1), PowsyblException, "minTargetP value (10001) is not between minP and maxP for component BAT");
    POWSYBL_ASSERT_THROW(activePowerControl.setMinTargetP(minP-1), PowsyblException, "minTargetP value (-10001) is not between minP and maxP for component BAT");

    activePowerControl.setMaxTargetP(200);
    activePowerControl.setMinTargetP(100);
    POWSYBL_ASSERT_THROW(activePowerControl.setMinTargetP(201), PowsyblException, "invalid targetP limits [201, 200]");
    POWSYBL_ASSERT_THROW(activePowerControl.setMaxTargetP(99), PowsyblException, "invalid targetP limits [100, 99]");

    // try to fool the extension
    bat.setMaxP(190);
    bat.setMinP(110);
    BOOST_CHECK_CLOSE(190.0, activePowerControl.getMaxTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(110.0, activePowerControl.getMinTargetP(), std::numeric_limits<double>::epsilon());

    // Test removing current variant
    network.getVariantManager().removeVariant("v3");
    POWSYBL_ASSERT_THROW(activePowerControl.getDroop(), PowsyblException, "Variant index not set");
}

BOOST_AUTO_TEST_CASE(variantWithOverrideTest) {
    Network network = createNetwork();
    Battery& bat = network.getBattery("BAT");
    ActivePowerControl& activePowerControl = bat.getExtension<ActivePowerControl>();
    activePowerControl.setMinTargetP(10.0).setMaxTargetP(100.0);

    // Testing variant cloning
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v1"});
    network.getVariantManager().cloneVariant("v1", "v2");
    network.getVariantManager().setWorkingVariant("v1");
    checkValues(activePowerControl, true, 4.0, 1.2, 10.0, 100.0);

    // Testing setting different values in the cloned variant and going back to the initial one
    activePowerControl.setDroop(6.0);
    activePowerControl.setParticipate(false);
    activePowerControl.setParticipationFactor(3.0);
    activePowerControl.setMaxTargetP(110.0).setMinTargetP(stdcxx::nan());
    checkValues(activePowerControl, false, 6.0, 3.0, stdcxx::nan(), 110.0);
    activePowerControl.setMinTargetP(11.0).setMaxTargetP(stdcxx::nan());
    checkValues(activePowerControl, false, 6.0, 3.0, 11.0, stdcxx::nan());
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    checkValues(activePowerControl, true, 4.0, 1.2, 10.0, 100.0);

    // Removes a variant then adds another variant to test variant recycling (hence calling allocateVariantArrayElement)
    network.getVariantManager().removeVariant("v1");
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v1","v3"});
    network.getVariantManager().setWorkingVariant("v1");
    checkValues(activePowerControl, true, 4.0, 1.2, 10.0, 100.0);
    network.getVariantManager().setWorkingVariant("v2");
    checkValues(activePowerControl, true, 4.0, 1.2, 10.0, 100.0);
    network.getVariantManager().setWorkingVariant("v3");
    checkValues(activePowerControl, true, 4.0, 1.2, 10.0, 100.0);

    // Test removing current variant
    network.getVariantManager().removeVariant("v3");
    POWSYBL_ASSERT_THROW(activePowerControl.getDroop(), PowsyblException, "Variant index not set");
}

BOOST_FIXTURE_TEST_CASE(ActivePowerControlXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();
    network.getGenerator("GEN").getExtension<ActivePowerControl>().setMaxTargetP(800.);
    network.getBattery("BAT").getExtension<ActivePowerControl>().setMinTargetP(10.);

    std::string filename = "activePowerControlWithoutLimits.xml";
    const std::string& networkStr = ResourceFixture::getResource("activePowerControlWithoutLimits.xml");
    const converter::xml::IidmXmlVersion& versionLimitsNotSupported = converter::xml::IidmXmlVersion::V1_12();

    const auto& writer = [&versionLimitsNotSupported, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(versionLimitsNotSupported.toString("."));
        Network::writeXml(filename, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };
    Network network2 = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, networkStr);

    Generator& generator = network2.getGenerator("GEN");
    ActivePowerControl& apcGen = generator.getExtension<ActivePowerControl>();
    Battery& battery = network2.getBattery("BAT");
    ActivePowerControl& apcBat = battery.getExtension<ActivePowerControl>();

    BOOST_CHECK(std::isnan(apcGen.getMinTargetP()));
    BOOST_CHECK(std::isnan(apcGen.getMaxTargetP()));
    BOOST_CHECK(std::isnan(apcBat.getMinTargetP()));
    BOOST_CHECK(std::isnan(apcBat.getMaxTargetP()));
}

BOOST_FIXTURE_TEST_CASE(ActivePowerControlXmlWithLimitSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();
    network.getGenerator("GEN").getExtension<ActivePowerControl>().setMaxTargetP(800.);
    network.getBattery("BAT").getExtension<ActivePowerControl>().setMinTargetP(10.);

    std::string filename = "activePowerControlWithLimit.xml";
    const std::string& networkStr = ResourceFixture::getResource("activePowerControlWithLimits.xml");
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION();

    const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        Network::writeXml(filename, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };
    Network network2 = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, networkStr);

    Generator& generator = network2.getGenerator("GEN");
    ActivePowerControl& apcGen = generator.getExtension<ActivePowerControl>();
    Battery& battery = network2.getBattery("BAT");
    ActivePowerControl& apcBat = battery.getExtension<ActivePowerControl>();

    BOOST_CHECK_CLOSE(800.0, apcGen.getMaxTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(apcGen.getMinTargetP()));
    BOOST_CHECK_CLOSE(10.0, apcBat.getMinTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(apcBat.getMaxTargetP()));

}

BOOST_FIXTURE_TEST_CASE(ActivePowerControlXml1_0Test, test::ResourceFixture) {
    Network network = createNetwork();

    std::string filename = "batteryNetworkWithActivePowerControlRoundTripRef.xml";
    const std::string& expected = ResourceFixture::getResource(filename);
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_0();

    const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.setVersion(version.toString("."));
        Network::writeXml(filename, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };
    Network network2 = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expected);

    Battery& battery = network2.getBattery("BAT");
    battery.getExtension<ActivePowerControl>();

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

