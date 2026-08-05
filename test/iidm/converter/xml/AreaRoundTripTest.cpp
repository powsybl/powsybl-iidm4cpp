/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network createNetworkWithAreas() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2020-03-04T13:20:30.476+01:00"));
    Network& subnetwork = network.newSubnetwork("sub","format");
    subnetwork.setCaseDate(stdcxx::DateTime::parse("2020-03-04T13:20:30.476+01:00"));

    Substation& s1 = network.newSubstation().setId("sub1").add();
    VoltageLevel& vl1 = s1.newVoltageLevel().setId("VL1").setNominalV(1).setTopologyKind(TopologyKind::BUS_BREAKER).add();
    network.newSubstation().setId("sub2").add();
    vl1.getBusBreakerView().newBus().setId("N1").add();
    vl1.getBusBreakerView().newBus().setId("N2").add();
    Line& line = network.newLine().setId("Line1").setVoltageLevel1("VL1").setBus1("N1").setVoltageLevel2("VL1").setBus2("N2").setR(0.0).setX(0.0).add();

    Substation& s2 = subnetwork.newSubstation().setId("sub3").add();
    VoltageLevel& vl2 = s2.newVoltageLevel().setId("VL2").setNominalV(1).setTopologyKind(TopologyKind::BUS_BREAKER).add();
    vl2.getBusBreakerView().newBus().setId("N3").add();

    Load& load1 = vl1.newLoad().setId("L1").setBus("N1").setP0(0).setQ0(0).add();
    BoundaryLine& boundaryLine = vl1.newBoundaryLine().setId("DL1").setBus("N2").setR(0.0).setX(0.0).setP0(0).setQ0(0).add();

    // Add area types and areas to the network
    std::string biddingZoneType = "BiddingZone";
    std::string controlAreaType = "ControlArea";
    network.newArea().setAreaType(biddingZoneType).setId("BidZoneId1").setName("BidZoneName1").addAreaBoundary(load1.getTerminal(), true)
               .addAreaBoundary(boundaryLine.getBoundary(), false).add();
    network.newArea().setAreaType(biddingZoneType).setId("BidZoneId2").setName("BidZoneName2").addAreaBoundary(line.getTerminal1(), true)
               .setInterchangeTarget(100.).add();
    network.newArea().setAreaType(controlAreaType).setId("ControlAreaId1").setName("ControlAreaName1").add();
    vl1.addArea(network.getArea("BidZoneId1"));
    vl1.addArea(network.getArea("ControlAreaId1"));

    subnetwork.newArea().setAreaType(biddingZoneType).setId("BidZoneId3").setName("BidZoneName3").add();
    vl2.addArea(subnetwork.getArea("BidZoneId3"));

    return network;
}



BOOST_AUTO_TEST_SUITE(AreaRoundTrip)



BOOST_FIXTURE_TEST_CASE(AreaRoundTripTest, test::ResourceFixture) {
    Network network = createNetworkWithAreas();

    std::string filename = "areaRoundTripRef.xml";

    //BackwardCompatibility check from V11
    for (const auto& version : iidm::converter::xml::IidmXmlVersion::all()) {
        if(version.get() >= IidmXmlVersion::V1_11()) {
            const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
                converter::ExportOptions options;
                options.setVersion(version.get().toString("."));
                Network::writeXml(filename, stream, n, options);
            };
            const auto& reader = [&filename](const std::string& xmlBytes) {
                std::istringstream stream(xmlBytes);
                return Network::readXml(filename, stream);
            };
            const auto& ref = test::converter::RoundTrip::getVersionedNetwork(filename, version);
            test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, ref.c_str());
        }
    }

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

