/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AbstractIdentifiableXml.hpp"

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Properties.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/xml/PropertiesXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::read(Parent& parent, NetworkXmlReaderContext& context) const {
    Adder adder = createAdder(parent);
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    const std::string& name = context.getAnonymizer().deanonymizeString(context.getReader().getOptionalAttributeValue(NAME, ""));
    adder.setId(id).setName(name);
    Added& identifiable = readRootElementAttributes(adder, context);
    readSubElements(identifiable, context);
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::readSubElements(Added& identifiable, NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == PROPERTY) {
        PropertiesXml::read(identifiable, context);
    } else {
        throw PowsyblException(logging::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), identifiable.getId()));
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::write(const Added& identifiable, const Parent& parent, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(IIDM_PREFIX, getRootElementName());

    context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(identifiable.getId()));
    if (identifiable.getId() != identifiable.getName()) {
        context.getWriter().writeAttribute(NAME, context.getAnonymizer().anonymizeString(identifiable.getName()));
    }
    writeRootElementAttributes(identifiable, parent, context);

    PropertiesXml::write(identifiable, context);

    writeSubElements(identifiable, parent, context);

    context.getWriter().writeEndElement();

    context.addExportedEquipment(identifiable);
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::writeSubElements(const Added& /*identifiable*/, const Parent& /*parent*/, xml::NetworkXmlWriterContext&/*context*/) const {
    // Nothing to do
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
