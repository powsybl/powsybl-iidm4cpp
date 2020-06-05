/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/entsoe/Xnode.hpp>
#include <powsybl/iidm/extensions/entsoe/XnodeAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

BOOST_AUTO_TEST_SUITE(XnodeTestSuite)

Network createNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setNominalVoltage(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl.getBusBreakerView().newBus()
        .setId("B")
        .add();
    DanglingLine& dl = vl.newDanglingLine()
        .setId("DL")
        .setBus("B")
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .setP0(0.0)
        .setQ0(0.0)
        .add();

    dl.newExtension<XnodeAdder>().withCode("XXXXXX11").add();

    return network;
}

BOOST_AUTO_TEST_CASE(XnodeTest) {
    Network network = createNetwork();
    DanglingLine& dl = network.getDanglingLine("DL");

    const auto& extension = dl.getExtension<Xnode>();

    BOOST_CHECK_EQUAL("xnode", extension.getName());
    BOOST_CHECK(stdcxx::areSame(dl, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("XXXXXX11", extension.getCode());
}

BOOST_FIXTURE_TEST_CASE(XnodeXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();

    const std::string& networkStr = ResourceFixture::getResource("xnode.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

