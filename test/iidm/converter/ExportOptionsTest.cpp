/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ExportOptionsTestSuite)

BOOST_AUTO_TEST_CASE(defaultConstructor) {
    ExportOptions options;

    BOOST_CHECK(!options.isAnonymized());
    options.setAnonymized(true);
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

    BOOST_TEST("", options.getVersion());
    options.setVersion("V1.0");
    BOOST_TEST("V1.0", options.getVersion());

    BOOST_CHECK(options.withExtension("abc"));
    BOOST_CHECK(options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(constructor) {
    ExportOptions options(true,  // withBranchSV
                          false,  // indent
                          true,  // onlyMainCc
                          TopologyLevel::BUS_BREAKER,  // topologyLevel
                          true,  // throwExceptionIfExtensionNotFound
                          "V1.0",
                          ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);

    BOOST_CHECK(!options.isIndent());
    BOOST_CHECK(options.isOnlyMainCc());
    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK_EQUAL(static_cast<int>(TopologyLevel::BUS_BREAKER), static_cast<int>(options.getTopologyLevel()));
    BOOST_CHECK(options.isWithBranchSV());
    BOOST_TEST("V1.0", options.getVersion());
    BOOST_CHECK_EQUAL(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION, options.getIidmVersionIncompatibilityBehavior());

    ExportOptions options2(true,  // withBranchSV
                          false,  // indent
                          true,  // onlyMainCc
                          TopologyLevel::BUS_BREAKER,  // topologyLevel
                          true,  // throwExceptionIfExtensionNotFound
                          "V1.0");
    BOOST_CHECK(!options2.isIndent());
    BOOST_CHECK(options2.isOnlyMainCc());
    BOOST_CHECK(options2.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK_EQUAL(TopologyLevel::BUS_BREAKER, options2.getTopologyLevel());
    BOOST_CHECK(options2.isWithBranchSV());
    BOOST_TEST("V1.0", options2.getVersion());
    BOOST_CHECK_EQUAL(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION, options2.getIidmVersionIncompatibilityBehavior());
}

BOOST_AUTO_TEST_CASE(initFromProperties) {
    stdcxx::Properties properties;
    properties.set(ExportOptions::ANONYMISED, "true");
    properties.set(ExportOptions::INDENT, "false");
    properties.set(ExportOptions::ONLY_MAIN_CC, "false");
    properties.set(ExportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, "true");
    properties.set(ExportOptions::TOPOLOGY_LEVEL, "NODE_BREAKER");
    properties.set(ExportOptions::WITH_BRANCH_STATE_VARIABLES, "true");
    properties.set(ExportOptions::EXTENSIONS_LIST, "");
    properties.set(ExportOptions::VERSION, "1.0");
    properties.set(ExportOptions::IIDM_VERSION_INCOMPATIBILITY_BEHAVIOR, "LOG_ERROR");

    ExportOptions options(properties);

    BOOST_CHECK(options.isAnonymized());
    BOOST_CHECK(!options.isIndent());
    BOOST_CHECK(!options.isOnlyMainCc());
    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK_EQUAL(TopologyLevel::NODE_BREAKER, options.getTopologyLevel());
    BOOST_CHECK(options.isWithBranchSV());
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
    BOOST_CHECK_EQUAL("1.0", options.getVersion());
    BOOST_CHECK_EQUAL(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR, options.getIidmVersionIncompatibilityBehavior());
}

BOOST_AUTO_TEST_CASE(checkAllExtensions) {
    stdcxx::Properties properties;
    ExportOptions options(properties);
    BOOST_CHECK(options.withExtension("abc"));
    BOOST_CHECK(options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkNoExtension) {
    stdcxx::Properties properties;
    properties.set(ExportOptions::EXTENSIONS_LIST, "");

    ExportOptions options(properties);
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkSomeExtensions) {
    stdcxx::Properties properties;

    properties.set(ExportOptions::EXTENSIONS_LIST, "loadFoo,loadBar");
    ExportOptions options(properties);
    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(options.withExtension("loadBar"));
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));

    properties.set(ExportOptions::EXTENSIONS_LIST, "loadFoo:loadBar");
    ExportOptions options2(properties);
    BOOST_CHECK(options2.withExtension("loadFoo"));
    BOOST_CHECK(options2.withExtension("loadBar"));
    BOOST_CHECK(!options2.withExtension("abc"));
    BOOST_CHECK(!options2.withExtension("def"));

    properties.set(ExportOptions::EXTENSIONS_LIST, "loadFoo");
    ExportOptions options3(properties);
    BOOST_CHECK(options3.withExtension("loadFoo"));
    BOOST_CHECK(!options3.withExtension("loadBar"));
    BOOST_CHECK(!options3.withExtension("abc"));
    BOOST_CHECK(!options3.withExtension("def"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
