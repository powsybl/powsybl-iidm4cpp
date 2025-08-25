/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void PropertiesXml::read(Identifiable& identifiable, NetworkXmlReaderContext& context) {
    read(context)(identifiable);
}

void PropertiesXml::read(std::vector<std::function<void(Identifiable&)>>& toApply, const NetworkXmlReaderContext& context) {
    toApply.emplace_back(read(context));
}

std::function<void(Identifiable&)> PropertiesXml::read(const NetworkXmlReaderContext& context) {
    if(context.getReader().getLocalName() != PROPERTY) {
        throw PowsyblException(stdcxx::format("Unexpected element name <%1%> (expected <%2%>)", context.getReader().getLocalName(), PROPERTY));
    }

    const std::string& name = context.getReader().getAttributeValue(NAME);
    const std::string& value = context.getReader().getAttributeValue(VALUE);

    std::function<void(Identifiable &)> fun = [name,value](Identifiable &identifiable) {
        identifiable.setProperty(name, value);
    };

    return fun;
}

void PropertiesXml::write(const Identifiable& identifiable, NetworkXmlWriterContext& context) {
    for (const auto& name : identifiable.getPropertyNames()) {
        const auto& value = identifiable.getProperty(name);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), PROPERTY);
        context.getWriter().writeAttribute(NAME, name);
        context.getWriter().writeAttribute(VALUE, value);
        context.getWriter().writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
