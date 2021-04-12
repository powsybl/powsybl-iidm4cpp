/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/CurrentLimitsAdder.hpp>

#include <powsybl/iidm/OperationalLimitsHolder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

CurrentLimitsAdder::CurrentLimitsAdder(OperationalLimitsHolder& owner) :
    LoadingLimitsAdder(owner) {
}

CurrentLimits& CurrentLimitsAdder::add() {
    checkLoadingLimits();
    m_owner.setOperationalLimits(LimitType::CURRENT, stdcxx::make_unique<CurrentLimits>(m_owner, m_permanentLimit, std::move(m_temporaryLimits)));
    return m_owner.getOperationalLimits<CurrentLimits>(LimitType::CURRENT).get();
}

}  // namespace iidm

}  // namespace powsybl
