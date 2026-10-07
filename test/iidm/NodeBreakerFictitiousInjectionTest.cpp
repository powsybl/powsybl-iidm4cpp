/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/test/AssertionUtils.hpp>


#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(NBKFictitiousInjectionTestSuite)


BOOST_AUTO_TEST_CASE(testGetFictitiousP0AndFictitiousQ0) {
    Network network = createNetworkTest1();
    Bus& bus = network.getVoltageLevel("voltageLevel1").getBusBreakerView().getBus("voltageLevel1_0");
    
    // Initial state
    BOOST_CHECK(std::isnan(bus.getFictitiousP0()));
    BOOST_CHECK(std::isnan(bus.getFictitiousQ0()));

    //Create a new variant
    VariantManager& variantManager = network.getVariantManager();
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), "duplicateState");

    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    bus.setFictitiousP0(10);
    bus.setFictitiousQ0(20);
    BOOST_CHECK_CLOSE(10, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());

    //duplicated not modified
    variantManager.setWorkingVariant("duplicateState");
    BOOST_CHECK(std::isnan(bus.getFictitiousP0()));
    BOOST_CHECK(std::isnan(bus.getFictitiousQ0()));

    //Clone a new variant
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), "otherDuplicateState");
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(10, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());
    variantManager.setWorkingVariant("otherDuplicateState");
    bus.getFictitiousP0();
    BOOST_CHECK_CLOSE(10, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());

    bus.setFictitiousP0(5);
    bus.setFictitiousQ0(10);
    BOOST_CHECK_CLOSE(5, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());

    //duplicated not modified, still no injections
    variantManager.setWorkingVariant("duplicateState");
    BOOST_CHECK(std::isnan(bus.getFictitiousP0()));
    BOOST_CHECK(std::isnan(bus.getFictitiousQ0()));

    // Testing getFictitious back in initialState, after removing fictitiousInjections
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(10, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());
    bus.setFictitiousP0(0.0);
    bus.setFictitiousQ0(0.0);
    BOOST_CHECK(std::isnan(bus.getFictitiousP0()));
    BOOST_CHECK(std::isnan(bus.getFictitiousQ0()));

    //delete variant and clone a new one
    variantManager.removeVariant("duplicateState");
    variantManager.cloneVariant("otherDuplicateState", "anotherDuplicateState");
    variantManager.setWorkingVariant("anotherDuplicateState");
    BOOST_CHECK_CLOSE(5, bus.getFictitiousP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10, bus.getFictitiousQ0(), std::numeric_limits<double>::epsilon());

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
