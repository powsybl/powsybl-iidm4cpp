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
        converter::xml::ExtensionXmlVersions({
            {"http://www.itesla_project.eu/schema/iidm/ext/terminal_mock/1_0", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_1(), {1,0}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_1", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_1(), converter::xml::IidmXmlVersion::V1_2(), {1,1}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_2", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_2(), converter::xml::IidmXmlVersion::V1_3(), {1,2}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_3", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_3(), converter::xml::IidmXmlVersion::V1_4(), {1,3}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_4", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_4(), converter::xml::IidmXmlVersion::V1_5(), {1,4}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_5", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_5(), converter::xml::IidmXmlVersion::V1_6(), {1,5}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_6", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_6(), converter::xml::IidmXmlVersion::V1_7(), {1,6}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_7", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_7(), converter::xml::IidmXmlVersion::V1_8(), {1,7}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_8", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_8(), converter::xml::IidmXmlVersion::V1_9(), {1,8}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_9", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_9(), converter::xml::IidmXmlVersion::V1_10(), {1,9}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_10", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_10(), converter::xml::IidmXmlVersion::V1_11(), {1,10}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_11", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_11(), converter::xml::IidmXmlVersion::V1_12(), {1,11}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_12", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_12(), converter::xml::IidmXmlVersion::V1_13(), {1,12}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_13", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_13(), converter::xml::IidmXmlVersion::V1_14(), {1,13}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_14", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_14(), converter::xml::IidmXmlVersion::V1_15(), {1,14}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_15", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_15(), converter::xml::IidmXmlVersion::V1_16(), {1,15}},
            {"http://www.powsybl.org/schema/iidm/ext/terminal_mock/1_16", "mock", "terminalMock",
            converter::xml::IidmXmlVersion::V1_16(), {1,16}}
        })){
}

Extension& TerminalMockXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    checkReadingCompatibility(context);

    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    std::unique_ptr<TerminalMockExt> terminalMockExt = stdcxx::make_unique<TerminalMockExt>(load);
    context.getReader().readUntilEndElement(getExtensionName(), [&terminalMockExt, &load, &context]() {
        if (context.getReader().getLocalName() == "terminal") {
            const std::string& id = context.getReader().getAttributeValue(ID);
            const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            const std::string& number = context.getReader().getOptionalAttributeValue(NUMBER, "");
            Terminal& terminal = TerminalRefXml::resolve(id, side, number, load.getNetwork());
            terminalMockExt->setTerminal(stdcxx::ref(terminal));
        } else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    extendable.addExtension(std::move(terminalMockExt));
    return extendable.getExtension<TerminalMockExt>();
}

void TerminalMockXmlSerializer::write(const Extension& extension, NetworkXmlWriterContext& context) const {
    const auto& terminalMockExt = safeCast<TerminalMockExt>(extension);
    TerminalRefXml::writeTerminalRef(terminalMockExt.getTerminal(), context, getNamespacePrefix(), "terminal", context.getWriter());
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
