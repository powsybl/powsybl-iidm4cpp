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
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TopologyLevelXmlUtil.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Terminal& TerminalRefXml::readTerminal(Network& network, NetworkXmlReaderContext& context) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    const std::string side = context.getReader().getOptionalAttributeValue(SIDE, "");
    const std::string number = context.getReader().getOptionalAttributeValue(NUMBER, "");
    return TerminalRefXml::resolve(id, side, number, network);
}

void TerminalRefXml::readTerminalRef(Network& network, NetworkXmlReaderContext& context, const std::function<void(Terminal&)>& endTaskTerminalConsumer) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    const std::string side = context.getReader().getOptionalAttributeValue(SIDE, "");
    const std::string number = context.getReader().getOptionalAttributeValue(NUMBER, "");
    context.addEndTask(XmlReaderEndTask::Step::AFTER_EXTENSIONS, [&network, id, side, number, endTaskTerminalConsumer]() {
        Terminal& terminal = resolve(id, side, number, network);
        endTaskTerminalConsumer(terminal);
    });
}

Terminal& TerminalRefXml::resolve(const std::string& id, const std::string& side, const std::string& terminalNumber, Network& network) {
    ThreeSides threeSide = ThreeSides::ONE;
    if(!side.empty() && !terminalNumber.empty()) {
        throw PowsyblException(stdcxx::format("Terminal reference specifies both terminal side and number: '%1%'", id));
    }
    if(!terminalNumber.empty()){
        return TerminalRefXml::resolve(id, Enum::fromString<TerminalNumber>(terminalNumber), network);
    }
    if(!side.empty()) {
        threeSide = Enum::fromString<ThreeSides>(side);
    }
    return TerminalRefXml::resolve(id, threeSide, network);
}

Terminal& TerminalRefXml::resolve(const std::string& id, ThreeSides side, Network& network) {
    const auto& identifiableRef = network.find<Identifiable>(id);
    if (!identifiableRef) {
        throw PowsyblException(stdcxx::format("Terminal reference identifiable not found: '%1%'", id));
    }
    auto& identifiable = identifiableRef.get();

    return Terminal::getTerminal(identifiable, side);
}

Terminal& TerminalRefXml::resolve(const std::string& id, TerminalNumber number, Network& network) {
    const auto& identifiableRef = network.find<Identifiable>(id);
    if (!identifiableRef) {
        throw PowsyblException(stdcxx::format("Terminal reference identifiable not found: '%1%'", id));
    }
    auto& identifiable = identifiableRef.get();

    return Terminal::getTerminal(identifiable, number);
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& elementName) {
    writeTerminalRef(terminal, context, context.getVersion().getPrefix(), elementName);
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName) {
    writeTerminalRef(terminal, context, nsPrefix, elementName, context.getWriter());
}

void TerminalRefXml::writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName, powsybl::xml::XmlStreamWriter& writer) {
    writer.writeStartElement(nsPrefix, elementName);
    writeTerminalRefAttribute(terminal, context);
    writer.writeEndElement();
}

void TerminalRefXml::writeTerminalRefAttribute(const Terminal& terminal, NetworkXmlWriterContext& context) {
    checkTerminal(terminal, context);

    const auto& c = terminal.getConnectable();
    context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(c.get().getId()));

    auto optSide = Terminal::getConnectableSide(terminal);
    if(optSide.has_value() && *optSide != ThreeSides::UNDEFINED) {
        context.getWriter().writeAttribute(SIDE, Enum::toString(*optSide));
    }

    auto optNumber = Terminal::getConnectableTerminalNumber(terminal);
    if(optNumber.has_value() && *optNumber != TerminalNumber::UNDEFINED) {
        context.getWriter().writeAttribute(NUMBER, Enum::toString(*optNumber));
    }

}

void TerminalRefXml::checkTerminal(const Terminal& terminal, NetworkXmlWriterContext& context) {
    const auto& c = terminal.getConnectable();
    if (!context.getFilter().test(c)) {
        throw PowsyblException(stdcxx::format("Oups, terminal ref point to a filtered equipment %1%", c.get().getId()));
    }
    if (terminal.getVoltageLevel().getTopologyKind() == TopologyKind::NODE_BREAKER &&
            TopologyLevelXmlUtil::determineTopologyLevel(terminal.getVoltageLevel(), context) != TopologyLevel::NODE_BREAKER &&
            stdcxx::isInstanceOf<BusbarSection>(terminal.getConnectable())) {
        throw PowsyblException(stdcxx::format("Terminal ref should not point to a busbar section (here %1%). Try to export in node-breaker or delete this terminal ref.", terminal.getConnectable().get().getId()));
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

