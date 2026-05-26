/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorStartupXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorStartup.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorStartupAdder.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorStartupXmlSerializer::GeneratorStartupXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("startup", "network", "gs",
        converter::xml::ExtensionXmlVersions({
            {"http://www.itesla_project.eu/schema/iidm/ext/generator_startup/1_0", "gs", "startup",
            converter::xml::IidmXmlVersion::V1_0(), {1,0}, "itesla"},
            {"http://www.powsybl.org/schema/iidm/ext/generator_startup/1_0", "gs", "startup",
            converter::xml::IidmXmlVersion::V1_0(), {1,0}},
            {"http://www.powsybl.org/schema/iidm/ext/generator_startup/1_1", "gs", "startup",
            converter::xml::IidmXmlVersion::V1_0(), {1,1}}
        })){
}

Extension& GeneratorStartupXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }

    double predefinedActivePowerSetpoint = stdcxx::nan();
    double startupCost = stdcxx::nan();
    const auto& extensionVersion = getExtensionVersionImported(context);
    if(extensionVersion < versionOf("1.1")) {
        predefinedActivePowerSetpoint = context.getReader().getOptionalAttributeValue("predefinedActivePowerSetpoint", stdcxx::nan());
        startupCost = context.getReader().getOptionalAttributeValue("startUpCost", stdcxx::nan());
    } else {
        predefinedActivePowerSetpoint = context.getReader().getOptionalAttributeValue("plannedActivePowerSetpoint", stdcxx::nan());
        startupCost = context.getReader().getOptionalAttributeValue("startupCost", stdcxx::nan());
    }
    const double& marginalCost = context.getReader().getOptionalAttributeValue("marginalCost", stdcxx::nan());
    const double& plannedOutageRate = context.getReader().getOptionalAttributeValue("plannedOutageRate", stdcxx::nan());
    const double& forcedOutageRate = context.getReader().getOptionalAttributeValue("forcedOutageRate", stdcxx::nan());

    extendable.newExtension<GeneratorStartupAdder>()
        .withPredefinedActivePowerSetpoint(predefinedActivePowerSetpoint)
        .withStartupCost(startupCost)
        .withMarginalCost(marginalCost)
        .withPlannedOutageRate(plannedOutageRate)
        .withForcedOutageRate(forcedOutageRate)
        .add();
    return extendable.getExtension<GeneratorStartup>();
}

void GeneratorStartupXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& startup = safeCast<GeneratorStartup>(extension);
    const auto& extensionVersion = getExtensionVersionToExport(context);
    
    if(extensionVersion < versionOf("1.1")) {
        context.getWriter().writeAttribute("predefinedActivePowerSetpoint", startup.getPredefinedActivePowerSetpoint());
        context.getWriter().writeAttribute("startUpCost", startup.getStartupCost());
    } else {
        context.getWriter().writeAttribute("plannedActivePowerSetpoint", startup.getPredefinedActivePowerSetpoint());
        context.getWriter().writeAttribute("startupCost", startup.getStartupCost());
    }
    context.getWriter().writeAttribute("marginalCost", startup.getMarginalCost());
    context.getWriter().writeAttribute("plannedOutageRate", startup.getPlannedOutageRate());
    context.getWriter().writeAttribute("forcedOutageRate", startup.getForcedOutageRate());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

