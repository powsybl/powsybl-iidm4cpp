/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltageRegulationXmlSerializer.hpp>

#include <powsybl/iidm/Battery.hpp>
// #include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/iidm/extensions/iidm/VoltageRegulation.hpp>
#include <powsybl/iidm/extensions/iidm/VoltageRegulationAdder.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltageRegulationXmlSerializer::VoltageRegulationXmlSerializer() :
    converter::xml::AbstractVersionableExtensionXmlSerializer("voltageRegulation", "network", "vr",
        converter::xml::VersionsCompatibilityBuilder()
            .put(converter::xml::IidmXmlVersion::V1_0(), {"1.0", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_1(), {"1.1"})
            .put(converter::xml::IidmXmlVersion::V1_2(), {"1.2", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_3(), {"1.3", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_4(), {"1.4", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_5(), {"1.5", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_6(), {"1.6", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_7(), {"1.7", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_8(), {"1.8", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_9(), {"1.9", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_10(), {"1.10", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_11(), {"1.11", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_12(), {"1.12", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_13(), {"1.1"})
            .put(converter::xml::IidmXmlVersion::V1_14(), {"1.1"})
            .build(),
      stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0-legacy", "http://www.itesla_project.eu/schema/iidm/ext/voltageregulation/1_0")
            .put("1.0", "http://www.itesla_project.eu/schema/iidm/ext/voltage_regulation/1_0")
            .put("1.1", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_1")
            .put("1.2", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_2")
            .put("1.3", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_3")
            .put("1.4", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_4")
            .put("1.5", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_5")
            .put("1.6", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_6")
            .put("1.7", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_7")
            .put("1.8", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_8")
            .put("1.9", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_9")
            .put("1.10", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_10")
            .put("1.11", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_11")
            .put("1.12", "http://www.powsybl.org/schema/iidm/ext/voltage_regulation/1_12")
            .build()) {
}

Extension& VoltageRegulationXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Battery>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
    }
    auto& battery = dynamic_cast<Battery&>(extendable);
    
    const auto& voltageRegulatorOn = context.getReader().getAttributeValue<bool>("voltageRegulatorOn");
    const auto& targetV = context.getReader().getAttributeValue<double>("targetV");

    extendable.newExtension<VoltageRegulationAdder>()
                .withVoltageRegulatorOn(voltageRegulatorOn)
                .withTargetV(targetV)
                .add();
    VoltageRegulation& voltageRegulation = extendable.getExtension<VoltageRegulation>();

    context.getReader().readUntilEndElement(getExtensionName(), [this, &battery, &voltageRegulation, &context]() {
        if (context.getReader().getLocalName() == "terminalRef") {
            converter::xml::TerminalRefXml::readTerminalRef(battery.getNetwork(), context, [&voltageRegulation](Terminal& regulationTerminal) {
                voltageRegulation.setRegulatingTerminal(stdcxx::ref<Terminal>(regulationTerminal));
            });
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    return voltageRegulation;
}

void VoltageRegulationXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& voltageRegulation = safeCast<VoltageRegulation>(extension);
    context.getWriter().writeAttribute("voltageRegulatorOn", voltageRegulation.isVoltageRegulatorOn());
    context.getWriter().writeAttribute("targetV", voltageRegulation.getTargetV());

    if (voltageRegulation.getRegulatingTerminal()
        && !stdcxx::areSame(voltageRegulation.getRegulatingTerminal().get().getBusBreakerView().getConnectableBus().get(),
            voltageRegulation.getExtendable<Battery>().get().getTerminal().getBusBreakerView().getConnectableBus().get())) {
        converter::xml::TerminalRefXml::writeTerminalRef(voltageRegulation.getRegulatingTerminal().get(), context, getNamespacePrefix(), "terminalRef", context.getWriter());
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

