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
    const Network& network = createNetwork();

    // Substations
    BOOST_CHECK_EQUAL(1UL, network.getSubstationCount());
    BOOST_CHECK_EQUAL(1UL, boost::size(network.getSubstations()));

    // Identifiables
    std::set<std::string> expected = {"LOAD1", "S1", "VL1", "VL1_BUS1", "VL2"};
    std::set<std::string> actual;
    for (const auto& identifiable : network.getIdentifiables()) {
        actual.insert(identifiable.get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());

    // VoltageLevels
    BOOST_CHECK_EQUAL(2UL, network.getVoltageLevelCount());
    BOOST_CHECK_EQUAL(2UL, boost::size(network.getVoltageLevels()));
    std::vector<std::string> expectedVL = {"VL1", "VL2"};
    std::vector<std::string> actualVL;
    for (const auto& vl : network.getVoltageLevels()) {
        actualVL.push_back(vl.get().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedVL.cbegin(), expectedVL.cend(), actualVL.cbegin(), actualVL.cend());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
