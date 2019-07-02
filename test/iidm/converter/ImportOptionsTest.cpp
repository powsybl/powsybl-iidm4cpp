/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/ImportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ImportOptionsTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    ImportOptions options;

    BOOST_CHECK(!options.isThrowExceptionIfExtensionNotFound());
    options.setThrowExceptionIfExtensionNotFound(true);
    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
