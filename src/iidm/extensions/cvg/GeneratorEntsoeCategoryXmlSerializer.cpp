/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/GeneratorEntsoeCategoryXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/GeneratorEntsoeCategory.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

GeneratorEntsoeCategoryXmlSerializer::GeneratorEntsoeCategoryXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("entsoeCategory", "network", "http://www.itesla_project.eu/schema/iidm/ext/generator_entsoe_category/1_0", "gec") {
}

std::unique_ptr<Extension> GeneratorEntsoeCategoryXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    auto& generator = dynamic_cast<Generator&>(extendable);

    const std::string& code = context.getReader().readUntilEndElement(getExtensionName());
    return stdcxx::make_unique<GeneratorEntsoeCategory>(generator, std::stoul(code));
}

void GeneratorEntsoeCategoryXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& entsoeCategory = safeCast<GeneratorEntsoeCategory>(extension);
    context.getExtensionsWriter().writeCharacters(std::to_string(entsoeCategory.getCode()));
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

