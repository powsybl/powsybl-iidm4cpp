/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LinePositionAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>
#include <powsybl/iidm/extensions/iidm/LinePosition.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LinePositionAdder::LinePositionAdder(Extendable& extendable) :
    ExtensionAdder(extendable){
    m_coordinates.clear();
}

std::unique_ptr<Extension> LinePositionAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Line>(extendable)) {
        return stdcxx::make_unique<LinePosition>(dynamic_cast<Line&>(extendable), m_coordinates);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Line>()));
}

LinePositionAdder& LinePositionAdder::withCoordinates(std::vector<Coordinate> coords) {
    m_coordinates = coords;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
