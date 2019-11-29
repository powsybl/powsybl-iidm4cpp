/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/extensions/ExtensionRoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ActivePowerControlTestSuite)

Network createNetwork() {
    Network network("networkId", "sourceFormat");
    Substation& p1 = network.newSubstation()
        .setId("P1")
        .setCountry(Country::FR)
        .setTso("RTE")
        .setGeographicalTags({"A"})
        .add();
    VoltageLevel& vlgen = p1.newVoltageLevel()
        .setId("VLGEN")
        .setNominalVoltage(24.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    Bus& ngen = vlgen.getBusBreakerView().newBus()
        .setId("NGEN")
        .add();
    vlgen.newGenerator()
        .setId("GEN")
        .setBus(ngen.getId())
        .setConnectableBus(ngen.getId())
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();
    vlgen.newBattery()
        .setId("BAT1")
        .setName("BAT1_NAME")
        .setBus(ngen.getId())
        .setConnectableBus(ngen.getId())
        .setP0(100.0)
        .setQ0(200.0)
        .setMinP(-200.0)
        .setMaxP(300.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_CASE(ActivePowerControlTest) {
    Network network = createNetwork();
    Generator& gen = network.getGenerator("GEN");

    ActivePowerControl apcGen(gen, true, 1.1);
    BOOST_CHECK_EQUAL("activePowerControl", apcGen.getName());
    BOOST_CHECK_CLOSE(1.1, apcGen.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(apcGen.isParticipate());
    BOOST_CHECK(stdcxx::areSame(gen, apcGen.getExtendable<Generator>().get()));

    Battery& battery = network.getBattery("BAT1");

    ActivePowerControl apcBat(battery, false, 2.2);
    BOOST_CHECK_EQUAL("activePowerControl", apcBat.getName());
    BOOST_CHECK_CLOSE(2.2, apcBat.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!apcBat.isParticipate());
    BOOST_CHECK(stdcxx::areSame(battery, apcBat.getExtendable<Battery>().get()));
}

BOOST_FIXTURE_TEST_CASE(ActivePowerControlXmlSerializerTest, ResourceFixture) {
    Network network = createNetwork();
    Generator& gen = network.getGenerator("GEN");

    ActivePowerControl ext(gen, true, 1.1);
    ActivePowerControlXmlSerializer serializer;

    const std::string& extensionStr = ResourceFixture::getResource("/extensions/iidm/activePowerControl.xml");

    ExtensionRoundTrip::runXml(gen, ext, serializer, extensionStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

