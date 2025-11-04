/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADINGLIMITSUTIL_HXX
#define POWSYBL_IIDM_LOADINGLIMITSUTIL_HXX

#include <powsybl/iidm/LoadingLimits.hpp>

#include <powsybl/iidm/LoadingLimitsAdder.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

namespace LoadingLimitsUtil {

template<typename L, typename A>
void fixMissingPermanentLimit(LoadingLimitsAdder<L, A>& limitsAdder, double missingPermanentLimitPercentage) {
    if(!std::isnan(limitsAdder.getPermanentLimit())) {
        return;
    }

    logging::Logger& logger = logging::LoggerFactory::getLogger("powsybl::iidm::util::LoadingLimitsUtil");

    double lowestTemporaryLimitWithInfiniteAcceptableDuration = std::numeric_limits<double>::max();
    bool hasTemporaryLimitWithInfiniteAcceptableDuration = false;
    auto names = limitsAdder.getTemporaryLimitNames();
    for (const std::string& name : names) {
        if (limitsAdder.getTemporaryLimitAcceptableDuration(name) == std::numeric_limits<unsigned long>::max()) {
            hasTemporaryLimitWithInfiniteAcceptableDuration = true;
            lowestTemporaryLimitWithInfiniteAcceptableDuration =
                std::min(lowestTemporaryLimitWithInfiniteAcceptableDuration, limitsAdder.getTemporaryLimitValue(name));
            limitsAdder.removeTemporaryLimit(name);
        }
    }

    if (hasTemporaryLimitWithInfiniteAcceptableDuration) {
        logger.warn(stdcxx::format("Operational limits without permanent limit (%1%) is considered with permanent limit equal to lowest temporary limit value with infinite acceptable duration : %2%",
                           stdcxx::nan(), lowestTemporaryLimitWithInfiniteAcceptableDuration));
        limitsAdder.setPermanentLimit(lowestTemporaryLimitWithInfiniteAcceptableDuration);
    } else {
        double firstTemporaryLimit = limitsAdder.getLowestTemporaryLimitValue();
        double percentage = missingPermanentLimitPercentage / 100.;
        double fixedPermanentLimit = firstTemporaryLimit * percentage;
        logger.warn(stdcxx::format("Operational limits without permanent limit (%1%) is considered with permanent limit equal to lowest temporary limit value weighted by a coefficient (%2%) : %3%",
                           stdcxx::nan(), percentage, fixedPermanentLimit));
        limitsAdder.setPermanentLimit(fixedPermanentLimit);
    }
}

}  // namespace LoadingLimitsUtil

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADINGLIMITSADDER_HXX