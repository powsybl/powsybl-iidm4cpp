/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/test/unit_test.hpp>

#include "iidm/converter/Parameter.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ParameterTestSuite)

BOOST_AUTO_TEST_CASE(scopeTest) {

    auto param = Parameter("test-param", Parameter::Type::BOOLEAN, "", "false");
    BOOST_CHECK(Parameter::Scope::FUNCTIONAL==param.getScope());
    auto param2 = Parameter("test-param2", Parameter::Type::BOOLEAN, "", "false", Parameter::Scope::TECHNICAL);
    BOOST_CHECK(Parameter::Scope::TECHNICAL==param2.getScope());

}

BOOST_AUTO_TEST_CASE(categoryKeyTest) {

    auto param = Parameter("test-param", Parameter::Type::BOOLEAN, "", "false");
    BOOST_CHECK(param.getCategoryKey().empty());
    auto param2 = Parameter("test-param2", Parameter::Type::BOOLEAN, "", "false", Parameter::Scope::TECHNICAL);
    BOOST_CHECK(param2.getCategoryKey().empty());
    auto param3 = Parameter("test-param3", Parameter::Type::BOOLEAN, "", "false", Parameter::Scope::TECHNICAL, "Key");
    BOOST_CHECK_EQUAL("Key", param3.getCategoryKey());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
