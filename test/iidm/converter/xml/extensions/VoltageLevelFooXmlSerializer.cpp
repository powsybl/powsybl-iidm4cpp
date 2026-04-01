/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageLevelFooXmlSerializer.hpp"

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/VoltageLevelFooExt.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

VoltageLevelFooXmlSerializer::VoltageLevelFooXmlSerializer() :
    AbstractExtensionXmlSerializer("voltageLevelFoo", "network", "foo", "http://www.itesla_project.eu/schema/iidm/ext/voltagelevelfoo/1_0") {
}

Extension& VoltageLevelFooXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& /*context*/) const {
    if (!stdcxx::isInstanceOf<VoltageLevel>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<VoltageLevel>()));
    }
    auto& vl = dynamic_cast<VoltageLevel&>(extendable);

    extendable.addExtension(stdcxx::make_unique<powsybl::network::VoltageLevelFooExt>(vl));
    return extendable.getExtension<powsybl::network::VoltageLevelFooExt>();
}

void VoltageLevelFooXmlSerializer::write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const {
    // Nothing to do
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
