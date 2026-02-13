/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadingLimits.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
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

LoadingLimits::LoadingLimits(OperationalLimitsGroup& owner, double permanentLimit, const TemporaryLimits& temporaryLimits, 
const FictitiousLimits& fictitiousLimits) :
    m_limitsGroup(owner),
    m_permanentLimit(permanentLimit),
    m_temporaryLimits(temporaryLimits),
    m_fictitiousLimits(fictitiousLimits) {
        //limit validations must be performed before calling this constructor (in each adders)
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
    checkPermanentLimit(m_limitsGroup.get().getValidable(), permanentLimit, getTemporaryLimits(), m_limitsGroup.get().getNetwork().getMinimumValidationLevel());
    m_permanentLimit = permanentLimit;
    m_limitsGroup.get().getNetwork().invalidateValidationLevel();
    return *this;
}

LoadingLimits& LoadingLimits::setTemporaryLimitValue(unsigned long acceptableDuration, double temporaryLimitValue) {
    if(std::isnan(temporaryLimitValue) || temporaryLimitValue < 0.0) {
        throw ValidationException(m_limitsGroup.get().getValidable(), "temporary limit value must be >= 0");
    }
    //Identify the limit to modify:
    if(m_temporaryLimits.find(acceptableDuration) == m_temporaryLimits.end()) {
        throw ValidationException(m_limitsGroup.get().getValidable(), "No temporary limit found for the given acceptable duration");
    }
    TemporaryLimit& tl = getTemporaryLimit(acceptableDuration);

    if(!isTemporaryLimitValueValid(acceptableDuration, temporaryLimitValue)){
        logging::Logger& logger = logging::LoggerFactory::getLogger<LoadingLimits>();
        logger.warn(stdcxx::format("%1% Temporary limit value changed from %2% to %3%, but it is not valid", 
                m_limitsGroup.get().getValidable().getMessageHeader(), tl.getValue(), temporaryLimitValue));
    }

    TemporaryLimit newTl(tl.getName(), temporaryLimitValue, acceptableDuration, tl.isFictitious());
    m_temporaryLimits.erase(acceptableDuration);
    m_temporaryLimits.insert(std::make_pair(acceptableDuration, std::move(newTl)));

    return *this;
}

bool LoadingLimits::isTemporaryLimitValueValid(unsigned long acceptableDuration, double temporaryLimitValue) {

    auto it = m_temporaryLimits.find(acceptableDuration);

    if(it == m_temporaryLimits.end()) {
        return false;
    }

    //Compare with adjacent limits:
    bool checkAgainstBigger = true;
    bool checkAgainstSmaller = true;
    if(it != m_temporaryLimits.begin()) {
        auto tl_previous = --it;
        checkAgainstBigger = tl_previous->second.getValue() <= temporaryLimitValue;
        //reset iterator
        ++it;
    }
    auto tl_next = ++it;
    if(tl_next != m_temporaryLimits.end()) {
        checkAgainstSmaller = temporaryLimitValue <= tl_next->second.getValue();
    }

    return temporaryLimitValue > m_permanentLimit && checkAgainstBigger && checkAgainstSmaller;


}

}  // namespace iidm

}  // namespace powsybl
