/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(PropertiesTestSuite)

BOOST_AUTO_TEST_CASE(AddProperty) {
    Properties props;
    BOOST_CHECK_EQUAL(0UL, props.size());
    BOOST_CHECK_EQUAL(true, props.isEmpty());

    props.set("key1", "value1");
    BOOST_CHECK_EQUAL(1UL, props.size());
    BOOST_CHECK_EQUAL(false, props.isEmpty());

    props.set("key2", "value2");
    BOOST_CHECK_EQUAL(2UL, props.size());

    props.set("key3", "value3");
    BOOST_CHECK_EQUAL(3UL, props.size());

    props.set("key4", "value4");
    BOOST_CHECK_EQUAL(4UL, props.size());

    props.set("key5", "value5")
         .set("key6", "value6")
         .set("key7", "value7")
         .set("key8", "value8");
    BOOST_CHECK_EQUAL(8UL, props.size());

    props.set("key8", "newValue");
    BOOST_CHECK_EQUAL(props.get("key8"), "newValue");

    BOOST_CHECK(props.contains("key5"));
    BOOST_CHECK(!props.contains("key9"));
}

BOOST_AUTO_TEST_CASE(RemoveProperty) {
    Properties props;

    props.set("key1", "value1")
         .set("key2", "value2")
         .set("key3", "value3")
         .set("key4", "value4");
    BOOST_CHECK_EQUAL(4UL, props.size());

    props.remove("key3");
    BOOST_CHECK_EQUAL(3UL, props.size());

    props.remove("key4");
    BOOST_CHECK_EQUAL(2UL, props.size());

    // removing a key that does not exists does nothing
    props.remove("key5");
    BOOST_CHECK_EQUAL(2UL, props.size());

    char suffix = '1';
    for (const auto& it : props) {
        BOOST_CHECK_EQUAL(std::string("key") + suffix, it.first);
        BOOST_CHECK_EQUAL(std::string("value") + suffix, it.second);
        suffix++;
    }

    props.clear();
    BOOST_CHECK_EQUAL(0UL, props.size());
    BOOST_CHECK_EQUAL(true, props.isEmpty());
}

BOOST_AUTO_TEST_CASE(GetProperty) {
    Properties props;

    props.set("key1", "value1")
         .set("key2", "value2")
         .set("key3", "value3")
         .set("key4", "value4");

    BOOST_CHECK_EQUAL(4UL, props.size());

    BOOST_CHECK_EQUAL(props.get("key1"), "value1");
    BOOST_CHECK_EQUAL(props.get("key2"), "value2");
    BOOST_CHECK_EQUAL(props.get("key3"), "value3");
    BOOST_CHECK_EQUAL(props.get("key4"), "value4");

    BOOST_CHECK_EQUAL(4UL, props.size());

    POWSYBL_ASSERT_THROW(props.get("key9"),
                         powsybl::PowsyblException, "Property key9 does not exist");
    BOOST_CHECK_EQUAL(props.get("key9", "value9"), "value9");

    POWSYBL_ASSERT_THROW(props.get("key9"),
                         powsybl::PowsyblException, "Property key9 does not exist");
}

BOOST_AUTO_TEST_CASE(networkProperties) {
    Network n("test", "test");

    BOOST_CHECK_EQUAL(n.getProperties().size(), 0UL);

    n.getProperties().set("key1", "value1").set("key2", "value2");
    BOOST_CHECK_EQUAL(n.getProperties().size(), 2UL);

    BOOST_CHECK_EQUAL(n.getProperties().get("key1"), "value1");

    POWSYBL_ASSERT_THROW(n.getProperties().get("key3"),
                         powsybl::PowsyblException, "Property key3 does not exist");

    n.getProperties().remove("key1");
    BOOST_CHECK_EQUAL(n.getProperties().size(), 1UL);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
