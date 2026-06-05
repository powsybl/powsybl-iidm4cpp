/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/AbstractOptions.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(AbstractOptionsTestSuite)

class AbstractOptionTest : public AbstractOptions<AbstractOptionTest> {

public:
    const stdcxx::optional<std::set<std::string>>& getIncludedExtensions() {
        return AbstractOptions::m_includedExtensions;
    }
    const stdcxx::optional<std::set<std::string>>& getExcludedExtensions() {
        return AbstractOptions::m_excludedExtensions;
    }

};

BOOST_AUTO_TEST_CASE(defaultConstructor) {
    AbstractOptionTest options;

    BOOST_CHECK(options.isWithAutomationSystems());
    BOOST_CHECK(!options.isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK(options.withAllExtensions());
    BOOST_CHECK(!options.withNoExtension());
}

BOOST_AUTO_TEST_CASE(testExcludeAndIncludedExtension) {
    AbstractOptionTest options;
    options.addIncludedExtension("loadFoo") // only "loadFoo" is included
            .addIncludedExtension("loadBar") // only "loadFoo" and "loadBar" are included
            .addExcludedExtension("loadBar"); // "loadBar" is excluded so only "loadFoo" is included

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK_EQUAL(1, includedExtensions->size());
    BOOST_CHECK(includedExtensions->count("loadFoo"));
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)

    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(!options.withExtension("loadBar"));
}

BOOST_AUTO_TEST_CASE(testIncludeAndExcludedExtension) {
    AbstractOptionTest options;
    options.addExcludedExtension("loadFoo") // only "loadFoo" is excluded
            .addExcludedExtension("loadBar") // only "loadFoo" and "loadBar" are excluded
            .addIncludedExtension("loadBar"); // "loadBar" is included so only "loadFoo" is excluded

    auto excludedExtensions = options.getExcludedExtensions();
    BOOST_CHECK(excludedExtensions.has_value());
    BOOST_CHECK_EQUAL(1, excludedExtensions->size());
    BOOST_CHECK(excludedExtensions->count("loadFoo"));
    BOOST_CHECK(!options.getIncludedExtensions()); // The second list is null (i.e., the optional is empty)

    BOOST_CHECK(options.withExtension("loadBar"));
    BOOST_CHECK(!options.withExtension("loadFoo"));
}

BOOST_AUTO_TEST_CASE(testIncludeAndExcludedOtherExtension) {
    AbstractOptionTest options;
    options.addExcludedExtension("loadFoo"); // only "loadFoo" is excluded
    options.addExcludedExtension("loadTest"); // only "loadFoo" and "loadTest" are excluded
    options.addIncludedExtension("loadBar"); // "loadBar" is not excluded, so it is already included
    options.addExcludedExtension("loadBar"); // only "loadFoo", "loadTest" and "loadBar" are excluded

    auto excludedExtensions = options.getExcludedExtensions();
    BOOST_CHECK(excludedExtensions.has_value());
    BOOST_CHECK(!options.getIncludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(3, excludedExtensions->size());
    BOOST_CHECK(!options.withAllExtensions());
    for (const auto& ext : {"loadFoo", "loadTest", "loadBar"}) {
        BOOST_CHECK(excludedExtensions->count(ext));
        BOOST_CHECK(!options.withExtension(ext));
    }
}

BOOST_AUTO_TEST_CASE(testExcludeAndIncludedOtherExtension) {
    AbstractOptionTest options;
    options.addIncludedExtension("loadFoo"); // only "loadFoo" is included
    options.addIncludedExtension("loadTest"); // only "loadFoo" and "loadTest" are included
    options.addExcludedExtension("loadBar"); // "loadBar" is not included, so it is already excluded
    options.addIncludedExtension("loadBar"); // only "loadFoo", "loadTest" and "loadBar" are included

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(3, includedExtensions->size());
    BOOST_CHECK(!options.withAllExtensions());
    for (const auto& ext : {"loadFoo", "loadTest", "loadBar"}) {
        BOOST_CHECK(includedExtensions->count(ext));
        BOOST_CHECK(options.withExtension(ext));
    }
}

BOOST_AUTO_TEST_CASE(testAddExcludedExtensionThenAddManyIncludedExtensions) {
    AbstractOptionTest options;
    options.addExcludedExtension("loadFoo"); // only "loadFoo" is excluded
    options.addIncludedExtension("loadTest"); // "loadTest" is not excluded, so it is already included
    options.addIncludedExtension("loadFoo"); // "loadFoo" is now removed from the exclusion list, so it is included
    options.addIncludedExtension("loadBar"); // "loadBar" is not excluded, so it is already included

    auto excludedExtensions = options.getExcludedExtensions();
    BOOST_CHECK(excludedExtensions.has_value());
    BOOST_CHECK(!options.getIncludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(0, excludedExtensions->size());
    
    BOOST_CHECK(options.withAllExtensions());
}

BOOST_AUTO_TEST_CASE(testAddIncludedExtensionThenAddManyExcludedExtensions) {
    AbstractOptionTest options;
    options.addIncludedExtension("loadFoo"); // only "loadFoo" is included
    options.addExcludedExtension("loadTest"); // "loadTest" is not included, so it is already excluded
    options.addExcludedExtension("loadFoo"); // "loadFoo" is now removed from the inclusion list, so it is excluded
    options.addExcludedExtension("loadBar"); // "loadBar" is not included, so it is already excluded

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(0, includedExtensions->size());

    BOOST_CHECK(options.withNoExtension());
}

BOOST_AUTO_TEST_CASE(testIncludeExtensionsAndSetExcludedExtensions) {
    AbstractOptionTest options;
    options.addIncludedExtension("loadFoo"); // only "loadFoo" is included
    options.setExcludedExtensions({"loadFoo", "loadBar"}); // only "loadFoo" and "loadBar" are excluded

    auto excludedExtensions = options.getExcludedExtensions();
    BOOST_CHECK(excludedExtensions.has_value());
    BOOST_CHECK(!options.getIncludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(2, excludedExtensions->size());
    BOOST_CHECK(excludedExtensions->count("loadFoo"));
    BOOST_CHECK(excludedExtensions->count("loadBar"));

    BOOST_CHECK(!options.withExtension("loadFoo"));
    BOOST_CHECK(!options.withExtension("loadBar"));
}

BOOST_AUTO_TEST_CASE(testExcludeExtensionsAndSetIncludedExtensions) {
    AbstractOptionTest options;
    options.addExcludedExtension("loadFoo"); // only "loadFoo" is excluded
    options.setIncludedExtensions({"loadFoo", "loadBar"}); // only "loadFoo" and "loadBar" are included

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(2, includedExtensions->size());
    BOOST_CHECK(includedExtensions->count("loadFoo"));
    BOOST_CHECK(includedExtensions->count("loadBar"));

    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(options.withExtension("loadBar"));
}

BOOST_AUTO_TEST_CASE(testSetExcludedThenSetEmptyIncluded) {
    AbstractOptionTest options;
    options.setExcludedExtensions({"loadFoo", "loadTest"}); // only "loadFoo" and "loadBar" are excluded
    options.setIncludedExtensions({}); // The included extensions list is empty: all extensions are excluded

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(0, includedExtensions->size());

    BOOST_CHECK(options.withNoExtension());
}

BOOST_AUTO_TEST_CASE(testSetIncludedThenSetExcluded) {
    AbstractOptionTest options;
    options.setIncludedExtensions({"loadFoo", "loadTest"}); // only "loadFoo" and "loadBar" are included
    options.setExcludedExtensions({"loadBar"}); // only "loadBar" is excluded

    auto excludedExtensions = options.getExcludedExtensions();
    BOOST_CHECK(excludedExtensions.has_value());
    BOOST_CHECK(!options.getIncludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(1, excludedExtensions->size());
    BOOST_CHECK(excludedExtensions->count("loadBar"));

    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(options.withExtension("loadTest"));
    BOOST_CHECK(!options.withExtension("loadBar"));
    BOOST_CHECK(!options.withNoExtension());
    BOOST_CHECK(!options.withAllExtensions());
}

BOOST_AUTO_TEST_CASE(testWithNoExtension) {
    AbstractOptionTest options;

    // Both lists are null
    BOOST_CHECK(!options.withNoExtension());

    // Define excluded extensions
    options.setExcludedExtensions({"loadBar"});
    BOOST_CHECK(!options.withNoExtension()); // We cannot say if there will be no extension or not, so false is returned

    // Define the list of included extensions as empty (also remove exluded list)
    options.setIncludedExtensions({});
    BOOST_CHECK(options.withNoExtension());

    // Define a list of included extensions
    options.setIncludedExtensions({"loadFoo"});
    BOOST_CHECK(!options.withNoExtension());
}

BOOST_AUTO_TEST_CASE(testWithAllExtensions) {
    AbstractOptionTest options;

    // Both lists are null
    BOOST_CHECK(options.withAllExtensions());

    // Define the list of excluded extensions as empty
    options.setExcludedExtensions({});
    BOOST_CHECK(options.withAllExtensions());

    // Define excluded extensions
    options.setExcludedExtensions({"loadBar"});
    BOOST_CHECK(!options.withAllExtensions());

    // Define the list of included extensions as empty
    options.setIncludedExtensions({});
    BOOST_CHECK(!options.withAllExtensions());

    // Define a list of included extensions
    options.setIncludedExtensions({"loadFoo"});
    BOOST_CHECK(!options.withAllExtensions());
}

BOOST_AUTO_TEST_CASE(testExcludeAndIncludedExtensionSuccessive) {
    AbstractOptionTest options;
    options.addIncludedExtension("loadFoo")
            .addIncludedExtension("loadBar") // only "loadFoo" and "loadBar" are included
            .addExcludedExtension("loadBar") // "loadBar" is excluded so only "loadFoo" is included
            .addExcludedExtension("loadTest"); // "loadTest" was not included before, still is not

    auto includedExtensions = options.getIncludedExtensions();
    BOOST_CHECK(includedExtensions.has_value());
    BOOST_CHECK(!options.getExcludedExtensions()); // The second list is null (i.e., the optional is empty)
    BOOST_CHECK_EQUAL(1, includedExtensions->size());
    BOOST_CHECK(includedExtensions->count("loadFoo"));

    BOOST_CHECK(options.withExtension("loadFoo"));
    BOOST_CHECK(!options.withExtension("loadTest"));
    BOOST_CHECK(!options.withExtension("loadBar"));

}



BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
