/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(VariantManagerTestSuite)

BOOST_AUTO_TEST_CASE(cloneVariant) {
    Network network = createNetwork();

    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1"});
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "s1"), PowsyblException, "Target variant 's1' already exists");
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"}), PowsyblException, "Target variant 's1' already exists");
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "s2");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    load1.setP0(150);
    load1.setQ0(120);
    BOOST_CHECK_CLOSE(150, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "s2", true);
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"}, true);
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());
    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
