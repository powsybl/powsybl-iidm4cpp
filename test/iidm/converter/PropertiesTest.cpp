/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/iidm/converter/Properties.hpp>
#include <powsybl/logging/MessageFormat.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ConverterPropertiesTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    converter::Properties properties;

    properties.put("boolTrue", "true");
    properties.put("boolFalse", "false");
    BOOST_CHECK(properties.get<bool>("boolTrue"));
    BOOST_CHECK(!properties.get<bool>("boolFalse"));
    BOOST_CHECK_EQUAL("true", properties.get<std::string>("boolTrue"));

    BOOST_CHECK(properties.contains("boolTrue"));
    BOOST_CHECK(!properties.contains("missingAttr"));
    POWSYBL_ASSERT_THROW(properties.get<std::string>("missingAttr"), PowsyblException, "Property missingAttr does not exist");
}

BOOST_AUTO_TEST_CASE(initFromMap) {
    std::map<std::string, std::string> map;
    map[INDENT] = "true";
    map[THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND] = "false";

    converter::Properties properties(map);

    BOOST_CHECK(properties.get<bool>(INDENT));
    BOOST_CHECK(!properties.get<bool>(THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND));
    BOOST_CHECK(!properties.contains(TOPOLOGY_LEVEL));
    POWSYBL_ASSERT_THROW(properties.get<std::string>(TOPOLOGY_LEVEL), PowsyblException, "Property iidm.export.xml.topology-level does not exist");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
