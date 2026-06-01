/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Overload.hpp>

namespace powsybl {

namespace iidm {

Overload::Overload(const LoadingLimits::TemporaryLimit& temporaryLimit, const std::string& previousLimitName, double previousLimit, double limitReductionCoefficient) :
    m_temporaryLimit(temporaryLimit),
    m_previousLimitName(previousLimitName),
    m_previousLimit(previousLimit),
    m_limitReductionCoefficient(limitReductionCoefficient) {
}

Overload::Overload(const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient) :
    Overload(UNACCEPTABLE_LIMIT(), previousLimitName, previousLimit, limitReductionCoefficient) {

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

double Overload::getLimitReductionCoefficient() const {
    return m_limitReductionCoefficient;
}

const LoadingLimits::TemporaryLimit& Overload::UNACCEPTABLE_LIMIT() {
    static LoadingLimits::TemporaryLimit UNACCEPTABLE_LIMIT("Unacceptable", std::numeric_limits<double>::infinity(), 0UL, true);
    return UNACCEPTABLE_LIMIT;
}


}  // namespace iidm

}  // namespace powsybl