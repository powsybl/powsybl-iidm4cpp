/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/shortcircuits/GeneratorShortCircuitsXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/shortcircuits/GeneratorShortCircuits.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

GeneratorShortCircuitsXmlSerializer::GeneratorShortCircuitsXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("generatorShortCircuits", "network", "http://www.itesla_project.eu/schema/iidm/ext/generator_short_circuits/1_0", "gsc") {
}

std::unique_ptr<Extension> GeneratorShortCircuitsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    auto& generator = dynamic_cast<Generator&>(extendable);

    const auto& transientReactance = context.getReader().getAttributeValue<double>("transientReactance");
    const auto& stepUpTransformerReactance = context.getReader().getAttributeValue<double>("stepUpTransformerReactance");
    return stdcxx::make_unique<GeneratorShortCircuits>(generator, transientReactance, stepUpTransformerReactance);
}

void GeneratorShortCircuitsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& shortCircuits = safeCast<GeneratorShortCircuits>(extension);
    context.getExtensionsWriter().writeAttribute("transientReactance", shortCircuits.getTransientReactance());
    context.getExtensionsWriter().writeAttribute("stepUpTransformerReactance", shortCircuits.getStepUpTransformerReactance());
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

