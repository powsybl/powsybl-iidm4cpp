/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

ApparentPowerLimitsAdder::ApparentPowerLimitsAdder(OperationalLimitsHolder& owner) :
    LoadingLimitsAdder(owner) {
}

ApparentPowerLimits& ApparentPowerLimitsAdder::add() {
    checkLoadingLimits();
    m_owner.setOperationalLimits(LimitType::APPARENT_POWER, stdcxx::make_unique<ApparentPowerLimits>(m_owner, m_permanentLimit, m_temporaryLimits));
    return m_owner.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER).get();
}

}  // namespace iidm

}  // namespace powsybl
