/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ApparentPowerLimits.hpp>

#include <powsybl/iidm/OperationalLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

ApparentPowerLimits::ApparentPowerLimits(OperationalLimitsHolder& owner, double permanentLimit, TemporaryLimits temporaryLimits) :
    LoadingLimits(owner, permanentLimit, std::move(temporaryLimits)) {
}

const LimitType& ApparentPowerLimits::getLimitType() const {
    static LimitType s_type = LimitType::APPARENT_POWER;
    return s_type;
}

void ApparentPowerLimits::remove() {
    m_owner.setOperationalLimits(LimitType::APPARENT_POWER, std::unique_ptr<OperationalLimits>());
}

}  // namespace iidm

}  // namespace powsybl
