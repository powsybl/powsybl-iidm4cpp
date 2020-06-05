/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/CurrentLimits.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

CurrentLimits::TemporaryLimit::TemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious) :
    m_name(name),
    m_value(value),
    m_acceptableDuration(acceptableDuration),
    m_fictitious(fictitious) {
}

unsigned long CurrentLimits::TemporaryLimit::getAcceptableDuration() const {
    return m_acceptableDuration;
}

const std::string& CurrentLimits::TemporaryLimit::getName() const {
    return m_name;
}

double CurrentLimits::TemporaryLimit::getValue() const {
    return m_value;
}

bool CurrentLimits::TemporaryLimit::isFictitious() const {
    return m_fictitious;
}

CurrentLimits::CurrentLimits(Validable& owner, double permanentLimit, TemporaryLimits temporaryLimits) :
    m_owner(owner),
    m_permanentLimit(checkPermanentLimit(owner, permanentLimit)),
    m_temporaryLimits(std::move(temporaryLimits)) {
}

double CurrentLimits::getPermanentLimit() const {
    return m_permanentLimit;
}

const CurrentLimits::TemporaryLimit& CurrentLimits::getTemporaryLimit(unsigned long acceptableDuration) const {
    return m_temporaryLimits.at(acceptableDuration);
}

CurrentLimits::TemporaryLimit& CurrentLimits::getTemporaryLimit(unsigned long acceptableDuration) {
    return m_temporaryLimits.at(acceptableDuration);
}

std::vector<std::reference_wrapper<const CurrentLimits::TemporaryLimit>> CurrentLimits::getTemporaryLimits() const {
    std::vector<std::reference_wrapper<const TemporaryLimit>> limits;

    for (const auto& itLimit : m_temporaryLimits) {
        limits.emplace_back(std::cref(itLimit.second));
    }

    return limits;
}

std::vector<std::reference_wrapper<CurrentLimits::TemporaryLimit>> CurrentLimits::getTemporaryLimits() {
    std::vector<std::reference_wrapper<TemporaryLimit>> limits;

    for (auto& itLimit : m_temporaryLimits) {
        limits.emplace_back(std::ref(itLimit.second));
    }

    return limits;
}

double CurrentLimits::getTemporaryLimitValue(unsigned long acceptableDuration) const {
    auto tl = m_temporaryLimits.find(acceptableDuration);
    return (tl != m_temporaryLimits.end()) ? tl->second.getValue() : stdcxx::nan();
}

CurrentLimits& CurrentLimits::setPermanentLimit(double permanentLimit) {
    m_permanentLimit = checkPermanentLimit(m_owner, permanentLimit);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
