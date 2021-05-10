/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ACTIVEPOWERLIMITS_HPP
#define POWSYBL_IIDM_ACTIVEPOWERLIMITS_HPP

#include <powsybl/iidm/LoadingLimits.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits : public LoadingLimits {
public:  // LoadingLimits
    const LimitType& getLimitType() const override;

    void remove() override;

public:
    ActivePowerLimits(OperationalLimitsHolder& owner, double permanentLimit, TemporaryLimits temporaryLimits);

    ActivePowerLimits(const ActivePowerLimits&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of LoadingLimits is not marked noexcept
    ActivePowerLimits(ActivePowerLimits&&) = default;  // NOSONAR

    ~ActivePowerLimits() override = default;

    ActivePowerLimits& operator=(const ActivePowerLimits&) = delete;

    ActivePowerLimits& operator=(ActivePowerLimits&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ACTIVEPOWERLIMITS_HPP
