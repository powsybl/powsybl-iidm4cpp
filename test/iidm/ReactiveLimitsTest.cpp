/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

class ReactiveLimitsHolderMock : public ReactiveLimitsHolder, public Validable {
public:
    std::string getMessageHeader() const override { return ""; };
};

BOOST_AUTO_TEST_SUITE(ReactiveLimitsTestSuite)

BOOST_AUTO_TEST_CASE(MinMaxReactiveLimitsTest) {
    ReactiveLimitsHolderMock mock;

    MinMaxReactiveLimitsAdder adder = mock.newMinMaxReactiveLimits();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Minimum reactive power is not set");
    adder.setMinQ(2.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Maximum reactive power is not set");
    adder.setMaxQ(1.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Maximum reactive power is expected to be greater than or equal to minimum reactive power");

    adder.setMinQ(1.0)
        .setMaxQ(2.0)
        .add();

    auto& limits = mock.getReactiveLimits<MinMaxReactiveLimits>();
    POWSYBL_ASSERT_ENUM_EQ(ReactiveLimitsKind::MIN_MAX, limits.getKind());
    BOOST_CHECK_CLOSE(1.0, limits.getMinQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, limits.getMinQ(0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, limits.getMaxQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, limits.getMaxQ(0), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(mock.getReactiveLimits<ReactiveCapabilityCurve>(), ValidationException,
        "Incorrect reactive limits type ReactiveCapabilityCurve, expected MinMaxReactiveLimits");
}

BOOST_AUTO_TEST_CASE(ReactiveCapabilityCurveTest) {
    ReactiveLimitsHolderMock mock;

    ReactiveCapabilityCurveAdder adder = mock.newReactiveCapabilityCurve();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "A reactive capability curve should have at least two points");

    ReactiveCapabilityCurveAdder::PointAdder pointAdder = adder.beginPoint();
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "P is not set");
    pointAdder.setP(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "P is not set");
    pointAdder.setP(0.0);
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "min Q is not set");
    pointAdder.setMinQ(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "min Q is not set");
    pointAdder.setMinQ(-10.0);
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "max Q is not set");
    pointAdder.setMaxQ(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "max Q is not set");
    pointAdder.setMaxQ(10.0);
    pointAdder.endPoint();

    ReactiveCapabilityCurveAdder::PointAdder pointAdder2 = adder.beginPoint()
        .setP(0)
        .setMinQ(10)
        .setMaxQ(20);
    POWSYBL_ASSERT_THROW(pointAdder2.endPoint(), ValidationException, "A point is already defined for active power 0");

    adder.beginPoint()
        .setP(1)
        .setMinQ(15)
        .setMaxQ(25)
        .endPoint()
        .beginPoint()
        .setP(2)
        .setMinQ(10)
        .setMaxQ(20)
        .endPoint()
        .add();

    auto& limits = mock.getReactiveLimits<ReactiveCapabilityCurve>();
    POWSYBL_ASSERT_ENUM_EQ(ReactiveLimitsKind::CURVE, limits.getKind());
    BOOST_CHECK_EQUAL(3UL, limits.getPointCount());
    BOOST_CHECK_CLOSE(0.0, limits.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-10.0, limits.getMinQ(-1.5), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-10.0, limits.getMinQ(0.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(15.0, limits.getMinQ(1.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(12.5, limits.getMinQ(1.5), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, limits.getMinQ(2.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, limits.getMinQ(3.5), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, limits.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, limits.getMaxQ(-1.5), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, limits.getMaxQ(0.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(25.0, limits.getMaxQ(1.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(22.5, limits.getMaxQ(1.5), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, limits.getMaxQ(2.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, limits.getMaxQ(3.5), std::numeric_limits<double>::epsilon());

    const auto& actual = limits.getPoints();
    std::map<double, ReactiveCapabilityCurve::Point> expected = { { 0.0, { 0.0, -10.0, 10.0 } }, { 1.0, { 1.0, 15.0, 25.0 } }, { 2.0, { 2.0, 10.0, 20.0 } } };
    auto itE = expected.begin();
    for (const auto& actualP : actual) {
        const ReactiveCapabilityCurve::Point& expectedP = itE->second;
        BOOST_CHECK_CLOSE(actualP.getP(), expectedP.getP(), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(actualP.getMinQ(), expectedP.getMinQ(), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(actualP.getMaxQ(), expectedP.getMaxQ(), std::numeric_limits<double>::epsilon());
        ++itE;
    }
    BOOST_CHECK(itE == expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
