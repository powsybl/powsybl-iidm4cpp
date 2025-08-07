/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>
#include <powsybl/iidm/extensions/iidm/LinePosition.hpp>
#include <powsybl/iidm/extensions/iidm/LinePositionAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SubstationPosition.hpp>
#include <powsybl/iidm/extensions/iidm/SubstationPositionAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(SubstationAndLinePositionTestSuite)

BOOST_AUTO_TEST_CASE(SubstationAndLinePositionConstructor) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    Substation& p1 = network.getSubstation("P1");
    p1.newExtension<SubstationPositionAdder>()
        .withCoordinates(Coordinate(48, 2))
        .add();

    auto& substationPosition = p1.getExtension<SubstationPosition>();
    BOOST_CHECK_EQUAL("substationPosition", substationPosition.getName());
    BOOST_CHECK(stdcxx::areSame(p1, substationPosition.getExtendable().get()));
    BOOST_CHECK(Coordinate(48, 2) == substationPosition.getCoordinates());

    Line& l1 = network.getLine("NHV1_NHV2_1");
    l1.newExtension<LinePositionAdder>()
        .withCoordinates({Coordinate(48, 2), Coordinate(48.1, 2.1)})
        .add();

    auto& linePosition = l1.getExtension<LinePosition>();
    BOOST_CHECK_EQUAL("linePosition", linePosition.getName());
    BOOST_CHECK(stdcxx::areSame(l1, linePosition.getExtendable().get()));
    BOOST_CHECK_EQUAL(2, linePosition.getCoordinates().size());
    BOOST_CHECK(Coordinate(48, 2) == linePosition.getCoordinates().at(0));
    BOOST_CHECK(Coordinate(48.1, 2.1) == linePosition.getCoordinates().at(1));
    BOOST_CHECK(std::vector<Coordinate>({Coordinate(48, 2), Coordinate(48.1, 2.1)}) == linePosition.getCoordinates());

}

BOOST_FIXTURE_TEST_CASE(SubstationAndLinePositionSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));

    network.getSubstation("P1").newExtension<SubstationPositionAdder>()
        .withCoordinates(Coordinate(48, 2))
        .add();

    network.getLine("NHV1_NHV2_1").newExtension<LinePositionAdder>()
        .withCoordinates({Coordinate(48, 2), Coordinate(48.1, 2.1)})
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/substationAndLinePosition.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
        
    std::istringstream stream(networkStr);
    Network networkRef = Network::readXml("/substationAndLinePosition.xml", stream);

    test::converter::RoundTrip::runXml(networkRef, networkStr);

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
