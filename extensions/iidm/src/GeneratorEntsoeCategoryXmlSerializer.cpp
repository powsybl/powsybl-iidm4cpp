/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
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

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorEntsoeCategoryXmlSerializer::GeneratorEntsoeCategoryXmlSerializer() :
    converter::xml::AbstractVersionableExtensionXmlSerializer("entsoeCategory", "network", "gec",
        converter::xml::ExtensionXmlVersions({
            {"http://www.itesla_project.eu/schema/iidm/ext/generator_entsoe_category/1_0", "gec", "entsoeCategory", 
                converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_16(), {1,0}},
            {"http://www.powsybl.org/schema/iidm/ext/generator_entsoe_category/1_1", "gec", "entsoeCategory", 
                converter::xml::IidmXmlVersion::V1_16(), {1,1}}
        })) {
}

Extension& GeneratorEntsoeCategoryXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    const std::string& code = context.getReader().readUntilEndElement(getExtensionName());
    unsigned long codeUL = std::stoul(code);

    const auto& extensionVersion = getExtensionVersionImported(context);
    if (codeUL == 0 && extensionVersion == versionOf("1.0")) {
        throw powsybl::xml::XmlStreamException(stdcxx::format("Extension entsoeCategory for Generator %1% : code = 0 not allowed in version 1.0",
                        dynamic_cast<Generator&>(extendable).getId()));
    }

    extendable.newExtension<GeneratorEntsoeCategoryAdder>()
        .withCode(codeUL)
        .add();
    return extendable.getExtension<GeneratorEntsoeCategory>();
}

void GeneratorEntsoeCategoryXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& entsoeCategory = safeCast<GeneratorEntsoeCategory>(extension);
    context.getWriter().writeCharacters(std::to_string(entsoeCategory.getCode()));
}

bool GeneratorEntsoeCategoryXmlSerializer::isSerializable(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& entsoeCategory = safeCast<GeneratorEntsoeCategory>(extension);
    if(entsoeCategory.getCode() == 0 && getExtensionVersionToExport(context) == versionOf("1.0")) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<GeneratorEntsoeCategoryXmlSerializer>();
        logger.warn("Extension entsoeCategory not valid for Generator: %1%. Reason: code = 0 not allowed in version 1.0.",
                        entsoeCategory.getExtendable<Generator>().get().getId());
        return false;
    }
    return true;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

