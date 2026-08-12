/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/LimitViolationUtils.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace LimitViolationUtils {

PermanentLimitCheckResult checkPermanentLimitIfAny(const LoadingLimits& limits, double limitReduction, double i) {
    return {!std::isnan(limits.getPermanentLimit()) && !std::isnan(i) && std::isgreaterequal(i, limits.getPermanentLimit() * limitReduction),
            limits.getPermanentLimit(),
            limitReduction,
            limits.getLimitsGroupId()};
}

bool checkPermanentLimit(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::const_range<LoadingLimits> allSelectedLimits = branch.getAllSelectedLoadingLimits(type, side);
    for (const auto& limits : allSelectedLimits) {
        if(checkPermanentLimitIfAny(limits, limitReduction, i).m_isOverloaded) {
            return true;
        }
    }
    return false;
}
bool checkPermanentLimit(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::const_range<LoadingLimits> allSelectedLimits = transformer.getLeg(side).getAllSelectedLoadingLimits(type);
    for (const auto& limits : allSelectedLimits) {
        if(checkPermanentLimitIfAny(limits, limitReduction, i).m_isOverloaded) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Overload> getOverload(const LoadingLimits& limits, double limitReduction, double i) {
    std::unique_ptr<Overload> res;

    if (!std::isnan(limits.getPermanentLimit()) && !std::isnan(i)) {
        std::string previousLimitName = PERMANENT_LIMIT_NAME;
        const std::string& limitsGroupId = limits.getLimitsGroupId();
        double previousLimit = limits.getPermanentLimit();
        bool bcheckLastTemporaryLimit = false; 
        for (const auto& tl : limits.getTemporaryLimits()) { // iterate in ascending order
            if (std::isgreaterequal(i, previousLimit * limitReduction) && std::isless(i, tl.getValue() * limitReduction)) {
                res = stdcxx::make_unique<Overload>(tl, limitsGroupId, previousLimitName, previousLimit, limitReduction);
                return res;
            }
            
            previousLimitName = tl.getName();
            previousLimit = tl.getValue();
            bcheckLastTemporaryLimit = true;
        }
        if(bcheckLastTemporaryLimit && std::isgreaterequal(i, previousLimit * limitReduction)) {
            res = stdcxx::make_unique<Overload>(limitsGroupId, previousLimitName, previousLimit, limitReduction);
        }
    }

    return res;
}

std::unique_ptr<Overload> checkTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = branch.getLimits(type, side);
    if(static_cast<bool>(limits)) {
        return getOverload(limits.get(), limitReduction, i);
    }
    return std::unique_ptr<Overload>();
}
std::unique_ptr<Overload> checkTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = transformer.getLeg(side).getLimits(type);
    if(static_cast<bool>(limits)) {
        return getOverload(limits.get(), limitReduction, i);
    }
    return std::unique_ptr<Overload>();
}
std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type) {
    std::vector<std::unique_ptr<Overload>> overloads;
    stdcxx::const_range<LoadingLimits> allSelectedLimits = branch.getAllSelectedLoadingLimits(type, side);

    for (const auto& limits : allSelectedLimits) {
        std::unique_ptr<Overload> overload = getOverload(limits, limitReduction, i);
        if(static_cast<bool>(overload)) {
            overloads.emplace_back(std::move(overload));
        }
    }
    return overloads;
}
std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type) {
    std::vector<std::unique_ptr<Overload>> overloads;
    stdcxx::const_range<LoadingLimits> allSelectedLimits = transformer.getLeg(side).getAllSelectedLoadingLimits(type);

    for (const auto& limits : allSelectedLimits) {
        std::unique_ptr<Overload> overload = getOverload(limits, limitReduction, i);
        if(static_cast<bool>(overload)) {
            overloads.emplace_back(std::move(overload));
        }
    }
    return overloads;
}

double getValueForLimit(const Terminal& terminal, const LimitType& type) {
    switch (type) {
        case LimitType::ACTIVE_POWER:
            return terminal.getP();

        case LimitType::APPARENT_POWER:
            return std::sqrt(terminal.getP() * terminal.getP() + terminal.getQ() * terminal.getQ());

        case LimitType::CURRENT:
            return terminal.getI();

        case LimitType::VOLTAGE:
        case LimitType::VOLTAGE_ANGLE:
        default:
            throw AssertionError(stdcxx::format("Getting %1% limits is not supported", type));
    }
}

}  // namespace LimitViolationUtils

}  // namespace iidm

}  // namespace powsybl
