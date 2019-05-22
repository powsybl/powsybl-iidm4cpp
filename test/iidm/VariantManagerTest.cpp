/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/ContainerLogger.hpp>
#include <powsybl/logging/LogMessage.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(VariantManagerTestSuite)

BOOST_AUTO_TEST_CASE(integrity)
{
    Network network = createNetwork();

    BOOST_CHECK_EQUAL(1ul, network.getLoadCount());

    VariantManager& variantManager = network.getVariantManager();
    BOOST_CHECK_EQUAL(0ul, variantManager.getVariantIndex());

    POWSYBL_ASSERT_THROW(variantManager.cloneVariant(VariantManager::getInitialVariantId(), {}), PowsyblException, "Empty target variant id list");
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, variantManager.getVariantArraySize());
    std::set<std::string> expected = {VariantManager::getInitialVariantId(), "s1", "s2"};
    std::set<std::string> actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());


    variantManager.setWorkingVariant("s1");
    BOOST_CHECK_EQUAL(1ul, variantManager.getVariantIndex());
    variantManager.setWorkingVariant("s2");
    BOOST_CHECK_EQUAL(2ul, variantManager.getVariantIndex());
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL(0ul, variantManager.getVariantIndex());

    variantManager.removeVariant("s1");
    BOOST_CHECK_EQUAL(3ul, variantManager.getVariantArraySize());
    expected = {VariantManager::getInitialVariantId(), "s2"};
    actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());

    POWSYBL_ASSERT_THROW(variantManager.cloneVariant("s2", "s2"), PowsyblException, "Target variant 's2' already exists");
    POWSYBL_ASSERT_THROW(variantManager.cloneVariant("s4", "s3"), PowsyblException, "Variant 's4' not found");
    variantManager.cloneVariant("s2", "s3", false);
    POWSYBL_ASSERT_THROW(variantManager.cloneVariant("s2", "s3", false), PowsyblException, "Target variant 's3' already exists");
    BOOST_CHECK_NO_THROW(variantManager.cloneVariant("s2", "s3", true));
    variantManager.setWorkingVariant("s3");
    expected = {VariantManager::getInitialVariantId(), "s2", "s3"};
    actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());

    POWSYBL_ASSERT_THROW(variantManager.cloneVariant("s2", {"s2", "s3"}, false), PowsyblException, "Target variant 's2' already exists");
    BOOST_CHECK_NO_THROW(variantManager.cloneVariant("s2", {"s2", "s3"}, true));

    POWSYBL_ASSERT_THROW(variantManager.cloneVariant("s2", {"s1", "s3"}, false), PowsyblException, "Target variant 's3' already exists");
    BOOST_CHECK_EQUAL(4ul, variantManager.getVariantArraySize());
    expected = {VariantManager::getInitialVariantId(), "s1", "s2", "s3"};
    actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
    std::set<unsigned long> expectedIndices = {0UL, 1UL, 2UL, 3UL};
    std::set<unsigned long> actualIndices = variantManager.getVariantIndices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedIndices.cbegin(), expectedIndices.cend(), actualIndices.cbegin(), actualIndices.cend());

    const std::string workingVariantId = variantManager.getWorkingVariantId();
    BOOST_CHECK_EQUAL("s3", workingVariantId);
    BOOST_CHECK_EQUAL(1ul, variantManager.getVariantIndex());
    variantManager.removeVariant("s3");
    BOOST_CHECK_EQUAL(4ul, variantManager.getVariantArraySize());
    expected = {VariantManager::getInitialVariantId(), "s1", "s2"};
    actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
    POWSYBL_ASSERT_THROW(variantManager.removeVariant(workingVariantId), PowsyblException, "Variant 's3' not found");

    variantManager.removeVariant("s2");
    expected = {VariantManager::getInitialVariantId(), "s1"};
    actual = variantManager.getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());

    const Network& cNetwork = network;
    expected = variantManager.getVariantIds();
    actual = cNetwork.getVariantManager().getVariantIds();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend());
    POWSYBL_ASSERT_THROW(cNetwork.getVariantManager().getWorkingVariantId(), PowsyblException, "Variant index not set");
    const std::string& initialVariantId = VariantManager::getInitialVariantId();
    BOOST_CHECK_EQUAL("InitialVariant", initialVariantId);
    POWSYBL_ASSERT_THROW(variantManager.removeVariant(initialVariantId), PowsyblException, "Removing initial variant is forbidden");
}

BOOST_AUTO_TEST_CASE(logs)
{
    logging::LoggerFactory::getInstance().addLogger("powsybl::iidm::VariantManager", stdcxx::make_unique<logging::ContainerLogger>());

    Network network = createNetwork();
    logging::ContainerLogger& logger = dynamic_cast<logging::ContainerLogger&>(logging::LoggerFactory::getLogger("powsybl::iidm::VariantManager"));

    VariantManager& variantManager = network.getVariantManager();
    BOOST_CHECK_EQUAL(0ul, logger.size());
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, variantManager.getVariantArraySize());
    BOOST_CHECK_EQUAL(2ul, logger.size());
    int indexLog = 0;
    powsybl::logging::LogMessage logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Creating variants [s1, s2]", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logMessage.getLevel());

    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Extending variant array size to 3 (+2)", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::TRACE, logMessage.getLevel());

    variantManager.removeVariant("s1");
    BOOST_CHECK_EQUAL(4ul, logger.size());
    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Removing variant 's1'", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logMessage.getLevel());

    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Deleting variant array element at index 1", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::TRACE, logMessage.getLevel());

    variantManager.cloneVariant("s2", "s3", false);
    BOOST_CHECK_EQUAL(6ul, logger.size());
    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Creating variants [s3]", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logMessage.getLevel());

    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Recycling variant array indexes {1}", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::TRACE, logMessage.getLevel());

    BOOST_CHECK_NO_THROW(variantManager.cloneVariant("s2", "s3", true));
    BOOST_CHECK_EQUAL(8ul, logger.size());
    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Creating variants [s3]", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logMessage.getLevel());

    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Overwriting variant array indexes {1}", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::TRACE, logMessage.getLevel());

    variantManager.removeVariant("s2");
    BOOST_CHECK_EQUAL(10ul, logger.size());
    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Removing variant 's2'", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::DEBUG, logMessage.getLevel());

    logMessage = logger.getLogMessage(indexLog++);
    BOOST_CHECK_EQUAL("Reducing variant array size to 2", logMessage.getMessage());
    POWSYBL_ASSERT_ENUM_EQ(logging::Level::TRACE, logMessage.getLevel());

    logging::LoggerFactory::getInstance().removeLogger("powsybl::iidm::VariantManager");
}

BOOST_AUTO_TEST_CASE(constructor)
{
    Network network = createNetwork();
    VariantManager& variantManger = network.getVariantManager();

    variantManger.cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, variantManger.getVariantArraySize());
    VariantManager variantManager2(std::move(variantManger));
    BOOST_CHECK_EQUAL(3ul, variantManager2.getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(forEachVariantTest)
{
    Network network = createNetwork();
    VariantManager& variantManger = network.getVariantManager();

    variantManger.cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3ul, variantManger.getVariantArraySize());
    unsigned long variantCount = 0UL;
    variantManger.forEachVariant([&variantCount]() {
        variantCount++;
    });
    BOOST_CHECK_EQUAL(3ul, variantCount);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
