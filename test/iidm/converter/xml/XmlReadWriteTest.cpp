/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Properties.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/logging/ConsoleLogger.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlReadWriteTest)

BOOST_FIXTURE_TEST_CASE(SubstationOptions, ResourceFixture) {

    iidm::Network network("eurostag1", "test");

    // network properties a ignored for now on
    network.getProperties().set("propNetwork1", "valNetwork1");
    network.getProperties().set("propNetwork3", "valNetwork3");
    network.getProperties().set("propNetwork2", "valNetwork2");

    network.setCaseDate(stdcxx::DateTime::parse("2019-06-17T13:16:03Z"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setName("Substation1")
        .setCountry(iidm::Country::BE)
        .setTso("TSO1")
        .setGeographicalTags({"A"})
        .add();
    s1.getProperties().set("s1Prop1", "s1Val1");
    s1.getProperties().set("s1Prop3", "s1Val3");
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(iidm::Country::FR)
        .setTso("TSO2")
        .setGeographicalTags({"A", "B", "C", "D", "E"})
        .add();
    s2.getProperties().set("s2Prop4", "s2Val4");
    s2.getProperties().set("s2Prop6", "s2Val6");
    s2.getProperties().set("s2Prop5", "s2Val5");

    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("vl1")
        .setName("vl1")
        .setNominalVoltage(1.23456789)
        .setLowVoltageLimit(2.3456789)
        .setHighVoltageLimit(3.456789)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    Bus& bus1 = vl1.getBusBreakerView().newBus().setId("bus1").add();
    bus1.setAngle(60.0).setV(200.0);

    bus1.getProperties().set("bus1Prop1", "bus1Value1");
    bus1.getProperties().set("bus1Prop3", "bus1Value3");

    Load &load1 = vl1.newLoad()
        .setId("load1")
        .setName("NameLoad1")
        .setBus(bus1.getId())
        .setLoadType(LoadType::FICTITIOUS)
        .setConnectableBus(bus1.getId())
        .setP0(555.5)
        .setQ0(222.2)
        .add();
    load1.getProperties().set("propLoad1", "valueLoad1");
    load1.getProperties().set("propLoad2", "valueLoad2");

    Bus& bus2 = vl1.getBusBreakerView().newBus().setId("bus2").add();
    bus2.setAngle(62.0).setV(202.0);

    Load &load2 = vl1.newLoad()
        .setId("load2")
        .setBus(bus2.getId())
        .setLoadType(LoadType::AUXILIARY)
        .setConnectableBus(bus2.getId())
        .setP0(666.6)
        .setQ0(333.3)
        .add();
    load2.getProperties().set("load2Prop1", "load2Value1");
    load2.getProperties().set("load2Prop2", "load2Value2");

    Bus& bus3 = vl1.getBusBreakerView().newBus().setId("bus3").add();
    bus3.setAngle(63.0).setV(203.0);
    bus3.getProperties().set("bus3Prop1", "bus3Value1");
    bus3.getProperties().set("bus3Prop2", "bus3Value2");

    VoltageLevel& vl2 = s1.newVoltageLevel().setId("vl2")
        .setName("vl2")
        .setNominalVoltage(4.56789)
        .setLowVoltageLimit(5.6789)
        .setHighVoltageLimit(6.789)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getProperties().set("vl2Prop1", "vl2Value1");
    vl2.getProperties().set("vl2Prop2", "vl2Value2");

    Bus& bus4 = vl2.getBusBreakerView().newBus().setId("bus4").add();
    bus4.setAngle(64.0).setV(204.0);

    Bus& bus5 = vl2.getBusBreakerView().newBus().setId("bus5").add();
    bus5.setAngle(65.0).setV(205.0);

    Bus& bus6 = vl2.getBusBreakerView().newBus().setId("bus6").add();
    bus6.setAngle(66.0).setV(206.0);
    bus6.getProperties().set("bus6Prop1", "bus6Value1");
    bus6.getProperties().set("bus6Prop2", "bus6Value2");

    const std::string& reference = ResourceFixture::getResource("/substationOptions.xml");

    powsybl::iidm::converter::RoundTrip::runXml(network, reference);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
