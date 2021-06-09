/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/CurrentLimits.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

CurrentLimits::CurrentLimits(OperationalLimitsHolder& owner, double permanentLimit, LoadingLimits::TemporaryLimits&& temporaryLimits) :
    LoadingLimits(owner, permanentLimit, std::move(temporaryLimits)) {
}

const LimitType& CurrentLimits::getLimitType() const {
    static LimitType s_type = LimitType::CURRENT;
    return s_type;
}

}  // namespace iidm

}  // namespace powsybl
