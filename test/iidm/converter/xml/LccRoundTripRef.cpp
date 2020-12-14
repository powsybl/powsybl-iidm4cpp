/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network createNonLinear() {
    Network network("shuntTestCase", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2019-09-30T16:29:18.263+02:00"));

    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(380)
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
        .setNominalVoltage(220)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getBusBreakerView().newBus()
        .setId("B2")
        .add();

    vl2.newLoad()
        .setId("LOAD")
        .setConnectableBus("B2")
        .setBus("B2")
        .setP0(100.0)
        .setQ0(50.0)
        .add();

    ShuntCompensator& sc = vl1.newShuntCompensator()
        .setId("SHUNT")
        .setBus("B1")
        .setConnectableBus("B1")
        .setSectionCount(1)
        .setVoltageRegulatorOn(true)
        .setRegulatingTerminal(stdcxx::ref(network.getLoad("LOAD").getTerminal()))
        .setTargetV(200)
        .setTargetDeadband(5.0)
        .newNonLinearModel()
        .beginSection()
        .setB(1e-5)
        .setG(0.0)
        .endSection()
        .beginSection()
        .setB(2e-2)
        .setG(3e-1)
        .endSection()
        .add()
        .add();
    sc.setProperty("test", "test");

    return network;
}

BOOST_AUTO_TEST_SUITE(LccRoundTrip)

BOOST_FIXTURE_TEST_CASE(LccRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("LccRoundTripRef.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(ShuntLinearRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("shuntRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("shuntRoundTripRef.xml", IidmXmlVersion::V1_2());
}

BOOST_FIXTURE_TEST_CASE(ShuntNonLinearRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("nonLinearShuntRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility from version 1.2
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("nonLinearShuntRoundTripRef.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    Network network = createNonLinear();

    // check that it fails for versions previous to 1.2
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_3(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        POWSYBL_ASSERT_THROW(Network::writeXml(ss, network, options), PowsyblException, stdcxx::format("shunt.shuntNonLinearModel is not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.3", version.toString(".")).c_str());
    });

    // check that it doesn't fail for versions previous to 1.2 when log error is the IIDM version incompatibility behavior
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_3(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("nonLinearShuntRoundTripRef.xml", version));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

