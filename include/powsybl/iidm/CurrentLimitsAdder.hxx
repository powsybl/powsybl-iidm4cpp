/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CURRENTLIMITSADDER_HXX
#define POWSYBL_IIDM_CURRENTLIMITSADDER_HXX

#include <cmath>
#include <unordered_set>

#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

const std::string& checkNotEmpty(const Validable& validable, const std::string& value, const std::string& message);

double checkPermanentLimit(const Validable& validable, double permanentLimit);

template <typename S, typename O>
CurrentLimitsAdder<S, O>::TemporaryLimitAdder::TemporaryLimitAdder(CurrentLimitsAdder<S, O>& parent) :
    m_parent(parent),
    m_name(),
    m_value(stdcxx::nan()),
    m_acceptableDuration(),
    m_fictitious() {
}

template <typename S, typename O>
CurrentLimitsAdder<S, O>& CurrentLimitsAdder<S, O>::TemporaryLimitAdder::endTemporaryLimit() {
    if (std::isnan(m_value)) {
        throw ValidationException(m_parent.getOwner(), "temporary limit value is not set");
    }
    if (std::islessequal(m_value, 0.0)) {
        throw ValidationException(m_parent.getOwner(), "temporary limit value must be > 0");
    }
    if (!m_acceptableDuration) {
        throw ValidationException(m_parent.getOwner(), "acceptable duration is not set");
    }
    checkNotEmpty(m_parent.getOwner(), m_name, "name is not set");

    return m_parent.addTemporaryLimit(m_name, m_value, *m_acceptableDuration, m_fictitious);
}

template <typename S, typename O>
typename CurrentLimitsAdder<S, O>::TemporaryLimitAdder& CurrentLimitsAdder<S, O>::TemporaryLimitAdder::setAcceptableDuration(unsigned long duration) {
    m_acceptableDuration = duration;
    return *this;
}

template <typename S, typename O>
typename CurrentLimitsAdder<S, O>::TemporaryLimitAdder& CurrentLimitsAdder<S, O>::TemporaryLimitAdder::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return *this;
}

template <typename S, typename O>
typename CurrentLimitsAdder<S, O>::TemporaryLimitAdder& CurrentLimitsAdder<S, O>::TemporaryLimitAdder::setName(const std::string& name) {
    m_name = name;
    return *this;
}

template <typename S, typename O>
typename CurrentLimitsAdder<S, O>::TemporaryLimitAdder& CurrentLimitsAdder<S, O>::TemporaryLimitAdder::setValue(double value) {
    m_value = value;
    return *this;
}

template <typename S, typename O>
CurrentLimitsAdder<S, O>::CurrentLimitsAdder(const S& side, O& owner) :
    m_side(side),
    m_owner(owner),
    m_permanentLimit(stdcxx::nan()),
    m_temporaryLimits() {
}

template <typename S, typename O>
CurrentLimits& CurrentLimitsAdder<S, O>::add() {
    checkPermanentLimit(m_owner, m_permanentLimit);
    checkTemporaryLimits();

    std::unique_ptr<CurrentLimits> ptrLimits = stdcxx::make_unique<CurrentLimits>(m_owner, m_permanentLimit, m_temporaryLimits);
    CurrentLimits& refLimits = *ptrLimits;
    m_owner.setCurrentLimits(m_side, std::move(ptrLimits));

    return refLimits;
}

template <typename S, typename O>
CurrentLimitsAdder<S, O>& CurrentLimitsAdder<S, O>::addTemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious) {
    m_temporaryLimits.emplace(std::make_pair(acceptableDuration, CurrentLimits::TemporaryLimit(name, value, acceptableDuration, fictitious)));
    return *this;
}

template <typename S, typename O>
typename CurrentLimitsAdder<S, O>::TemporaryLimitAdder CurrentLimitsAdder<S, O>::beginTemporaryLimit() {
    return TemporaryLimitAdder(*this);
}

template <typename S, typename O>
void CurrentLimitsAdder<S, O>::checkTemporaryLimits() const {
    logging::Logger& logger = logging::LoggerFactory::getLogger<CurrentLimitsAdder<S, O>>();

    // check temporary limits are consistent with permanent
    double previousLimit = stdcxx::nan();
    for (const auto& element : m_temporaryLimits) {
        const double limitValue = element.second.getValue();
        if (std::islessequal(limitValue, m_permanentLimit)) {
            logger.debug("%1%temporary limit should be greater than permanent limit", m_owner.getMessageHeader());
        }
        if (std::isnan(previousLimit)) {
            previousLimit = limitValue;
        } else if (std::islessequal(limitValue, previousLimit)) {
            logger.debug("%1%temporary limits should be in ascending value order", m_owner.getMessageHeader());
        }
    }

    // check name unicity
    std::unordered_set<std::string> names;
    std::for_each(m_temporaryLimits.cbegin(), m_temporaryLimits.cend(), [this, &names](const std::pair<unsigned long, CurrentLimits::TemporaryLimit>& element) {
        const auto& res = names.insert(element.second.getName());
        if (!res.second) {
            throw ValidationException(m_owner, logging::format("2 temporary limits have the same name %1%", element.second.getName()));
        }
    });
}

template <typename S, typename O>
const O& CurrentLimitsAdder<S, O>::getOwner() const {
    return m_owner;
}

template <typename S, typename O>
CurrentLimitsAdder<S, O>& CurrentLimitsAdder<S, O>::setPermanentLimit(double limit) {
    m_permanentLimit = limit;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CURRENTLIMITSADDER_HXX
