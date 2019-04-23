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

BOOST_AUTO_TEST_CASE(NetworkIteratorTest) {
    const Network& network = createNetwork();

    BOOST_CHECK_EQUAL(1ul, network.getSubstationCount());

    std::set<std::string> expected = {"LOAD1", "S1", "VL1", "VL1_BUS1", "VL2"};
    std::set<std::string> actual;
    for (NetworkIndex::const_iterator<Identifiable> it = network.cbegin<Identifiable>(); it != network.cend<Identifiable>(); ++it) {
        actual.insert(it().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
}

BOOST_AUTO_TEST_CASE(NetworkFastIteratorTest) {
    const Network& network = createNetwork();

    BOOST_CHECK_EQUAL(2ul, network.getVoltageLevelCount());

    std::vector<std::string> expected = {"VL1", "VL2"};
    std::vector<std::string> actual;
    for (NetworkIndex::const_iterator<VoltageLevel> it = network.cbegin<VoltageLevel>(); it != network.cend<VoltageLevel>(); ++it) {
        actual.push_back(it().getId());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
