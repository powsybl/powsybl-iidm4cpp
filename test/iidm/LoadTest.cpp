/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(LoadTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createNetwork();
    unsigned long loadCount = network.getLoadCount();

    Load& load = network.getLoad("LOAD1");
    BOOST_CHECK_EQUAL("LOAD1", load.getId());
    BOOST_CHECK_EQUAL("LOAD1_NAME", load.getName());
    BOOST_CHECK_EQUAL(ConnectableType::LOAD, load.getType());
    std::ostringstream oss;
    oss << load.getType();
    BOOST_CHECK_EQUAL("LOAD", oss.str());
    BOOST_CHECK_EQUAL(LoadType::UNDEFINED, load.getLoadType());
    BOOST_CHECK_CLOSE(50.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40.0, load.getQ0(), std::numeric_limits<double>::epsilon());

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    LoadAdder adder = vl1.newLoad()
        .setId("LOAD1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': p0 is invalid");
    adder.setP0(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': q0 is invalid");
    adder.setQ0(40.0);

    adder.setLoadType(static_cast<LoadType>(5));
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected load type value: 5");
    adder.setLoadType(LoadType::AUXILIARY);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Object 'LOAD1' already exists (powsybl::iidm::Load)");
    adder.setId("UNIQUE_LOAD_ID");

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(loadCount + 1, network.getLoadCount());
    BOOST_CHECK_EQUAL(loadCount + 1, boost::size(network.getLoads()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_EQUAL(LoadType::UNDEFINED, load1.getLoadType());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(load1, load1.setLoadType(LoadType::FICTITIOUS)));
    BOOST_CHECK_EQUAL(LoadType::FICTITIOUS, load1.getLoadType());
    POWSYBL_ASSERT_THROW(load1.setLoadType(static_cast<LoadType>(7)), AssertionError, "Unexpected load type value: 7");

    BOOST_TEST(stdcxx::areSame(load1, load1.setP0(100)));
    BOOST_CHECK_CLOSE(100, load1.getP0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(load1.setP0(stdcxx::nan()), ValidationException, "Load 'LOAD1': p0 is invalid");

    BOOST_TEST(stdcxx::areSame(load1, load1.setQ0(100)));
    BOOST_CHECK_CLOSE(100, load1.getQ0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(load1.setQ0(stdcxx::nan()), ValidationException, "Load 'LOAD1': q0 is invalid");

    load1.remove();
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD1"), PowsyblException, "Unable to find to the identifiable 'LOAD1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    load1.setP0(100);
    load1.setQ0(80);
    BOOST_CHECK_CLOSE(100, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(80, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    load1.setP0(150);
    load1.setQ0(120);
    BOOST_CHECK_CLOSE(150, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_CLOSE(150, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3ul, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1ul, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
