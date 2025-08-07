/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SubstationPosition.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SubstationPosition::SubstationPosition(Substation& substation, const Coordinate& coords) :
    Extension(substation) {
    m_coordinates = coords;
}

void SubstationPosition::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Substation>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Substation>()));
    }
}

const Coordinate& SubstationPosition::getCoordinates() const {
    return m_coordinates;
}

const std::string& SubstationPosition::getName() const {
    static std::string s_name = "substationPosition";
    return s_name;
}

const std::type_index& SubstationPosition::getType() const {
    static std::type_index s_type = typeid(SubstationPosition);
    return s_type;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
