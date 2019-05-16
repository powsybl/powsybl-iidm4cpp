/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ReactiveLimitsHolder.hpp>

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ReactiveLimitsHolder::ReactiveLimitsHolder() :
    m_reactiveLimits(stdcxx::make_unique<MinMaxReactiveLimits>()) {
}

MinMaxReactiveLimitsAdder ReactiveLimitsHolder::newMinMaxReactiveLimits() {
    return MinMaxReactiveLimitsAdder(*this);
}

ReactiveCapabilityCurveAdder ReactiveLimitsHolder::newReactiveCapabilityCurve() {
    return ReactiveCapabilityCurveAdder(*this);
}

void ReactiveLimitsHolder::setReactiveLimits(std::unique_ptr<ReactiveLimits>&& limits) {
    m_reactiveLimits = std::move(limits);
}

}  // namespace iidm

}  // namespace powsybl
