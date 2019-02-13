/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ConnectableType.hpp>

#include <array>

#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

/**
 * toString template specialization for ConnectableType
 */
template <>
std::string toString(const iidm::ConnectableType& value) {
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

    unsigned int val = static_cast<unsigned int>(value);
    return (val < s_typeNames.size()) ? s_typeNames.at(val) : format("%1%", val);
}

}  // namespace logging

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const ConnectableType& type) {
    stream << logging::toString(type);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
