/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE iidm
#include <boost/test/unit_test.hpp>

#include <powsybl/test/ExtensionFixture.hpp>
#include <powsybl/test/XmlFixture.hpp>

namespace powsybl {

namespace test {

BOOST_TEST_GLOBAL_FIXTURE(ExtensionFixture);
BOOST_TEST_GLOBAL_FIXTURE(XmlFixture);

}  // namespace test

}  // namespace powsybl
