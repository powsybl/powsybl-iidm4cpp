/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_VOLTAGELEVELS_HPP
#define POWSYBL_IIDM_UTIL_VOLTAGELEVELS_HPP

#include <set>

#include <powsybl/iidm/IdentifiableType.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace VoltageLevels {

static const std::set<IdentifiableType> MULTIPLE_TERMINALS_CONNECTABLE_TYPES = {
    IdentifiableType::LINE,
    IdentifiableType::TWO_WINDINGS_TRANSFORMER,
    IdentifiableType::THREE_WINDINGS_TRANSFORMER
};

void checkRemovability(const VoltageLevel& voltageLevel);

}  // namespace VoltageLevels

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_VOLTAGELEVELS_HPP
