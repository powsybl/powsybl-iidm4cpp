/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/cvg/CongestionManagement.hpp>
#include <powsybl/iidm/extensions/cvg/CongestionManagementXmlSerializer.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

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

BOOST_AUTO_TEST_SUITE(CongestionManagementTestSuite)

BOOST_AUTO_TEST_CASE(CongestionManagementConstructor) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");

    CongestionManagement extension(battery, true);
    BOOST_CHECK_EQUAL("congestionManagement", extension.getName());
    BOOST_CHECK(true == extension.isEnabled());

    extension.setEnabled(false);
    BOOST_CHECK(false == extension.isEnabled());

    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));

    extension.extendVariantArraySize(3UL, 10UL, 0UL);
    std::set<unsigned long> indexes;
    indexes.insert(0UL);
    indexes.insert(2UL);
    extension.allocateVariantArrayElement(indexes, 1UL);
    extension.deleteVariantArrayElement(0UL);
    extension.reduceVariantArraySize(5UL);
}

BOOST_FIXTURE_TEST_CASE(CongestionManagementXmlSerializerTest, ResourceFixture) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    battery.addExtension(Extension::create<CongestionManagement>(battery, true));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("congestionManagement", stdcxx::make_unique<CongestionManagementXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/congestionManagement.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

