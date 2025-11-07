/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ActivePowerLimits.hpp>

namespace powsybl {

namespace iidm {

ActivePowerLimits::ActivePowerLimits(OperationalLimitsGroup& owner, double permanentLimit, const TemporaryLimits& temporaryLimits, const FictitiousLimits& fictitiousLimits) :
    LoadingLimits(owner, permanentLimit, temporaryLimits, fictitiousLimits) {
}

const LimitType& ActivePowerLimits::getLimitType() const {
    static LimitType s_type = LimitType::ACTIVE_POWER;
    return s_type;
}

void ActivePowerLimits::remove() {
    m_limitsGroup.get().removeActivePowerLimits();
}

}  // namespace iidm

}  // namespace powsybl
