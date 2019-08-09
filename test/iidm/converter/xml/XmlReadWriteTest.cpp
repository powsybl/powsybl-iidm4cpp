/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
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
        .setCountry(iidm::Country::BE)
        .setTso("TSO1")
        .setGeographicalTags({"A"})
        .add();
    s1.getProperties().set("prop1S1", "val1S1");
    s1.getProperties().set("prop3S1", "val3S1");
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(iidm::Country::FR)
        .setTso("TSO2")
        .setGeographicalTags({"A", "B", "C", "D", "E"})
        .add();
    s2.getProperties().set("prop4S2", "val4S2");
    s2.getProperties().set("prop6S2", "val6S2");
    s2.getProperties().set("prop5S2", "val5S2");

    VoltageLevel& vl1 = s1.newVoltageLevel().setId("vl1")
        .setName("VoltageLevel1")
        .setNominalVoltage(1.23456789)
        .setLowVoltageLimit(2.3456789)
        .setHighVoltageLimit(3.456789)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    iidm::Bus& nloadVl1 = vl1.getBusBreakerView().newBus().setId("NLOAD_VL1").add();
    nloadVl1.setAngle(60.0).setV(200.0);

    nloadVl1.getProperties().set("nLoadVl1_Prop1", "nLoadVl1_Value1");
    nloadVl1.getProperties().set("nLoadVl1_Prop3", "nLoadVl1_Value3");

    iidm::Bus& nload2Vl1 = vl1.getBusBreakerView().newBus().setId("NLOAD2_VL1").add();
    nload2Vl1.setAngle(62.0).setV(202.0);

    iidm::Bus& nload3Vl1 = vl1.getBusBreakerView().newBus().setId("NLOAD3_VL1").add();
    nload3Vl1.setAngle(63.0).setV(203.0);
    nload3Vl1.getProperties().set("nLoad3Vl1_Prop1", "nLoad3Vl1_Value1");
    nload3Vl1.getProperties().set("nLoad3Vl1_Prop2", "nLoad3Vl1_Value2");

    VoltageLevel& vl2 = s1.newVoltageLevel().setId("vl2")
        .setName("VoltageLevel2")
        .setNominalVoltage(4.56789)
        .setLowVoltageLimit(5.6789)
        .setHighVoltageLimit(6.789)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();

    iidm::Bus& nloadVl2 = vl2.getBusBreakerView().newBus().setId("NLOAD_VL2").add();
    nloadVl2.setAngle(64.0).setV(204.0);

    iidm::Bus& nload2Vl2 = vl2.getBusBreakerView().newBus().setId("NLOAD2_VL2").add();
    nload2Vl2.setAngle(65.0).setV(205.0);

    Bus& nload3Vl2 = vl2.getBusBreakerView().newBus().setId("NLOAD3_VL2").add();
    nload3Vl2.setAngle(66.0).setV(206.0);
    nload3Vl2.getProperties().set("nload3Vl2_Prop1", "nload3Vl2_Value1");
    nload3Vl2.getProperties().set("nload3Vl2_Prop2", "nload3Vl2_Value2");

    vl2.getProperties().set("vlProp1", "vlValue1");
    vl2.getProperties().set("vlProp2", "vlValue2");

    const std::string& reference = ResourceFixture::getResource("/substationOptions.xml");

    powsybl::iidm::converter::RoundTrip::runXml(network, reference);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
