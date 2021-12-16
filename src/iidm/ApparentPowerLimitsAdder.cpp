/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

ApparentPowerLimitsAdder::ApparentPowerLimitsAdder(OperationalLimitsOwner& owner) :
    LoadingLimitsAdder(owner) {
}

ApparentPowerLimits& ApparentPowerLimitsAdder::add() {
    checkLoadingLimits();
    auto limit = m_owner.setOperationalLimits(LimitType::APPARENT_POWER, stdcxx::make_unique<ApparentPowerLimits>(m_owner, getPermanentLimit(), getTemporaryLimits()));
    return static_cast<ApparentPowerLimits&>(limit.get());
}

}  // namespace iidm

}  // namespace powsybl
