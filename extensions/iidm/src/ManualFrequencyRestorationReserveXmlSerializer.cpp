/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserveXmlSerializer.hpp>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserve.hpp>
#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserveAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include <iostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ManualFrequencyRestorationReserveXmlSerializer::ManualFrequencyRestorationReserveXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("manualFrequencyRestorationReserve", "network", "mfrr", "http://www.powsybl.org/schema/iidm/ext/manual_frequency_restoration_reserve/1_0") {
}

Extension& ManualFrequencyRestorationReserveXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Injection>(extendable)) {
         throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Injection>()));
    }

    const auto& participate = context.getReader().getAttributeValue<bool>("participate");

    extendable.newExtension<ManualFrequencyRestorationReserveAdder>()
                .withParticipate(participate)
                .add();
    return extendable.getExtension<ManualFrequencyRestorationReserve>();
}

void ManualFrequencyRestorationReserveXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& mfrr = safeCast<ManualFrequencyRestorationReserve>(extension);

    context.getWriter().writeAttribute("participate", mfrr.isParticipate());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
