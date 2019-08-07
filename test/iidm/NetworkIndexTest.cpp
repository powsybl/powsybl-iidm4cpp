/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(NetworkIndexTestSuite)

BOOST_AUTO_TEST_CASE(RangeTest) {
    Network network = createNetwork();
    const Network& cNetwork = network;

    // Substations
    BOOST_CHECK_EQUAL(1UL, cNetwork.getSubstationCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(cNetwork.getSubstations()));
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getSubstations()));

    // Identifiables
    std::set<std::string> expected = {"LOAD1", "S1", "VL1", "VL1_BUS1", "VL2"};
    std::set<std::string> actual;
    std::set<std::string> actualConst;
    for (const auto& identifiable : network.getIdentifiables()) {
        actual.insert(identifiable.getId());
    }
    for (const auto& identifiable : cNetwork.getIdentifiables()) {
        actualConst.insert(identifiable.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actualConst.cbegin(), actualConst.cend());

    // VoltageLevels
    BOOST_CHECK_EQUAL(2UL, cNetwork.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(cNetwork.getVoltageLevels()));
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getVoltageLevels()));
    std::vector<std::string> expectedVL = {"VL1", "VL2"};
    std::vector<std::string> actualVL;
    std::vector<std::string> actualConstVL;
    for (const auto& vl : network.getVoltageLevels()) {
        actualVL.push_back(vl.getId());
    }
    for (const auto& vl : cNetwork.getVoltageLevels()) {
        actualConstVL.push_back(vl.getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedVL.cbegin(), expectedVL.cend(), actualVL.cbegin(), actualVL.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedVL.cbegin(), expectedVL.cend(), actualConstVL.cbegin(), actualConstVL.cend());

    // StatefulObjects
    BOOST_CHECK_EQUAL(4UL, boost::size(cNetwork.getStatefulObjects()));
    BOOST_CHECK_EQUAL(4UL, boost::size(network.getStatefulObjects()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
