/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADINGLIMITSADDER_HXX
#define POWSYBL_IIDM_LOADINGLIMITSADDER_HXX

#include <powsybl/iidm/LoadingLimitsAdder.hpp>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/util/LoadingLimitsUtil.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

template <typename L, typename A>
LoadingLimitsAdder<L, A>::TemporaryLimitAdder::TemporaryLimitAdder(OperationalLimitsGroup& owner, LoadingLimitsAdder<L, A>& parent) :
    m_owner(owner),
    m_parent(parent) {
}

template <typename L, typename A>
LoadingLimitsAdder<L, A>& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::endTemporaryLimit() {
    if (std::isnan(m_value)) {
        throw ValidationException(m_owner, "temporary limit value is not set");
    }
    if (m_value < 0) {
        throw ValidationException(m_owner, "temporary limit value must be >= 0");
    }
    if (m_value == 0.0) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<LoadingLimitsAdder>();
        logger.info(stdcxx::format("%1%temporary limit value is set to 0", m_owner.getMessageHeader()));
    }
    if (!m_acceptableDuration) {
        throw ValidationException(m_owner, "acceptable duration is not set");
    }
    checkAndGetUniqueName();
    return m_parent.addTemporaryLimit(m_name, m_value, *m_acceptableDuration, m_fictitious);
}

template <typename L, typename A>
void LoadingLimitsAdder<L, A>::TemporaryLimitAdder::checkAndGetUniqueName() {
    if (m_name.empty()) {
        throw ValidationException(m_owner, "name is not set");
    }
    if (m_ensureNameUnicity) {
        unsigned long i = 0UL;
        std::string uniqueName = m_name;
        while (i < std::numeric_limits<unsigned long>::max() && m_parent.nameExists(uniqueName)) {
            uniqueName = m_name + stdcxx::format("#%1%", i);
            i++;
        }
        m_name = uniqueName;
    }
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::ensureNameUnicity() {
    m_ensureNameUnicity = true;
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::setAcceptableDuration(unsigned long duration) {
    m_acceptableDuration = duration;
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::setName(const std::string& name) {
    m_name = name;
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::setValue(double value) {
    m_value = value;
    return *this;
}

template <typename L, typename A>
LoadingLimitsAdder<L, A>::LoadingLimitsAdder(OperationalLimitsGroup& owner) :
    m_owner(owner) {
}

template <typename L, typename A>
LoadingLimitsAdder<L, A>& LoadingLimitsAdder<L, A>::addTemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious) {
    if (fictitious && acceptableDuration == std::numeric_limits<unsigned long>::max()) {
        m_fictitiousLimits.emplace(name, LoadingLimits::TemporaryLimit(name, value, acceptableDuration, fictitious));
    } else {
        m_temporaryLimits.emplace(acceptableDuration, LoadingLimits::TemporaryLimit(name, value, acceptableDuration, fictitious));
    }
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder LoadingLimitsAdder<L, A>::beginTemporaryLimit() {
    return TemporaryLimitAdder(m_owner, *this);
}

template <typename L, typename A>
ValidationLevel LoadingLimitsAdder<L, A>::checkLoadingLimits() const {
    return powsybl::iidm::checkLoadingLimits(m_owner, m_permanentLimit, m_temporaryLimits | boost::adaptors::map_values, m_owner.getNetwork().getMinimumValidationLevel());
}

template <typename L, typename A>
double LoadingLimitsAdder<L, A>::getPermanentLimit() const {
    return m_permanentLimit;
}

template <typename L, typename A>
const LoadingLimits::TemporaryLimits& LoadingLimitsAdder<L, A>::getTemporaryLimits() const {
    return m_temporaryLimits;
}

template <typename L, typename A>
const LoadingLimits::FictitiousLimits& LoadingLimitsAdder<L, A>::getFictitiousLimits() const {
    return m_fictitiousLimits;
}

template <typename L, typename A>
bool LoadingLimitsAdder<L, A>::hasTemporaryLimits() const {
    return !m_temporaryLimits.empty();
}

template <typename L, typename A>
bool LoadingLimitsAdder<L, A>::nameExists(const std::string& name) const {
    auto it = std::find_if(m_temporaryLimits.begin(), m_temporaryLimits.end(), [&name](const std::pair<unsigned long, LoadingLimits::TemporaryLimit>& item) {
        return item.second.getName() == name;
    });
    return it != m_temporaryLimits.end();
}

template <typename L, typename A>
A& LoadingLimitsAdder<L, A>::setPermanentLimit(double limit) {
    m_permanentLimit = limit;
    return static_cast<A&>(*this);
}

template <typename L, typename A>
stdcxx::optional<LoadingLimits::TemporaryLimit> LoadingLimitsAdder<L, A>::getTemporaryLimitByName(const std::string& name) const {
    stdcxx::optional<LoadingLimits::TemporaryLimit> foundLimit;
    for (auto it = m_temporaryLimits.cbegin(); it != m_temporaryLimits.cend() ; it++) {
        if(it->second.getName() == name) {
            foundLimit = it->second;
            break;
        }
    }
    return foundLimit;
}

template <typename L, typename A>
double LoadingLimitsAdder<L, A>::getTemporaryLimitValue(unsigned long acceptableDuration) const {
    const auto& it = m_temporaryLimits.find(acceptableDuration);
    return (it == m_temporaryLimits.cend()) ? stdcxx::nan() : m_temporaryLimits.at(acceptableDuration).getValue();
}

template <typename L, typename A>
double LoadingLimitsAdder<L, A>::getTemporaryLimitValue(const std::string& name) const {
    if(getTemporaryLimitByName(name).has_value()) {
        return getTemporaryLimitByName(name).get().getValue();
    }
    return stdcxx::nan();
}

template <typename L, typename A>
unsigned long LoadingLimitsAdder<L, A>::getTemporaryLimitAcceptableDuration(const std::string& name) const {
    if(getTemporaryLimitByName(name).has_value()) {
        return getTemporaryLimitByName(name).get().getAcceptableDuration();
    }
    return std::numeric_limits<unsigned long>::max();
}

template <typename L, typename A>
double LoadingLimitsAdder<L, A>::getLowestTemporaryLimitValue() {
    double lowestLimit = stdcxx::nan();
    for (auto it = m_temporaryLimits.cbegin(); it != m_temporaryLimits.cend() ; it++) {
        if(std::isnan(lowestLimit) || it->second.getValue() < lowestLimit) {
            lowestLimit = it->second.getValue();
        }
    }
    return lowestLimit;
}

template <typename L, typename A>
const std::vector<std::string> LoadingLimitsAdder<L, A>::getTemporaryLimitNames() const {
    std::vector<std::string> limitsNames;
    for (auto it = m_temporaryLimits.cbegin(); it != m_temporaryLimits.cend() ; it++) {
        limitsNames.emplace_back(it->second.getName());
    }
    return limitsNames;
}


template <typename L, typename A>
void LoadingLimitsAdder<L, A>::removeTemporaryLimit(const std::string& name) {
    for (auto it = m_temporaryLimits.cbegin(); it != m_temporaryLimits.cend() ; /*no increment*/) {
        if (it->second.getName() == name) {
            it = m_temporaryLimits.erase(it);
        } else {
            it++;
        }
    }
}

template <typename L, typename A>
A& LoadingLimitsAdder<L, A>::fixLimits() {
    return fixLimits(100.0);
}

template <typename L, typename A>
A& LoadingLimitsAdder<L, A>::fixLimits(double missingPermanentLimitPercentage) {
    LoadingLimitsUtil::fixMissingPermanentLimit<L, A>(*this, missingPermanentLimitPercentage);
    return static_cast<A&>(*this);
}


}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADINGLIMITSADDER_HXX
