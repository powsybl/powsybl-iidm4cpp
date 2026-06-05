/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(ImportOptionsTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    ImportOptions options;

    BOOST_CHECK(!options.isThrowExceptionIfExtensionNotFound());
    options.setThrowExceptionIfExtensionNotFound(true);
    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());

    BOOST_CHECK(options.isWithAutomationSystems());
    options.setWithAutomationSystems(false);
    BOOST_CHECK(!options.isWithAutomationSystems());

    BOOST_CHECK_CLOSE(100.0, options.getMissingPermanentLimitPercentage(), std::numeric_limits<double>::epsilon());
    options.setMissingPermanentLimitPercentage(95.0);
    BOOST_CHECK_CLOSE(95.0, options.getMissingPermanentLimitPercentage(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(!options.getMinimalValidationLevel().has_value());
    options.setMinimalValidationLevel("STEADY_STATE_HYPOTHESIS");
    BOOST_CHECK(options.getMinimalValidationLevel().has_value());
    BOOST_CHECK_EQUAL(options.getMinimalValidationLevel().get(), ValidationLevel::STEADY_STATE_HYPOTHESIS);

    options.addIncludedExtension("test");
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(!options.withExtension("test2"));

    options.resetExtensions(); //All Included
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(options.withExtension("test2"));
    BOOST_CHECK(options.withExtension("abc"));

    options.setIncludedExtensions({"test","test3"});
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(!options.withExtension("test2"));
    BOOST_CHECK(options.withExtension("test3"));
    BOOST_CHECK(!options.withExtension("abc"));

    options.addExcludedExtension("test3");
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(!options.withExtension("test2"));
    BOOST_CHECK(!options.withExtension("test3"));
    BOOST_CHECK(!options.withExtension("abc"));

    options.addExcludedExtension("test"); //Included list now empty : all Excluded
    BOOST_CHECK(!options.withExtension("test"));
    BOOST_CHECK(!options.withExtension("test2"));
    BOOST_CHECK(!options.withExtension("test3"));
    BOOST_CHECK(!options.withExtension("abc"));

    options.resetExtensions(); //All Included
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(options.withExtension("test2"));
    BOOST_CHECK(options.withExtension("test3"));
    BOOST_CHECK(options.withExtension("abc"));

}

BOOST_AUTO_TEST_CASE(initFromProperties) {
    stdcxx::Properties properties;
    properties.set(ImportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, "true");
    properties.set(ImportOptions::EXTENSIONS_INCLUDED_LIST, ""); //All Excluded
    properties.set(ImportOptions::WITH_AUTOMATION_SYSTEMS, "false");
    properties.set(ImportOptions::MISSING_PERMANENT_LIMIT_PERCENTAGE, "50.0");
    properties.set(ImportOptions::MINIMAL_VALIDATION_LEVEL, "STEADY_STATE_HYPOTHESIS");

    ImportOptions options(properties);

    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK(!options.isWithAutomationSystems());
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
    BOOST_CHECK_CLOSE(50.0, options.getMissingPermanentLimitPercentage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(options.getMinimalValidationLevel().has_value());
    BOOST_CHECK_EQUAL(options.getMinimalValidationLevel().get(), ValidationLevel::STEADY_STATE_HYPOTHESIS);
}

BOOST_AUTO_TEST_CASE(checkAllExtensions) {
    stdcxx::Properties properties;
    ImportOptions options(properties);
    BOOST_CHECK(options.withExtension("abc"));
    BOOST_CHECK(options.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_EXCLUDED_LIST, "");
    ImportOptions options2(properties);
    BOOST_CHECK(options2.withExtension("abc"));
    BOOST_CHECK(options2.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkNoExtension) {
    stdcxx::Properties properties;
    properties.set(ImportOptions::EXTENSIONS_INCLUDED_LIST, "");

    ImportOptions options(properties);
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkSomeExtensions) {
    stdcxx::Properties properties;

    properties.set(ImportOptions::EXTENSIONS_INCLUDED_LIST, "loadFoo,loadBar");
    ImportOptions options(properties);
    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(options.withExtension("loadBar"));
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_INCLUDED_LIST, "loadFoo:loadBar");
    ImportOptions options2(properties);
    BOOST_CHECK(options2.withExtension("loadFoo"));
    BOOST_CHECK(options2.withExtension("loadBar"));
    BOOST_CHECK(!options2.withExtension("abc"));
    BOOST_CHECK(!options2.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_INCLUDED_LIST, "loadFoo");
    ImportOptions options3(properties);
    BOOST_CHECK(options3.withExtension("loadFoo"));
    BOOST_CHECK(!options3.withExtension("loadBar"));
    BOOST_CHECK(!options3.withExtension("abc"));
    BOOST_CHECK(!options3.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_EXCLUDED_LIST, "loadFoo,loadBar");

    POWSYBL_ASSERT_THROW(new ImportOptions(properties), PowsyblException, "You can't define both included and excluded extensions in parameters.");
    properties.remove(ImportOptions::EXTENSIONS_INCLUDED_LIST);
    ImportOptions options4(properties);
    BOOST_CHECK(!options4.withExtension("loadFoo"));
    BOOST_CHECK(!options4.withExtension("loadBar"));
    BOOST_CHECK(options4.withExtension("abc"));
    BOOST_CHECK(options4.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_EXCLUDED_LIST, "loadBar");
    ImportOptions options5(properties);
    BOOST_CHECK(options5.withExtension("loadFoo"));
    BOOST_CHECK(!options5.withExtension("loadBar"));
    BOOST_CHECK(options5.withExtension("abc"));
    BOOST_CHECK(options5.withExtension("def"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
