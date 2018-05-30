/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ENERGYSOURCE_HPP
#define POWSYBL_IIDM_ENERGYSOURCE_HPP

namespace powsybl {

namespace iidm {

enum class EnergySource : bool {
    HYDRO = false,
    NUCLEAR = false,
    WIND = true,
    THERMAL = false,
    SOLAR = true,
    OTHER = false
};

inline bool isIntermittent(const EnergySource& energySource) {
    return static_cast<bool>(energySource);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ENERGYSOURCE_HPP
