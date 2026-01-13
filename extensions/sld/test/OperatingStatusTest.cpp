/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/sld/OperatingStatus.hpp>
#include <powsybl/iidm/extensions/sld/OperatingStatusAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
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

    POWSYBL_ASSERT_THROW(line.newExtension<OperatingStatusAdder>().add(), PowsyblException, "Undefined status value");

    line.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::PLANNED_OUTAGE)
        .add();

    auto& operatingStatus = line.getExtension<OperatingStatus>();
    BOOST_CHECK_EQUAL(OperatingStatus::Status::PLANNED_OUTAGE, operatingStatus.getStatus());

    BOOST_CHECK(stdcxx::areSame(operatingStatus, operatingStatus.setStatus(OperatingStatus::Status::FORCED_OUTAGE)));
    BOOST_CHECK_EQUAL(OperatingStatus::Status::FORCED_OUTAGE, operatingStatus.getStatus());

    Network network2 = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Line& l = network2.getLine("LINE_S2S3");
    l.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::FORCED_OUTAGE)
        .add();
    auto& operatingStatus2 = l.getExtension<OperatingStatus>();
    BOOST_CHECK_EQUAL("operatingStatus", operatingStatus2.getName());
    BOOST_CHECK(stdcxx::areSame(l, operatingStatus2.getExtendable().get()));
    BOOST_CHECK_EQUAL(OperatingStatus::Status::FORCED_OUTAGE, operatingStatus2.getStatus());
    operatingStatus2.setStatus(OperatingStatus::Status::IN_OPERATION);
    BOOST_CHECK_EQUAL(OperatingStatus::Status::IN_OPERATION, operatingStatus2.getStatus());

    TwoWindingsTransformer& twt = network2.getTwoWindingsTransformer("TWT");
    twt.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::PLANNED_OUTAGE)
        .add();

    HvdcLine& hvdcl = network2.getHvdcLine("HVDC1");
    hvdcl.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::PLANNED_OUTAGE)
        .add();

    BusbarSection& bbs = network2.getBusbarSection("S1VL1_BBS");
        bbs.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::FORCED_OUTAGE)
        .add();

    Generator& g = network2.getGenerator("GH1");
    OperatingStatusAdder operatingStatusAdder = g.newExtension<OperatingStatusAdder>()
                                                    .withStatus(OperatingStatus::Status::PLANNED_OUTAGE);

    POWSYBL_ASSERT_THROW(operatingStatusAdder.add(), PowsyblException, "Operating status extension is not allowed on identifiable type: GENERATOR");
}

BOOST_FIXTURE_TEST_CASE(OperatingStatusXmlSerializerTest, test::ResourceFixture) {
    Network network = createTestNetwork();
    Line& line = network.getLine("L");
    line.newExtension<OperatingStatusAdder>()
        .withStatus(OperatingStatus::Status::PLANNED_OUTAGE)
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/operatingStatusRef.xml");
    // const std::string& networkStr = ResourceFixture::getResource("/branchStatusRef.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_FIXTURE_TEST_CASE(BranchStatusXmlSerializerBackwardCompatibilityTest, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("branchStatusRef.xml"));
    
    Line& line = network.getLine("L");

    OperatingStatus& operatingStatus = line.getExtension<OperatingStatus>();
    BOOST_CHECK_EQUAL(OperatingStatus::Status::PLANNED_OUTAGE, operatingStatus.getStatus());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
