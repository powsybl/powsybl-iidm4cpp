/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ConnectableType.hpp>

#include <array>

namespace powsybl {

namespace iidm {

const std::string& getConnectableTypeName(const ConnectableType& type) {
    static std::array<std::string, 10> s_typeNames {{
        "BUSBAR_SECTION",
        "LINE",
        "TWO_WINDINGS_TRANSFORMER",
        "THREE_WINDINGS_TRANSFORMER",
        "GENERATOR",
        "LOAD",
        "SHUNT_COMPENSATOR",
        "DANGLING_LINE",
        "STATIC_VAR_COMPENSATOR",
        "HVDC_CONVERTER_STATION"
    }};

    return s_typeNames.at(static_cast<unsigned int>(type));
}

std::ostream& operator<<(std::ostream& stream, const ConnectableType& type) {
    stream << getConnectableTypeName(type);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
