/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SubstationPositionXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SubstationPositionXmlSerializer::SubstationPositionXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("substationPosition", "network", "sp", "http://www.powsybl.org/schema/iidm/ext/substation_position/1_0") {
}

Extension& SubstationPositionXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Substation>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Substation>()));
    }

    Coordinate coordinates;
    const xml::XmlStreamReader& reader = context.getReader();
    reader.readUntilEndElement("substationPosition", [&reader, &coordinates](){
        double latitude = reader.getAttributeValue<double>(LATITUDE);
        double longitude = reader.getAttributeValue<double>(LONGITUDE);
        coordinates = Coordinate(latitude, longitude);
    });

    extendable.newExtension<SubstationPositionAdder>()
        .withCoordinates(coordinates)
        .add();
    return extendable.getExtension<SubstationPosition>();
}

void SubstationPositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& substationPosition = safeCast<SubstationPosition>(extension);

    context.getWriter().writeStartElement(getNamespacePrefix(), COORDINATE);
    context.getWriter().writeAttribute(LONGITUDE, substationPosition.getCoordinates().getLongitude());
    context.getWriter().writeAttribute(LATITUDE, substationPosition.getCoordinates().getLatitude());
    context.getWriter().writeEndElement();

}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
