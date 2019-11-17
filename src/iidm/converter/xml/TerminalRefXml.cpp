/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Terminal& TerminalRefXml::readTerminalRef(Network& network, const std::string& id, const std::string& side) {
    auto& identifiable = network.get<Identifiable>(id);
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

    throw AssertionError(logging::format("Unexpected Identifiable instance: %1%", stdcxx::demangle(identifiable)));
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& elementName) {
    writeTerminalRef(terminal, context, IIDM_PREFIX, elementName);
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName) {
    writeTerminalRef(terminal, context, nsPrefix, elementName, context.getWriter());
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName, powsybl::xml::XmlStreamWriter& writer) {
    const auto& c = terminal.getConnectable();
    if (!context.getFilter().test(c)) {
        throw PowsyblException(logging::format("Oups, terminal ref point to a filtered equipment %1%", c.get().getId()));
    }
    writer.writeStartElement(nsPrefix, elementName);
    writer.writeAttribute(ID, context.getAnonymizer().anonymizeString(c.get().getId()));
    if (c.get().getTerminals().size() > 1) {
        if (stdcxx::isInstanceOf<Injection>(c.get())) {
            // nothing to do
        } else if (stdcxx::isInstanceOf<Branch>(c.get())) {
            const auto& branch = dynamic_cast<const Branch&>(c.get());
            writer.writeAttribute(SIDE, Enum::toString(branch.getSide(terminal)));
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(c.get())) {
            const auto& twt = dynamic_cast<const ThreeWindingsTransformer&>(c.get());
            writer.writeAttribute(SIDE, Enum::toString(twt.getSide(terminal)));
        } else {
            throw AssertionError(logging::format("Unexpected Connectable instance: %1%", stdcxx::demangle(c.get())));
        }
    }
    writer.writeEndElement();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

