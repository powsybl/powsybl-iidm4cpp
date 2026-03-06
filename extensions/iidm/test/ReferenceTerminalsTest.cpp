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
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh2.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh3.getTerminal().getReferrers().size());

    //Add Variants:
    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "variant1";
    std::string variant2 = "variant2";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2});

    // add gh2 to variant1
    variantManager.setWorkingVariant(variant1);
    ext.addReferenceTerminal(gh2.getTerminal());
    BOOST_CHECK_EQUAL(1, gh2.getTerminal().getReferrers().size());
    // add gh3 to variant2
    variantManager.setWorkingVariant(variant2);
    ext.addReferenceTerminal(gh3.getTerminal());
    BOOST_CHECK_EQUAL(1, gh3.getTerminal().getReferrers().size());

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
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh2.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(1, gh3.getTerminal().getReferrers().size());

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
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh2.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(1, gh3.getTerminal().getReferrers().size());

    // test array resize for coverage completeness
    variantManager.removeVariant(variant2);
    BOOST_CHECK_EQUAL(1, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh2.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(1, gh3.getTerminal().getReferrers().size());

    //remove extension  unregisters terminals correctly:
    network.removeExtension<ReferenceTerminals>();
    BOOST_CHECK_EQUAL(0, gh1.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh2.getTerminal().getReferrers().size());
    BOOST_CHECK_EQUAL(0, gh3.getTerminal().getReferrers().size());

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


BOOST_AUTO_TEST_CASE(ReferenceTerminalsWithSubnetworks) {
    Network network("Root", "format0");
    Network& subnetwork1 = network.newSubnetwork("Sub1", "format1");
    Network& subnetwork2 = network.newSubnetwork("Sub2", "format2");

    Substation& s1 = subnetwork1.newSubstation()
            .setId("S1")
            .add();
    Substation& s2 = subnetwork1.newSubstation()
            .setId("S2")
            .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
                          .setId("VL1")
                          .setNominalV(400)
                          .setTopologyKind(TopologyKind::BUS_BREAKER)
                          .add();
    vl1.getBusBreakerView().newBus().setId("B1").add();
    vl1.getBusBreakerView().newBus().setId("B2").add();
    Generator& gen1 = vl1.newGenerator()
                  .setId("G1")
                  .setBus("B1")
                  .setConnectableBus("B1")
                  .setTargetP(100)
                  .setTargetQ(100)
                  .setTargetV(400)
                  .setMinP(0)
                  .setMaxP(200)
                  .setVoltageRegulatorOn(true)
                  .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
                          .setId("VL2")
                          .setNominalV(400)
                          .setTopologyKind(TopologyKind::BUS_BREAKER)
                          .add();
    vl2.getBusBreakerView().newBus().setId("B3").add();
    vl2.getBusBreakerView().newBus().setId("B4").add();
    Generator& gen2 = vl2.newGenerator()
                  .setId("G2")
                  .setBus("B3")
                  .setConnectableBus("B3")
                  .setTargetP(100)
                  .setTargetQ(100)
                  .setTargetV(400)
                  .setMinP(0)
                  .setMaxP(200)
                  .setVoltageRegulatorOn(true)
                  .add();

    Terminal& g1Terminal = gen1.getTerminal();
    Terminal& g2Terminal = gen2.getTerminal();

    // gen1 is in subnetwork1 and can be added to subnetwork extension
        subnetwork1.newExtension<ReferenceTerminalsAdder>()
                .withTerminals({stdcxx::ref(g1Terminal)})
                .add();
    // gen2 is in subnetwork1 and can be added to root network extension
        network.newExtension<ReferenceTerminalsAdder>()
                .withTerminals({stdcxx::ref(g2Terminal)})
                .add();

    //subnetwork only has gen1 extension
        ReferenceTerminals& extSubnetwork = subnetwork1.getExtension<ReferenceTerminals>();
        BOOST_CHECK_EQUAL(1, extSubnetwork.getReferenceTerminals().size());
        BOOST_CHECK(stdcxx::areSame(extSubnetwork.getReferenceTerminals().at(0).get(),gen1.getTerminal()));
    //rootnetwork only has gen2 extension
        ReferenceTerminals& extNetwork = network.getExtension<ReferenceTerminals>();
        BOOST_CHECK_EQUAL(1, extNetwork.getReferenceTerminals().size());
        BOOST_CHECK(stdcxx::areSame(extNetwork.getReferenceTerminals().at(0).get(),gen2.getTerminal()));

    //we can reset everything from root network:
        ReferenceTerminals::reset(network);
        BOOST_CHECK(ReferenceTerminals::getTerminals(network).empty());
        BOOST_CHECK(ReferenceTerminals::getTerminals(subnetwork1).empty());

    // And can easily add on the root network with this method
        ReferenceTerminals::addTerminal(g1Terminal);
        BOOST_CHECK_EQUAL(1, extNetwork.getReferenceTerminals().size());
        BOOST_CHECK_EQUAL(0, extSubnetwork.getReferenceTerminals().size()); //not added into the subnetwork
        //same using the static methods from ReferenceTerminals
        BOOST_CHECK_EQUAL(1, ReferenceTerminals::getTerminals(network).size());
        BOOST_CHECK_EQUAL(0, ReferenceTerminals::getTerminals(subnetwork1).size());

    //can also add into the subnetwork
        extSubnetwork.addReferenceTerminal(g1Terminal);
        BOOST_CHECK_EQUAL(1, extNetwork.getReferenceTerminals().size());
        BOOST_CHECK_EQUAL(1, extSubnetwork.getReferenceTerminals().size());
        BOOST_CHECK(stdcxx::areSame(extNetwork.getReferenceTerminals().at(0).get(),extSubnetwork.getReferenceTerminals().at(0).get()));

    //can't add terminal from one subnetwork into another subnetwork:
        auto adder = subnetwork2.newExtension<ReferenceTerminalsAdder>()
                .withTerminals({stdcxx::ref(g1Terminal)});
        POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Terminal given is not in the right Network (Sub1 instead of Sub2)");
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
