/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadMockExtXmlSerializer.hpp"

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/stdcxx/map.hpp>

#include "LoadMockExt.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

LoadMockExtXmlSerializer::LoadMockExtXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("loadMock", "network", "lmock",
        converter::xml::ExtensionXmlVersions({
            {"http://www.powsybl.org/schema/iidm/ext/load_element_mock/1_0", "lmock", "loadElementMock",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_1(), {0,1}},
            {"http://www.powsybl.org/schema/iidm/ext/load_element_mock/1_1", "lem", "loadEltMock",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_1(), {0,2}},
            {"http://www.powsybl.org/schema/iidm/ext/load_mock/1_0", "lmock", "loadMock",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_1(), {1,0}},
            {"http://www.powsybl.org/schema/iidm/ext/load_mock/1_1", "lmock", "loadMock",
            converter::xml::IidmXmlVersion::V1_1(), {1,1}},
            {"http://www.powsybl.org/schema/iidm/ext/load_mock/1_2", "lmock", "loadMock",
            converter::xml::IidmXmlVersion::V1_1(), {1,2}}
        })){
}

Extension& LoadMockExtXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    checkReadingCompatibility(context);

    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    extendable.addExtension(stdcxx::make_unique<LoadMockExt>(load));
    return extendable.getExtension<LoadMockExt>();
}

void LoadMockExtXmlSerializer::write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const {
    // Nothing to do
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
