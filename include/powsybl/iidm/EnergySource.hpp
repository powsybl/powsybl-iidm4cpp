/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ENERGYSOURCE_HPP
#define POWSYBL_IIDM_ENERGYSOURCE_HPP

#include <string>

namespace powsybl {

namespace iidm {

enum class EnergySource : unsigned char {
    HYDRO,
    NUCLEAR,
    WIND,
    THERMAL,
    SOLAR,
    OTHER
};

inline bool isIntermittent(const EnergySource& energySource) {
    return energySource == EnergySource::WIND || energySource == EnergySource::SOLAR;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ENERGYSOURCE_HPP
