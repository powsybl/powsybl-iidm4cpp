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
#include <powsybl/iidm/extensions/entsoe/XnodeAdder.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

XnodeXmlSerializer::XnodeXmlSerializer() :
    AbstractExtensionXmlSerializer("xnode", "network", "xn", "http://www.itesla_project.eu/schema/iidm/ext/xnode/1_0") {
}

Extension& XnodeXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& code = context.getReader().getAttributeValue("code");
    extendable.newExtension<XnodeAdder>().withCode(code).add();
    return extendable.getExtension<Xnode>();
}

void XnodeXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& xnode = safeCast<Xnode>(extension);
    context.getExtensionsWriter().writeAttribute("code", xnode.getCode());
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
