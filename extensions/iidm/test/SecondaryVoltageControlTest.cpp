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
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlAdder.hpp>

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

    SecondaryVoltageControlAdder controlAdder = network.newExtension<SecondaryVoltageControlAdder>();

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

    SecondaryVoltageControl& controlExtension = network.getExtension<SecondaryVoltageControl>();
    BOOST_CHECK_EQUAL("secondaryVoltageControl", controlExtension.getName());
    BOOST_CHECK(stdcxx::areSame(network, controlExtension.getExtendable().get()));

    BOOST_CHECK_EQUAL(2, controlExtension.getControlZones().size());
    ControlZone z1 = controlExtension.getControlZones().at(0);


    BOOST_CHECK_EQUAL("z1", z1.getName());
    BOOST_CHECK(!z1.getPilotPoint().getBusbarSectionsOrBusesIds().empty());
    BOOST_CHECK_EQUAL("NLOAD", z1.getPilotPoint().getBusbarSectionsOrBusesIds().at(0));
    BOOST_CHECK_CLOSE(15.0, z1.getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, z1.getControlUnits().size());
    BOOST_CHECK_EQUAL("GEN", z1.getControlUnits().at(0).getId());
    BOOST_CHECK(!z1.getControlUnits().at(0).isParticipate());
    BOOST_CHECK_EQUAL("GEN2", z1.getControlUnits().at(1).getId());
    BOOST_CHECK(z1.getControlUnits().at(1).isParticipate());
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

    SecondaryVoltageControl& control2 = network2.getExtension<SecondaryVoltageControl>();

    BOOST_CHECK_EQUAL(2, control2.getControlZones().size());
    BOOST_CHECK_EQUAL("NLOAD", control2.getControlZones().at(0).getPilotPoint().getBusbarSectionsOrBusesIds().at(0));
    BOOST_CHECK_EQUAL(2, control2.getControlZones().at(1).getPilotPoint().getBusbarSectionsOrBusesIds().size());
    BOOST_CHECK_EQUAL("NLOAD2", control2.getControlZones().at(1).getPilotPoint().getBusbarSectionsOrBusesIds().at(1));
    BOOST_CHECK_CLOSE(15.2, control2.getControlZones().at(1).getPilotPoint().getTargetV(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

