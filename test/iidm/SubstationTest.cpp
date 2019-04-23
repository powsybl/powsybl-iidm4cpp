/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"
#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(SubstationTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createNetwork();

    BOOST_CHECK_EQUAL(1ul, network.getSubstationCount());

    Substation& substation = network.getSubstation("S1");
    BOOST_CHECK_EQUAL("S1", substation.getId());
    BOOST_CHECK_EQUAL("S1_NAME", substation.getName());
    POWSYBL_ASSERT_ENUM_EQ(Country::FR, substation.getCountry());
    BOOST_CHECK_EQUAL("TSO", substation.getTso());
}

BOOST_AUTO_TEST_CASE(country) {
    Network network("id", "test");

    SubstationAdder substationAdder = network.newSubstation();
    substationAdder
        .setId("S");
    POWSYBL_ASSERT_THROW(substationAdder.add(), ValidationException, "Substation 'S': Country is not set");
    BOOST_CHECK_EQUAL(0ul, network.getSubstationCount());

    substationAdder.setCountry(Country::FR);
    substationAdder.setGeographicalTags({"FR", "DE"});
    substationAdder.addGeographicalTag("IT");
    BOOST_CHECK_NO_THROW(substationAdder.add());
    BOOST_CHECK_EQUAL(1ul, network.getSubstationCount());

    Substation& substation = network.getSubstation("S");
    const Substation& cSubstation = network.getSubstation("S");
    BOOST_TEST(stdcxx::areSame(substation, cSubstation));
    BOOST_TEST(stdcxx::areSame(network, cSubstation.getNetwork()));
    POWSYBL_ASSERT_ENUM_EQ(Country::BE, substation.setCountry(Country::BE).setTso("TSO2").getCountry());
    BOOST_CHECK_EQUAL("TSO2", substation.getTso());
    BOOST_CHECK_EQUAL(3, substation.getGeographicalTags().size());
    substation.addGeographicalTag("LU");
    BOOST_CHECK_EQUAL(4, substation.getGeographicalTags().size());
    substation.addGeographicalTag("IT");
    BOOST_CHECK_EQUAL(4, substation.getGeographicalTags().size());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
