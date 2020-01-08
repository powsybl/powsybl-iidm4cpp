/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/shortcircuits/VoltageLevelShortCircuitsXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/shortcircuits/VoltageLevelShortCircuits.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

VoltageLevelShortCircuitsXmlSerializer::VoltageLevelShortCircuitsXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("voltageLevelShortCircuits", "network", "http://www.itesla_project.eu/schema/iidm/ext/voltagelevel_short_circuits/1_0", "vlsc") {
}

std::unique_ptr<Extension> VoltageLevelShortCircuitsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<VoltageLevel>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<VoltageLevel>()));
    }
    auto& voltageLevel = dynamic_cast<VoltageLevel&>(extendable);

    const auto& minShortCircuitsCurrent = context.getReader().getAttributeValue<double>("minShortCircuitsCurrent");
    const auto& maxShortCircuitsCurrent = context.getReader().getAttributeValue<double>("maxShortCircuitsCurrent");
    return stdcxx::make_unique<VoltageLevelShortCircuits>(voltageLevel, minShortCircuitsCurrent, maxShortCircuitsCurrent);
}

void VoltageLevelShortCircuitsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& shortCircuits = safeCast<VoltageLevelShortCircuits>(extension);
    context.getExtensionsWriter().writeAttribute("minShortCircuitsCurrent", shortCircuits.getMinShortCircuitsCurrent());
    context.getExtensionsWriter().writeAttribute("maxShortCircuitsCurrent", shortCircuits.getMaxShortCircuitsCurrent());
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

