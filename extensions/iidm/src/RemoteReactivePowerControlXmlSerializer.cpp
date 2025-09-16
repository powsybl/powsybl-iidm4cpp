/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControlXmlSerializer.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControlAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

RemoteReactivePowerControlXmlSerializer::RemoteReactivePowerControlXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("generatorRemoteReactivePowerControl", "network", "rrpc", "http://www.powsybl.org/schema/iidm/ext/remote_reactive_power_control/1_0") {
}

Extension& RemoteReactivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    auto& generator = dynamic_cast<Generator&>(extendable);

    converter::xml::IidmXmlUtil::assertMinimumVersion(getName(), "", converter::xml::ErrorMessage::NOT_SUPPORTED, converter::xml::IidmXmlVersion::V1_5(), context);
    RemoteReactivePowerControlAdder rrpcAdder = extendable.newExtension<RemoteReactivePowerControlAdder>();
    bool enabled = context.getReader().getAttributeValue<bool>(ENABLED);
    double targetQ = context.getReader().getAttributeValue<double>(TARGETQ);
    Terminal& terminal = converter::xml::TerminalRefXml::readTerminal(generator.getNetwork(), context);

    rrpcAdder.withEnabled(enabled)
            .withTargetQ(targetQ)
            .withRegulatingTerminal(terminal)
            .add();

    return extendable.getExtension<RemoteReactivePowerControl>();
}

void RemoteReactivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& rrpcExtension = safeCast<RemoteReactivePowerControl>(extension);

    converter::xml::IidmXmlUtil::assertMinimumVersion(getName(), "", converter::xml::ErrorMessage::NOT_SUPPORTED, converter::xml::IidmXmlVersion::V1_5(), context);

    context.getWriter().writeAttribute(ENABLED, rrpcExtension.isEnabled());
    context.getWriter().writeAttribute(TARGETQ, rrpcExtension.getTargetQ());
    converter::xml::TerminalRefXml::writeTerminalRefAttribute(rrpcExtension.getRegulatingTerminal(), context);

}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
