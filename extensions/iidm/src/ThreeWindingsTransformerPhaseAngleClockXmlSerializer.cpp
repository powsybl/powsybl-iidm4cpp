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

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockAdder.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerPhaseAngleClockXmlSerializer::ThreeWindingsTransformerPhaseAngleClockXmlSerializer() :
    AbstractExtensionXmlSerializer("threeWindingsTransformerPhaseAngleClock", "network", "threewtpac", "http://www.powsybl.org/schema/iidm/ext/three_windings_transformer_phase_angle_clock/1_0") {
}

Extension& ThreeWindingsTransformerPhaseAngleClockXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& phaseAngleClockLeg2 = context.getReader().getOptionalAttributeValue("phaseAngleClockLeg2", 0UL);
    const auto& phaseAngleClockLeg3 = context.getReader().getOptionalAttributeValue("phaseAngleClockLeg3", 0UL);

    extendable.newExtension<ThreeWindingsTransformerPhaseAngleClockAdder>().withPhaseAngleClockLeg2(phaseAngleClockLeg2).withPhaseAngleClockLeg3(phaseAngleClockLeg3).add();
    return extendable.getExtension<ThreeWindingsTransformerPhaseAngleClock>();
}

    void ThreeWindingsTransformerPhaseAngleClockXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
        const auto& ext = safeCast<ThreeWindingsTransformerPhaseAngleClock>(extension);

        context.getWriter().writeOptionalAttribute("phaseAngleClockLeg2", ext.getPhaseAngleClockLeg2(), 0UL);
        context.getWriter().writeOptionalAttribute("phaseAngleClockLeg3", ext.getPhaseAngleClockLeg3(), 0UL);
    }

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

