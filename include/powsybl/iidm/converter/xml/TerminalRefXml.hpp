/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP

#include <string>
#include <functional>

#include <powsybl/iidm/ThreeSides.hpp>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class Network;
class Terminal;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class TerminalRefXml {
public:
    static Terminal& readTerminal(Network& network, NetworkXmlReaderContext& context);

    static void readTerminalRef(Network& network, NetworkXmlReaderContext& context, const std::function<void(Terminal&)>& endTaskTerminalConsumer);

    static Terminal& resolve(const std::string& id, const std::string& side, Network& network);

    static Terminal& resolve(const std::string& id, ThreeSides side, Network& network);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& elementName);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName, powsybl::xml::XmlStreamWriter& writer);

    static void writeTerminalRefAttribute(const Terminal& terminal, NetworkXmlWriterContext& context);

private:
    static void checkTerminal(const Terminal& terminal, const NetworkXmlWriterContext& context);

    TerminalRefXml() = default;

    ~TerminalRefXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP

