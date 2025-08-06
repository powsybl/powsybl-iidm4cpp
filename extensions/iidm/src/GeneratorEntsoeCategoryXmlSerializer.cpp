/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategoryXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategory.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategoryAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorEntsoeCategoryXmlSerializer::GeneratorEntsoeCategoryXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("entsoeCategory", "network", "gec", "http://www.itesla_project.eu/schema/iidm/ext/generator_entsoe_category/1_0") {
}

Extension& GeneratorEntsoeCategoryXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    const std::string& code = context.getReader().readUntilEndElement(getExtensionName());
    extendable.newExtension<GeneratorEntsoeCategoryAdder>()
        .withCode(std::stoul(code))
        .add();
    return extendable.getExtension<GeneratorEntsoeCategory>();
}

void GeneratorEntsoeCategoryXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& entsoeCategory = safeCast<GeneratorEntsoeCategory>(extension);
    context.getWriter().writeCharacters(std::to_string(entsoeCategory.getCode()));
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

