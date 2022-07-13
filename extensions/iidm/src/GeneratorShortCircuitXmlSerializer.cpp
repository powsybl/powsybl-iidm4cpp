/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuitXmlSerializer.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuitAdder.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorShortCircuitXmlSerializer::GeneratorShortCircuitXmlSerializer() :
    AbstractExtensionXmlSerializer("generatorShortCircuit", "network", "gsc", "http://www.itesla_project.eu/schema/iidm/ext/generator_short_circuit/1_0") {
}

Extension& GeneratorShortCircuitXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    double directSubtransX = context.getReader().getOptionalAttributeValue(DIRECT_SUBTRANS_X, stdcxx::nan());
    double directTransX = context.getReader().getOptionalAttributeValue(DIRECT_TRANS_X, stdcxx::nan());
    double stepUpTransformerX = context.getReader().getOptionalAttributeValue(STEP_UP_TRANSFORMER_X, stdcxx::nan());
    extendable.newExtension<GeneratorShortCircuitAdder>()
        .withDirectSubtransX(directSubtransX)
        .withDirectTransX(directTransX)
        .withStepUpTransformerX(stepUpTransformerX)
        .add();
    return extendable.getExtension<GeneratorShortCircuit>();
}

void GeneratorShortCircuitXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& gsc = safeCast<GeneratorShortCircuit>(extension);
    context.getWriter().writeAttribute(DIRECT_SUBTRANS_X, gsc.getDirectSubtransX());
    context.getWriter().writeAttribute(DIRECT_TRANS_X, gsc.getDirectTransX());
    context.getWriter().writeAttribute(STEP_UP_TRANSFORMER_X, gsc.getStepUpTransformerX());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
