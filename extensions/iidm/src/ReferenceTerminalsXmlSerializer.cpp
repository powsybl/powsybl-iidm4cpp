/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferenceTerminalsXmlSerializer.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminals.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminalsAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferenceTerminalsXmlSerializer::ReferenceTerminalsXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("referenceTerminals", "network", "reft", "http://www.powsybl.org/schema/iidm/ext/reference_terminals/1_0") {
}

bool ReferenceTerminalsXmlSerializer::isSerializable(const Extension& extension) const {
    const auto& refTerminals = safeCast<ReferenceTerminals>(extension);
    return !refTerminals.getReferenceTerminals().empty();
}

Extension& ReferenceTerminalsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Network>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Network>()));
    }
    auto& network = dynamic_cast<Network&>(extendable);

    std::vector<stdcxx::Reference<Terminal>> terminals;
    context.getReader().readUntilEndElement("referenceTerminals", [&terminals, &network, &context]() {
        if (context.getReader().getLocalName() == "referenceTerminal") {
            Terminal& terminal = converter::xml::TerminalRefXml::readTerminal(network, context);
            terminals.push_back(stdcxx::ref(terminal));
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    network.newExtension<ReferenceTerminalsAdder>()
                .withTerminals(terminals)
                .add();
    auto& ext = network.getExtension<ReferenceTerminals>();
    return ext;
}

void ReferenceTerminalsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& referenceTerminals = safeCast<ReferenceTerminals>(extension);
    xml::XmlStreamWriter& writer = context.getWriter();
    for (const auto& refTerminal : referenceTerminals.getReferenceTerminals()) {
        writer.writeStartElement(getNamespacePrefix(), "referenceTerminal");
        converter::xml::TerminalRefXml::writeTerminalRefAttribute(refTerminal, context);
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
