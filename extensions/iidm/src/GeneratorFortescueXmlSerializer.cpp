/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorFortescueXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorFortescueAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorFortescueXmlSerializer::GeneratorFortescueXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("generatorFortescue", "network", "gf", "http://www.powsybl.org/schema/iidm/ext/generator_fortescue/1_0") {
}

Extension& GeneratorFortescueXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }

    const xml::XmlStreamReader& reader = context.getReader();
    double rz = reader.getOptionalAttributeValue(FORTESCUE_RZ, stdcxx::nan());
    double xz = reader.getOptionalAttributeValue(FORTESCUE_XZ, stdcxx::nan());
    double rn = reader.getOptionalAttributeValue(FORTESCUE_RN, stdcxx::nan());
    double xn = reader.getOptionalAttributeValue(FORTESCUE_XN, stdcxx::nan());
    bool toGround = reader.getAttributeValue<bool>(FORTESCUE_GROUNDED);
    double groundingR = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_R, 0.0);
    double groundingX = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_X, 0.0);

    extendable.newExtension<GeneratorFortescueAdder>()
                .withRz(rz)
                .withXz(xz)
                .withRn(rn)
                .withXn(xn)
                .withGrounded(toGround)
                .withGroundingR(groundingR)
                .withGroundingX(groundingX)
                .add();
    return extendable.getExtension<GeneratorFortescue>();
}

void GeneratorFortescueXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& fortescueGen = safeCast<GeneratorFortescue>(extension);

    xml::XmlStreamWriter& writer = context.getWriter();
    writer.writeOptionalAttribute(FORTESCUE_RZ,fortescueGen.getRz());
    writer.writeOptionalAttribute(FORTESCUE_XZ,fortescueGen.getXz());
    writer.writeOptionalAttribute(FORTESCUE_RN,fortescueGen.getRn());
    writer.writeOptionalAttribute(FORTESCUE_XN,fortescueGen.getXn());
    writer.writeAttribute(FORTESCUE_GROUNDED, fortescueGen.isGrounded());
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_R,fortescueGen.getGroundingR(), 0.0);
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_X,fortescueGen.getGroundingX(), 0.0);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
