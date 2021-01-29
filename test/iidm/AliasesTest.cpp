/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/EnergySource.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

static Network createNetwork() {
    Network network("network", "test");
    Substation& substation1 = network.newSubstation()
        .setId("substation1")
        .setCountry(Country::FR)
        .setTso("TSO1")
        .setGeographicalTags({"region1"})
        .add();
    VoltageLevel& voltageLevel1 = substation1.newVoltageLevel()
        .setId("voltageLevel1")
        .setNominalVoltage(400)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    VoltageLevel::NodeBreakerView& topology1 = voltageLevel1.getNodeBreakerView();
    BusbarSection& voltageLevel1BusbarSection1 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection1")
        .setNode(0)
        .add();
    BusbarSection& voltageLevel1BusbarSection2 = topology1.newBusbarSection()
        .setId("voltageLevel1BusbarSection2")
        .setNode(1)
        .add();
    topology1.newBreaker()
        .setId("voltageLevel1Breaker1")
        .setRetained(true)
        .setOpen(false)
        .setNode1(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    Load& load1 = voltageLevel1.newLoad()
        .setId("load1")
        .setNode(2)
        .setP0(10)
        .setQ0(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Disconnector1")
        .setOpen(false)
        .setNode1(load1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(3)
        .add();
    topology1.newDisconnector()
        .setId("load1Breaker1")
        .setOpen(false)
        .setNode1(3)
        .setNode2(voltageLevel1BusbarSection1.getTerminal().getNodeBreakerView().getNode())
        .add();
    Generator& generator1 = voltageLevel1.newGenerator()
        .setId("generator1")
        .setEnergySource(EnergySource::NUCLEAR)
        .setMinP(200.0)
        .setMaxP(900.0)
        .setVoltageRegulatorOn(true)
        .setTargetP(900.0)
        .setTargetV(380.0)
        .setNode(5)
        .add();
    generator1.newReactiveCapabilityCurve()
        .beginPoint().setP(200.0).setMinQ(300.0).setMaxQ(500.0).endPoint()
        .beginPoint().setP(900.0).setMinQ(300.0).setMaxQ(500.0).endPoint()
        .add();
    topology1.newDisconnector()
        .setId("generator1Disconnector1")
        .setOpen(false)
        .setNode1(generator1.getTerminal().getNodeBreakerView().getNode())
        .setNode2(6)
        .add();
    topology1.newDisconnector()
        .setId("generator1Breaker1")
        .setOpen(false)
        .setNode1(6)
        .setNode2(voltageLevel1BusbarSection2.getTerminal().getNodeBreakerView().getNode())
        .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(AliasesTestSuite)

BOOST_AUTO_TEST_CASE(canAddAliases) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    BOOST_CHECK(load.getAliases().empty());
    load.addAlias("Load alias");

    const auto& aliases = load.getAliases();
    BOOST_CHECK_EQUAL(1, boost::size(aliases));
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "Load alias") != aliases.end());
    BOOST_CHECK_NO_THROW(network.getLoad("Load alias"));
    BOOST_CHECK(stdcxx::areSame(network.getLoad("Load alias"), load));
    BOOST_CHECK(network.getLoad("load1").getAliasType("Load alias").empty());
}

BOOST_AUTO_TEST_CASE(canAddAliasesWithTypes) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    BOOST_CHECK(load.getAliases().empty());
    BOOST_CHECK(!load.getAliasFromType("alias type"));
    load.addAlias("Load alias", "alias type");

    POWSYBL_ASSERT_THROW(load.addAlias("Load alias", "alias type"), PowsyblException, "load1 already has an alias of type alias type");

    load.addAlias("Load alias");

    const auto& aliases = load.getAliases();
    BOOST_CHECK_EQUAL(1, boost::size(aliases));
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "Load alias") != aliases.end());
    BOOST_CHECK(load.getAliasFromType("alias type"));
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "Load alias") != aliases.end());
    BOOST_CHECK_EQUAL("Load alias", *load.getAliasFromType("alias type"));
    BOOST_CHECK_NO_THROW(network.getLoad("Load alias"));
    BOOST_CHECK(stdcxx::areSame(load, network.getLoad("Load alias")));
    BOOST_CHECK_EQUAL("alias type", load.getAliasType("Load alias"));

    load.addAlias("Another alias2");
    BOOST_CHECK_EQUAL(2, boost::size(load.getAliases()));
    load.removeAlias("Another alias2");
    BOOST_CHECK(load.getAliasFromType("alias type"));
}

BOOST_AUTO_TEST_CASE(canRemoveAliases) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    BOOST_CHECK(!load.hasAliases());
    BOOST_CHECK(load.getAliases().empty());
    load.addAlias("Load alias");

    const auto& aliases = load.getAliases();
    BOOST_CHECK_EQUAL(1, boost::size(aliases));
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "Load alias") != aliases.end());

    load.removeAlias("Load alias");
    BOOST_CHECK(!load.hasAliases());
    BOOST_CHECK(load.getAliases().empty());
}

BOOST_AUTO_TEST_CASE(silentlyIgnoreAffectingObjectsIdAsAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    BOOST_CHECK(load.getAliases().empty());
    load.addAlias("load1");
    BOOST_CHECK(load.getAliases().empty());
    BOOST_CHECK(stdcxx::areSame(load, network.getLoad("load1")));
}

BOOST_AUTO_TEST_CASE(silentlyIgnoreAffectingTwiceSameIdToAnObject) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    BOOST_CHECK(load.getAliases().empty());
    load.addAlias("Load alias");
    load.addAlias("Load alias");

    const auto& aliases = load.getAliases();
    BOOST_CHECK_EQUAL(1, boost::size(aliases));
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "Load alias") != aliases.end());
}

BOOST_AUTO_TEST_CASE(ensureAliasUnicityFromId) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    load.addAlias("generator1", true);
    const auto& aliases = load.getAliases();
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "generator1") == aliases.end());
    BOOST_CHECK(std::find(aliases.begin(), aliases.end(), "generator1#0") != aliases.end());
}

BOOST_AUTO_TEST_CASE(ensureAliasUnicityFromAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    Generator& generator = network.getGenerator("generator1");

    load.addAlias("alias");
    generator.addAlias("alias", true);

    const auto& loadAliases = load.getAliases();
    BOOST_CHECK(std::find(loadAliases.begin(), loadAliases.end(), "alias") != loadAliases.end());

    const auto& generatorAliases = generator.getAliases();
    BOOST_CHECK(std::find(generatorAliases.begin(), generatorAliases.end(), "alias") == generatorAliases.end());
    BOOST_CHECK(std::find(generatorAliases.begin(), generatorAliases.end(), "alias#0") != generatorAliases.end());
}

BOOST_AUTO_TEST_CASE(failWhenDuplicatedAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    Generator& generator = network.getGenerator("generator1");
    load.addAlias("Alias");
    POWSYBL_ASSERT_THROW(generator.addAlias("Alias"), PowsyblException, "Object (powsybl::iidm::Generator) with alias 'Alias' cannot be created because alias already refers to object (powsybl::iidm::Load) with ID 'load1'");
}

BOOST_AUTO_TEST_CASE(failWhenAliasEqualToAnId) {
    Network network = createNetwork();
    Generator& generator = network.getGenerator("generator1");
    POWSYBL_ASSERT_THROW(generator.addAlias("load1"), PowsyblException, "Object (powsybl::iidm::Generator) with alias 'load1' cannot be created because alias already refers to object (powsybl::iidm::Load) with ID 'load1'");
}

BOOST_AUTO_TEST_CASE(failWhenRemovingNonExistingAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    POWSYBL_ASSERT_THROW(load.removeAlias("Load alias"), PowsyblException, "No alias 'Load alias' found in the network");
}

BOOST_AUTO_TEST_CASE(failWhenRemovingBadAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    Generator& generator = network.getGenerator("generator1");

    load.addAlias("Load alias");
    generator.addAlias("Generator alias");

    POWSYBL_ASSERT_THROW(load.removeAlias("Generator alias"), PowsyblException, "Alias 'Generator alias' does not correspond to object 'load1'");
}

BOOST_AUTO_TEST_CASE(FindIdentifiableFromAlias) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");
    load.addAlias("Load alias");

    BOOST_CHECK(stdcxx::areSame(load, network.find("Load alias").get()));
}

BOOST_AUTO_TEST_CASE(emptyAliasType) {
    Network network = createNetwork();
    Load& load = network.getLoad("load1");

    load.addAlias("Load alias#0");
    BOOST_CHECK_EQUAL(1, boost::size(load.getAliases()));
    BOOST_CHECK(load.getAliasType("").empty());

    load.addAlias("Load alias#0", true);
    BOOST_CHECK_EQUAL(2, boost::size(load.getAliases()));
    POWSYBL_ASSERT_THROW(load.getAliasFromType(""), PowsyblException, "Invalid alias type: ");
    BOOST_CHECK(!load.getAliasFromType("fake"));
    BOOST_CHECK(load.getAliasType("").empty());

    load.addAlias("Load alias#1", "");
    BOOST_CHECK_EQUAL(3, boost::size(load.getAliases()));
    BOOST_CHECK_EQUAL("", load.getAliasType("Load alias#1"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
