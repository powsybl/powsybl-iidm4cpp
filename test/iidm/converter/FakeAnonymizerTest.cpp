/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/FakeAnonymizer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(FakeAnonymizerTestSuite)

BOOST_AUTO_TEST_CASE(fakeAnonymizationTest) {
    FakeAnonymizer anonymizer;

    BOOST_CHECK_EQUAL("test1", anonymizer.anonymizeString("test1"));
    BOOST_CHECK_EQUAL(static_cast<unsigned int>(Country::FI),
                      static_cast<unsigned int>(anonymizer.anonymizeCountry(Country::FI)));

    BOOST_CHECK_EQUAL("test2", anonymizer.deanonymizeString("test2"));
    BOOST_CHECK_EQUAL(static_cast<unsigned int>(Country::BE),
                      static_cast<unsigned int>(anonymizer.deanonymizeCountry(Country::BE)));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
