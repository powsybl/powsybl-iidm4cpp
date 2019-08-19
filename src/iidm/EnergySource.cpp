/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/EnergySource.hpp>

#include <array>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

const std::array<std::string, 6>& getEnergySourceNames() {
    static std::array<std::string, 6> s_energySourceNames {{
        "HYDRO",
        "NUCLEAR",
        "WIND",
        "THERMAL",
        "SOLAR",
        "OTHER"
    }};
    return s_energySourceNames;
}

EnergySource getEnergySource(const std::string& energySourceName) {
    const auto& names = getEnergySourceNames();
    const auto& it = std::find(names.cbegin(), names.cend(), energySourceName);
    if (it == names.cend()) {
        throw PowsyblException(logging::format("Unable to get energy source from '%1%'", energySourceName));
    }
    return static_cast<EnergySource>(it - names.cbegin());
}

std::string getEnergySourceName(const EnergySource& energySource) {
    return logging::toString(iidm::getEnergySourceNames(), energySource);
}

}  // namespace iidm

}  // namespace powsybl

