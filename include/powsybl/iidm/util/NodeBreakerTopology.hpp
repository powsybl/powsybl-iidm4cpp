/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_NODEBREAKERTOPOLOGY_HPP
#define POWSYBL_IIDM_UTIL_NODEBREAKERTOPOLOGY_HPP

#include <powsybl/iidm/VoltageLevelViews.hpp>

namespace powsybl {

namespace iidm {

namespace NodeBreakerTopology {

unsigned long newStandardConnection(BusbarSection& bb);

void removeIsolatedSwitches(voltage_level::NodeBreakerView& topo);

} // namespace NodeBreakerTopology

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_NODEBREAKERTOPOLOGY_HPP
