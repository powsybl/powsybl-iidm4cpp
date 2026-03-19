/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "OverloadManagementSystemMockXmlSerializer.hpp"

#include "OverloadManagementSystemMockExt.hpp"
#include <powsybl/iidm/OverloadManagementSystem.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

OverloadManagementSystemMockXmlSerializer::OverloadManagementSystemMockXmlSerializer() :
    AbstractExtensionXmlSerializer("omsMock", "network", "omsmock", "http://www.powsybl.org/schema/iidm/ext/overloadmanagementsystem_mock/1_0") {
}

Extension& OverloadManagementSystemMockXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<OverloadManagementSystem>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<OverloadManagementSystem>()));
    }
    auto& oms = dynamic_cast<OverloadManagementSystem&>(extendable);

    const std::string& foo = context.getReader().getAttributeValue("foo");
    extendable.addExtension(stdcxx::make_unique<OverloadManagementSystemMockExt>(oms, foo));
    return extendable.getExtension<OverloadManagementSystemMockExt>();
}

void OverloadManagementSystemMockXmlSerializer::write(const Extension& extension, NetworkXmlWriterContext& context) const {
    const auto& omsMockExt = safeCast<OverloadManagementSystemMockExt>(extension);
    context.getWriter().writeAttribute("foo", omsMockExt.getFoo());
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
