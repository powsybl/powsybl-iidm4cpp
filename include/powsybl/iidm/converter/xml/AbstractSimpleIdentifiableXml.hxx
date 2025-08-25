/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HXX

#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/AliasesXml.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder, typename Parent>
void AbstractSimpleIdentifiableXml<Added, Adder, Parent>::read(Parent& parent, NetworkXmlReaderContext& context) const {
    Adder adder = this->createAdder(parent);
    AbstractIdentifiableXml<Added, Adder, Parent>::readIdentifierAttributes(adder, context);

    Added& identifiable = readRootElementAttributes(adder, context);
    readSubElements(identifiable, context);
}

template <typename Added, typename Adder, typename Parent>
void AbstractSimpleIdentifiableXml<Added, Adder, Parent>::readSubElements(Added& identifiable, NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == PROPERTY) {
        PropertiesXml::read(identifiable, context);
    } else if (context.getReader().getLocalName() == ALIAS) {
        IidmXmlUtil::assertMinimumVersion(this->getRootElementName(), ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
        AliasesXml::read(identifiable, context);
    } else {
        throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), identifiable.getId()));
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HXX