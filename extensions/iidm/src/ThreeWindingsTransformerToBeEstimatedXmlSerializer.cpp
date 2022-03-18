/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimatedXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimated.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimatedAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerToBeEstimatedXmlSerializer::ThreeWindingsTransformerToBeEstimatedXmlSerializer() :
    AbstractExtensionXmlSerializer("threeWindingsTransformerToBeEstimated", "network", "threettbe", "http://www.powsybl.org/schema/iidm/ext/three_windings_transformer_to_be_estimated/1_0") {
}

Extension& ThreeWindingsTransformerToBeEstimatedXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    extendable.newExtension<ThreeWindingsTransformerToBeEstimatedAdder>()
        .withRatioTapChanger1Status(context.getReader().getAttributeValue<bool>(RATIO_TAP_CHANGER_1_STATUS))
        .withRatioTapChanger2Status(context.getReader().getAttributeValue<bool>(RATIO_TAP_CHANGER_2_STATUS))
        .withRatioTapChanger3Status(context.getReader().getAttributeValue<bool>(RATIO_TAP_CHANGER_3_STATUS))
        .withPhaseTapChanger1Status(context.getReader().getAttributeValue<bool>(PHASE_TAP_CHANGER_1_STATUS))
        .withPhaseTapChanger2Status(context.getReader().getAttributeValue<bool>(PHASE_TAP_CHANGER_2_STATUS))
        .withPhaseTapChanger3Status(context.getReader().getAttributeValue<bool>(PHASE_TAP_CHANGER_3_STATUS))
        .add();
    return extendable.getExtension<ThreeWindingsTransformerToBeEstimated>();
}

void ThreeWindingsTransformerToBeEstimatedXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& ext = safeCast<ThreeWindingsTransformerToBeEstimated>(extension);
    context.getWriter().writeAttribute(RATIO_TAP_CHANGER_1_STATUS, ext.shouldEstimateRatioTapChanger1());
    context.getWriter().writeAttribute(RATIO_TAP_CHANGER_2_STATUS, ext.shouldEstimateRatioTapChanger2());
    context.getWriter().writeAttribute(RATIO_TAP_CHANGER_3_STATUS, ext.shouldEstimateRatioTapChanger3());
    context.getWriter().writeAttribute(PHASE_TAP_CHANGER_1_STATUS, ext.shouldEstimatePhaseTapChanger1());
    context.getWriter().writeAttribute(PHASE_TAP_CHANGER_2_STATUS, ext.shouldEstimatePhaseTapChanger2());
    context.getWriter().writeAttribute(PHASE_TAP_CHANGER_3_STATUS, ext.shouldEstimatePhaseTapChanger3());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
