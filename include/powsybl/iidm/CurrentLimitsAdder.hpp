/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CURRENTLIMITSADDER_HPP
#define POWSYBL_IIDM_CURRENTLIMITSADDER_HPP

#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/LoadingLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

class CurrentLimitsAdder : public LoadingLimitsAdder<CurrentLimits, CurrentLimitsAdder> {
public:  // OperationalLimitsAdder
    CurrentLimits& add() override;

public:
    explicit CurrentLimitsAdder(OperationalLimitsHolder& owner);

    CurrentLimitsAdder(const CurrentLimitsAdder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of LoadingLimitsAdder is not marked noexcept
    CurrentLimitsAdder(CurrentLimitsAdder&&) = default;  // NOSONAR

    ~CurrentLimitsAdder() noexcept override = default;

    CurrentLimitsAdder& operator=(const CurrentLimitsAdder&) = delete;

    CurrentLimitsAdder& operator=(CurrentLimitsAdder&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CURRENTLIMITSADDER_HPP
