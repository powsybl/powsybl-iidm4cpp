/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <limits>
#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/Properties.hpp>

#include "iidm/converter/ConversionParameters.hpp"
#include "iidm/converter/Parameter.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ConversionParametersTestSuite)

BOOST_AUTO_TEST_CASE(testConversionParameters) {
    stdcxx::Properties properties;
    properties.set("test-param-boolean", "true");
    Parameter paramBoolean("test-param-boolean", Parameter::Type::BOOLEAN, "", "false");
    Parameter paramBoolean2("test-param-boolean2", Parameter::Type::BOOLEAN, "", "false");
    BOOST_CHECK(ConversionParameters::readBooleanParameter(properties, paramBoolean));
    BOOST_CHECK(ConversionParameters::readBooleanParameter(properties, paramBoolean));
    BOOST_CHECK(!ConversionParameters::readBooleanParameter(properties, paramBoolean2));

    properties.set("test-param-string", "TestProperty");
    Parameter paramString("test-param-string", Parameter::Type::STRING, "", "TestParam");
    Parameter paramString2("test-param-string2", Parameter::Type::STRING, "", "TestParam");
    BOOST_CHECK_EQUAL("TestProperty", ConversionParameters::readStringParameter(properties, paramString));
    BOOST_CHECK_EQUAL("TestProperty", ConversionParameters::readStringParameter(properties, paramString));
    BOOST_CHECK_EQUAL("TestParam", ConversionParameters::readStringParameter(properties, paramString2));

    std::vector<std::string> stringList;
    stringList.emplace_back("Test1Property");
    stringList.emplace_back("Test2Property");
    std::vector<std::string> stringList2;
    stringList2.emplace_back("Test1Param");
    stringList2.emplace_back("Test2Param");
    properties.set("test-param-string-list", boost::algorithm::join(stringList, ","));
    Parameter paramStringList("test-param-string-list", Parameter::Type::STRING_LIST, "", boost::algorithm::join(stringList2, ","));
    Parameter paramStringList2("test-param-string-list2", Parameter::Type::STRING_LIST, "", boost::algorithm::join(stringList2, ","));
    BOOST_CHECK_EQUAL("Test1Property", ConversionParameters::readStringListParameter(properties, paramStringList)[0]);
    BOOST_CHECK_EQUAL("Test2Property", ConversionParameters::readStringListParameter(properties, paramStringList)[1]);
    BOOST_CHECK_EQUAL("Test1Param", ConversionParameters::readStringListParameter(properties, paramStringList2)[0]);
    BOOST_CHECK_EQUAL("Test2Param", ConversionParameters::readStringListParameter(properties, paramStringList2)[1]);

    properties.set("test-param-double", "0.06");
    Parameter paramDouble("test-param-double", Parameter::Type::DOUBLE, "", "0.08");
    Parameter paramDouble2("test-param-double2", Parameter::Type::DOUBLE, "", "0.08");
    BOOST_CHECK_CLOSE(0.06, ConversionParameters::readDoubleParameter(properties, paramDouble), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.08, ConversionParameters::readDoubleParameter(properties, paramDouble2), std::numeric_limits<double>::epsilon());
}

}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
