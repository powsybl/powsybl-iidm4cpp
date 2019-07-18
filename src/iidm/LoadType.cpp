/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <array>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace logging {

static const std::array<std::string, 3>& getLoadTypeNames() {
    static std::array<std::string, 3> s_typeNames {{
        "UNDEFINED",
        "AUXILIARY",
        "FICTITIOUS"
    }};

    return s_typeNames;
}

/**
 * toString template specialization for LoadType
 */
template <>
std::string toString(const iidm::LoadType& value) {
    return toString(getLoadTypeNames(), value);
}

}  // namespace logging

namespace iidm {

std::ostream& operator<<(std::ostream& stream, const LoadType& type) {
    stream << logging::toString(type);

    return stream;
}

LoadType getLoadType(const std::string& loadTypeName) {
    const auto& names = logging::getLoadTypeNames();
    const auto& it = std::find(names.cbegin(), names.cend(), loadTypeName);
    if (it == names.cend()) {
        throw PowsyblException(logging::format("Unable to retrieve load type from '%1%'", loadTypeName));
    }
    return static_cast<LoadType>(it - names.cbegin());
}

std::string getLoadTypeName(const LoadType& loadType) {
    return logging::toString(loadType);
}

}  // namespace iidm

}  // namespace powsybl
