/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuitXmlSerializer.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuitAdder.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

IdentifiableShortCircuitXmlSerializer::IdentifiableShortCircuitXmlSerializer() :
    AbstractExtensionXmlSerializer("identifiableShortCircuit", "network", "isc", "http://www.powsybl.org/schema/iidm/ext/identifiable_short_circuit/1_0") {
}

Extension& IdentifiableShortCircuitXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Identifiable>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
    }
    const auto& ipMax = context.getReader().getAttributeValue<double>(IP_MAX);
    const auto& ipMin = context.getReader().getOptionalAttributeValue(IP_MIN, stdcxx::nan());
    extendable.newExtension<IdentifiableShortCircuitAdder>()
        .withIpMax(ipMax)
        .withIpMin(ipMin)
        .add();
    return extendable.getExtension<IdentifiableShortCircuit>();
}

void IdentifiableShortCircuitXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& isc = safeCast<IdentifiableShortCircuit>(extension);
    context.getWriter().writeAttribute(IP_MAX, isc.getIpMax());
    context.getWriter().writeAttribute(IP_MIN, isc.getIpMin());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
