/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadingLimits.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

LoadingLimits::TemporaryLimit::TemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool hasOverloadingProtection) :
    m_name(name),
    m_value(value),
    m_acceptableDuration(acceptableDuration),
    m_isFictitious(hasOverloadingProtection) {
}

unsigned long LoadingLimits::TemporaryLimit::getAcceptableDuration() const {
    return m_acceptableDuration;
}

const std::string& LoadingLimits::TemporaryLimit::getName() const {
    return m_name;
}

double LoadingLimits::TemporaryLimit::getValue() const {
    return m_value;
}

bool LoadingLimits::TemporaryLimit::isFictitious() const {
    return m_isFictitious;
}

LoadingLimits::LoadingLimits(OperationalLimitsOwner& owner, double permanentLimit, const TemporaryLimits& temporaryLimits, 
const FictitiousLimits& fictitiousLimits) :
    OperationalLimits(owner),
    m_permanentLimit(permanentLimit),
    m_temporaryLimits(temporaryLimits),
    m_fictitiousLimits(fictitiousLimits) {
}

double LoadingLimits::getPermanentLimit() const {
    return m_permanentLimit;
}

const LoadingLimits::TemporaryLimit& LoadingLimits::getTemporaryLimit(unsigned long acceptableDuration) const {
    return m_temporaryLimits.at(acceptableDuration);
}

LoadingLimits::TemporaryLimit& LoadingLimits::getTemporaryLimit(unsigned long acceptableDuration) {
    return m_temporaryLimits.at(acceptableDuration);
}

stdcxx::const_range<LoadingLimits::TemporaryLimit> LoadingLimits::getTemporaryLimits() const {
    return m_temporaryLimits | boost::adaptors::map_values;
}

stdcxx::range<LoadingLimits::TemporaryLimit> LoadingLimits::getTemporaryLimits() {
    return m_temporaryLimits | boost::adaptors::map_values;
}

stdcxx::const_range<LoadingLimits::TemporaryLimit> LoadingLimits::getFictitiousLimits() const {
    return m_fictitiousLimits | boost::adaptors::map_values;
}

stdcxx::range<LoadingLimits::TemporaryLimit> LoadingLimits::getFictitiousLimits() {
    return m_fictitiousLimits | boost::adaptors::map_values;
}

double LoadingLimits::getTemporaryLimitValue(unsigned long acceptableDuration) const {
    auto tl = m_temporaryLimits.find(acceptableDuration);
    return (tl != m_temporaryLimits.end()) ? tl->second.getValue() : stdcxx::nan();
}

LoadingLimits& LoadingLimits::setPermanentLimit(double permanentLimit) {
    checkPermanentLimit(m_owner, permanentLimit);
    m_permanentLimit = permanentLimit;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
