/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPosition.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPositionAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

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
        .setNominalV(400.0)
        .setLowVoltageLimit(380.0)
        .setHighVoltageLimit(420.0)
        .add();

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

    POWSYBL_ASSERT_THROW(busbarSection.newExtension<BusbarSectionPositionAdder>().add(), PowsyblException, "Undefined value for busbar index");
    POWSYBL_ASSERT_THROW(busbarSection.newExtension<BusbarSectionPositionAdder>().withBusbarIndex(1UL).add(), PowsyblException, "Undefined value for section index");
    BOOST_CHECK_NO_THROW(busbarSection.newExtension<BusbarSectionPositionAdder>().withBusbarIndex(1UL).withSectionIndex(2UL).add());

    auto& extension = busbarSection.getExtension<BusbarSectionPosition>();
    BOOST_CHECK_EQUAL("busbarSectionPosition", extension.getName());
    BOOST_CHECK(stdcxx::areSame(busbarSection, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL(1UL, extension.getBusbarIndex());
    BOOST_CHECK_EQUAL(2UL, extension.getSectionIndex());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setBusbarIndex(3UL)));
    BOOST_CHECK_EQUAL(3UL, extension.getBusbarIndex());

    BOOST_CHECK(stdcxx::areSame(extension, extension.setSectionIndex(4UL)));
    BOOST_CHECK_EQUAL(4UL, extension.getSectionIndex());
}

BOOST_FIXTURE_TEST_CASE(BusbarSectionPositionXmlSerializerTest, test::ResourceFixture) {
    Network network = createBusbarSectionNetwork();
    BusbarSection& busbarSection = network.getBusbarSection("BBS");
    busbarSection.newExtension<BusbarSectionPositionAdder>().withBusbarIndex(2UL).withSectionIndex(3UL).add();

    const std::string& networkStr = ResourceFixture::getResource("/busbarSectionPosition.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
