/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LinePositionXmlSerializer.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>
#include <powsybl/iidm/extensions/iidm/LinePosition.hpp>
#include <powsybl/iidm/extensions/iidm/LinePositionAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LinePositionXmlSerializer::LinePositionXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("linePosition", "network", "lp", "http://www.powsybl.org/schema/iidm/ext/line_position/1_0") {
}

Extension& LinePositionXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Line>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Line>()));
    }

    std::vector<Coordinate> coordinates;
    const xml::XmlStreamReader& reader = context.getReader();
    reader.readUntilEndElement("linePosition", [&reader, &coordinates](){
        if (reader.getLocalName() == COORDINATE) {
            double latitude = reader.getAttributeValue<double>(LATITUDE);
            double longitude = reader.getAttributeValue<double>(LONGITUDE);
            coordinates.emplace_back(latitude, longitude);
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", reader.getLocalName()));
        }
    });

    extendable.newExtension<LinePositionAdder>()
        .withCoordinates(coordinates)
        .add();
    return extendable.getExtension<LinePosition>();
}

void LinePositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& linePosition = safeCast<LinePosition>(extension);

    for(const auto& coord : linePosition.getCoordinates()) {
        context.getWriter().writeStartElement(getNamespacePrefix(), COORDINATE);
        context.getWriter().writeAttribute(LONGITUDE, coord.getLongitude());
        context.getWriter().writeAttribute(LATITUDE, coord.getLatitude());
        context.getWriter().writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
