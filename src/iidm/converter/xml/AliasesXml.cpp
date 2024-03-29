/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AliasesXml.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void AliasesXml::read(Identifiable& identifiable, const NetworkXmlReaderContext& context) {
    std::string aliasType;
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &aliasType]() {
        aliasType = context.getReader().getOptionalAttributeValue(TYPE, "");
    });
    identifiable.addAlias(context.getAnonymizer().deanonymizeString(context.getReader().readCharacters()), aliasType);
}

void AliasesXml::write(const Identifiable& identifiable, const std::string& rootElementName, NetworkXmlWriterContext& context) {
    IidmXmlUtil::assertMinimumVersionIfNotDefault(identifiable.hasAliases(), rootElementName, ALIAS, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
    for (const std::string& alias : identifiable.getAliases()) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), ALIAS);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &alias, &identifiable]() {
            const auto& typeAlias = identifiable.getAliasType(alias);
            if (!typeAlias.empty()) {
                context.getWriter().writeAttribute(TYPE, typeAlias);
            }
        });
        context.getWriter().writeCharacters(context.getAnonymizer().anonymizeString(alias));
        context.getWriter().writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
