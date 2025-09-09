/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LineFortescueXmlSerializer.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/LineFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/LineFortescueAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LineFortescueXmlSerializer::LineFortescueXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("lineFortescue", "network", "lf", "http://www.powsybl.org/schema/iidm/ext/line_fortescue/1_0") {
}

Extension& LineFortescueXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Line>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Line>()));
    }

    const xml::XmlStreamReader& reader = context.getReader();
    double rz = reader.getOptionalAttributeValue(FORTESCUE_RZ, stdcxx::nan());
    double xz = reader.getOptionalAttributeValue(FORTESCUE_XZ, stdcxx::nan());

    extendable.newExtension<LineFortescueAdder>()
                .withRz(rz)
                .withXz(xz)
                .add();
    return extendable.getExtension<LineFortescue>();
}

void LineFortescueXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& fortescueLine = safeCast<LineFortescue>(extension);

    xml::XmlStreamWriter& writer = context.getWriter();
    writer.writeOptionalAttribute(FORTESCUE_RZ,fortescueLine.getRz());
    writer.writeOptionalAttribute(FORTESCUE_XZ,fortescueLine.getXz());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
