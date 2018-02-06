/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTABLETYPE_HPP
#define POWSYBL_IIDM_CONNECTABLETYPE_HPP

#include <ostream>

namespace powsybl {

namespace iidm {

enum class ConnectableType : unsigned int {
    BUSBAR_SECTION,
    LINE,
    TWO_WINDINGS_TRANSFORMER,
    THREE_WINDINGS_TRANSFORMER,
    GENERATOR,
    LOAD,
    SHUNT_COMPENSATOR,
    DANGLING_LINE,
    STATIC_VAR_COMPENSATOR,
    HVDC_CONVERTER_STATION
};

std::ostream& operator<<(std::ostream& stream, const ConnectableType& type);

}

}

#endif  // POWSYBL_IIDM_CONNECTABLETYPE_HPP
