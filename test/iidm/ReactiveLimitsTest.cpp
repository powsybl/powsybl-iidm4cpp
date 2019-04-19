/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

class ReactiveLimitsHolderMock : public ReactiveLimitsHolder, public Validable {
public:
    std::string getMessageHeader() const override { return ""; };
};

TEST(ReactiveLimits, MinMaxReactiveLimits) {
    ReactiveLimitsHolderMock mock;

    MinMaxReactiveLimitsAdder adder = mock.newMinMaxReactiveLimits();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Minimum reactive power is not set");
    adder.setMinQ(2);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Maximum reactive power is not set");
    adder.setMaxQ(1);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Maximum reactive power is expected to be greater than or equal to minimum reactive power");

    adder.setMinQ(1)
        .setMaxQ(2)
        .add();

    auto& limits = mock.getReactiveLimits<MinMaxReactiveLimits>();
    ASSERT_EQ(ReactiveLimitsKind::MIN_MAX, limits.getKind());
    ASSERT_EQ(1ul, limits.getMinQ());
    ASSERT_EQ(1ul, limits.getMinQ(0));
    ASSERT_EQ(2ul, limits.getMaxQ());
    ASSERT_EQ(2ul, limits.getMaxQ(0));

    POWSYBL_ASSERT_THROW(mock.getReactiveLimits<ReactiveCapabilityCurve>(), ValidationException,
        "Incorrect reactive limits type ReactiveCapabilityCurve, expected MinMaxReactiveLimits");
}

TEST(ReactiveLimits, ReactiveCapabilityCurve) {
    ReactiveLimitsHolderMock mock;

    ReactiveCapabilityCurveAdder adder = mock.newReactiveCapabilityCurve();
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "A reactive capability curve should have at least two points");

    ReactiveCapabilityCurveAdder::PointAdder pointAdder = adder.beginPoint();
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "P is not set");
    pointAdder.setP(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "P is not set");
    pointAdder.setP(0);
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "min Q is not set");
    pointAdder.setMinQ(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "min Q is not set");
    pointAdder.setMinQ(-10);
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "max Q is not set");
    pointAdder.setMaxQ(stdcxx::nan());
    POWSYBL_ASSERT_THROW(pointAdder.endPoint(), ValidationException, "max Q is not set");
    pointAdder.setMaxQ(10);
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
    ASSERT_EQ(ReactiveLimitsKind::CURVE, limits.getKind());
    ASSERT_EQ(3ul, limits.getPointCount());
    ASSERT_EQ(0, limits.getMinP());
    ASSERT_EQ(-10, limits.getMinQ(0));
    ASSERT_EQ(15, limits.getMinQ(1));
    ASSERT_EQ(12.5, limits.getMinQ(1.5));
    ASSERT_EQ(10, limits.getMinQ(3.5));
    ASSERT_EQ(2, limits.getMaxP());
    ASSERT_EQ(10, limits.getMaxQ(0));
    ASSERT_EQ(10, limits.getMaxQ(-1.5));
    ASSERT_EQ(22.5, limits.getMaxQ(1.5));

    ASSERT_EQ(10, limits.getMinQ(4));
}

}  // namespace iidm

}  // namespace powsybl
