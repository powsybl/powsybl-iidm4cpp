/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/MergedXnodeXmlSerializer.hpp>

#include <powsybl/iidm/Line.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/entsoe/MergedXnode.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnodeAdder.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

MergedXnodeXmlSerializer::MergedXnodeXmlSerializer() :
    AbstractExtensionXmlSerializer("mergedXnode", "network", "mxn", "http://www.itesla_project.eu/schema/iidm/ext/merged_xnode/1_0") {
}

Extension& MergedXnodeXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& rdp = context.getReader().getAttributeValue<double>("rdp");
    const auto& xdp = context.getReader().getAttributeValue<double>("xdp");
    const auto& xnodeP1 = context.getReader().getAttributeValue<double>("xnodeP1");
    const auto& xnodeQ1 = context.getReader().getAttributeValue<double>("xnodeQ1");
    const auto& xnodeP2 = context.getReader().getAttributeValue<double>("xnodeP2");
    const auto& xnodeQ2 = context.getReader().getAttributeValue<double>("xnodeQ2");
    const auto& code = context.getReader().getAttributeValue("code");

    extendable.newExtension<MergedXnodeAdder>().withRdp(rdp).withXdp(xdp).withXnodeP1(xnodeP1).withXnodeQ1(xnodeQ1).withXnodeP2(xnodeP2).withXnodeQ2(xnodeQ2).withLine1Name("").withLine2Name("").withCode(code).add();
    return extendable.getExtension<MergedXnode>();
}

void MergedXnodeXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& mergedXnode = safeCast<MergedXnode>(extension);
    context.getWriter().writeAttribute("rdp", mergedXnode.getRdp());
    context.getWriter().writeAttribute("xdp", mergedXnode.getXdp());
    context.getWriter().writeAttribute("xnodeP1", mergedXnode.getXnodeP1());
    context.getWriter().writeAttribute("xnodeQ1", mergedXnode.getXnodeQ1());
    context.getWriter().writeAttribute("xnodeP2", mergedXnode.getXnodeP2());
    context.getWriter().writeAttribute("xnodeQ2", mergedXnode.getXnodeQ2());
    context.getWriter().writeAttribute("code", mergedXnode.getCode());
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
