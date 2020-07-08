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

    options.addExtension("test");
    BOOST_CHECK(options.withExtension("test"));
    BOOST_CHECK(!options.withExtension("test2"));
}

BOOST_AUTO_TEST_CASE(initFromProperties) {
    stdcxx::Properties properties;
    properties.set(ImportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, "true");
    properties.set(ImportOptions::EXTENSIONS_LIST, "");

    ImportOptions options(properties);

    BOOST_CHECK(options.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkAllExtensions) {
    stdcxx::Properties properties;
    ImportOptions options(properties);
    BOOST_CHECK(options.withExtension("abc"));
    BOOST_CHECK(options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkNoExtension) {
    stdcxx::Properties properties;
    properties.set(ImportOptions::EXTENSIONS_LIST, "");

    ImportOptions options(properties);
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));
}

BOOST_AUTO_TEST_CASE(checkSomeExtensions) {
    stdcxx::Properties properties;

    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo,loadBar");
    ImportOptions options(properties);
    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(options.withExtension("loadBar"));
    BOOST_CHECK(!options.withExtension("abc"));
    BOOST_CHECK(!options.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo:loadBar");
    ImportOptions options2(properties);
    BOOST_CHECK(options2.withExtension("loadFoo"));
    BOOST_CHECK(options2.withExtension("loadBar"));
    BOOST_CHECK(!options2.withExtension("abc"));
    BOOST_CHECK(!options2.withExtension("def"));

    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo");
    ImportOptions options3(properties);
    BOOST_CHECK(options3.withExtension("loadFoo"));
    BOOST_CHECK(!options3.withExtension("loadBar"));
    BOOST_CHECK(!options3.withExtension("abc"));
    BOOST_CHECK(!options3.withExtension("def"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
