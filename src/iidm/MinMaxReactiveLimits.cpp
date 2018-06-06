/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>

namespace powsybl {

namespace iidm {

MinMaxReactiveLimits::MinMaxReactiveLimits(double minQ, double maxQ) :
    m_minQ(minQ),
    m_maxQ(maxQ) {
}

ReactiveLimitsKind MinMaxReactiveLimits::getKind() const {
    return ReactiveLimitsKind::MIN_MAX;
}

double MinMaxReactiveLimits::getMaxQ() const {
    return m_maxQ;
}

double MinMaxReactiveLimits::getMaxQ(double /*p*/) const {
    return m_maxQ;
}

double MinMaxReactiveLimits::getMinQ() const {
    return m_minQ;
}

double MinMaxReactiveLimits::getMinQ(double /*p*/) const {
    return m_minQ;
}

}  // namespace iidm

}  // namespace powsybl
