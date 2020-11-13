/**
 * Copyright (c) 2020 RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BOOST_AUTO_TEST_SUITE(ConnectablePositionTestSuite)

BOOST_AUTO_TEST_CASE(ConnectablePositionConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");

    ConnectablePosition::Feeder feeder("feeder", 0, ConnectablePosition::Direction::TOP);
    ConnectablePosition::Feeder feeder1("feeder1", 1, ConnectablePosition::Direction::BOTTOM);
    ConnectablePosition::Feeder feeder2("feeder2", 2, ConnectablePosition::Direction::TOP);
    ConnectablePosition::Feeder feeder3("feeder3", 3, ConnectablePosition::Direction::BOTTOM);

    BOOST_CHECK_EQUAL("feeder3", feeder3.getName());
    BOOST_CHECK_EQUAL(3, feeder3.getOrder());
    BOOST_CHECK_EQUAL(static_cast<int>(ConnectablePosition::Direction::BOTTOM), static_cast<int>(feeder3.getDirection()));

    ConnectablePosition::OptionalFeeder refFeeder(feeder);
    ConnectablePosition::OptionalFeeder refFeeder1(feeder1);
    ConnectablePosition::OptionalFeeder refFeeder2(feeder2);
    ConnectablePosition::OptionalFeeder refFeeder3(feeder3);
    ConnectablePosition::OptionalFeeder refFeederNull;

    feeder3.setOrder(10)
           .setDirection(ConnectablePosition::Direction::TOP)
           .setName("feederTmp");
    BOOST_CHECK_EQUAL("feederTmp", feeder3.getName());
    BOOST_CHECK_EQUAL(10, feeder3.getOrder());
    BOOST_CHECK_EQUAL(static_cast<int>(ConnectablePosition::Direction::TOP), static_cast<int>(feeder3.getDirection()));

    feeder3.setOrder(2)
        .setDirection(ConnectablePosition::Direction::BOTTOM)
        .setName("feeder3");

    ConnectablePosition extension(load, refFeeder, refFeederNull, refFeederNull, refFeederNull);
    BOOST_CHECK_EQUAL("position", extension.getName());
    BOOST_CHECK(stdcxx::areSame(load, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("feeder", extension.getFeeder().get().getName());
    BOOST_CHECK_EQUAL(static_cast<char>(ConnectablePosition::Direction::TOP), static_cast<char>(extension.getFeeder().get().getDirection()));
    BOOST_CHECK_EQUAL(0, extension.getFeeder().get().getOrder());

    BOOST_CHECK(!extension.getFeeder1());
    BOOST_CHECK(!extension.getFeeder2());
    BOOST_CHECK(!extension.getFeeder3());

    extension.setFeeders(refFeederNull, refFeeder1, refFeeder2, refFeeder3);

    // error cases
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeederNull, refFeederNull, refFeederNull, refFeederNull), PowsyblException, "invalid feeder");
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeeder, refFeederNull, refFeeder2, refFeederNull), PowsyblException, "feeder and feeder 1|2|3 are exclusives");
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeeder, refFeederNull, refFeederNull, refFeeder3), PowsyblException, "feeder and feeder 1|2|3 are exclusives");
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeederNull, refFeeder1, refFeederNull, refFeeder3), PowsyblException, "feeder 1|2|3 have to be set in the right order");
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeederNull, refFeederNull, refFeeder2, refFeederNull), PowsyblException, "feeder 1|2|3 have to be set in the right order");
    POWSYBL_ASSERT_THROW(extension.setFeeders(refFeederNull, refFeederNull, refFeederNull, refFeeder3), PowsyblException, "feeder 1|2|3 have to be set in the right order");
}

BOOST_FIXTURE_TEST_CASE(ConnectablePositionXmlSerializerFeeder0Test, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Load& load = network.getLoad("LOAD");

    ConnectablePosition::Feeder feeder("feeder", 0, ConnectablePosition::Direction::BOTTOM);
    ConnectablePosition::OptionalFeeder refFeederNull;
    ConnectablePosition::OptionalFeeder refFeeder(feeder);

    load.addExtension(Extension::create<ConnectablePosition>(load, refFeeder, refFeederNull, refFeederNull, refFeederNull));

    const std::string& networkStr = ResourceFixture::getResource("/connectablePositionFeeder0.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_FIXTURE_TEST_CASE(ConnectablePositionXmlSerializerFeeders123Test, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Load& load = network.getLoad("LOAD");

    ConnectablePosition::Feeder feeder1("feeder1", 1, ConnectablePosition::Direction::BOTTOM);
    ConnectablePosition::Feeder feeder2("feeder2", 2, ConnectablePosition::Direction::TOP);
    ConnectablePosition::Feeder feeder3("feeder3", 3, ConnectablePosition::Direction::BOTTOM);

    ConnectablePosition::OptionalFeeder refFeeder1(feeder1);
    ConnectablePosition::OptionalFeeder refFeeder2(feeder2);
    ConnectablePosition::OptionalFeeder refFeeder3(feeder3);

    load.addExtension(Extension::create<ConnectablePosition>(load, ConnectablePosition::OptionalFeeder(), refFeeder1, refFeeder2, refFeeder3));

    const std::string& networkStr = ResourceFixture::getResource("/connectablePositionFeeders123.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

