/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(BusBranchTopologyRoundTrip)

BOOST_FIXTURE_TEST_CASE(BusBranchTopologyIncompatibilityOpenSwitches, test::ResourceFixture) {
    std::string filenameNBK = "voltageLevelTopology/busBranchSwitchesOpenNBK.xml";
    std::string filenameRef = "voltageLevelTopology/busBranchSwitchesOpenKeepOriginal.xml";

    //Read nodebreaker network with all VoltageLevel VL1 switches opened:
    Network networkNBK = Network::readXml(ResourceFixture::getResourcePath(filenameNBK));
    BOOST_CHECK_EQUAL(networkNBK.getVoltageLevel("VL1").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(0UL, networkNBK.getVoltageLevel("VL1").getBusView().getBusCount());
    BOOST_CHECK_EQUAL(networkNBK.getVoltageLevel("VL2").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(1UL, networkNBK.getVoltageLevel("VL2").getBusView().getBusCount());

    //Write this network in BUS_BRANCH topology:
    stdcxx::Properties properties;
    properties.set(converter::ExportOptions::TOPOLOGY_LEVEL, "BUS_BRANCH");

    //Default : throw Exception when incompatibility :
    std::stringstream ostream;
    POWSYBL_ASSERT_THROW( Network::writeXml("exportError.xiidm", ostream, networkNBK, ExportOptions(properties)), PowsyblException, "Cannot export voltage level 'VL1' in BUS_BRANCH topology: this would lead to an invalid IIDM.");

    //set ExportOptions incompatibility behavior:
    properties.set(converter::ExportOptions::BUS_BRANCH_VOLTAGE_LEVEL_INCOMPATIBILITY_BEHAVIOR, "KEEP_ORIGINAL_TOPOLOGY");
    ExportOptions options(properties);

    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    //VL1 incompatibility will result in an export in NBK, while VL2 has a bus so it can be exported in BUS_BRANCH
    test::converter::RoundTrip::writeXmlTest(networkNBK, writer, ResourceFixture::getResource(filenameRef));

    //And this can be read correctly :
    Network networkBusBranch = Network::readXml(ResourceFixture::getResourcePath(filenameRef));
    BOOST_CHECK_EQUAL(networkBusBranch.getVoltageLevel("VL1").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(0UL, networkBusBranch.getVoltageLevel("VL1").getBusView().getBusCount());
    BOOST_CHECK_EQUAL(networkBusBranch.getVoltageLevel("VL2").getTopologyKind(), TopologyKind::BUS_BREAKER);
    BOOST_CHECK_EQUAL(1UL, networkBusBranch.getVoltageLevel("VL2").getBusView().getBusCount());

}

BOOST_FIXTURE_TEST_CASE(BusBranchTopologyIncompatibilityMissingConnectableBus, test::ResourceFixture) {
    std::string filenameNBK = "voltageLevelTopology/busBranchConnectableBusNBK.xml";
    std::string filenameRef = "voltageLevelTopology/busBranchConnectableBusKeepOriginal.xml";

    //Read nodebreaker network with all VoltageLevel VL1 switches opened:
    Network networkNBK = Network::readXml(ResourceFixture::getResourcePath(filenameNBK));
    BOOST_CHECK_EQUAL(networkNBK.getVoltageLevel("VL1").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(0UL, networkNBK.getVoltageLevel("VL1").getBusView().getBusCount());
    BOOST_CHECK_EQUAL(networkNBK.getVoltageLevel("VL2").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(1UL, networkNBK.getVoltageLevel("VL2").getBusView().getBusCount());

    //Write this network in BUS_BRANCH topology:
    stdcxx::Properties properties;
    properties.set(converter::ExportOptions::TOPOLOGY_LEVEL, "BUS_BRANCH");

    //Default : throw Exception when incompatibility :
    std::stringstream ostream;
    POWSYBL_ASSERT_THROW( Network::writeXml("exportError.xiidm", ostream, networkNBK, ExportOptions(properties)), PowsyblException, "Cannot export voltage level 'VL1' in BUS_BRANCH topology: this would lead to an invalid IIDM.");

    //set ExportOptions incompatibility behavior:
    properties.set(converter::ExportOptions::BUS_BRANCH_VOLTAGE_LEVEL_INCOMPATIBILITY_BEHAVIOR, "KEEP_ORIGINAL_TOPOLOGY");
    ExportOptions options(properties);

    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };
    //VL1 incompatibility will result in an export in NBK, while VL2 has a bus so it can be exported in BUS_BRANCH
    test::converter::RoundTrip::writeXmlTest(networkNBK, writer, ResourceFixture::getResource(filenameRef));

    //And this can be read correctly :
    Network networkBusBranch = Network::readXml(ResourceFixture::getResourcePath(filenameRef));
    BOOST_CHECK_EQUAL(networkBusBranch.getVoltageLevel("VL1").getTopologyKind(), TopologyKind::NODE_BREAKER);
    BOOST_CHECK_EQUAL(0UL, networkBusBranch.getVoltageLevel("VL1").getBusView().getBusCount());
    BOOST_CHECK_EQUAL(networkBusBranch.getVoltageLevel("VL2").getTopologyKind(), TopologyKind::BUS_BREAKER);
    BOOST_CHECK_EQUAL(1UL, networkBusBranch.getVoltageLevel("VL2").getBusView().getBusCount());

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

