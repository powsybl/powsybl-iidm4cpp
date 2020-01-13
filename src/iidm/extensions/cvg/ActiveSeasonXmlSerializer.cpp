/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/ActiveSeasonXmlSerializer.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/ActiveSeason.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

ActiveSeasonXmlSerializer::ActiveSeasonXmlSerializer() :
    ExtensionXmlSerializer("activeSeason", "network", "http://www.itesla_project.eu/schema/iidm/ext/active-season/1_0", "as") {
}

std::unique_ptr<Extension> ActiveSeasonXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Connectable>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
    }
    auto& connectable = dynamic_cast<Connectable&>(extendable);

    const std::string& value = context.getReader().readUntilEndElement(getExtensionName());
    return stdcxx::make_unique<ActiveSeason>(connectable, value);
}

void ActiveSeasonXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& activeSeason = safeCast<ActiveSeason>(extension);
    context.getExtensionsWriter().writeCharacters(activeSeason.getValue());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

