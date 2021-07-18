/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP

#include <string>

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
    static Terminal& readTerminalRef(Network& network, const std::string& id, const std::string& side);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& elementName);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName);

    static void writeTerminalRef(const Terminal& terminal, NetworkXmlWriterContext& context, const std::string& nsPrefix, const std::string& elementName, powsybl::xml::XmlStreamWriter& writer);

    static void writeTerminalRefAttribute(const Terminal& terminal, NetworkXmlWriterContext& context);

private:
    TerminalRefXml() = default;

    ~TerminalRefXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TERMINALREFXML_HPP

