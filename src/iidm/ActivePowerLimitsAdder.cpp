/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ActivePowerLimitsAdder::ActivePowerLimitsAdder(OperationalLimitsHolder& owner) :
    LoadingLimitsAdder(owner) {
}

ActivePowerLimits& ActivePowerLimitsAdder::add() {
    checkLoadingLimits();
    return m_owner.setOperationalLimits(LimitType::ACTIVE_POWER, stdcxx::make_unique<ActivePowerLimits>(m_owner, m_permanentLimit, std::move(m_temporaryLimits))).get();
}

}  // namespace iidm

}  // namespace powsybl
