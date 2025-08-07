/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SubstationPositionAdder.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SubstationPositionAdder::SubstationPositionAdder(Extendable& extendable) :
    ExtensionAdder(extendable){
}

std::unique_ptr<Extension> SubstationPositionAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Substation>(extendable)) {
        return stdcxx::make_unique<SubstationPosition>(dynamic_cast<Substation&>(extendable), m_coordinates);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Substation>()));
}

SubstationPositionAdder& SubstationPositionAdder::withCoordinates(const Coordinate& coords) {
    m_coordinates = coords;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
