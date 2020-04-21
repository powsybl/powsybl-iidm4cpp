/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockXmlSerializer.hpp>

#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClock.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerPhaseAngleClockXmlSerializer::TwoWindingsTransformerPhaseAngleClockXmlSerializer() :
    ExtensionXmlSerializer("twoWindingsTransformerPhaseAngleClock", "network", logging::format("http://www.%1%/schema/iidm/ext/two_windings_transformer_phase_angle_clock/1_0", converter::IIDM_DOMAIN), "twowtpac") {
}

std::unique_ptr<Extension> TwoWindingsTransformerPhaseAngleClockXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<TwoWindingsTransformer>()));
    }
    auto& transformer = dynamic_cast<TwoWindingsTransformer&>(extendable);

    const auto& phaseAngleClock = context.getReader().getOptionalAttributeValue("phaseAngleClock", 0UL);

    return stdcxx::make_unique<Extension, TwoWindingsTransformerPhaseAngleClock>(transformer, phaseAngleClock);
}

void TwoWindingsTransformerPhaseAngleClockXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& ext = safeCast<TwoWindingsTransformerPhaseAngleClock>(extension);

    context.getExtensionsWriter().writeOptionalAttribute("phaseAngleClock", ext.getPhaseAngleClock(), 0UL);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
