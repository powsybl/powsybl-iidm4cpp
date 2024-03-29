/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Terminal& TerminalRefXml::readTerminalRef(Network& network, const std::string& id, const std::string& side) {
    const auto& identifiableRef = network.find<Identifiable>(id);
    if (!identifiableRef) {
        throw PowsyblException(stdcxx::format("Terminal reference identifiable not found: '%1%'", id));
    }
    auto& identifiable = identifiableRef.get();
    if (stdcxx::isInstanceOf<Injection>(identifiable)) {
        return dynamic_cast<Injection&>(identifiable).getTerminal();
    }
    if (stdcxx::isInstanceOf<Branch>(identifiable)) {
        return dynamic_cast<Branch&>(identifiable).getTerminal(Enum::fromString<Branch::Side>(side));
    }
    if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)) {
        auto& twt = dynamic_cast<ThreeWindingsTransformer&>(identifiable);
        return twt.getTerminal(Enum::fromString<ThreeWindingsTransformer::Side>(side));
    }

    throw PowsyblException(stdcxx::format("Unexpected terminal reference identifiable instance: %1%", stdcxx::demangle(identifiable)));
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& elementName) {
    writeTerminalRef(terminal, context, context.getVersion().getPrefix(), elementName);
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName) {
    writeTerminalRef(terminal, context, nsPrefix, elementName, context.getWriter());
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName, powsybl::xml::XmlStreamWriter& writer) {
    const auto& c = terminal.getConnectable();
    if (!context.getFilter().test(c)) {
        throw PowsyblException(stdcxx::format("Oups, terminal ref point to a filtered equipment %1%", c.get().getId()));
    }
    if (terminal.getVoltageLevel().getTopologyKind() == TopologyKind::NODE_BREAKER &&
            context.getOptions().getTopologyLevel() != TopologyLevel::NODE_BREAKER &&
            stdcxx::isInstanceOf<BusbarSection>(terminal.getConnectable())) {
        throw PowsyblException(stdcxx::format("Terminal ref should not point to a busbar section (here %1%). Try to export in node-breaker or delete this terminal ref.", terminal.getConnectable().get().getId()));
    }
    writer.writeStartElement(nsPrefix, elementName);
    writeTerminalRefAttribute(terminal, context);
    writer.writeEndElement();
}

void TerminalRefXml::writeTerminalRefAttribute(const Terminal& terminal, NetworkXmlWriterContext& context) {
    const auto& c = terminal.getConnectable();
    if (!context.getFilter().test(c)) {
        throw PowsyblException(stdcxx::format("Oups, terminal ref point to a filtered equipment %1%", c.get().getId()));
    }
    if (terminal.getVoltageLevel().getTopologyKind() == TopologyKind::NODE_BREAKER &&
        context.getOptions().getTopologyLevel() != TopologyLevel::NODE_BREAKER &&
        stdcxx::isInstanceOf<BusbarSection>(terminal.getConnectable())) {
        throw PowsyblException(stdcxx::format("Terminal ref should not point to a busbar section (here %1%). Try to export in node-breaker or delete this terminal ref.", terminal.getConnectable().get().getId()));
    }
    context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(c.get().getId()));
    if (c.get().getTerminals().size() > 1) {
        if (stdcxx::isInstanceOf<Injection>(c.get())) {
            // nothing to do
        } else if (stdcxx::isInstanceOf<Branch>(c.get())) {
            const auto& branch = dynamic_cast<const Branch&>(c.get());
            context.getWriter().writeAttribute(SIDE, Enum::toString(branch.getSide(terminal)));
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(c.get())) {
            const auto& twt = dynamic_cast<const ThreeWindingsTransformer&>(c.get());
            context.getWriter().writeAttribute(SIDE, Enum::toString(twt.getSide(terminal)));
        } else {
            throw AssertionError(stdcxx::format("Unexpected Connectable instance: %1%", stdcxx::demangle(c.get())));
        }
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

