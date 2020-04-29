/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/XnodeXmlSerializer.hpp>

#include <powsybl/iidm/DanglingLine.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/entsoe/Xnode.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

XnodeXmlSerializer::XnodeXmlSerializer() :
    AbstractExtensionXmlSerializer("xnode", "network", "xn", "http://www.itesla_project.eu/schema/iidm/ext/xnode/1_0") {
}

std::unique_ptr<Extension> XnodeXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<DanglingLine>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<DanglingLine>()));
    }
    auto& dl = dynamic_cast<DanglingLine&>(extendable);

    const auto& code = context.getReader().getAttributeValue("code");

    return stdcxx::make_unique<Extension, Xnode>(dl, code);
}

void XnodeXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& xnode = safeCast<Xnode>(extension);
    context.getExtensionsWriter().writeAttribute("code", xnode.getCode());
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
