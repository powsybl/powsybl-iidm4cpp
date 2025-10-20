/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferencePrioritiesXmlSerializer.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePrioritiesAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriority.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorityAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferencePrioritiesXmlSerializer::ReferencePrioritiesXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("referencePriorities", "network", "refpri", "http://www.powsybl.org/schema/iidm/ext/reference_priorities/1_0") {
}

bool ReferencePrioritiesXmlSerializer::isSerializable(const Extension& extension) const {
    const auto& priorities = safeCast<ReferencePriorities>(extension);
    return !priorities.getReferencePriorities().empty();
}

Extension& ReferencePrioritiesXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Connectable>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
    }
    auto& connectable = dynamic_cast<Connectable&>(extendable);
    connectable.newExtension<ReferencePrioritiesAdder>().add();
    auto& priorities = connectable.getExtension<ReferencePriorities>();
    context.getReader().readUntilEndElement("referencePriorities", [&priorities, &connectable , &context]() {
        if (context.getReader().getLocalName() == "referencePriority") {
            unsigned long priority = context.getReader().getAttributeValue<unsigned long>(PRIORITY);
            Terminal& terminal = converter::xml::TerminalRefXml::readTerminal(connectable.getNetwork(), context);
            priorities.newReferencePriority()
                .setPriority(priority)
                .setTerminal(terminal)
                .add();
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });
    return priorities;
}

void ReferencePrioritiesXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& priorities = safeCast<ReferencePriorities>(extension);
    xml::XmlStreamWriter& writer = context.getWriter();
    for (const ReferencePriority& refPriority : priorities.getReferencePriorities()) {
        writer.writeStartElement(getNamespacePrefix(), "referencePriority");
        writer.writeAttribute(PRIORITY, refPriority.getPriority());
        converter::xml::TerminalRefXml::writeTerminalRefAttribute(refPriority.getTerminal(), context);
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
