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

bool checkPermanentLimitIfAny(stdcxx::CReference<LoadingLimits>& limits, double limitReduction, double i) {
    return static_cast<bool>(limits)
           && !std::isnan(limits.get().getPermanentLimit())
           && !std::isnan(i)
           && std::isgreaterequal(i, limits.get().getPermanentLimit() * limitReduction);
}

bool checkPermanentLimit(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = branch.getLimits(type, side);
    return checkPermanentLimitIfAny(limits, limitReduction, i);
}
bool checkPermanentLimit(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = transformer.getLeg(side).getLimits(type);
    return checkPermanentLimitIfAny(limits, limitReduction, i);
}

std::unique_ptr<Overload> getOverload(stdcxx::CReference<LoadingLimits>& limits, double limitReduction, double i) {
    std::unique_ptr<Overload> res;

    if (static_cast<bool>(limits) && !std::isnan(limits.get().getPermanentLimit()) && !std::isnan(i)) {
        std::string previousLimitName = PERMANENT_LIMIT_NAME;
        double previousLimit = limits.get().getPermanentLimit();
        for (const auto& tl : limits.get().getTemporaryLimits()) { // iterate in ascending order
            if (std::isgreaterequal(i, previousLimit * limitReduction) && std::isless(i, tl.getValue() * limitReduction)) {
                res = stdcxx::make_unique<Overload>(tl, previousLimitName, previousLimit);
                break;
            }
            previousLimitName = tl.getName();
            previousLimit = tl.getValue();
        }
    }

    return res;
}

std::unique_ptr<Overload> checkTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = branch.getLimits(type, side);
    return getOverload(limits, limitReduction, i);
}
std::unique_ptr<Overload> checkTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type) {
    stdcxx::CReference<LoadingLimits> limits = transformer.getLeg(side).getLimits(type);
    return getOverload(limits, limitReduction, i);
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
