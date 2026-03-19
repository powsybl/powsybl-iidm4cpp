/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HXX

#include <powsybl/iidm/converter/xml/AbstractComplexIdentifiableXml.hpp>

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
void AbstractComplexIdentifiableXml<Added, Adder, Parent>::read(Parent& parent, NetworkXmlReaderContext& context) const {
    if(!postponeElementCreation()) {
        std::vector<std::function<void(Identifiable&)>> toApply;
        Adder adder = this->createAdder(parent);
        std::string id = AbstractIdentifiableXml<Added, Adder, Parent>::readIdentifierAttributes(adder, context);
        readRootElementAttributes(adder, toApply, context);
        readSubElements(id, adder, toApply, context);

        Added& identifiable = adder.add();
        for(auto func : toApply) {
            func(identifiable);
        }
    } else {
        readAndPostponeCreation(parent, context);
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractComplexIdentifiableXml<Added, Adder, Parent>::readSubElements(const std::string& id, std::vector<std::function<void(Identifiable&)>>& toApply , NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == PROPERTY) {
        PropertiesXml::read(toApply, context);
    } else if (context.getReader().getLocalName() == ALIAS) {
        IidmXmlUtil::assertMinimumVersion(this->getRootElementName(), ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
        AliasesXml::read(toApply, context);
    } else {
        throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), id));
    }
}

template <typename Added, typename Adder, typename Parent>
stdcxx::optional<XmlReaderEndTask::Step> AbstractComplexIdentifiableXml<Added, Adder, Parent>::postponeElementCreation() const {
    return stdcxx::optional<XmlReaderEndTask::Step>();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HXX