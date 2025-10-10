/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Overload.hpp>

namespace powsybl {

namespace iidm {

Overload::Overload(const LoadingLimits::TemporaryLimit& temporaryLimit, const std::string& previousLimitName, double previousLimit) :
    m_temporaryLimit(temporaryLimit),
    m_previousLimitName(previousLimitName),
    m_previousLimit(previousLimit) {
}

double Overload::getPreviousLimit() const {
    return m_previousLimit;
}

const std::string& Overload::getPreviousLimitName() const {
    return m_previousLimitName;
}

const LoadingLimits::TemporaryLimit& Overload::getTemporaryLimit() const {
    return m_temporaryLimit;
}


}  // namespace iidm

}  // namespace powsybl