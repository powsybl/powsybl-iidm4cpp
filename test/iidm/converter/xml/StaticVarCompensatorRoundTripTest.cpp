/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(StaticVarCompensatorRoundTrip)

BOOST_FIXTURE_TEST_CASE(StaticVarCompensatorRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("staticVarCompensatorRoundTripRef.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(RegulatingStaticVarCompensatorTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("regulatingStaticVarCompensatorRoundTripRef.xml", IidmXmlVersion::V1_1());
}

BOOST_FIXTURE_TEST_CASE(FaultyRegulatingStaticVarCompensatorTest, test::ResourceFixture) {
    POWSYBL_ASSERT_THROW(test::converter::RoundTrip::roundTripVersionedXmlTest("faultyRegulatingStaticVarCompensatorRoundTripRef.xml", IidmXmlVersion::V1_0()),
        powsybl::PowsyblException, "staticVarCompensator.regulatingTerminal is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
