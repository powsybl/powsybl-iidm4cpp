/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/CurrentLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

CurrentLimitsAdder::CurrentLimitsAdder(OperationalLimitsGroup& owner) :
    LoadingLimitsAdder(owner) {
}

CurrentLimits& CurrentLimitsAdder::add() {
    checkLoadingLimits();
    auto limit = m_owner.setOperationalLimits(LimitType::CURRENT, std::make_shared<CurrentLimits>(m_owner, getPermanentLimit(), getTemporaryLimits(), getFictitiousLimits()));
    return static_cast<CurrentLimits&>(limit.get());
}

}  // namespace iidm

}  // namespace powsybl
