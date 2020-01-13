/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/cvg/BusbarSectionPosition.hpp>
#include <powsybl/iidm/extensions/cvg/BusbarSectionPositionXmlSerializer.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(BusbarSectionPositionTestSuite)

Network createBusbarSectionNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));

    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl.getNodeBreakerView().setNodeCount(5);

    vl.getNodeBreakerView().newBusbarSection()
        .setId("BBS")
        .setName("BBS_NAME")
        .setNode(0)
        .add();

    return network;
}

BOOST_AUTO_TEST_CASE(BusbarSectionPositionConstructor) {
    Network network = createBusbarSectionNetwork();
    BusbarSection& busbarSection = network.getBusbarSection("BBS");

    BusbarSectionPosition extension(busbarSection, 1UL, 2UL);
    BOOST_CHECK_EQUAL("busbarSectionPosition", extension.getName());
    BOOST_CHECK(stdcxx::areSame(busbarSection, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL(1UL, extension.getBusbarIndex());
    BOOST_CHECK_EQUAL(2UL, extension.getSectionIndex());

    extension.setBusbarIndex(3UL);
    BOOST_CHECK_EQUAL(3UL, extension.getBusbarIndex());

    extension.setSectionIndex(4UL);
    BOOST_CHECK_EQUAL(4UL, extension.getSectionIndex());
}

BOOST_FIXTURE_TEST_CASE(BusbarSectionPositionXmlSerializerTest, ResourceFixture) {
    Network network = createBusbarSectionNetwork();
    BusbarSection& busbarSection = network.getBusbarSection("BBS");
    busbarSection.addExtension(Extension::create<BusbarSectionPosition>(busbarSection, 2UL, 3UL));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("busbarSectionPosition", stdcxx::make_unique<BusbarSectionPositionXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/busbarSectionPosition.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

