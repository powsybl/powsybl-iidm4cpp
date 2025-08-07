/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LinePosition.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LinePosition::LinePosition(Line& line, std::vector<Coordinate> coords) :
    Extension(line) {
    m_coordinates = coords;
}

void LinePosition::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Line>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Line>()));
    }
}

const std::vector<Coordinate>& LinePosition::getCoordinates() const {
    return m_coordinates;
}

const std::string& LinePosition::getName() const {
    static std::string s_name = "linePosition";
    return s_name;
}

const std::type_index& LinePosition::getType() const {
    static std::type_index s_type = typeid(LinePosition);
    return s_type;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
