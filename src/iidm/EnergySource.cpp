/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/EnergySource.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
IIDM_DECLSPEC const std::initializer_list<std::string>& getNames<EnergySource>() {
    static std::initializer_list<std::string> s_energySourceNames {
        "HYDRO",
        "NUCLEAR",
        "WIND",
        "THERMAL",
        "SOLAR",
        "OTHER"
    };
    return s_energySourceNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl

