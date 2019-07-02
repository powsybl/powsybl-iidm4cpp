/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/ExportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ExportOptionsTestSuite)

BOOST_AUTO_TEST_CASE(defaultConstructor) {
    ExportOptions options;

    BOOST_CHECK(!options.isAnonymized());
    options.setAnonymised(true);
    BOOST_CHECK(options.isAnonymized());

    BOOST_CHECK(options.isIndent());
    options.setIndent(false);
    BOOST_CHECK(!options.isIndent());

    BOOST_CHECK(!options.isOnlyMainCc());
    options.setOnlyMainCc(true);
    BOOST_CHECK(options.isOnlyMainCc());

    BOOST_CHECK(!options.isThrowExceptionIfExtensionNotFound());
    options.setThrowExceptionIfExtensionNotFound(true);
    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());

    BOOST_CHECK_EQUAL(static_cast<int>(TopologyLevel::NODE_BREAKER), static_cast<int>(options.getTopologyLevel()));
    options.setTopologyLevel(TopologyLevel::BUS_BREAKER);
    BOOST_CHECK_EQUAL(static_cast<int>(TopologyLevel::BUS_BREAKER), static_cast<int>(options.getTopologyLevel()));

    BOOST_CHECK(options.isWithBranchSV());
    options.setWithBranchSV(false);
    BOOST_CHECK(!options.isWithBranchSV());
}

BOOST_AUTO_TEST_CASE(constructor) {
    ExportOptions options(true, // withBranchSV
                          false, // indent
                          true, // onlyMainCc
                          TopologyLevel::BUS_BREAKER,  // topologyLevel
                          true);// throwExceptionIfExtensionNotFound

    BOOST_CHECK(!options.isIndent());

    BOOST_CHECK(options.isOnlyMainCc());

    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());

    BOOST_CHECK_EQUAL(static_cast<int>(TopologyLevel::BUS_BREAKER), static_cast<int>(options.getTopologyLevel()));

    BOOST_CHECK(options.isWithBranchSV());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
