/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockXmlSerializer.hpp>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerPhaseAngleClockXmlSerializer::ThreeWindingsTransformerPhaseAngleClockXmlSerializer() :
    ExtensionXmlSerializer("threeWindingsTransformerPhaseAngleClock", "network", logging::format("http://www.%1%/schema/iidm/ext/three_windings_transformer_phase_angle_clock/1_0", converter::IIDM_DOMAIN), "threewtpac") {
}

std::unique_ptr<Extension> ThreeWindingsTransformerPhaseAngleClockXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<ThreeWindingsTransformer>()));
    }
    auto& transformer = dynamic_cast<ThreeWindingsTransformer&>(extendable);

    const auto& phaseAngleClockLeg2 = context.getReader().getOptionalAttributeValue("phaseAngleClockLeg2", 0UL);
    const auto& phaseAngleClockLeg3 = context.getReader().getOptionalAttributeValue("phaseAngleClockLeg3", 0UL);

    return stdcxx::make_unique<Extension, ThreeWindingsTransformerPhaseAngleClock>(transformer, phaseAngleClockLeg2, phaseAngleClockLeg3);
}

void ThreeWindingsTransformerPhaseAngleClockXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& ext = safeCast<ThreeWindingsTransformerPhaseAngleClock>(extension);

    context.getExtensionsWriter().writeOptionalAttribute("phaseAngleClockLeg2", ext.getPhaseAngleClockLeg2(), 0UL);
    context.getExtensionsWriter().writeOptionalAttribute("phaseAngleClockLeg3", ext.getPhaseAngleClockLeg3(), 0UL);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
