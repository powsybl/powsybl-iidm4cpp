/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AliasesXml.hpp"

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
    identifiable.addAlias(context.getReader().readUntilEndElement(ALIAS));
}

void AliasesXml::write(const Identifiable& identifiable, const std::string& rootElementName, NetworkXmlWriterContext& context) {
    IidmXmlUtil::assertMinimumVersionIfNotDefault(!identifiable.getAliases().empty(), rootElementName, ALIAS, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_3(), context.getVersion());
    for (const std::string& alias : identifiable.getAliases()) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), ALIAS);
        context.getWriter().writeCharacters(context.getAnonymizer().anonymizeString(alias));
        context.getWriter().writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
