/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/SlackTerminal.hpp>
#include <powsybl/iidm/extensions/SlackTerminalAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

void fillBusBreakerNetwork(Network& network, const std::string& subnetworkId) {
    Substation& s = network.newSubstation()
        .setId(subnetworkId + "S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId(subnetworkId + "VL")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl.getBusBreakerView().newBus()
        .setId(subnetworkId + "B")
        .add();
    vl.newLoad()
        .setId(subnetworkId + "L")
        .setBus(subnetworkId + "B")
        .setConnectableBus(subnetworkId + "B")
        .setP0(100)
        .setQ0(50)
        .add();

    network.newSubstation()
        .setId(subnetworkId + "S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s.newVoltageLevel()
        .setId(subnetworkId + "VL1")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId(subnetworkId + "B1")
        .add();
    vl1.newGenerator()
        .setId(subnetworkId + "GE")
        .setBus(subnetworkId + "B1")
        .setConnectableBus(subnetworkId + "B1")
        .setTargetP(100)
        .setMinP(0)
        .setMaxP(110)
        .setTargetV(380)
        .setVoltageRegulatorOn(true)
        .add();

    network.newLine()
        .setId(subnetworkId + "LI")
        .setR(0.05)
        .setX(1.)
        .setG1(0.)
        .setG2(0.)
        .setB1(0.)
        .setB2(0.)
        .setVoltageLevel1(subnetworkId + "VL")
        .setVoltageLevel2(subnetworkId + "VL1")
        .setBus1(subnetworkId + "B")
        .setBus2(subnetworkId + "B1")
        .add();
}

Network createBusBreakerNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    fillBusBreakerNetwork(network, "");
    return network;
}

BOOST_AUTO_TEST_SUITE(SlackTerminalTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createBusBreakerNetwork();
    VoltageLevel& vl0 = network.getVoltageLevel("VL");
    SlackTerminalAdder adder = vl0.newExtension<SlackTerminalAdder>();

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Terminal needs to be set to create a SlackTerminal extension");

    // Defining the slackTerminal on the first Terminal
    const std::string& busBusBreakerId = "B";
    const Terminal& terminal = *boost::begin(network.getBusBreakerView().getBus(busBusBreakerId).get().getConnectedTerminals());
    adder.withTerminal(terminal).add();

    for (VoltageLevel& vl : network.getVoltageLevels()) {
        const auto& slackTerminal = vl.findExtension<SlackTerminal>();
        if (slackTerminal) {
            BOOST_CHECK_EQUAL(busBusBreakerId, slackTerminal.get().getTerminal().get().getBusBreakerView().getBus().get().getId());
            BOOST_CHECK_EQUAL("VL_0", slackTerminal.get().getTerminal().get().getBusView().getBus().get().getId());
        }
    }
}

BOOST_AUTO_TEST_CASE(variantsCloneTest) {
    std::string variant1 = "variant1";
    std::string variant2 = "variant2";
    std::string variant3 = "variant3";

    // Creates the extension
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vl = network.getVoltageLevel("VLLOAD");
    SlackTerminal::attach(network.getBusBreakerView().getBus("NLOAD"));
    auto& slackTerminal = vl.getExtension<SlackTerminal>();
    Terminal& t0 = slackTerminal.getTerminal();

    // Testing variant cloning
    VariantManager& variantManager = network.getVariantManager();
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), variant1);
    variantManager.cloneVariant(variant1, variant2);
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(stdcxx::areSame(t0, slackTerminal.getTerminal().get()));

    // Removes a variant then adds another variant to test variant recycling (hence calling allocateVariantArrayElement)
    variantManager.removeVariant(variant1);
    std::initializer_list<std::string> targetVariantIds = {variant1, variant3};
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), targetVariantIds);
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(stdcxx::areSame(t0, slackTerminal.getTerminal().get()));
    variantManager.setWorkingVariant(variant3);
    BOOST_CHECK(stdcxx::areSame(t0, slackTerminal.getTerminal().get()));

    // Test removing current variant
    variantManager.removeVariant(variant3);
    POWSYBL_ASSERT_THROW(slackTerminal.getTerminal(), PowsyblException, "Variant index not set");
}

BOOST_AUTO_TEST_CASE(vlErrorTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vl = network.getVoltageLevel("VLHV1");

    // Adding a terminal in the wrong voltage level
    Terminal& wrongTerminal = *network.getBusBreakerView().getBus("NLOAD").get().getConnectedTerminals().begin();
    SlackTerminalAdder slackTerminalAdder = vl.newExtension<SlackTerminalAdder>().withTerminal(wrongTerminal);
    POWSYBL_ASSERT_THROW(slackTerminalAdder.add(), PowsyblException, "Terminal given is not in the right VoltageLevel (VLLOAD instead of VLHV1)");

    // First adding a terminal in the right voltage level...
    Terminal& terminal = *boost::begin(network.getBusBreakerView().getBus("NHV1").get().getConnectedTerminals());
    slackTerminalAdder.withTerminal(terminal).add();
    auto& slackTerminal = vl.getExtension<SlackTerminal>();

    // ... then setting a terminal in the wrong voltage level
    Terminal& wrongTerminal2 = *boost::begin(network.getBusBreakerView().getBus("NHV2").get().getConnectedTerminals());
    POWSYBL_ASSERT_THROW(slackTerminal.setTerminal(stdcxx::ref(wrongTerminal2), true), PowsyblException, "Terminal given is not in the right VoltageLevel (VLHV2 instead of VLHV1)");
}

BOOST_AUTO_TEST_CASE(variantsResetTest) {
    const std::string& variant1 = "variant1";
    const std::string& variant2 = "variant2";

    // Creates 2 variants before creating the extension
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VariantManager& variantManager = network.getVariantManager();
    std::initializer_list<std::string> targetVariantIds = {variant1, variant2};
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), targetVariantIds);
    variantManager.setWorkingVariant(variant2);

    // Creates the extension
    VoltageLevel& vlgen = network.getVoltageLevel("VLGEN");
    SlackTerminal::attach(network.getBusBreakerView().getBus("NGEN"));
    auto& stGen = vlgen.getExtension<SlackTerminal>();
    Terminal& tGen = stGen.getTerminal();
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    // Testing that only current variant was set
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref<Terminal>());
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref(tGen));
    BOOST_CHECK(stdcxx::areSame(stGen.getTerminal().get(), tGen));
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref(tGen));
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    // Testing the cleanable property of the slackTerminal
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());
    BOOST_CHECK_EQUAL(0, tGen.getReferrers().size());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref(tGen)).isEmpty());
    BOOST_CHECK_EQUAL(1, tGen.getReferrers().size());

    // Testing the cleanIfEmpty boolean
    stGen.setTerminal(stdcxx::ref<Terminal>(), false);
    BOOST_CHECK_NO_THROW(vlgen.getExtension<SlackTerminal>());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref(tGen)).isEmpty());
    BOOST_CHECK_NO_THROW(vlgen.getExtension<SlackTerminal>());
    stGen.setTerminal(stdcxx::ref<Terminal>(), true);
    POWSYBL_ASSERT_THROW(vlgen.getExtension<SlackTerminal>(), PowsyblException, "Extension powsybl::iidm::extensions::SlackTerminal not found");
    BOOST_CHECK_EQUAL(0, tGen.getReferrers().size());

    // Creates an extension on another voltageLevel
    VoltageLevel& vlhv1 = network.getVoltageLevel("VLLOAD");
    SlackTerminal::attach(network.getBusBreakerView().getBus("NLOAD"));
    auto& stLoad = vlhv1.getExtension<SlackTerminal>();
    BOOST_CHECK_EQUAL("NLOAD", stLoad.getTerminal().get().getBusBreakerView().getBus().get().getId());
    BOOST_CHECK(!stLoad.isEmpty());

    // Reset the SlackTerminal of VLGEN voltageLevel to its previous value
    SlackTerminal::reset(vlgen, stdcxx::ref(tGen));
    auto& stGen2 = vlgen.getExtension<SlackTerminal>();
    BOOST_CHECK(stdcxx::areSame(tGen, stGen2.getTerminal().get()));
    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK_NO_THROW(stGen2.getTerminal());

    // Removes all SlackTerminals from network
    variantManager.setWorkingVariant(variant1);
    SlackTerminal::reset(network);
    POWSYBL_ASSERT_THROW(vlgen.getExtension<SlackTerminal>(), PowsyblException, "Extension powsybl::iidm::extensions::SlackTerminal not found");
    POWSYBL_ASSERT_THROW(vlhv1.getExtension<SlackTerminal>(), PowsyblException, "Extension powsybl::iidm::extensions::SlackTerminal not found");
    BOOST_CHECK_EQUAL(0, tGen.getReferrers().size());
}

BOOST_AUTO_TEST_CASE(removeTerminal) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    //Add a second generator:
    VoltageLevel& vlgen = network.getVoltageLevel("VLGEN");
    Bus& ngen = vlgen.getBusBreakerView().getBus("NGEN").get();
    Generator& generator2 = vlgen.newGenerator()
                .setId("GEN2")
                .setBus(ngen.getId())
                .setConnectableBus(ngen.getId())
                .setMinP(-9999.99)
                .setMaxP(9999.99)
                .setVoltageRegulatorOn(true)
                .setTargetV(24.5)
                .setTargetP(607.0)
                .setTargetQ(301.0)
                .add();
    generator2.newReactiveCapabilityCurve()
                .beginPoint()
                .setP(3.0)
                .setMaxQ(5.0)
                .setMinQ(4.0)
                .endPoint()
                .beginPoint()
                .setP(0.0)
                .setMaxQ(7.0)
                .setMinQ(6.0)
                .endPoint()
                .beginPoint()
                .setP(1.0)
                .setMaxQ(5.0)
                .setMinQ(4.0)
                .endPoint()
                .add();

    vlgen.newExtension<SlackTerminalAdder>().withTerminal(generator2.getTerminal()).add();

    auto& slackTerminal = vlgen.getExtension<SlackTerminal>();
    POWSYBL_ASSERT_REF_TRUE( slackTerminal.getTerminal() );
    BOOST_CHECK_EQUAL(1, generator2.getTerminal().getReferrers().size());

    generator2.remove();
    POWSYBL_ASSERT_REF_FALSE(vlgen.findExtension<SlackTerminal>());

}

BOOST_AUTO_TEST_CASE(testWithSubnetworks) {
    Network network("test", "test");
    Network& subnetwork1 = network.newSubnetwork("subnetwork1","1");
    Network& subnetwork2 = network.newSubnetwork("subnetwork2","2");
    fillBusBreakerNetwork(subnetwork1, "1_");
    fillBusBreakerNetwork(subnetwork2, "2_");

    SlackTerminal::attach(subnetwork1.getBusBreakerView().getBus("1_B"));
    SlackTerminal::attach(subnetwork2.getBusBreakerView().getBus("2_B"));
    BOOST_CHECK_NO_THROW(network.getVoltageLevel("1_VL").getExtension<SlackTerminal>());
    BOOST_CHECK_NO_THROW(network.getVoltageLevel("2_VL").getExtension<SlackTerminal>());

    //we can reset everything from root network:
    SlackTerminal::reset(network);
    POWSYBL_ASSERT_REF_FALSE(network.getVoltageLevel("1_VL").findExtension<SlackTerminal>());
    POWSYBL_ASSERT_REF_FALSE(network.getVoltageLevel("2_VL").findExtension<SlackTerminal>());

    //Or reset only a subnetwork:
    SlackTerminal::attach(subnetwork1.getBusBreakerView().getBus("1_B"));
    SlackTerminal::attach(subnetwork2.getBusBreakerView().getBus("2_B"));
    SlackTerminal::reset(subnetwork1);
    POWSYBL_ASSERT_REF_FALSE(network.getVoltageLevel("1_VL").findExtension<SlackTerminal>());
    POWSYBL_ASSERT_REF_TRUE(network.getVoltageLevel("2_VL").findExtension<SlackTerminal>());
}

BOOST_FIXTURE_TEST_CASE(SlackTerminalXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));
    VoltageLevel& voltageLevel = network.getVoltageLevel("VLHV2");
    Bus& bus = network.getBusBreakerView().getBus("NHV2");
    Terminal& terminal = *bus.getConnectedTerminals().begin();

    voltageLevel.newExtension<SlackTerminalAdder>().withTerminal(terminal).add();

    const std::string& networkStr = test::converter::RoundTrip::getVersionedNetwork("slackTerminal.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_CASE(UnsupportedExtensionVersion) {

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));
    VoltageLevel& voltageLevel = network.getVoltageLevel("VLHV2");
    Bus& bus = network.getBusBreakerView().getBus("NHV2");
    Terminal& terminal = *bus.getConnectedTerminals().begin();

    voltageLevel.newExtension<SlackTerminalAdder>().withTerminal(terminal).add();

    powsybl::iidm::converter::ExportOptions options = powsybl::iidm::converter::ExportOptions()
        .setVersion(converter::xml::IidmXmlVersion::V1_2().toString("."))
        .setThrowExceptionIfExtensionNotFound(true);

    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };

    std::stringstream buffer;
    POWSYBL_ASSERT_THROW(writer(network, buffer),PowsyblException, "Version 1.2 does not support slackTerminal extension");
    buffer.str("");
    
    const std::string& networkRef = test::converter::RoundTrip::getVersionedNetwork("extensionTooRecentExportTest.xml", converter::xml::IidmXmlVersion::V1_2());
    options.setThrowExceptionIfExtensionNotFound(false);
    writer(network, buffer);
    test::converter::RoundTrip::compareXml(networkRef, buffer.str());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
