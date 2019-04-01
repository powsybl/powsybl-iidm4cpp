/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LIMITVIOLATIONUTILS_HPP
#define POWSYBL_IIDM_LIMITVIOLATIONUTILS_HPP

#include <powsybl/iidm/Branch.hpp>

namespace powsybl {

namespace iidm {

namespace LimitViolationUtils {

bool checkPermanentLimit(const Branch& branch, const Branch::Side& side, double limitReduction, double i);

std::unique_ptr<Branch::Overload> checkTemporaryLimits(const Branch& branch, const Branch::Side& side, double limitReduction, double i);

}  // namespace LimitViolationUtils

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LIMITVIOLATIONUTILS_HPP
