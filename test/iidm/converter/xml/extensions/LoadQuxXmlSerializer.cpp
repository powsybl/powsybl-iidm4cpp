/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadQuxXmlSerializer.hpp"

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/stdcxx/map.hpp>

#include "LoadQuxExt.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

LoadQuxXmlSerializer::LoadQuxXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("loadQux", "network", "lq",
        VersionsCompatibilityBuilder()
            .put(IidmXmlVersion::V1_0(), {"1.0"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0", "http://www.powsybl.org/schema/iidm/ext/load_qux/1_0")
            .build()) {
}

std::unique_ptr<Extension> LoadQuxXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    checkReadingCompatibility(context);

    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    return stdcxx::make_unique<LoadQuxExt>(load);
}

void LoadQuxXmlSerializer::write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const {
    // Nothing to do
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
