/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimatedXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimated.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimatedAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerToBeEstimatedXmlSerializer::TwoWindingsTransformerToBeEstimatedXmlSerializer() :
    AbstractExtensionXmlSerializer("twoWindingsTransformerToBeEstimated", "network", "twottbe", "http://www.powsybl.org/schema/iidm/ext/two_windings_transformer_to_be_estimated/1_0") {
}

Extension& TwoWindingsTransformerToBeEstimatedXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    extendable.newExtension<TwoWindingsTransformerToBeEstimatedAdder>()
        .withRatioTapChangerStatus(context.getReader().getAttributeValue<bool>(RATIO_TAP_CHANGER_STATUS))
        .withPhaseTapChangerStatus(context.getReader().getAttributeValue<bool>(PHASE_TAP_CHANGER_STATUS))
        .add();
    return extendable.getExtension<TwoWindingsTransformerToBeEstimated>();
}

void TwoWindingsTransformerToBeEstimatedXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& ext = safeCast<TwoWindingsTransformerToBeEstimated>(extension);
    context.getWriter().writeAttribute(RATIO_TAP_CHANGER_STATUS, ext.shouldEstimateRatioTapChanger());
    context.getWriter().writeAttribute(PHASE_TAP_CHANGER_STATUS, ext.shouldEstimatePhaseTapChanger());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
