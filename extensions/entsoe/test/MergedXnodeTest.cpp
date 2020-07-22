/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnode.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnodeAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

BOOST_AUTO_TEST_SUITE(MergedXnodeTestSuite)

Network createNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("VL2")
        .setNominalVoltage(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getBusBreakerView().newBus()
        .setId("B2")
        .add();
    Line& line = network.newLine()
        .setId("L")
        .setVoltageLevel1("VL1")
        .setBus1("B1")
        .setVoltageLevel2("VL2")
        .setBus2("B2")
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setG2(0.0)
        .setB1(0.0)
        .setB2(0.0)
        .add();

    line.newExtension<MergedXnodeAdder>().withRdp(0.5).withXdp(0.5).withXnodeP1(1.0).withXnodeQ1(2.0).withXnodeP2(3.0).withXnodeQ2(4.0).withLine1Name("line1").withLine2Name("line2").withCode("XXXXXX11").add();

    return network;
}

BOOST_AUTO_TEST_CASE(MergedXnodeTest) {
    Network network = createNetwork();
    Line& line = network.getLine("L");

    auto& extension = line.getExtension<MergedXnode>();

    BOOST_CHECK_EQUAL("mergedXnode", extension.getName());
    BOOST_CHECK(stdcxx::areSame(line, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("XXXXXX11", extension.getCode());
    BOOST_CHECK_EQUAL("line1", extension.getLine1Name());
    BOOST_CHECK_EQUAL("line2", extension.getLine2Name());
    BOOST_CHECK_CLOSE(0.5, extension.getRdp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, extension.getXdp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, extension.getXnodeP1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, extension.getXnodeQ1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, extension.getXnodeP2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.0, extension.getXnodeQ2(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(MergedXnode(line, 10, 0.2, 0.3, 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Invalid divider position: 10");
    POWSYBL_ASSERT_THROW(extension.setRdp(10), PowsyblException, "Invalid divider position: 10");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 10, 0.3, 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Invalid divider position: 10");
    POWSYBL_ASSERT_THROW(extension.setXdp(10), PowsyblException, "Invalid divider position: 10");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, stdcxx::nan(), 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeP1(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, stdcxx::nan(), 0.5, 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeQ1(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, 0.4, stdcxx::nan(), 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeP2(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, 0.4, 0.5, stdcxx::nan(), "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeQ2(stdcxx::nan()), PowsyblException, "Power flow is invalid");
}

BOOST_FIXTURE_TEST_CASE(XnodeXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();

    const std::string& networkStr = ResourceFixture::getResource("mergedXnode.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

