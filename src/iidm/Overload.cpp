/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Overload.hpp>

namespace powsybl {

namespace iidm {

bool Overload::UnacceptableTemporaryLimit::hasProperty() const {
    return false;
}
bool Overload::UnacceptableTemporaryLimit::hasProperty(const std::string& /*key*/) const {
    return false;
}
const std::string& Overload::UnacceptableTemporaryLimit::getProperty(const std::string& /*key*/) const {
    throw PowsyblException("Overload::UnacceptableTemporaryLimit does not support Properties.");
}
const std::string& Overload::UnacceptableTemporaryLimit::getProperty(const std::string& /*key*/, const std::string& /*defaultValue*/) const {
    throw PowsyblException("Overload::UnacceptableTemporaryLimit does not support Properties.");
}
stdcxx::optional<std::string> Overload::UnacceptableTemporaryLimit::setProperty(const std::string& /*key*/, const std::string& /*value*/) {
    throw PowsyblException("Overload::UnacceptableTemporaryLimit does not support Properties.");
}
bool Overload::UnacceptableTemporaryLimit::removeProperty(const std::string& /*key*/) {
    return false;
}
stdcxx::const_range<std::string> Overload::UnacceptableTemporaryLimit::getPropertyNames() const {
    return stdcxx::const_range<std::string>();
}
void Overload::UnacceptableTemporaryLimit::copyPropertiesTo(PropertiesHolder& /*propertiesHolder*/) const {
    throw PowsyblException("Overload::UnacceptableTemporaryLimit does not support Properties.");
}


Overload::Overload(const LoadingLimits::TemporaryLimit& temporaryLimit, const std::string& operationalLimitsGroupId, const std::string& previousLimitName, double previousLimit, double limitReductionCoefficient) :
    m_temporaryLimit(new LoadingLimits::TemporaryLimit(temporaryLimit)),
    m_operationLimitsGroupId(operationalLimitsGroupId),
    m_previousLimitName(previousLimitName),
    m_previousLimit(previousLimit),
    m_limitReductionCoefficient(limitReductionCoefficient) {
}

Overload::Overload(const std::string& operationalLimitsGroupId, const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient) :
    m_temporaryLimit(new UnacceptableTemporaryLimit()),
    m_operationLimitsGroupId(operationalLimitsGroupId),
    m_previousLimitName(previousLimitName),
    m_previousLimit(previousLimit),
    m_limitReductionCoefficient(limitReductionCoefficient) {

}

double Overload::getPreviousLimit() const {
    return m_previousLimit;
}

const std::string& Overload::getPreviousLimitName() const {
    return m_previousLimitName;
}

const LoadingLimits::TemporaryLimit& Overload::getTemporaryLimit() const {
    return *m_temporaryLimit;
}

double Overload::getLimitReductionCoefficient() const {
    return m_limitReductionCoefficient;
}

const std::string& Overload::getOperationalLimitsGroupId() const {
    return m_operationLimitsGroupId;
}

}  // namespace iidm

}  // namespace powsybl