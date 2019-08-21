/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(TopologyLevelTestSuite)

BOOST_AUTO_TEST_CASE(getMinTopologyLevelTest)
{
    BOOST_CHECK_EQUAL(TopologyLevel::NODE_BREAKER, getMinTopologyLevel(TopologyKind::NODE_BREAKER, TopologyLevel::NODE_BREAKER));
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BREAKER, getMinTopologyLevel(TopologyKind::NODE_BREAKER, TopologyLevel::BUS_BREAKER));
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BRANCH, getMinTopologyLevel(TopologyKind::NODE_BREAKER, TopologyLevel::BUS_BRANCH));

    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BREAKER, getMinTopologyLevel(TopologyKind::BUS_BREAKER, TopologyLevel::NODE_BREAKER));
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BREAKER, getMinTopologyLevel(TopologyKind::BUS_BREAKER, TopologyLevel::BUS_BREAKER));
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BRANCH, getMinTopologyLevel(TopologyKind::BUS_BREAKER, TopologyLevel::BUS_BRANCH));

    BOOST_CHECK_EQUAL(static_cast<TopologyLevel>(5), getMinTopologyLevel(TopologyKind::BUS_BREAKER, static_cast<TopologyLevel>(5)));
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BREAKER, getMinTopologyLevel(static_cast<TopologyKind>(5), TopologyLevel::NODE_BREAKER));
    BOOST_CHECK_EQUAL(static_cast<TopologyLevel>(5), getMinTopologyLevel(static_cast<TopologyKind>(6), static_cast<TopologyLevel>(5)));
}

BOOST_AUTO_TEST_CASE(getTopologyKindTest)
{
    BOOST_CHECK_EQUAL(TopologyKind::NODE_BREAKER, getTopologyKind(TopologyLevel::NODE_BREAKER));
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, getTopologyKind(TopologyLevel::BUS_BREAKER));
    BOOST_CHECK_EQUAL(TopologyKind::BUS_BREAKER, getTopologyKind(TopologyLevel::BUS_BRANCH));

    POWSYBL_ASSERT_THROW(getTopologyKind(static_cast<TopologyLevel>(5)), PowsyblException, "No topology kind associated to topology level 5");

    POWSYBL_ASSERT_THROW(getTopologyKind("INVALID"), PowsyblException, "Unable to retrieve topology kind from 'INVALID'");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
