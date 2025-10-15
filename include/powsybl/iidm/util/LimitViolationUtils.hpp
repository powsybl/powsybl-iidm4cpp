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

bool checkPermanentLimit(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type);
bool checkPermanentLimit(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type);

std::unique_ptr<Overload> checkTemporaryLimits(const Branch& branch, const TwoSides& side, double limitReduction, double i, const LimitType& type);
std::unique_ptr<Overload> checkTemporaryLimits(const ThreeWindingsTransformer& transformer, const ThreeSides& side, double limitReduction, double i, const LimitType& type);

double getValueForLimit(const Terminal& terminal, const LimitType& type);

}  // namespace LimitViolationUtils

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_LIMITVIOLATIONUTILS_HPP
