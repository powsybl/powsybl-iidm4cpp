/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/IdentifiableType.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<IdentifiableType>() {
    static std::initializer_list<std::string> s_typeNames {
        "NETWORK",
        "SUBSTATION",
        "VOLTAGE_LEVEL",
        "HVDC_LINE",
        "BUS",
        "SWITCH",
        "BUSBAR_SECTION",
        "LINE",
        "TWO_WINDINGS_TRANSFORMER",
        "THREE_WINDINGS_TRANSFORMER",
        "GENERATOR",
        "BATTERY",
        "LOAD",
        "SHUNT_COMPENSATOR",
        "DANGLING_LINE",
        "STATIC_VAR_COMPENSATOR",
        "HVDC_CONVERTER_STATION"
    };
    return s_typeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
