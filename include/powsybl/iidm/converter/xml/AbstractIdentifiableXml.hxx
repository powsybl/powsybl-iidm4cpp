/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HXX

#include <powsybl/iidm/converter/xml/AbstractIdentifiableXml.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/AliasesXml.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

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
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(FICTITIOUS, false);
        adder.setFictitious(fictitious);
    });
    readElement(id, adder, context);
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::readElement(const std::string& /*id*/, Adder& adder, NetworkXmlReaderContext& context) const {
    Added& identifiable = readRootElementAttributes(adder, context);
    readSubElements(identifiable, context);
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::readSubElements(Added& identifiable, NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == PROPERTY) {
        PropertiesXml::read(identifiable, context);
    } else if (context.getReader().getLocalName() == ALIAS) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
        AliasesXml::read(identifiable, context);
    } else {
        throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), identifiable.getId()));
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractIdentifiableXml<Added, Adder, Parent>::write(const Added& identifiable, const Parent& parent, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(context.getVersion().getPrefix(), getRootElementName());

    context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(identifiable.getId()));
    const auto& name = identifiable.getOptionalName();
    if (!name.empty()) {
        context.getWriter().writeAttribute(NAME, context.getAnonymizer().anonymizeString(name));
    }

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &identifiable]() {
        context.getWriter().writeOptionalAttribute(FICTITIOUS, identifiable.isFictitious(), false);
    });

    writeRootElementAttributes(identifiable, parent, context);

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [this, &context, &identifiable]() {
        AliasesXml::write(identifiable, getRootElementName(), context);
    });

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

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HXX
