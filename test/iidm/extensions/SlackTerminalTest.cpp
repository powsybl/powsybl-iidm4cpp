/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
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

namespace powsybl {

namespace iidm {

namespace extensions {

Network createBusBreakerNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
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
    vl.newLoad()
        .setId("L")
        .setBus("B")
        .setConnectableBus("B")
        .setP0(100)
        .setQ0(50)
        .add();

    network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s.newVoltageLevel()
        .setId("VL1")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    vl1.newGenerator()
        .setId("GE")
        .setBus("B1")
        .setConnectableBus("B1")
        .setTargetP(100)
        .setMinP(0)
        .setMaxP(110)
        .setTargetV(380)
        .setVoltageRegulatorOn(true)
        .add();

    network.newLine()
        .setId("LI")
        .setR(0.05)
        .setX(1.)
        .setG1(0.)
        .setG2(0.)
        .setB1(0.)
        .setB2(0.)
        .setVoltageLevel1("VL")
        .setVoltageLevel2("VL1")
        .setBus1("B")
        .setBus2("B1")
        .add();

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

    // Testing that only current variant was set
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref<Terminal>());
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref(tGen));
    BOOST_CHECK(stdcxx::areSame(stGen.getTerminal().get(), tGen));
    stGen.setTerminal(stdcxx::cref<Terminal>());
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::cref(tGen));
    BOOST_CHECK(stdcxx::areSame(stGen.getTerminal().get(), tGen));

    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(!stGen.getTerminal());
    stGen.setTerminal(stdcxx::ref(tGen));

    // Testing the cleanable property of the slackTerminal
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());

    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());

    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK(stGen.setTerminal(stdcxx::ref<Terminal>()).isEmpty());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref(tGen)).isEmpty());

    // Testing the cleanIfEmpty boolean
    stGen.setTerminal(stdcxx::ref<Terminal>(), false);
    BOOST_CHECK_NO_THROW(vlgen.getExtension<SlackTerminal>());
    BOOST_CHECK(!stGen.setTerminal(stdcxx::ref(tGen)).isEmpty());
    BOOST_CHECK_NO_THROW(vlgen.getExtension<SlackTerminal>());
    stGen.setTerminal(stdcxx::cref<Terminal>(), false);
    BOOST_CHECK_NO_THROW(vlgen.getExtension<SlackTerminal>());
    stGen.setTerminal(stdcxx::ref<Terminal>(), true);
    POWSYBL_ASSERT_THROW(vlgen.getExtension<SlackTerminal>(), PowsyblException, "Extension powsybl::iidm::extensions::SlackTerminal not found");

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
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
