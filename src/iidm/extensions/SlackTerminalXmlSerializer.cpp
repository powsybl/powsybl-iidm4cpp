/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SlackTerminalXmlSerializer.hpp"

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/iidm/extensions/SlackTerminal.hpp>
#include <powsybl/iidm/extensions/SlackTerminalAdder.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

SlackTerminalXmlSerializer::SlackTerminalXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("slackTerminal", "network", "slt",
          converter::xml::VersionsCompatibilityBuilder()
              .put(converter::xml::IidmXmlVersion::V1_3(), {"1.0"})
              .put(converter::xml::IidmXmlVersion::V1_4(), {"1.1"})
              .put(converter::xml::IidmXmlVersion::V1_5(), {"1.2"})
              .put(converter::xml::IidmXmlVersion::V1_6(), {"1.2"})
              .build(),
          stdcxx::MapBuilder<std::string, std::string>()
              .put("1.0", "http://www.powsybl.org/schema/iidm/ext/slack_terminal/1_0")
              .put("1.1", "http://www.powsybl.org/schema/iidm/ext/slack_terminal/1_1")
              .put("1.2", "http://www.powsybl.org/schema/iidm/ext/slack_terminal/1_2")
              .build()) {
}

bool SlackTerminalXmlSerializer::isSerializable(const Extension& extension) const {
    if (stdcxx::isInstanceOf<iidm::extensions::SlackTerminal>(extension)) {
        const auto& slackTerminal = dynamic_cast<const iidm::extensions::SlackTerminal&>(extension);
        return slackTerminal.getTerminal();
    }
    return false;
}

Extension& SlackTerminalXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<VoltageLevel>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Terminal>()));
    }
    auto& voltageLevel = dynamic_cast<VoltageLevel&>(extendable);
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(converter::ID));
    const std::string& side = context.getReader().getAttributeValue(converter::SIDE);
    Terminal& terminal = converter::xml::TerminalRefXml::readTerminalRef(voltageLevel.getNetwork(), id, side);
    extendable.newExtension<SlackTerminalAdder>().withTerminal(terminal).add();
    return extendable.getExtension<iidm::extensions::SlackTerminal>();
}

void SlackTerminalXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& slackTerminal = safeCast<iidm::extensions::SlackTerminal>(extension);
    const auto& terminal = slackTerminal.getTerminal();
    if (terminal) {
        converter::xml::TerminalRefXml::writeTerminalRefAttribute(slackTerminal.getTerminal().get(), context);
    }
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
