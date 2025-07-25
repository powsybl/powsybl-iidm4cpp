/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLETYPE_HPP
#define POWSYBL_IIDM_IDENTIFIABLETYPE_HPP

#include <iosfwd>

namespace powsybl {

namespace iidm {

enum class IdentifiableType : unsigned char {
    NETWORK,
    SUBSTATION,
    VOLTAGE_LEVEL,
    HVDC_LINE,
    BUS,
    SWITCH,
    BUSBAR_SECTION,
    LINE,
    TWO_WINDINGS_TRANSFORMER,
    THREE_WINDINGS_TRANSFORMER,
    GENERATOR,
    BATTERY,
    LOAD,
    SHUNT_COMPENSATOR,
    DANGLING_LINE,
    STATIC_VAR_COMPENSATOR,
    HVDC_CONVERTER_STATION
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDENTIFIABLETYPE_HPP
