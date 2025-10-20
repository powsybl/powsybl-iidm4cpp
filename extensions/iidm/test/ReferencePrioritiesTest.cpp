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
#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePrioritiesAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriority.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorityAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ReferencePrioritiesTestSuite)

BOOST_AUTO_TEST_CASE(ReferencePriorityTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    BusbarSection& bbs1 = network.getBusbarSection("S1VL1_BBS");
    BusbarSection& bbs2 = network.getBusbarSection("S2VL1_BBS");
    BusbarSection& bbs3 = network.getBusbarSection("S3VL1_BBS");
    Generator& gh1 = network.getGenerator("GH1");
    Generator& gh2 = network.getGenerator("GH2");
    Generator& gh3 = network.getGenerator("GH3");
    Load& ld1 = network.getLoad("LD1");
    Load& ld2 = network.getLoad("LD2");
    Load& ld3 = network.getLoad("LD3");
    Line& lineS2S3 = network.getLine("LINE_S2S3");
    Line& lineS3S4 = network.getLine("LINE_S3S4");

    ReferencePriority::set(bbs1, 5);
    ReferencePriority::set(bbs1, 3); //Replace previous ReferencePriority
    ReferencePriority::set(bbs2, 8);
    ReferencePriority::set(bbs3, 9);
    ReferencePriority::set(gh1, 1);
    ReferencePriority::set(gh2, 2);
    ReferencePriority::set(gh3, 3);
    ReferencePriority::set(ld1, 2);
    ReferencePriority::set(ld2, 5);
    ReferencePriority::set(ld3, 6);
    ReferencePriority::set(lineS2S3, TwoSides::ONE, 3);
    ReferencePriority::set(lineS2S3, TwoSides::TWO, 0);

    BOOST_CHECK_EQUAL(3, ReferencePriority::get(bbs1));
    BOOST_CHECK_EQUAL(8, ReferencePriority::get(bbs2));
    BOOST_CHECK_EQUAL(9, ReferencePriority::get(bbs3));
    BOOST_CHECK_EQUAL(1, ReferencePriority::get(gh1));
    BOOST_CHECK_EQUAL(2, ReferencePriority::get(gh2));
    BOOST_CHECK_EQUAL(3, ReferencePriority::get(gh3));
    BOOST_CHECK_EQUAL(2, ReferencePriority::get(ld1));
    BOOST_CHECK_EQUAL(5, ReferencePriority::get(ld2));
    BOOST_CHECK_EQUAL(6, ReferencePriority::get(ld3));
    BOOST_CHECK_EQUAL(3, ReferencePriority::get(lineS2S3, TwoSides::ONE));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(lineS2S3, TwoSides::TWO));

    POWSYBL_ASSERT_REF_FALSE(lineS3S4.findExtension<ReferencePriorities>());

    std::vector<std::shared_ptr<ReferencePriority>> referencePriorities = ReferencePriorities::get(network);
    BOOST_CHECK_EQUAL(10, referencePriorities.size());
    BOOST_CHECK(stdcxx::areSame(gh1.getTerminal(), referencePriorities.at(0)->getTerminal()));       // p1
    BOOST_CHECK(stdcxx::areSame(gh2.getTerminal(), referencePriorities.at(1)->getTerminal()));       // p2 - gen
    BOOST_CHECK(stdcxx::areSame(ld1.getTerminal(), referencePriorities.at(2)->getTerminal()));       // p2 - load
    BOOST_CHECK(stdcxx::areSame(gh3.getTerminal(), referencePriorities.at(3)->getTerminal()));       // p3 - gen
    BOOST_CHECK(stdcxx::areSame(bbs1.getTerminal(), referencePriorities.at(4)->getTerminal()));      // p3 - bbs
    BOOST_CHECK(stdcxx::areSame(lineS2S3.getTerminal1(), referencePriorities.at(5)->getTerminal())); // p3 - line
    BOOST_CHECK(stdcxx::areSame(ld2.getTerminal(), referencePriorities.at(6)->getTerminal()));       // p5
    BOOST_CHECK(stdcxx::areSame(ld3.getTerminal(), referencePriorities.at(7)->getTerminal()));       // p6
    BOOST_CHECK(stdcxx::areSame(bbs2.getTerminal(), referencePriorities.at(8)->getTerminal()));      // p8
    BOOST_CHECK(stdcxx::areSame(bbs3.getTerminal(), referencePriorities.at(9)->getTerminal()));      // p9

    //Add Variants:
    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "variant1";
    std::string variant2 = "variant2";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2});

    // add gh2 priority to 20 in variant1
    variantManager.setWorkingVariant(variant1);
    ReferencePriority::set(gh2, 20);
    // add gh3 priority to 0 in variant2
    variantManager.setWorkingVariant(variant2);
    ReferencePriority::set(gh3, 0);

    // initial variant unmodified
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL(2, ReferencePriority::get(gh2));
    BOOST_CHECK_EQUAL(3, ReferencePriority::get(gh3));
    // check variant 1 as expected
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK_EQUAL(20, ReferencePriority::get(gh2));
    // check variant 2 as expected
    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(gh3));

    // clear variant 1
    variantManager.setWorkingVariant(variant1);
    ReferencePriorities::deleteReferencePriorities(network);
    // check variant 1 empty
    BOOST_CHECK(ReferencePriorities::get(network).empty());
    // check other variants unchanged
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL(10, ReferencePriorities::get(network).size());
    variantManager.setWorkingVariant(variant2);
    BOOST_CHECK_EQUAL(9, ReferencePriorities::get(network).size());

    // test variant recycling in array
    std::string variant3 = "variant3";
    variantManager.removeVariant(variant1);
    variantManager.cloneVariant(variant2, variant3);
    variantManager.setWorkingVariant(variant3);
    BOOST_CHECK_EQUAL(9, ReferencePriorities::get(network).size());
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(gh3));

    // test array resize
    variantManager.removeVariant(variant2);

    //Delete all reference priorities
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    referencePriorities = ReferencePriorities::get(network);
    BOOST_CHECK_EQUAL(10, referencePriorities.size());

    ReferencePriorities::deleteReferencePriorities(network);
    BOOST_CHECK_EQUAL(0, ReferencePriorities::get(network).size());
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(bbs1));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(gh1));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(ld1));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(lineS2S3, TwoSides::ONE));
}

BOOST_AUTO_TEST_CASE(ReferencePriorityThreeWindingsTransformerTest) {
    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::create();

    ThreeWindingsTransformer& t3wf = network.getThreeWindingsTransformer("3WT");
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(t3wf, ThreeSides::ONE));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(t3wf, ThreeSides::TWO));
    BOOST_CHECK_EQUAL(0, ReferencePriority::get(t3wf, ThreeSides::THREE));

    ReferencePriority::set(t3wf, ThreeSides::ONE, 4);
    ReferencePriority::set(t3wf, ThreeSides::TWO, 5);
    ReferencePriority::set(t3wf, ThreeSides::THREE, 6);

    BOOST_CHECK_EQUAL(4, ReferencePriority::get(t3wf, ThreeSides::ONE));
    BOOST_CHECK_EQUAL(5, ReferencePriority::get(t3wf, ThreeSides::TWO));
    BOOST_CHECK_EQUAL(6, ReferencePriority::get(t3wf, ThreeSides::THREE));
}

BOOST_AUTO_TEST_CASE(ReferencePriorityNoTerminalTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Line& lineS3S4 = network.getLine("LINE_S3S4");

    lineS3S4.newExtension<ReferencePrioritiesAdder>().add();
    ReferencePriorityAdder adder = lineS3S4.getExtension<ReferencePriorities>().newReferencePriority();
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Undefined terminal for reference priority.");
}

BOOST_AUTO_TEST_CASE(ReferencePriorityTerminalNotInConnectableTest) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Line& lineS3S4 = network.getLine("LINE_S3S4");
    Generator& gh1 = network.getGenerator("GH1");

    lineS3S4.newExtension<ReferencePrioritiesAdder>().add();
    ReferencePriorityAdder adder = lineS3S4.getExtension<ReferencePriorities>().newReferencePriority().setTerminal(gh1.getTerminal());
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The provided terminal does not belong to this connectable");
}

BOOST_FIXTURE_TEST_CASE(ReferencePrioritiesXmlSerializerTest, test::ResourceFixture) {
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

    ReferencePriority::set(gen, 1);
    ReferencePriority::set(line, TwoSides::ONE, 2);
    ReferencePriority::set(line, TwoSides::TWO, 0);

    const std::string& networkStrRef = ResourceFixture::getResource("referencePrioritiesRef.xml");
    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
