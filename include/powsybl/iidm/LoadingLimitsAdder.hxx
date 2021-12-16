/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADINGLIMITSADDER_HXX
#define POWSYBL_IIDM_LOADINGLIMITSADDER_HXX

#include <powsybl/iidm/LoadingLimitsAdder.hpp>

#include <unordered_set>

#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/OperationalLimitsOwner.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

template <typename L, typename A>
LoadingLimitsAdder<L, A>::TemporaryLimitAdder::TemporaryLimitAdder(OperationalLimitsOwner& owner, LoadingLimitsAdder<L, A>& parent) :
    m_owner(owner),
    m_parent(parent) {
}

template <typename L, typename A>
LoadingLimitsAdder<L, A>& LoadingLimitsAdder<L, A>::TemporaryLimitAdder::endTemporaryLimit() {
    if (std::isnan(m_value)) {
        throw ValidationException(m_owner, "temporary limit value is not set");
    }
    if (m_value <= 0) {
        throw ValidationException(m_owner, "temporary limit value must be > 0");
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
LoadingLimitsAdder<L, A>::LoadingLimitsAdder(OperationalLimitsOwner& owner) :
    m_owner(owner) {
}

template <typename L, typename A>
LoadingLimitsAdder<L, A>& LoadingLimitsAdder<L, A>::addTemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious) {
    m_temporaryLimits.emplace(acceptableDuration, LoadingLimits::TemporaryLimit(name, value, acceptableDuration, fictitious));
    return *this;
}

template <typename L, typename A>
typename LoadingLimitsAdder<L, A>::TemporaryLimitAdder LoadingLimitsAdder<L, A>::beginTemporaryLimit() {
    return TemporaryLimitAdder(m_owner, *this);
}

template <typename L, typename A>
void LoadingLimitsAdder<L, A>::checkLoadingLimits() const {
    checkPermanentLimit(m_owner, m_permanentLimit);
    checkTemporaryLimits();
}

template <typename L, typename A>
void LoadingLimitsAdder<L, A>::checkTemporaryLimits() const {
    logging::Logger& logger = logging::LoggerFactory::getLogger<LoadingLimitsAdder<L, A>>();

    // check temporary limits are consistents with permanent
    double previousLimit = stdcxx::nan();
    for (const LoadingLimits::TemporaryLimit& tl : m_temporaryLimits | boost::adaptors::map_values) {
        if (tl.getValue() <= m_permanentLimit) {
            logger.debug(stdcxx::format("%1%temporary limit should be greater than permanent limit", m_owner.getMessageHeader()));
        }
        if (std::isnan(previousLimit)) {
            previousLimit = tl.getValue();
        } else if (tl.getValue() <= previousLimit) {
            logger.debug(stdcxx::format("%1%temporary limits should be in ascending value order", m_owner.getMessageHeader()));
        }
    }

    // check name unicity
    std::unordered_set<std::string> names;
    std::for_each(m_temporaryLimits.cbegin(), m_temporaryLimits.cend(), [this, &names](const std::pair<unsigned long, LoadingLimits::TemporaryLimit>& element) {
        const auto& res = names.insert(element.second.getName());
        if (!res.second) {
            throw ValidationException(m_owner, stdcxx::format("2 temporary limits have the same name %1%", element.second.getName()));
        }
    });
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

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADINGLIMITSADDER_HXX
