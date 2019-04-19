/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MINMAXREACTIVELIMITSADDER_HPP
#define POWSYBL_IIDM_MINMAXREACTIVELIMITSADDER_HPP

#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class MinMaxReactiveLimits;
class ReactiveLimitsHolder;

class MinMaxReactiveLimitsAdder {
public:
    explicit MinMaxReactiveLimitsAdder(ReactiveLimitsHolder& owner);

    ~MinMaxReactiveLimitsAdder() noexcept = default;

    MinMaxReactiveLimits& add();

    MinMaxReactiveLimitsAdder& setMaxQ(double maxQ);

    MinMaxReactiveLimitsAdder& setMinQ(double minQ);

private:
    ReactiveLimitsHolder& m_owner;

    stdcxx::optional<double> m_minQ;

    stdcxx::optional<double> m_maxQ;
};

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_MINMAXREACTIVELIMITSADDER_HPP
