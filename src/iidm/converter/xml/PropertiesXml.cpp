/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>

#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void PropertiesXml::read(PropertiesHolder& propertiesHolder, const NetworkXmlReaderContext& context) {
    read<PropertiesHolder>(context)(propertiesHolder);
}

void PropertiesXml::skip(const NetworkXmlReaderContext& context) {
    if(context.getReader().getLocalName() != PROPERTY) {
        throw PowsyblException(stdcxx::format("Unexpected element name <%1%> (expected <%2%>)", context.getReader().getLocalName(), PROPERTY));
    }

    context.getReader().getAttributeValue(NAME);
    context.getReader().getAttributeValue(VALUE);
}

void PropertiesXml::write(const PropertiesHolder& propertiesHolder, const std::string& nsprefix, powsybl::xml::XmlStreamWriter& writer) {
    for (const auto& name : propertiesHolder.getPropertyNames()) {
        const auto& value = propertiesHolder.getProperty(name);
        writer.writeStartElement(nsprefix, PROPERTY);
        writer.writeAttribute(NAME, name);
        writer.writeAttribute(VALUE, value);
        writer.writeEndElement();
    }
}

void PropertiesXml::write(const PropertiesHolder& propertiesHolder, NetworkXmlWriterContext& context) {
    write(propertiesHolder, context.getVersion().getPrefix(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
