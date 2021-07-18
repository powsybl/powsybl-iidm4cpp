/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/LimitViolationUtils.hpp>

#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace LimitViolationUtils {

bool checkPermanentLimit(const Branch& branch, const Branch::Side& side, double limitReduction, double i) {
    stdcxx::CReference<CurrentLimits> limits = branch.getCurrentLimits(side);
    return static_cast<bool>(limits)
           && !std::isnan(limits.get().getPermanentLimit())
           && !std::isnan(i)
           && std::isgreaterequal(i, limits.get().getPermanentLimit() * limitReduction);
}

std::unique_ptr<Branch::Overload> checkTemporaryLimits(const Branch& branch, const Branch::Side& side, double limitReduction, double i) {
    std::unique_ptr<Branch::Overload> res;
    stdcxx::CReference<CurrentLimits> limits = branch.getCurrentLimits(side);
    if (static_cast<bool>(limits) && !std::isnan(limits.get().getPermanentLimit()) && !std::isnan(i)) {
        std::string previousLimitName;
        double previousLimit = limits.get().getPermanentLimit();
        for (const auto& tl : limits.get().getTemporaryLimits()) { // iterate in ascending order
            if (std::isgreaterequal(i, previousLimit * limitReduction) && std::isless(i, tl.getValue() * limitReduction)) {
                res = stdcxx::make_unique<Branch::Overload>(tl, previousLimitName, previousLimit);
                break;
            }
            previousLimitName = tl.getName();
            previousLimit = tl.getValue();
        }
    }

    return res;
}

}  // namespace LimitViolationUtils

}  // namespace iidm

}  // namespace powsybl
