/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/OperatingStatusXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/sld/OperatingStatus.hpp>
#include <powsybl/iidm/extensions/sld/OperatingStatusAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

OperatingStatusXmlSerializer::OperatingStatusXmlSerializer() :
    AbstractExtensionXmlSerializer("operatingStatus", "network", "os", "http://www.powsybl.org/schema/iidm/ext/operating_status/1_0") {
}

Extension& OperatingStatusXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& status = Enum::fromString<OperatingStatus::Status>(context.getReader().readUntilEndElement(getExtensionName()));
    extendable.newExtension<OperatingStatusAdder>()
        .withStatus(status)
        .add();
    return extendable.getExtension<OperatingStatus>();
}

void OperatingStatusXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& operatingStatus = safeCast<OperatingStatus>(extension);
    context.getWriter().writeCharacters(Enum::toString(operatingStatus.getStatus()));
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
