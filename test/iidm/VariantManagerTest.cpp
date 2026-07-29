/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <thread>

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

    //retry cloning into existing s1 - throw - no variants change
    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "s1"), PowsyblException, "Target variants {s1} already exist");
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    //retry cloning into existing s1 and a new s2 - throw - no variants change
    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"}), PowsyblException, "Target variants {s1} already exist");
    BOOST_CHECK_EQUAL(2UL, network.getVariantManager().getVariantArraySize());

    //try cloning into a new s2 2 consecutive times - no throw - first extend variants to insert s2, then reallocate and overwrite s2
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s2", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    //removing s1 (not the last) - variants size unchanged
    network.getVariantManager().removeVariant("s1"); 
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    //cloning and overwrite into a new s3, s1 2 consecutive times, and existing s2 - no throw - s3 reuse previously removed s1 index, variants extended to insert s1, then overwrite s1 and s2
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s3", "s1", "s1", "s2"}, true); //
    BOOST_CHECK_EQUAL(4UL, network.getVariantManager().getVariantArraySize());

    //retry cloning into existing s1, s2 and s3 - throw - no variants change
    POWSYBL_ASSERT_THROW(network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s3", "s2"}), PowsyblException, "Target variants {s1, s2, s3} already exist");
    BOOST_CHECK_EQUAL(4UL, network.getVariantManager().getVariantArraySize());

    //clone and overwrite s1, s2 and s3
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2", "s3"}, true);
    BOOST_CHECK_EQUAL(4UL, network.getVariantManager().getVariantArraySize());

    //Remove all variants 
    network.getVariantManager().removeVariant("s3");
    network.getVariantManager().removeVariant("s2");
    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
    //try clone and overwrite into s1 and s2 and overwite s1 - no throw - extend for s1 and s2 and overwrite s1
    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s1", "s2"});
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

BOOST_AUTO_TEST_CASE(multithread) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");

    BOOST_CHECK(!network.getVariantManager().isVariantMultiThreadAccessAllowed());

    const unsigned long NB_VARIANTS = 100000UL;

    std::vector<std::string> variantsArray;
    for (unsigned long i = 0UL; i < NB_VARIANTS; ++i) {
        const std::string& variantName = std::to_string(i);
        network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), variantName, true);
        variantsArray.emplace_back(variantName);
    }

    // comment these 2 lines to reproduce the fact that default variant manager is not thread safe
    network.getVariantManager().allowVariantMultiThreadAccess(true);
    BOOST_CHECK(network.getVariantManager().isVariantMultiThreadAccessAllowed());

    BOOST_CHECK_EQUAL(0UL, network.getVariantManager().getVariantIndex());
    BOOST_CHECK_EQUAL(VariantManager::getInitialVariantId(), network.getVariantManager().getWorkingVariantId());

    const auto& updateEvenVariants = [&network, &load1, &variantsArray]() {
        for (unsigned long i = 0UL; i < variantsArray.size(); ++i) {
            if (i % 2 == 0) {
                network.getVariantManager().setWorkingVariant(variantsArray.at(i));
                load1.setP0(3.0 * i);
                load1.setQ0(5.0 * i);
            }
        }
    };

    const auto& updateOddVariants = [&network, &load1, &variantsArray]() {
        for (unsigned long i = 0UL; i < variantsArray.size(); ++i) {
            if (i % 2 != 0) {
                network.getVariantManager().setWorkingVariant(variantsArray.at(i));
                load1.setP0(2.0 * i);
                load1.setQ0(4.0 * i);
            }
        }
    };

    std::thread launcherS1(updateEvenVariants);
    std::thread launcherS2(updateOddVariants);
    launcherS1.join();
    launcherS2.join();

    unsigned long index = 0UL;
    for (const std::string& variant : variantsArray) {
        network.getVariantManager().setWorkingVariant(variant);
        if (index % 2 == 0) {
            BOOST_CHECK_CLOSE(3 * index, load1.getP0(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(5 * index, load1.getQ0(), std::numeric_limits<double>::epsilon());
        } else {
            BOOST_CHECK_CLOSE(2 * index, load1.getP0(), std::numeric_limits<double>::epsilon());
            BOOST_CHECK_CLOSE(4 * index, load1.getQ0(), std::numeric_limits<double>::epsilon());
        }
        index++;
    }
}

BOOST_AUTO_TEST_CASE(testVariantIndexKept) {
    Network network = createNetwork();

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "S1", true);
    network.getVariantManager().setWorkingVariant("S1");
    network.getVariantManager().allowVariantMultiThreadAccess(true);
    BOOST_CHECK_EQUAL("S1", network.getVariantManager().getWorkingVariantId());

    const auto& updateVariantFunction = [&network]() {
        network.getVariantManager().allowVariantMultiThreadAccess(true);
    };

    std::thread launcher(updateVariantFunction);
    launcher.join();

    BOOST_CHECK_EQUAL("S1", network.getVariantManager().getWorkingVariantId());
    network.getVariantManager().allowVariantMultiThreadAccess(false);
    BOOST_CHECK_EQUAL("S1", network.getVariantManager().getWorkingVariantId());
}

BOOST_AUTO_TEST_CASE(testMultipleSetAllowMultiThreadTrue) {
    Network network = createNetwork();

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), "S1", true);
    network.getVariantManager().allowVariantMultiThreadAccess(true);

    const auto& updateVariantFunction = [&network]() {
        network.getVariantManager().allowVariantMultiThreadAccess(true);

        const std::string& message = stdcxx::format("Variant index not set for current thread %1%", std::this_thread::get_id());
        POWSYBL_ASSERT_THROW(network.getVariantManager().getVariantIndex(), PowsyblException, message.c_str());

        network.getVariantManager().setWorkingVariant("S1");
        BOOST_CHECK_EQUAL("S1", network.getVariantManager().getWorkingVariantId());

        network.getVariantManager().removeVariant("S1");
        POWSYBL_ASSERT_THROW(network.getVariantManager().getVariantIndex(), PowsyblException, message.c_str());

        network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    };

    std::thread launcher(updateVariantFunction);
    launcher.join();

    BOOST_CHECK_EQUAL(VariantManager::getInitialVariantId(), network.getVariantManager().getWorkingVariantId());
    network.getVariantManager().allowVariantMultiThreadAccess(false);
    BOOST_CHECK_EQUAL(VariantManager::getInitialVariantId(), network.getVariantManager().getWorkingVariantId());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
