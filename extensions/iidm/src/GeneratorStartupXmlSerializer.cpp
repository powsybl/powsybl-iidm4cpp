/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
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
    converter::xml::AbstractVersionableExtensionXmlSerializer("startup", "network", "gs", 
        converter::xml::VersionsCompatibilityBuilder()
            .put(converter::xml::IidmXmlVersion::V1_0(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_1(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_2(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_3(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_4(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_5(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_6(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_7(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_8(), {"1.0-itesla", "1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_9(), {"1.0-itesla", "1.0", "1.1"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0-itesla", "http://www.itesla_project.eu/schema/iidm/ext/generator_startup/1_0")
            .put("1.0", "http://www.powsybl.org/schema/iidm/ext/generator_startup/1_0")
            .put("1.1", "http://www.powsybl.org/schema/iidm/ext/generator_startup/1_1")
            .build()) {
}

Extension& GeneratorStartupXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }

    double predefinedActivePowerSetpoint = stdcxx::nan();
    double startupCost = stdcxx::nan();
    const std::string extensionVersion = context.getExtensionVersion(*this);
    if (extensionVersion.empty()) {
        throw AssertionError(stdcxx::format("No version found for startup extension"));
    } else if(extensionVersion == "1.0" || extensionVersion == "1.0-itesla") {
        predefinedActivePowerSetpoint = context.getReader().getOptionalAttributeValue("predefinedActivePowerSetpoint", stdcxx::nan());
        startupCost = context.getReader().getOptionalAttributeValue("startUpCost", stdcxx::nan());
    } else if(extensionVersion == "1.1") {
        predefinedActivePowerSetpoint = context.getReader().getOptionalAttributeValue("plannedActivePowerSetpoint", stdcxx::nan());
        startupCost = context.getReader().getOptionalAttributeValue("startupCost", stdcxx::nan());
    } else {
        throw PowsyblException(stdcxx::format("Unsupported startup version: %1%", extensionVersion));
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
    std::string extensionVersion = context.getExtensionVersion("startup");
    if (extensionVersion.empty()) {
        extensionVersion = getVersion(context.getVersion());
    }

    if(extensionVersion == "1.0" || extensionVersion == "1.0-itesla") {
        context.getWriter().writeAttribute("predefinedActivePowerSetpoint", startup.getPredefinedActivePowerSetpoint());
        context.getWriter().writeAttribute("startUpCost", startup.getStartupCost());
    } else if(extensionVersion == "1.1") {
        context.getWriter().writeAttribute("plannedActivePowerSetpoint", startup.getPredefinedActivePowerSetpoint());
        context.getWriter().writeAttribute("startupCost", startup.getStartupCost());
    } else {
        throw PowsyblException(stdcxx::format("Unsupported startup version: %1%", extensionVersion));
    }
    context.getWriter().writeAttribute("marginalCost", startup.getMarginalCost());
    context.getWriter().writeAttribute("plannedOutageRate", startup.getPlannedOutageRate());
    context.getWriter().writeAttribute("forcedOutageRate", startup.getForcedOutageRate());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

