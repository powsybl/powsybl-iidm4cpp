/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/OperationalLimits.hpp>

#include <powsybl/iidm/OperationalLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

OperationalLimits::OperationalLimits(OperationalLimitsHolder& owner) :
    m_owner(owner) {
}

void OperationalLimits::remove() {
    m_owner.setOperationalLimits(getLimitType(), std::unique_ptr<OperationalLimits>());
}

}  // namespace iidm

}  // namespace powsybl
