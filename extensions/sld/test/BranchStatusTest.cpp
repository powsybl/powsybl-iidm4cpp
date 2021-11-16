/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/sld/BranchStatus.hpp>
#include <powsybl/iidm/extensions/sld/BranchStatusAdder.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BOOST_AUTO_TEST_SUITE(BranchStatusTestSuite)

Network createTestNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();

    s1.newVoltageLevel()
        .setId("VL")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();

    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();

    s2.newVoltageLevel()
        .setId("VL2")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();

    network.newLine()
        .setId("L")
        .setVoltageLevel1("VL")
        .setNode1(2)
        .setVoltageLevel2("VL2")
        .setNode2(0)
        .setR(1)
        .setX(1)
        .setG1(0)
        .setG2(0)
        .setB1(0)
        .setB2(0)
        .add();

    return network;
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createTestNetwork();
    Line& line = network.getLine("L");

    POWSYBL_ASSERT_THROW(line.newExtension<BranchStatusAdder>().add(), PowsyblException, "Undefined status value");

    line.newExtension<BranchStatusAdder>()
        .withStatus(BranchStatus::Status::PLANNED_OUTAGE)
        .add();

    auto& branchStatus = line.getExtension<BranchStatus>();
    BOOST_CHECK_EQUAL(BranchStatus::Status::PLANNED_OUTAGE, branchStatus.getStatus());

    BOOST_CHECK(stdcxx::areSame(branchStatus, branchStatus.setStatus(BranchStatus::Status::FORCED_OUTAGE)));
    BOOST_CHECK_EQUAL(BranchStatus::Status::FORCED_OUTAGE, branchStatus.getStatus());
}

BOOST_FIXTURE_TEST_CASE(BranchStatusXmlSerializerTest, test::ResourceFixture) {
    Network network = createTestNetwork();
    Line& line = network.getLine("L");
    line.newExtension<BranchStatusAdder>()
        .withStatus(BranchStatus::Status::PLANNED_OUTAGE)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/branchStatusRef.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
