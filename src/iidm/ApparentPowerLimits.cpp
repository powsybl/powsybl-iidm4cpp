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

ApparentPowerLimits::ApparentPowerLimits(OperationalLimitsOwner& owner, double permanentLimit, const TemporaryLimits& temporaryLimits, const FictitiousLimits& fictitiousLimits) :
    LoadingLimits(owner, permanentLimit, temporaryLimits, fictitiousLimits) {
}

const LimitType& ApparentPowerLimits::getLimitType() const {
    static LimitType s_type = LimitType::APPARENT_POWER;
    return s_type;
}

}  // namespace iidm

}  // namespace powsybl
