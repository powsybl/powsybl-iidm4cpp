/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/EntsoeAreaXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Substation.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/entsoe/EntsoeArea.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeGeographicalCode.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

EntsoeAreaXmlSerializer::EntsoeAreaXmlSerializer() :
    ExtensionXmlSerializer("entsoeArea", "network", "http://www.itesla_project.eu/schema/iidm/ext/entsoe_area/1_0", "ea") {

}

std::unique_ptr<Extension> EntsoeAreaXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Substation>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Substation>()));
    }
    auto& substation = dynamic_cast<Substation&>(extendable);

    const auto& code = Enum::fromString<EntsoeGeographicalCode>(context.getReader().readUntilEndElement(getExtensionName()));

    return stdcxx::make_unique<EntsoeArea>(substation, code);
}

void EntsoeAreaXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& entsoeArea = safeCast<EntsoeArea>(extension);
    context.getExtensionsWriter().writeCharacters(Enum::toString<EntsoeGeographicalCode>(entsoeArea.getCode()));
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
