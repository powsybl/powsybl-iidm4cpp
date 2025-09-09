/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LoadAsymmetricalXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/LoadAsymmetrical.hpp>
#include <powsybl/iidm/extensions/iidm/LoadAsymmetricalAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LoadAsymmetricalXmlSerializer::LoadAsymmetricalXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("loadAsymmetrical", "network", "las", "http://www.powsybl.org/schema/iidm/ext/load_asymmetrical/1_0") {
}

Extension& LoadAsymmetricalXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Load>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }

    const xml::XmlStreamReader& reader = context.getReader();
    const auto& connectionType = Enum::fromString<LoadConnectionType>(reader.getAttributeValue(CONNECTION_TYPE));
    double deltaPa = reader.getOptionalAttributeValue("deltaPa", 0.0);
    double deltaQa = reader.getOptionalAttributeValue("deltaQa", 0.0);
    double deltaPb = reader.getOptionalAttributeValue("deltaPb", 0.0);
    double deltaQb = reader.getOptionalAttributeValue("deltaQb", 0.0);
    double deltaPc = reader.getOptionalAttributeValue("deltaPc", 0.0);
    double deltaQc = reader.getOptionalAttributeValue("deltaQc", 0.0);

    extendable.newExtension<LoadAsymmetricalAdder>()
                .withConnectionType(connectionType)
                .withDeltaPa(deltaPa)
                .withDeltaQa(deltaQa)
                .withDeltaPb(deltaPb)
                .withDeltaQb(deltaQb)
                .withDeltaPc(deltaPc)
                .withDeltaQc(deltaQc)
                .add();
    return extendable.getExtension<LoadAsymmetrical>();
}

void LoadAsymmetricalXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& loadAsymmetrical = safeCast<LoadAsymmetrical>(extension);

    xml::XmlStreamWriter& writer = context.getWriter();
    writer.writeAttribute(CONNECTION_TYPE, Enum::toString(loadAsymmetrical.getConnectionType()));
    writer.writeOptionalAttribute("deltaPa", loadAsymmetrical.getDeltaPa(), 0.0);
    writer.writeOptionalAttribute("deltaQa", loadAsymmetrical.getDeltaQa(), 0.0);
    writer.writeOptionalAttribute("deltaPb", loadAsymmetrical.getDeltaPb(), 0.0);
    writer.writeOptionalAttribute("deltaQb", loadAsymmetrical.getDeltaQb(), 0.0);
    writer.writeOptionalAttribute("deltaPc", loadAsymmetrical.getDeltaPc(), 0.0);
    writer.writeOptionalAttribute("deltaQc", loadAsymmetrical.getDeltaQc(), 0.0);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
