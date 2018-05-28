/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/MinMaxReactiveLimitsAdder.hpp>

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

MinMaxReactiveLimitsAdder::MinMaxReactiveLimitsAdder(ReactiveLimitsHolder& owner):
    m_owner(owner) {
}

MinMaxReactiveLimitsAdder& MinMaxReactiveLimitsAdder::setMaxQ(double maxQ) {
    m_maxQ = maxQ;
    return *this;
}

MinMaxReactiveLimitsAdder& MinMaxReactiveLimitsAdder::setMinQ(double minQ) {
    m_minQ = minQ;
    return *this;
}

MinMaxReactiveLimits& MinMaxReactiveLimitsAdder::add() {
    const auto& owner = dynamic_cast<Validable&>(m_owner);

    checkOptional(owner, m_minQ, "Minimum reactive power is not set");
    checkOptional(owner, m_maxQ, "Maximum reactive power is not set");
    if (*m_maxQ < *m_minQ) {
        throw ValidationException(owner, "Maximum reactive power is expected to be greater than or equal to minimum reactive power");
    }

    std::unique_ptr<ReactiveLimits> limits = stdcxx::make_unique<MinMaxReactiveLimits>(*m_minQ, *m_maxQ);
    m_owner.setReactiveLimits(std::move(limits));

    return m_owner.getReactiveLimits<MinMaxReactiveLimits>();
}

}  // namespace iidm

}  // namespace powsybl
