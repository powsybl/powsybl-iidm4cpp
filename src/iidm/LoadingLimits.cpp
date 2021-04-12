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

LoadingLimits::LoadingLimits(OperationalLimitsHolder& owner, double permanentLimit, TemporaryLimits temporaryLimits) :
    m_owner(owner),
    m_permanentLimit(permanentLimit),
    m_temporaryLimits(std::move(temporaryLimits)) {
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

std::vector<std::reference_wrapper<const LoadingLimits::TemporaryLimit>> LoadingLimits::getTemporaryLimits() const {
    // to be discussed: return a range here? => some unit tests may be removed (CurrentLimitsTest.cpp)
    std::vector<std::reference_wrapper<const TemporaryLimit>> limits;

    for (const auto& itLimit : m_temporaryLimits) {
        limits.emplace_back(std::cref(itLimit.second));
    }

    return limits;
}

std::vector<std::reference_wrapper<LoadingLimits::TemporaryLimit>> LoadingLimits::getTemporaryLimits() {
    std::vector<std::reference_wrapper<TemporaryLimit>> limits;

    for (auto& itLimit : m_temporaryLimits) {
        limits.emplace_back(std::ref(itLimit.second));
    }

    return limits;
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
