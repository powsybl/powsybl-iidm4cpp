/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/shortcircuits/BatteryShortCircuits.hpp>
#include <powsybl/iidm/extensions/shortcircuits/BatteryShortCircuitsXmlSerializer.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

Network createBatteryNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newBattery()
        .setId("BAT1")
        .setName("BAT1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setP0(100.0)
        .setQ0(200.0)
        .setMinP(-200.0)
        .setMaxP(300.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_SUITE(BatteryShortCircuitsTestSuite)

BOOST_AUTO_TEST_CASE(BatteryShortCircuitsConstructor) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");

    BatteryShortCircuits extension(battery, 0.1, 0.2);
    BOOST_CHECK_EQUAL("batteryShortCircuits", extension.getName());
    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(0.1, extension.getTransientReactance(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, extension.getStepUpTransformerReactance(), std::numeric_limits<double>::epsilon());

    extension.setTransientReactance(0.3);
    extension.setStepUpTransformerReactance(0.4);
    BOOST_CHECK_CLOSE(0.3, extension.getTransientReactance(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, extension.getStepUpTransformerReactance(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setTransientReactance(stdcxx::nan()), PowsyblException, "Reactance is not defined.");
    POWSYBL_ASSERT_THROW(extension.setTransientReactance(-1.0), PowsyblException, "Reactance may not be negative.");
}

BOOST_FIXTURE_TEST_CASE(BatteryShortCircuitsXmlSerializerTest, ResourceFixture) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    battery.addExtension(Extension::create<BatteryShortCircuits>(battery, 1.0, 2.0));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("batteryShortCircuits", stdcxx::make_unique<BatteryShortCircuitsXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/shortcircuits/batteryShortCircuits.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

