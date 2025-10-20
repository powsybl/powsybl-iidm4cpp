/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>


#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminals.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminalsAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ReferenceTerminalsTestSuite)

BOOST_AUTO_TEST_CASE(ReferenceTerminalsTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Generator& gh1 = network.getGenerator("GH1");
    Generator& gh2 = network.getGenerator("GH2");
    Generator& gh3 = network.getGenerator("GH3");

    POWSYBL_ASSERT_REF_FALSE(network.findExtension<ReferenceTerminals>());

    network.newExtension<ReferenceTerminalsAdder>()
                .withTerminals({stdcxx::ref(gh1.getTerminal())})
                .add();
    POWSYBL_ASSERT_REF_TRUE(network.findExtension<ReferenceTerminals>());
    ReferenceTerminals& ext = network.getExtension<ReferenceTerminals>();
    BOOST_CHECK_EQUAL(1, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));

    // add another one, using static utility method
    ReferenceTerminals::addTerminal(gh2.getTerminal());
    BOOST_CHECK_EQUAL(2, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh2.getTerminal()));
    
    //add already existing and another one using extension method:
    ext.addReferenceTerminal(gh1.getTerminal())
        .addReferenceTerminal(gh2.getTerminal())
        .addReferenceTerminal(gh3.getTerminal());
    BOOST_CHECK_EQUAL(3, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh2.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(2).get(),gh3.getTerminal()));

    // clear
    ReferenceTerminals::reset(network);
    BOOST_CHECK_EQUAL(0, ext.getReferenceTerminals().size());
    POWSYBL_ASSERT_REF_TRUE(network.findExtension<ReferenceTerminals>());
    BOOST_CHECK(ReferenceTerminals::getTerminals(network).empty());

    //remove extension and add a new one
    network.removeExtension<ReferenceTerminals>();
    ReferenceTerminals::addTerminal(gh1.getTerminal());
    BOOST_CHECK_EQUAL(1, ReferenceTerminals::getTerminals(network).size());
    BOOST_CHECK(stdcxx::areSame(ReferenceTerminals::getTerminals(network).front().get(), gh1.getTerminal()));
}

BOOST_AUTO_TEST_CASE(ReferenceTerminalsVariantsTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Generator& gh1 = network.getGenerator("GH1");
    Generator& gh2 = network.getGenerator("GH2");
    Generator& gh3 = network.getGenerator("GH3");

    network.newExtension<ReferenceTerminalsAdder>()
                .withTerminals({stdcxx::ref(gh1.getTerminal())})
                .add();
    ReferenceTerminals& ext = network.getExtension<ReferenceTerminals>();

    //Add Variants:
    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "variant1";
    std::string variant2 = "variant2";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2});

    // add gh2 to variant1
    variantManager.setWorkingVariant(variant1);
    ext.addReferenceTerminal(gh2.getTerminal());
    // add gh3 to variant2
    variantManager.setWorkingVariant(variant2);
    ext.addReferenceTerminal(gh3.getTerminal());

    // initial variant unmodified
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL(1, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));

    // check variant 1 as expected
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK_EQUAL(2, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh2.getTerminal()));

    // check variant 2 as expected
    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK_EQUAL(2, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh3.getTerminal()));

    // clear variant 1
    variantManager.setWorkingVariant(variant1);
    ext.reset();
    // check variant 1 empty
    BOOST_CHECK_EQUAL(0, ext.getReferenceTerminals().size());

    // check other variants unchanged
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK_EQUAL(2, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh3.getTerminal()));

    // test variant recycling in array
    std::string variant3 = "variant3";
    variantManager.removeVariant(variant1);
    variantManager.cloneVariant(variant2, variant3);
    variantManager.setWorkingVariant(variant3);
    BOOST_CHECK_EQUAL(2, ext.getReferenceTerminals().size());
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(0).get(),gh1.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(ext.getReferenceTerminals().at(1).get(),gh3.getTerminal()));

    // test array resize for coverage completeness
    variantManager.removeVariant(variant2);
}

BOOST_AUTO_TEST_CASE(ReferenceTerminalsWrongNetworkTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Network other = powsybl::network::EurostagFactory::createTutorial1Network();

    Terminal& terminal = other.getBusBreakerView().getBus("NHV1").get().getConnectedTerminals().front();
    ReferenceTerminalsAdder adder = network.newExtension<ReferenceTerminalsAdder>();
    adder.withTerminals({stdcxx::ref(terminal)});
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Terminal given is not in the right Network (sim1 instead of fourSubstations)");
    POWSYBL_ASSERT_REF_FALSE(network.findExtension<ReferenceTerminals>());
    
    network.newExtension<ReferenceTerminalsAdder>().add();
    ReferenceTerminals& ext = network.getExtension<ReferenceTerminals>();
    POWSYBL_ASSERT_THROW(ext.addReferenceTerminal(terminal);, PowsyblException, "Terminal given is not in the right Network (sim1 instead of fourSubstations)");
}

BOOST_FIXTURE_TEST_CASE(ReferenceTerminalsXmlSerializerTest, test::ResourceFixture) {
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
    vl.getBusBreakerView().newBus().setId("B1").add();
    vl.getBusBreakerView().newBus().setId("B2").add();
    Generator& gen = vl.newGenerator()
                  .setId("G")
                  .setBus("B1")
                  .setConnectableBus("B1")
                  .setTargetP(100)
                  .setTargetQ(100)
                  .setTargetV(400)
                  .setMinP(0)
                  .setMaxP(200)
                  .setVoltageRegulatorOn(true)
                  .add();
    Line& line = network.newLine()
                   .setId("L12")
                   .setVoltageLevel1("VL")
                   .setBus1("B1")
                   .setVoltageLevel2("VL")
                   .setBus2("B2")
                   .setR(0)
                   .setX(1)
                   .setB1(0)
                   .setB2(0)
                   .setG1(0)
                   .setG2(0)
                   .add();

    ReferenceTerminals::addTerminal(gen.getTerminal());
    ReferenceTerminals::addTerminal(line.getTerminal1());

    const std::string& networkStrRef = ResourceFixture::getResource("referenceTerminalsRef.xml");
    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
