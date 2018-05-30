/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <gtest/gtest.h>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

TEST(Network, constructor) {
    EXPECT_THROW(Network("", ""), PowsyblException);
    EXPECT_THROW(Network("id", ""), ValidationException);

    Network network("id", "sourceFormat");
    EXPECT_EQ("id", network.getId());
    EXPECT_EQ("id", network.getName());
    EXPECT_EQ("sourceFormat", network.getSourceFormat());
    EXPECT_EQ(0, network.getForecastDistance());
}

TEST(Network, forecastDistance) {
    Network network("id", "test");

    EXPECT_THROW(network.setForecastDistance(-1), ValidationException);
    EXPECT_TRUE(stdcxx::areSame(network, network.setForecastDistance(1)));
    EXPECT_EQ(1, network.getForecastDistance());
}

}  // namespace iidm

}  // namespace powsybl
