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

    ApparentPowerLimits& apl = line.getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits()
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

    BOOST_CHECK(stdcxx::areSame(apl, line.getApparentPowerLimits(TwoSides::ONE).get()));
    BOOST_CHECK(stdcxx::areSame(apl, line.getApparentPowerLimits1().get()));
    BOOST_CHECK(!line.getApparentPowerLimits(TwoSides::TWO));
    BOOST_CHECK(!line.getApparentPowerLimits2());

    BOOST_CHECK(stdcxx::areSame(cApl, cLine.getApparentPowerLimits(TwoSides::ONE).get()));
    BOOST_CHECK(stdcxx::areSame(cApl, cLine.getApparentPowerLimits1().get()));
    BOOST_CHECK(!cLine.getApparentPowerLimits(TwoSides::TWO));
    BOOST_CHECK(!cLine.getApparentPowerLimits2());

    apl.remove();
    BOOST_CHECK(!line.getApparentPowerLimits1());
    BOOST_CHECK(!line.getApparentPowerLimits(TwoSides::ONE));

    POWSYBL_ASSERT_THROW(line.getApparentPowerLimits(static_cast<TwoSides>(5)), AssertionError, "Unexpected TwoSides value: 5");
    POWSYBL_ASSERT_THROW(line.getApparentPowerLimits(static_cast<TwoSides>(0)), AssertionError, "Unexpected TwoSides value: UNDEFINED");
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Line& line = network.getLine("NHV1_NHV2_2");
    ApparentPowerLimitsAdder apparentPowerLimitsAdder = line.getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits();
    apparentPowerLimitsAdder.setPermanentLimit(10);

    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': temporary limit value is not set");
    POWSYBL_ASSERT_THROW(apparentPowerLimitsAdder.beginTemporaryLimit().setValue(-1.0).endTemporaryLimit(), PowsyblException, "AC line 'NHV1_NHV2_2': temporary limit value must be >= 0");
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

    BOOST_CHECK_EQUAL(apl.getLimitsGroupId(), "DEFAULT");

    const ApparentPowerLimits::TemporaryLimit& tl1 = apl.getTemporaryLimit(20 * 60);
    BOOST_CHECK_EQUAL("20'", tl1.getName());
    BOOST_CHECK_CLOSE(100.0, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!tl1.isFictitious());

    const ApparentPowerLimits::TemporaryLimit& tl2 = apl.getTemporaryLimit(30 * 60);
    BOOST_CHECK_EQUAL("20'#0", tl2.getName());
    BOOST_CHECK_CLOSE(200.0, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(tl2.isFictitious());
}

BOOST_AUTO_TEST_CASE(testAdderByCopy) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_2");
    ApparentPowerLimitsAdder apparentPowerLimitsAdder = line.getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits();
    apparentPowerLimitsAdder.setPermanentLimit(10);
    const ApparentPowerLimits& apl1 = apparentPowerLimitsAdder.beginTemporaryLimit()
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

    //Set second limits by copy
    auto adder = line.getOrCreateSelectedOperationalLimitsGroup2().newApparentPowerLimits(apl1);
    adder.add();
    auto apl2 = line.getApparentPowerLimits2().get();

    BOOST_CHECK_EQUAL(apl1.getPermanentLimit(), apl2.getPermanentLimit());
    BOOST_CHECK_EQUAL(2UL, boost::size(apl2.getTemporaryLimits()));
    BOOST_CHECK_EQUAL(boost::size(apl1.getTemporaryLimits()), boost::size(apl2.getTemporaryLimits()));
    BOOST_CHECK_EQUAL(0UL, boost::size(apl2.getFictitiousLimits()));
    BOOST_CHECK_EQUAL(boost::size(apl1.getFictitiousLimits()), boost::size(apl2.getFictitiousLimits()));
    for(auto& tl1 : apl1.getTemporaryLimits()) {
        auto& tl2 = apl2.getTemporaryLimit(tl1.getAcceptableDuration());
        BOOST_CHECK_EQUAL(tl1.getName(), tl2.getName());
        BOOST_CHECK_EQUAL(tl1.getValue(), tl2.getValue());
        BOOST_CHECK_EQUAL(tl1.isFictitious(), tl2.isFictitious());
    }

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
