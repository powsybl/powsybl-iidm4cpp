/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescueXmlSerializer.hpp>

#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescueAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerFortescueXmlSerializer::TwoWindingsTransformerFortescueXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("twoWindingsTransformerFortescue", "network", "t2f", "http://www.powsybl.org/schema/iidm/ext/two_windings_transformer_fortescue/1_0") {
}

Extension& TwoWindingsTransformerFortescueXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<TwoWindingsTransformer>()));
    }

    const xml::XmlStreamReader& reader = context.getReader();
    double rz = reader.getOptionalAttributeValue(FORTESCUE_RZ, stdcxx::nan());
    double xz = reader.getOptionalAttributeValue(FORTESCUE_XZ, stdcxx::nan());
    bool freefluxes = reader.getAttributeValue<bool>(FORTESCUE_FREE_FLUXES);
    const auto& connectionType1 = Enum::fromString<WindingConnectionType>(reader.getAttributeValue(CONNECTION_TYPE_1));
    const auto& connectionType2 = Enum::fromString<WindingConnectionType>(reader.getAttributeValue(CONNECTION_TYPE_2));
    double groundingR1 = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_R_1, 0.0);
    double groundingX1 = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_X_1, 0.0);
    double groundingR2 = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_R_2, 0.0);
    double groundingX2 = reader.getOptionalAttributeValue(FORTESCUE_GROUNDING_X_2, 0.0);

    extendable.newExtension<TwoWindingsTransformerFortescueAdder>()
                .withRz(rz)
                .withXz(xz)
                .withFreeFluxes(freefluxes)
                .withConnectionType1(connectionType1)
                .withConnectionType2(connectionType2)
                .withGroundingR1(groundingR1)
                .withGroundingR2(groundingR2)
                .withGroundingX1(groundingX1)
                .withGroundingX2(groundingX2)
                .add();
    return extendable.getExtension<TwoWindingsTransformerFortescue>();
}

void TwoWindingsTransformerFortescueXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& fortescuetwt = safeCast<TwoWindingsTransformerFortescue>(extension);

    xml::XmlStreamWriter& writer = context.getWriter();
    writer.writeOptionalAttribute(FORTESCUE_RZ,fortescuetwt.getRz());
    writer.writeOptionalAttribute(FORTESCUE_XZ,fortescuetwt.getXz());
    writer.writeAttribute(FORTESCUE_FREE_FLUXES, fortescuetwt.isFreeFluxes());
    writer.writeAttribute(CONNECTION_TYPE_1, Enum::toString(fortescuetwt.getConnectionType1()));
    writer.writeAttribute(CONNECTION_TYPE_2, Enum::toString(fortescuetwt.getConnectionType2()));
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_R_1,fortescuetwt.getGroundingR1(), 0.0);
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_X_1,fortescuetwt.getGroundingX1(), 0.0);
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_R_2,fortescuetwt.getGroundingR2(), 0.0);
    writer.writeOptionalAttribute(FORTESCUE_GROUNDING_X_2,fortescuetwt.getGroundingX2(), 0.0);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
