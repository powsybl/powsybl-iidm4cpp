/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ActivePowerLimits.hpp>

#include <powsybl/iidm/OperationalLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

ActivePowerLimits::ActivePowerLimits(OperationalLimitsHolder& owner, double permanentLimit, TemporaryLimits temporaryLimits) :
    LoadingLimits(owner, permanentLimit, std::move(temporaryLimits)) {
}

const LimitType& ActivePowerLimits::getLimitType() const {
    static LimitType s_type = LimitType::ACTIVE_POWER;
    return s_type;
}

void ActivePowerLimits::remove() {
    m_owner.setOperationalLimits(LimitType::ACTIVE_POWER, std::unique_ptr<OperationalLimits>());
}

}  // namespace iidm

}  // namespace powsybl
