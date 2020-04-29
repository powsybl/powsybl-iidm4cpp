/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TerminalMockXmlSerializer.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

#include "TerminalMockExt.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

TerminalMockXmlSerializer::TerminalMockXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("terminalMock", "network", "mock",
        VersionsCompatibilityBuilder()
            .put(IidmXmlVersion::V1_0(), {"1.0"})
            .put(IidmXmlVersion::V1_1(), {"1.1"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0", "http://www.itesla_project.eu/schema/iidm/ext/terminal_mock/1_0")
            .put("1.1", "http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_1")
            .build()) {
}

std::unique_ptr<Extension> TerminalMockXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    checkReadingCompatibility(context);

    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    std::unique_ptr<TerminalMockExt> terminalMockExt = stdcxx::make_unique<TerminalMockExt>(load);
    context.getReader().readUntilEndElement(getExtensionName(), [&terminalMockExt, &load, &context]() {
        if (context.getReader().getLocalName() == "terminal") {
            const std::string& id = context.getReader().getAttributeValue(ID);
            const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            Terminal& terminal = TerminalRefXml::readTerminalRef(load.getNetwork(), id, side);
            terminalMockExt->setTerminal(stdcxx::ref(terminal));
        } else {
            throw AssertionError(logging::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    return std::move(terminalMockExt);
}

void TerminalMockXmlSerializer::write(const Extension& extension, NetworkXmlWriterContext& context) const {
    const auto& terminalMockExt = safeCast<TerminalMockExt>(extension);
    TerminalRefXml::writeTerminalRef(terminalMockExt.getTerminal(), context, getNamespacePrefix(), "terminal", context.getExtensionsWriter());
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
