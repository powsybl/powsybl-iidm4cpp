/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(EurostagRoundTrip)

BOOST_FIXTURE_TEST_CASE(EurostagRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(EurostagLfRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial1-lf.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(EurostagPropertiesRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1-properties.xml", IidmXmlVersion::all());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

