/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_APPARENTPOWERLIMITS_HPP
#define POWSYBL_IIDM_APPARENTPOWERLIMITS_HPP

#include <powsybl/iidm/LoadingLimits.hpp>

namespace powsybl {

namespace iidm {

class ApparentPowerLimits : public LoadingLimits {
public:  // LoadingLimits
    const LimitType& getLimitType() const override;

public:
    ApparentPowerLimits(OperationalLimitsOwner& owner, double permanentLimit, const TemporaryLimits& temporaryLimits);

    ApparentPowerLimits(const ApparentPowerLimits&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of LoadingLimits is not marked noexcept
    ApparentPowerLimits(ApparentPowerLimits&&) = default;  // NOSONAR

    ~ApparentPowerLimits() override = default;

    ApparentPowerLimits& operator=(const ApparentPowerLimits&) = delete;

    ApparentPowerLimits& operator=(ApparentPowerLimits&&) noexcept = delete;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_APPARENTPOWERLIMITS_HPP
