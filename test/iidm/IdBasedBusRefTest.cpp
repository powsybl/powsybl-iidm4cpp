/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/IdBasedBusRef.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(IdBasedBusRefTestSuite)

BOOST_AUTO_TEST_CASE(errors) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    POWSYBL_ASSERT_THROW(IdBasedBusRef("unknown").resolve(network, TopologyLevel::NODE_BREAKER), PowsyblException, "NODE_BREAKER is not supported in resolve a BusRef");

    BOOST_CHECK(!IdBasedBusRef("unknown").resolve(network, TopologyLevel::BUS_BREAKER));
    BOOST_CHECK(!IdBasedBusRef("unknown", Branch::Side::ONE).resolve(network, TopologyLevel::BUS_BREAKER));
    BOOST_CHECK(!IdBasedBusRef("unknown", Branch::Side::TWO).resolve(network, TopologyLevel::BUS_BREAKER));

    POWSYBL_ASSERT_THROW(IdBasedBusRef("P1").resolve(network, TopologyLevel::BUS_BREAKER), PowsyblException, "P1 is not a bus or injection.");
}

BOOST_AUTO_TEST_CASE(test) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN").resolve(network, TopologyLevel::BUS_BREAKER).get(), network.getBusBreakerView().getBus("NGEN").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN_NHV1", Branch::Side::ONE).resolve(network, TopologyLevel::BUS_BREAKER).get(), network.getBusBreakerView().getBus("NGEN").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN_NHV1", Branch::Side::TWO).resolve(network, TopologyLevel::BUS_BREAKER).get(), network.getBusBreakerView().getBus("NHV1").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("LOAD").resolve(network, TopologyLevel::BUS_BREAKER).get(), network.getBusBreakerView().getBus("NLOAD").get()));
}

BOOST_AUTO_TEST_CASE(busBranch) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN").resolve(network, TopologyLevel::BUS_BRANCH).get(), network.getBusView().getBus("VLGEN_0").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN_NHV1", Branch::Side::ONE).resolve(network, TopologyLevel::BUS_BRANCH).get(), network.getBusView().getBus("VLGEN_0").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("NGEN_NHV1", Branch::Side::TWO).resolve(network, TopologyLevel::BUS_BRANCH).get(), network.getBusView().getBus("VLHV1_0").get()));
    BOOST_CHECK(stdcxx::areSame(IdBasedBusRef("LOAD").resolve(network, TopologyLevel::BUS_BRANCH).get(), network.getBusView().getBus("VLLOAD_0").get()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl

