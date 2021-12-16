/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ACTIVEPOWERLIMITSADDER_HPP
#define POWSYBL_IIDM_ACTIVEPOWERLIMITSADDER_HPP

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/LoadingLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimitsAdder : public LoadingLimitsAdder<ActivePowerLimits, ActivePowerLimitsAdder> {
public:  // OperationalLimitsAdder
    ActivePowerLimits& add() override;

public:
    explicit ActivePowerLimitsAdder(OperationalLimitsOwner& owner);

    ActivePowerLimitsAdder(const ActivePowerLimitsAdder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of LoadingLimitsAdder is not marked noexcept
    ActivePowerLimitsAdder(ActivePowerLimitsAdder&&) = default;  // NOSONAR

    ~ActivePowerLimitsAdder() noexcept override = default;

    ActivePowerLimitsAdder& operator=(const ActivePowerLimitsAdder&) = delete;

    ActivePowerLimitsAdder& operator=(ActivePowerLimitsAdder&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ACTIVEPOWERLIMITSADDER_HPP
