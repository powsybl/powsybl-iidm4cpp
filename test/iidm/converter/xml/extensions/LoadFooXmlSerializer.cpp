/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadFooXmlSerializer.hpp"

#include <powsybl/iidm/Load.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

LoadFooXmlSerializer::LoadFooXmlSerializer() :
    AbstractExtensionXmlSerializer("loadFoo", "network", "foo", "http://www.itesla_project.eu/schema/iidm/ext/loadfoo/1_0") {
}

network::LoadFooExt& LoadFooXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& /*context*/) const {
    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    extendable.addExtension(stdcxx::make_unique<network::LoadFooExt>(load));
    return extendable.getExtension<network::LoadFooExt>();
}

void LoadFooXmlSerializer::write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const {
    // Nothing to do
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
