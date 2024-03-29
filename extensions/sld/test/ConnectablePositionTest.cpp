/**
 * Copyright (c) 2020 RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePositionAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

std::ostream& operator<<(std::ostream& stream, const ConnectablePosition::Direction& direction) {
    stream << Enum::toString(direction);
    return stream;
}

BOOST_AUTO_TEST_SUITE(ConnectablePositionTestSuite)

BOOST_AUTO_TEST_CASE(ConnectablePositionConstructor) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
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

    POWSYBL_ASSERT_THROW(load.newExtension<ConnectablePositionAdder>().newFeeder().add().add(), PowsyblException, "Feeder name is empty");
    POWSYBL_ASSERT_THROW(load.newExtension<ConnectablePositionAdder>().newFeeder().withName("feeder").add().add(), PowsyblException, "Feeder order is not set");
    POWSYBL_ASSERT_THROW(load.newExtension<ConnectablePositionAdder>().newFeeder().withName("feeder").withOrder(0).add().add(), PowsyblException, "Feeder direction is not set");

    ConnectablePositionAdder adder = load.newExtension<ConnectablePositionAdder>().newFeeder().withName("feeder").withOrder(0).withDirection(ConnectablePosition::Direction::TOP).add();
    BOOST_CHECK_NO_THROW(adder.add());

    auto& extension = load.getExtension<ConnectablePosition>();
    const auto& cExtension = load.getExtension<ConnectablePosition>();
    BOOST_CHECK_EQUAL("position", extension.getName());
    BOOST_CHECK(stdcxx::areSame(load, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("feeder", extension.getFeeder().get().getName());
    BOOST_CHECK_EQUAL(static_cast<char>(ConnectablePosition::Direction::TOP), static_cast<char>(extension.getFeeder().get().getDirection()));
    BOOST_CHECK_EQUAL(0, extension.getFeeder().get().getOrder());

    BOOST_CHECK(stdcxx::areSame(extension.getFeeder().get(), extension.getFeeder().get().setOrder(1)));
    BOOST_CHECK_EQUAL(1, extension.getFeeder().get().getOrder());
    BOOST_CHECK(stdcxx::areSame(extension.getFeeder().get(), extension.getFeeder().get().setName("test")));
    BOOST_CHECK_EQUAL("test", extension.getFeeder().get().getName());
    BOOST_CHECK(stdcxx::areSame(extension.getFeeder().get(), extension.getFeeder().get().setDirection(ConnectablePosition::Direction::BOTTOM)));
    BOOST_CHECK_EQUAL(ConnectablePosition::Direction::BOTTOM, extension.getFeeder().get().getDirection());

    BOOST_CHECK(!extension.getFeeder1());
    BOOST_CHECK(!extension.getFeeder2());
    BOOST_CHECK(!extension.getFeeder3());
    BOOST_CHECK(!cExtension.getFeeder1());
    BOOST_CHECK(!cExtension.getFeeder2());
    BOOST_CHECK(!cExtension.getFeeder3());
}

BOOST_FIXTURE_TEST_CASE(ConnectablePositionXmlSerializerFeeder0Test, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Load& load = network.getLoad("LOAD");

    ConnectablePosition::Feeder feeder("feeder", 0, ConnectablePosition::Direction::BOTTOM);
    ConnectablePosition::OptionalFeeder refFeederNull;
    ConnectablePosition::OptionalFeeder refFeeder(feeder);

    load.newExtension<ConnectablePositionAdder>().newFeeder().withOrder(0).withName("feeder").withDirection(ConnectablePosition::Direction::BOTTOM).add().add();

    const std::string& networkStr = ResourceFixture::getResource("/connectablePositionFeeder0.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_FIXTURE_TEST_CASE(ConnectablePositionXmlSerializerFeeders123Test, test::ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Load& load = network.getLoad("LOAD");

    load.newExtension<ConnectablePositionAdder>()
        .newFeeder1()
        .withOrder(1)
        .withName("feeder1")
        .withDirection(ConnectablePosition::Direction::BOTTOM)
        .add()
        .newFeeder2()
        .withOrder(2)
        .withName("feeder2")
        .withDirection(ConnectablePosition::Direction::TOP)
        .add()
        .newFeeder3()
        .withOrder(3)
        .withName("feeder3")
        .withDirection(ConnectablePosition::Direction::BOTTOM)
        .add()
        .add();

    const std::string& networkStr = ResourceFixture::getResource("/connectablePositionFeeders123.xml");
    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

