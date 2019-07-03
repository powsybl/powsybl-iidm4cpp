/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include "../src/iidm/converter/Version.hpp"  // TODO(mathbagu): to be checked add include path Vs use relative includes

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(VersionTest)

BOOST_AUTO_TEST_CASE(VersionOperatorsTest) {

    BOOST_CHECK(Version("1_10") == Version("1_10"));
    BOOST_CHECK(Version("1_10") == "1_10");

    BOOST_CHECK(Version("1_0") < Version("1_1"));
    BOOST_CHECK(Version("1_2") < Version("1_10"));
    BOOST_CHECK(Version("1_2") < "1_10");

    BOOST_CHECK(Version("1_30") > Version("1_10"));
    BOOST_CHECK(Version("1_30") > "1_10");

    BOOST_CHECK(Version("1_10") >= Version("1_0"));
    BOOST_CHECK(Version("1_10") >= Version("1_10"));
    BOOST_CHECK(Version("1_10") >= "1_0");
    BOOST_CHECK(Version("1_10") >= "1_10");

    BOOST_CHECK(Version("1_10") <= Version("1_10"));
    BOOST_CHECK(Version("1_0") <= Version("1_10"));
    BOOST_CHECK(Version("1_10") <= "1_10");
    BOOST_CHECK(Version("1_0") <= "1_10");
    BOOST_CHECK("1_10" >= Version("1_0"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
