/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_LIMITVIOLATIONUTILS_HPP
#define POWSYBL_IIDM_UTIL_LIMITVIOLATIONUTILS_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Overload.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

namespace LimitViolationUtils {

static std::string const PERMANENT_LIMIT_NAME = "permanent";

struct PermanentLimitCheckResult {
    bool m_isOverloaded;
    double m_permanentLimitValue;
    double m_limitReduction;
    std::string m_operationalLimitsGroupId;
};

/**
 * Gets the overload corresponding to the temporary limit that is directly above the value of i
 * @return a nullptr if i is below all temporary and permanent limits, or if the limits contains only a permanent limit (even if i is above said permanent limit)
 * or a pointer to an Overload, in the folowing cases :
 * - i is above a permanent limit with at least a temporary limit above
 * - i is above a temporary limit
 * In the second case, if we crossed all temporary limits, the Overload will consider a fictitious limit above with maximum i possible, and acceptable duration of 0
 */
std::unique_ptr<Overload> getOverload(const LoadingLimits& limits, double limitReduction, double i);


PermanentLimitCheckResult checkPermanentLimitIfAny(const LoadingLimits& limits, double limitReduction, double i);

bool checkPermanentLimit(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type);
bool checkPermanentLimit(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type);

std::unique_ptr<Overload> checkTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type);
std::unique_ptr<Overload> checkTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type);

/**
 * Checks the temporary limits on all the selected OperationalLimitsGroup for the given side of the branch
 * @return vector of all Overload for each violation that happened on the selected limits
 */
std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type);
/**
 * Checks the temporary limits on all the selected OperationalLimitsGroup for the given side of the transformer
 * @return vector of all Overload for each violation that happened on the selected limits
 */
std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type);

double getValueForLimit(const Terminal& terminal, const LimitType& type);

}  // namespace LimitViolationUtils

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_LIMITVIOLATIONUTILS_HPP
