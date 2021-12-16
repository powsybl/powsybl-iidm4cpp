/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_APPARENTPOWERLIMITSADDER_HPP
#define POWSYBL_IIDM_APPARENTPOWERLIMITSADDER_HPP

#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/LoadingLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

class ApparentPowerLimitsAdder : public LoadingLimitsAdder<ApparentPowerLimits, ApparentPowerLimitsAdder> {
public:  // OperationalLimitsAdder
    ApparentPowerLimits& add() override;

public:
    explicit ApparentPowerLimitsAdder(OperationalLimitsOwner& owner);

    ApparentPowerLimitsAdder(const ApparentPowerLimitsAdder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of LoadingLimitsAdder is not marked noexcept
    ApparentPowerLimitsAdder(ApparentPowerLimitsAdder&&) = default;  // NOSONAR

    ~ApparentPowerLimitsAdder() noexcept override = default;

    ApparentPowerLimitsAdder& operator=(const ApparentPowerLimitsAdder&) = delete;

    ApparentPowerLimitsAdder& operator=(ApparentPowerLimitsAdder&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_APPARENTPOWERLIMITSADDER_HPP
