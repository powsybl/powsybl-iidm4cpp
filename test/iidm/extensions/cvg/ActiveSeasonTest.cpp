/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/ActiveSeason.hpp>
#include <powsybl/iidm/extensions/cvg/ActiveSeasonXmlSerializer.hpp>
#include <powsybl/network/BatteryNetworkFactory.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(ActiveSeasonTestSuite)

BOOST_AUTO_TEST_CASE(ActiveSeasonConstructor) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& battery = network.getBattery("BAT");

    ActiveSeason extension(battery, "test");
    BOOST_CHECK_EQUAL("activeSeason", extension.getName());
    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("test", extension.getValue());

    extension.setValue("test2");
    BOOST_CHECK_EQUAL("test2", extension.getValue());
}

BOOST_FIXTURE_TEST_CASE(ActiveSeasonXmlSerializerTest, ResourceFixture) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& battery = network.getBattery("BAT");
    battery.addExtension(Extension::create<ActiveSeason>(battery, "test"));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("activeSeason", stdcxx::make_unique<ActiveSeasonXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/activeSeason.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

