/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_LOADINGLIMITSUTIL_HPP
#define POWSYBL_IIDM_UTIL_LOADINGLIMITSUTIL_HPP


namespace powsybl {

namespace iidm {

template <typename L, typename A>
class LoadingLimitsAdder;

namespace LoadingLimitsUtil {

template<typename L, typename A>
void fixMissingPermanentLimit(LoadingLimitsAdder<L, A>& limitsAdder, double missingPermanentLimitPercentage);

template<typename L, typename A>
void initializeFromLoadingLimits(LoadingLimitsAdder<L, A>& limitsAdder,  const L& limits);

}  // namespace LoadingLimitsUtil

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/util/LoadingLimitsUtil.hxx>

#endif  // POWSYBL_IIDM_UTIL_LOADINGLIMITSUTIL_HPP