/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_IDENTIFIABLES_HPP
#define POWSYBL_IIDM_UTIL_IDENTIFIABLES_HPP

#include <string>

#include <powsybl/stdcxx/Predicate.hpp>

namespace powsybl {

namespace iidm {

namespace Identifiables {

std::string getUniqueId(const std::string& baseId, const stdcxx::Predicate<std::string>& containsId);

}  // namespace Identifiables

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_IDENTIFIABLES_HPP
