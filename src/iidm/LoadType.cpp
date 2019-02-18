/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <array>

#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

/**
 * toString template specialization for LoadType
 */
template <>
std::string toString(const iidm::LoadType& value) {
    static std::array<std::string, 3> s_typeNames {{
        "UNDEFINED",
        "AUXILIARY",
        "FICTITIOUS"
    }};

    return toString(s_typeNames, value);
}

}  // namespace logging

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const LoadType& type) {
    stream << logging::toString(type);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
