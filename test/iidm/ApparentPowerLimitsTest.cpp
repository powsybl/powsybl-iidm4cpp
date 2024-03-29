/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ApparentPowerLimitsTestSuite)

BOOST_AUTO_TEST_CASE(BranchApi) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Line& line = network.getLine("NHV1_NHV2_2");
    const Line& cLine = network.getLine("NHV1_NHV2_2");

    ApparentPowerLimits& apl = line.newApparentPowerLimits1()
        .setPermanentLimit(10)
        .beginTemporaryLimit()
        .setName("20'")
        .setValue(200.0)
        .setAcceptableDuration(30 * 60)
        .setFictitious(true)
        .endTemporaryLimit()
        .add();
    const ApparentPowerLimits& cApl = apl;

    BOOST_CHECK_EQUAL(LimitType::APPARENT_POWER, apl.getLimitType());

    BOOST_CHECK(stdcxx::areSame(apl, line.getApparentPowerLimits(Branch::Side::ONE).get()));
    BOOST_CHECK(stdcxx::areSame(apl, line.getApparentPowerLimits1().get()));
    BOOST_CHECK(!line.getApparentPowerLimits(Branch::Side::TWO));
    BOOST_CHECK(!line.getApparentPowerLimits2());

    BOOST_CHECK(stdcxx::areSame(cApl, cLine.getApparentPowerLimits(Branch::Side::ONE).get()));
    BOOST_CHECK(stdcxx::areSame(cApl, cLine.getApparentPowerLimits1().get()));
    BOOST_CHECK(!cLine.getApparentPowerLimits(Branch::Side::TWO));
    BOOST_CHECK(!cLine.getApparentPowerLimits2());

    apl.remove();
    BOOST_CHECK(!line.getApparentPowerLimits1());
    BOOST_CHECK(!line.getApparentPowerLimits(Branch::Side::ONE));

    POWSYBL_ASSERT_THROW(line.getApparentPowerLimits(static_cast<Branch::Side>(5)), AssertionError, "Unexpected Side value: 5");
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Line& line = network.getLine("NHV1_NHV2_2");
    ApparentPowerLimitsAdder apparentPowerLimitsAdder = line.newApparentPowerLimits1();
    apparentPowerLimitsAdder.setPermanentLimit(10);

    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': temporary limit value is not set");
    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().setValue(-1.0).endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': temporary limit value must be > 0");
    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().setValue(1.0).endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': acceptable duration is not set");
    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().setValue(1.0).setAcceptableDuration(20 * 60).endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': name is not set");

    apparentPowerLimitsAdder.beginTemporaryLimit()
            .setName("20'")
            .setValue(100.0)
            .setAcceptableDuration(20 * 60)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("20'")
            .ensureNameUnicity()
            .setValue(200.0)
            .setAcceptableDuration(30 * 60)
            .setFictitious(true)
        .endTemporaryLimit()
        .add();

    // to ensure adder reusability
    const ApparentPowerLimits& apl = apparentPowerLimitsAdder.add();

    const ApparentPowerLimits::TemporaryLimit& tl1 = apl.getTemporaryLimit(20 * 60);
    BOOST_CHECK_EQUAL("20'", tl1.getName());
    BOOST_CHECK_CLOSE(100.0, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!tl1.isFictitious());

    const ApparentPowerLimits::TemporaryLimit& tl2 = apl.getTemporaryLimit(30 * 60);
    BOOST_CHECK_EQUAL("20'#0", tl2.getName());
    BOOST_CHECK_CLOSE(200.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(tl2.isFictitious());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
