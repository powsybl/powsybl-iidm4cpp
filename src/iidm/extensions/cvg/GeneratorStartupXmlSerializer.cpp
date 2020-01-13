/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/GeneratorStartupXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/GeneratorStartup.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

GeneratorStartupXmlSerializer::GeneratorStartupXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("startup", "network", "http://www.itesla_project.eu/schema/iidm/ext/generator_startup/1_0", "gs") {
}

std::unique_ptr<Extension> GeneratorStartupXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    auto& generator = dynamic_cast<Generator&>(extendable);

    const double& predefinedActivePowerSetpoint = context.getReader().getOptionalAttributeValue("predefinedActivePowerSetpoint", stdcxx::nan());
    const double& marginalCost = context.getReader().getOptionalAttributeValue("marginalCost", stdcxx::nan());
    const double& plannedOutageRate = context.getReader().getOptionalAttributeValue("plannedOutageRate", stdcxx::nan());
    const double& forcedOutageRate = context.getReader().getOptionalAttributeValue("forcedOutageRate", stdcxx::nan());
    return stdcxx::make_unique<GeneratorStartup>(generator, predefinedActivePowerSetpoint, marginalCost, plannedOutageRate, forcedOutageRate);
}

void GeneratorStartupXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& startup = safeCast<GeneratorStartup>(extension);
    context.getExtensionsWriter().writeAttribute("predefinedActivePowerSetpoint", startup.getPredefinedActivePowerSetpoint());
    context.getExtensionsWriter().writeAttribute("marginalCost", startup.getMarginalCost());
    context.getExtensionsWriter().writeAttribute("plannedOutageRate", startup.getPlannedOutageRate());
    context.getExtensionsWriter().writeAttribute("forcedOutageRate", startup.getForcedOutageRate());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

