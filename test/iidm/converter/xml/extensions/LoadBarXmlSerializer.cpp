/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadBarXmlSerializer.hpp"

#include <powsybl/iidm/Load.hpp>
#include <powsybl/network/LoadBarExt.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

LoadBarXmlSerializer::LoadBarXmlSerializer() :
    AbstractExtensionXmlSerializer("loadBar", "network", "bar", "http://www.itesla_project.eu/schema/iidm/ext/loadbar/1_0") {
}

std::unique_ptr<Extension> LoadBarXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& /*context*/) const {
    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    return stdcxx::make_unique<network::LoadBarExt>(load);
}

void LoadBarXmlSerializer::write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const {
    // Nothing to do
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
