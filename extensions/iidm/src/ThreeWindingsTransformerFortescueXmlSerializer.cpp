/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescueXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/LegFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/LegFortescueAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescueAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerFortescueXmlSerializer::ThreeWindingsTransformerFortescueXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("threeWindingsTransformerFortescue", "network", "t3f", "http://www.powsybl.org/schema/iidm/ext/three_windings_transformer_fortescue/1_0") {
}

Extension& ThreeWindingsTransformerFortescueXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<ThreeWindingsTransformer>()));
    }

    auto twtFAdder = extendable.newExtension<ThreeWindingsTransformerFortescueAdder>();

    context.getReader().readUntilEndElement(getExtensionName(), [this, &context, &twtFAdder]() {
        if (context.getReader().getLocalName() == "leg1") {
            LegFortescueAdder legAdder = twtFAdder.newLeg1();
            readLeg(legAdder, context);
        } else if (context.getReader().getLocalName() == "leg2") {
            LegFortescueAdder legAdder = twtFAdder.newLeg2();
            readLeg(legAdder, context);
        } else if (context.getReader().getLocalName() == "leg3") {
            LegFortescueAdder legAdder = twtFAdder.newLeg3();
            readLeg(legAdder, context);
        } else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });
    twtFAdder.add();
    return extendable.getExtension<ThreeWindingsTransformerFortescue>();
}

void ThreeWindingsTransformerFortescueXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& fortescueTwt = safeCast<ThreeWindingsTransformerFortescue>(extension);

    writeLeg("leg1", fortescueTwt.getLeg1(), context);
    writeLeg("leg2", fortescueTwt.getLeg2(), context);
    writeLeg("leg3", fortescueTwt.getLeg3(), context);

}


void ThreeWindingsTransformerFortescueXmlSerializer::readLeg(LegFortescueAdder& legAdder, const converter::xml::NetworkXmlReaderContext& context) const {

    double rz = context.getReader().getOptionalAttributeValue(FORTESCUE_RZ, stdcxx::nan());
    double xz = context.getReader().getOptionalAttributeValue(FORTESCUE_XZ, stdcxx::nan());
    bool freeFluxes = context.getReader().getAttributeValue<bool>(FORTESCUE_FREE_FLUXES);
    const auto& connectionType = Enum::fromString<WindingConnectionType>(context.getReader().getAttributeValue(CONNECTION_TYPE));
    double groundingR = context.getReader().getOptionalAttributeValue(FORTESCUE_GROUNDING_R, 0.0);
    double groundingX = context.getReader().getOptionalAttributeValue(FORTESCUE_GROUNDING_X, 0.0);
    legAdder.withRz(rz)
            .withXz(xz)
            .withFreeFluxes(freeFluxes)
            .withConnectionType(connectionType)
            .withGroundingR(groundingR)
            .withGroundingX(groundingX)
            .add();

}

void ThreeWindingsTransformerFortescueXmlSerializer::writeLeg(const std::string& name, const LegFortescue& leg, converter::xml::NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(getNamespacePrefix(), name);
    context.getWriter().writeOptionalAttribute(FORTESCUE_RZ, leg.getRz());
    context.getWriter().writeOptionalAttribute(FORTESCUE_XZ, leg.getXz());
    context.getWriter().writeAttribute(FORTESCUE_FREE_FLUXES, leg.isFreeFluxes());
    context.getWriter().writeAttribute(CONNECTION_TYPE, Enum::toString(leg.getConnectionType()));
    context.getWriter().writeOptionalAttribute(FORTESCUE_GROUNDING_R, leg.getGroundingR(), 0.0);
    context.getWriter().writeOptionalAttribute(FORTESCUE_GROUNDING_X, leg.getGroundingX(), 0.0);

    context.getWriter().writeEndElement();
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
