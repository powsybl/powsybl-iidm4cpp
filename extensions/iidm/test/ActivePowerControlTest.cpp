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
#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlAdder.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>

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
    battery.newExtension<ActivePowerControlAdder>().withParticipate(true).withDroop(4.0).add();

    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<ActivePowerControlAdder>().withParticipate(false).withDroop(3.0).add();

    return network;
}

BOOST_AUTO_TEST_CASE(ActivePowerControlTest) {
    Network network = createNetwork();

    Generator& generator = network.getGenerator("GEN");
    const ActivePowerControl& apcGen = generator.getExtension<ActivePowerControl>();
    BOOST_CHECK_EQUAL("activePowerControl", apcGen.getName());
    BOOST_CHECK_CLOSE(3, apcGen.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!apcGen.isParticipate());
    BOOST_CHECK(stdcxx::areSame(generator, apcGen.getExtendable<Generator>().get()));

    Battery& battery = network.getBattery("BAT");
    const ActivePowerControl& apcBat = battery.getExtension<ActivePowerControl>();
    BOOST_CHECK_EQUAL("activePowerControl", apcBat.getName());
    BOOST_CHECK_CLOSE(4, apcBat.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(apcBat.isParticipate());
    BOOST_CHECK(stdcxx::areSame(battery, apcBat.getExtendable<Battery>().get()));
}

BOOST_FIXTURE_TEST_CASE(ActivePowerControlXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();

    const std::string& networkStr = ResourceFixture::getResource("activePowerControl.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

