/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATIONXMLSERIALIZER_HXX
#define POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATIONXMLSERIALIZER_HXX

#include <powsybl/iidm/extensions/cvg/VoltageRegulationXmlSerializer.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/extensions/cvg/VoltageRegulation.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

template <typename T>
VoltageRegulationXmlSerializer<T>::VoltageRegulationXmlSerializer() :
    ExtensionXmlSerializer("voltageRegulation", "network", "http://www.itesla_project.eu/schema/iidm/ext/voltage_regulation/1_0", "vr") {
}

template <typename T>
std::unique_ptr<Extension> VoltageRegulationXmlSerializer<T>::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<T>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<T>()));
    }
    auto& component = dynamic_cast<T&>(extendable);

    const auto& voltageRegulatorOn = context.getReader().getAttributeValue<bool>("voltageRegulatorOn");
    const auto& targetV = context.getReader().getAttributeValue<double>("targetV");

    std::unique_ptr<VoltageRegulation<T>> voltageRegulation = stdcxx::make_unique<VoltageRegulation<T>>(component, voltageRegulatorOn, targetV);

    context.getReader().readUntilEndElement(getExtensionName(), [&context, &component, &voltageRegulation]() {
        // TODO(sebalaig) do we have to make Constants.hpp public?
        if (context.getReader().getLocalName() == "terminalRef") {
            const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue("id"));

            // TerminalRefXml::writeTerminalRef does not write "side" attribute if terminal.getConnectable() is an Injection
            // Java API XmlStreamWriter::getAttributeValue does not throw any exception if the attribute does not exist but returns ""
            // => same behaviour by using getOptionalAttributeValue
            const std::string& side = context.getReader().getOptionalAttributeValue("side", "");

            context.addEndTask([&component, &voltageRegulation, id, side]() {
                Network& network = component.getTerminal().getVoltageLevel().getSubstation().getNetwork();
                voltageRegulation->setRegulatingTerminal(stdcxx::cref(converter::xml::TerminalRefXml::readTerminalRef(network, id, side)));
            });
        } else {
            throw PowsyblException(logging::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    return stdcxx::make_unique<Extension, VoltageRegulation<T>>(*voltageRegulation);
}

template <typename T>
void VoltageRegulationXmlSerializer<T>::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& voltageRegulation = safeCast<VoltageRegulation<T>>(extension);
    context.getExtensionsWriter().writeAttribute("voltageRegulatorOn", voltageRegulation.isVoltageRegulatorOn());
    context.getExtensionsWriter().writeAttribute("targetV", voltageRegulation.getTargetV());

    if (voltageRegulation.getRegulatingTerminal()
        && !stdcxx::areSame(voltageRegulation.getRegulatingTerminal().get().getBusBreakerView().getConnectableBus().get(),
            voltageRegulation.template getExtendable<T>().get().getTerminal().getBusBreakerView().getConnectableBus().get())) {
        converter::xml::TerminalRefXml::writeTerminalRef(voltageRegulation.getRegulatingTerminal().get(), context, getNamespacePrefix(), "terminalRef", context.getExtensionsWriter());
    }
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATIONXMLSERIALIZER_HXX
