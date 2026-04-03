/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(SecondaryVoltageControlTestSuite)

Network createNetworkWithMoreGenerator() {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    VoltageLevel& vlgen = network.getVoltageLevel("VLGEN");
    Bus& ngen = vlgen.getBusBreakerView().getBus("NGEN");

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

    return network;
}

BOOST_AUTO_TEST_CASE(Constructor) {
    Network network = createNetworkWithMoreGenerator();

    auto controlAdder = network.newExtension<SecondaryVoltageControlAdder>();

    controlAdder.newControlZone()
                    .withName("z1")
                    .withPilotPoint(PilotPoint({"NLOAD"}, 15.0))
                    .addControlUnit(ControlUnit("GEN",false))
                    .addControlUnit(ControlUnit("GEN2"))
                    .add();


    controlAdder.addControlZone(ControlZone("z2",
                                            PilotPoint({"NLOAD"}, 15.1),
                                            {ControlUnit("GEN", false), ControlUnit("GEN2")}));
    controlAdder.add();

    auto& controlExtension = network.getExtension<SecondaryVoltageControl>();
    const SecondaryVoltageControl& constExt = network.getExtension<SecondaryVoltageControl>();
    BOOST_CHECK_EQUAL("secondaryVoltageControl", controlExtension.getName());
    BOOST_CHECK(stdcxx::areSame(network, controlExtension.getExtendable().get()));

    BOOST_CHECK_EQUAL(2, controlExtension.getControlZones().size());
    ControlZone& z1 = controlExtension.getControlZones().at(0);
    const ControlZone& cz1 = controlExtension.getControlZones().at(0);

    POWSYBL_ASSERT_REF_TRUE(controlExtension.getControlZone("z1"));
    POWSYBL_ASSERT_REF_FALSE(controlExtension.getControlZone("notFound"));
    BOOST_CHECK(constExt.getControlZone("z1").has_value());
    BOOST_CHECK(!constExt.getControlZone("notFound").has_value());
    
    BOOST_CHECK_EQUAL(z1.getName(), controlExtension.getControlZone("z1").get().getName());

    BOOST_CHECK_EQUAL("z1", z1.getName());
    BOOST_CHECK(!z1.getPilotPoint().getBusbarSectionsOrBusesIds().empty());
    BOOST_CHECK_EQUAL("NLOAD", z1.getPilotPoint().getBusbarSectionsOrBusesIds().at(0));
    BOOST_CHECK_CLOSE(15.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, z1.getControlUnits().size());
    BOOST_CHECK_EQUAL("GEN", z1.getControlUnits().at(0).getId());
    BOOST_CHECK(!z1.getControlUnits().at(0).isParticipate());
    BOOST_CHECK_EQUAL("GEN2", z1.getControlUnits().at(1).getId());
    BOOST_CHECK(z1.getControlUnits().at(1).isParticipate());

    BOOST_CHECK(cz1.getControlUnit("GEN").has_value());
    POWSYBL_ASSERT_REF_TRUE(z1.getControlUnit("GEN"));
    BOOST_CHECK(cz1.getControlUnit("GEN2").has_value());
    POWSYBL_ASSERT_REF_TRUE(z1.getControlUnit("GEN2"));
    BOOST_CHECK(!cz1.getControlUnit("notFound").has_value());
    POWSYBL_ASSERT_REF_FALSE(z1.getControlUnit("notFound"));
    BOOST_CHECK_EQUAL(z1.getControlUnits().at(0).getId(), z1.getControlUnit("GEN").get().getId());

    z1.getPilotPoint().setTargetV(16);
    BOOST_CHECK_CLOSE(16.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());

}

BOOST_FIXTURE_TEST_CASE(SecondaryVoltageControlXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetworkWithMoreGenerator();
    network.setCaseDate(stdcxx::DateTime::parse("2023-01-07T20:43:11.819+01:00"));

    network.newExtension<SecondaryVoltageControlAdder>()
                .addControlZone(ControlZone("z1",
                                            PilotPoint({"NLOAD"}, 15.0),
                                            {ControlUnit("GEN", false), ControlUnit("GEN2")}))
                .addControlZone(ControlZone("z2",
                                            PilotPoint({"NLOAD", "NLOAD2"}, 15.2),
                                            {ControlUnit("GEN")}))
                .add();

    auto& test1 = network.getExtension<SecondaryVoltageControl>();
    BOOST_CHECK_EQUAL(2, test1.getControlZones().size());

    const std::string& networkStrRef = ResourceFixture::getResource("/secondaryVoltageControlRef.xml");
    Network network2 = test::converter::RoundTrip::runXml(network, networkStrRef);

    auto& control2 = network2.getExtension<SecondaryVoltageControl>();

    BOOST_CHECK_EQUAL(2, control2.getControlZones().size());
    BOOST_CHECK_EQUAL("NLOAD", control2.getControlZones().at(0).getPilotPoint().getBusbarSectionsOrBusesIds().at(0));
    BOOST_CHECK_EQUAL(2, control2.getControlZones().at(1).getPilotPoint().getBusbarSectionsOrBusesIds().size());
    BOOST_CHECK_EQUAL("NLOAD2", control2.getControlZones().at(1).getPilotPoint().getBusbarSectionsOrBusesIds().at(1));
    BOOST_CHECK_CLOSE(15.2, control2.getControlZones().at(1).getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(multivariantTest) {

    Network network = createNetworkWithMoreGenerator();
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"v"});

    PilotPoint pilotpoint1({"NLOAD"}, 15.0);
    ControlUnit controlunit1("GEN",false);
    //PilotPoint and ControlUnit  do not manage variant outside of a SecondaryVoltageControl extension:
    BOOST_CHECK_CLOSE(15.0, pilotpoint1.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!controlunit1.isParticipate());
    //Switch to variant and set new values
    network.getVariantManager().setWorkingVariant("v");
    pilotpoint1.setTargetV(16.0);
    controlunit1.setParticipate(true);
    BOOST_CHECK_CLOSE(16.0, pilotpoint1.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(controlunit1.isParticipate());
    //Switch baack to initial variant, values have been modified:
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(16.0, pilotpoint1.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(controlunit1.isParticipate());
    //set back previous values
    pilotpoint1.setTargetV(15.0);
    controlunit1.setParticipate(false);

    //Add extension
    SecondaryVoltageControlAdder controlAdder = network.newExtension<SecondaryVoltageControlAdder>();
    controlAdder.newControlZone()
                    .withName("z1")
                    .withPilotPoint(pilotpoint1)
                    .addControlUnit(controlunit1)
                    .addControlUnit(ControlUnit("GEN2"))
                    .add();
    controlAdder.addControlZone(ControlZone("z2", PilotPoint({"NLOAD"}, 15.1), {ControlUnit("GEN", false), ControlUnit("GEN2")}));
    controlAdder.add();

    SecondaryVoltageControl& controlExtension = network.getExtension<SecondaryVoltageControl>();

    ControlZone& z1 = controlExtension.getControlZones().at(0);
    BOOST_CHECK_CLOSE(15.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    ControlUnit& cu1 = z1.getControlUnits().at(0);
    ControlUnit& cu2 = z1.getControlUnits().at(1);
    BOOST_CHECK(!cu1.isParticipate());
    BOOST_CHECK(cu2.isParticipate());

    //Switch to variant :
    network.getVariantManager().setWorkingVariant("v");
    z1.getPilotPoint().setTargetV(16.0);
    BOOST_CHECK_CLOSE(16.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    cu1.setParticipate(true);
    cu2.setParticipate(false);
    BOOST_CHECK(cu1.isParticipate());
    BOOST_CHECK(!cu2.isParticipate());

    BOOST_CHECK_CLOSE(16.0, controlExtension.getControlZones().at(0).getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(controlExtension.getControlZones().at(0).getControlUnits().at(0).isParticipate());
    BOOST_CHECK(!controlExtension.getControlZones().at(0).getControlUnits().at(1).isParticipate());

    //Check initial variant is nunchanged
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(15.0, controlExtension.getControlZones().at(0).getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!controlExtension.getControlZones().at(0).getControlUnits().at(0).isParticipate());
    BOOST_CHECK(controlExtension.getControlZones().at(0).getControlUnits().at(1).isParticipate());

    // check variant copy
    network.getVariantManager().cloneVariant("v", "v2");
    network.getVariantManager().setWorkingVariant("v2");
    BOOST_CHECK_CLOSE(16.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(cu1.isParticipate());
    BOOST_CHECK(!cu2.isParticipate());

    // remove variant 'v' and check 'v2' is unchanged
    network.getVariantManager().removeVariant("v");
    BOOST_CHECK_CLOSE(16.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(cu1.isParticipate());
    BOOST_CHECK(!cu2.isParticipate());

    // re-clone variant on 'v'
    network.getVariantManager().cloneVariant("v2", "v");
    network.getVariantManager().setWorkingVariant("v");
    BOOST_CHECK_CLOSE(16.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(cu1.isParticipate());
    BOOST_CHECK(!cu2.isParticipate());

    // remove all additional variants
    network.getVariantManager().removeVariant("v");
    network.getVariantManager().removeVariant("v2");
    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(15.0, controlExtension.getControlZones().at(0).getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!cu1.isParticipate());
    BOOST_CHECK(cu2.isParticipate());
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

